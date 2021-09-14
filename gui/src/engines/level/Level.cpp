#include "Level.h"
#include "ui_Level.h"

Level::Level(QWidget *parent, ResultsModel *resultsModel) :BaseClass(ENGINE::LEVEL, resultsModel, parent),ui(new Ui::Level),
    m_scanArguments(new level::ScanArguments)
{
    ui->setupUi(this);
    ///
    /// init...
    ///
    ui->targets->init("Targets");
    ui->wordlist->init("Wordlist");
    initBaseClass(ui->targets);
    scanConfig->name = tr("ScanConfig-Level");
    ///
    /// ...
    ///
    ui->buttonStop->setDisabled(true);
    ui->buttonPause->setDisabled(true);
    ui->progressBar->hide();
    //...
    resultsModel->level->setHorizontalHeaderLabels({"Subdomain Name:", "IpAddress"});
    //...
    ui->tableViewResults->setModel(resultsModel->level);
    //...
    m_scanArguments->wordlist = ui->wordlist->listWidget;
    m_scanArguments->targetList = ui->targets->listWidget;
    ///
    /// equally seperate the widgets...
    ///
    ui->splitter->setSizes(QList<int>() << static_cast<int>((this->width() * 0.50))
                                        << static_cast<int>((this->width() * 0.50)));
}

Level::~Level(){
    delete m_scanArguments;
    //...
    delete ui;
}

void Level::on_buttonStart_clicked(){
    ///
    /// checking if all requirements are satisfied before scan if not prompt error
    /// then exit function...
    ///
    if(ui->wordlist->listWidget->count() < 1){
        QMessageBox::warning(this, "Error!", "Please Enter wordlist For Enumeration!");
        return;
    }
    if(ui->targets->listWidget->count() < 1){
        QMessageBox::warning(this, "Error!", "Please Enter Target Subdomains For Enumeration!");
        return;
    }
    ///
    /// disabling & Enabling widgets...
    ///
    ui->buttonStart->setDisabled(true);
    ui->buttonPause->setEnabled(true);
    ui->buttonStop->setEnabled(true);
    ui->progressBar->reset();
    ui->progressBar->show();
    ///
    /// Resetting & Getting the arguments....
    ///
    m_scanArguments->progress = 0;
    m_scanArguments->currentLevel = 0;
    m_scanArguments->currentTargetToEnumerate = 0;
    m_scanArguments->currentWordlistToEnumerate = 0;
    m_scanArguments->maxLevel = ui->spinBoxLevels->value();
    m_lastScanResultsCount = resultsModel->level->rowCount();
    ui->progressBar->setMaximum(ui->targets->listWidget->count()*ui->wordlist->listWidget->count());
    ///
    /// subdomain level check...
    ///
    m_scanArguments->currentLevel++;
    //ui->label_level->setNum(m_scanArguments->currentLevel);
    ///
    /// start enumeration...
    ///
    startScan();
    //...
    sendStatus("[*] Enumerating Multi-Level Subdomains...");
    logs("[START] Enumerating Multi-Level Subdomains...");
}

void Level::on_buttonPause_clicked(){
    ///
    /// if the scan was already paused, then this current click is to
    /// Resume the scan, just call the startScan, with the same arguments and
    /// it will continue at where it ended...
    ///
    if(scanStatus->isPaused)
    {
        ui->buttonPause->setText("Pause");
        scanStatus->isPaused = false;
        //...
        startScan();
        //...
        sendStatus("[RESUME] Resumed Subdomain Enumeration!");
        logs("[RESUME] Resumed Subdomain Enumeration!");
    }
    else
    {
        scanStatus->isPaused = true;
        emit stopScan();
    }
}

void Level::on_buttonStop_clicked(){
    scanStatus->isStopped = true;
    emit stopScan();
}

void Level::startScan(){
    ///
    /// if the numner of threads is greater than the number of wordlists, set the
    /// number of threads to use to the number of wordlists available to avoid
    /// creating more threads than needed...
    ///
    int wordlistCount = ui->wordlist->listWidget->count();
    int threadsCount = scanConfig->threadsCount;
    if(threadsCount > wordlistCount)
    {
        threadsCount = wordlistCount;
    }
    activeThreads = threadsCount;
    ///
    /// starting the loop to enumerate subdmains according to the number of threads...
    ///
    for(int i = 0; i < threadsCount; i++)
    {
        level::Scanner *scanner = new level::Scanner(scanConfig, m_scanArguments);
        QThread *cThread = new QThread;
        scanner->startScan(cThread);
        scanner->moveToThread(cThread);
        //...
        connect(scanner, SIGNAL(scanResult(QString, QString)), this, SLOT(scanResult(QString, QString)));
        connect(scanner, SIGNAL(scanProgress(int)), ui->progressBar, SLOT(setValue(int)));
        connect(scanner, SIGNAL(scanLog(QString)), this, SLOT(logs(QString)));
        connect(cThread, SIGNAL(finished()), this, SLOT(scanThreadEnd()));
        connect(cThread, SIGNAL(finished()), scanner, SLOT(deleteLater()));
        connect(cThread, SIGNAL(finished()), cThread, SLOT(deleteLater()));
        connect(this, SIGNAL(stopScan()), scanner, SLOT(stopScan()));
        //...
        cThread->start();
    }
    scanStatus->isRunning = true;
}

void Level::nextLevel(){
    ///
    /// first Clear the last level targetList of all previous subdomains...
    ///
    ui->targets->listWidget->clear();
    ///
    /// then copy the newly enumerated subdomains from results model to the
    /// new targetList...
    ///
    while(m_lastScanResultsCount < resultsModel->level->rowCount())
    {
        ui->targets->add(resultsModel->level->item(m_lastScanResultsCount, 0)->text());
        m_lastScanResultsCount++;
    }
    ///
    /// Resetting the arguments....
    ///
    ui->progressBar->reset();
    m_scanArguments->progress = 0;
    m_scanArguments->currentTargetToEnumerate = 0;
    m_scanArguments->currentWordlistToEnumerate = 0;
    m_lastScanResultsCount = resultsModel->level->rowCount();
    ui->progressBar->setMaximum(ui->targets->listWidget->count()*ui->wordlist->listWidget->count());
    ///
    /// subdomain level check...
    ///
    m_scanArguments->currentLevel++;
    //ui->label_level->setNum(m_scanArguments->currentLevel);
    ///
    /// start another scan on a higher level than previous scan...
    ///
    startScan();
    //...
    sendStatus("[*] Enumerating subdomains on Level: "+QString::number(m_scanArguments->currentLevel));
    logs("[*] Enumerating subdomains on Level: "+QString::number(m_scanArguments->currentLevel));
}

void Level::scanThreadEnd(){
    activeThreads--;
    if(activeThreads != 0)
    {
        return;
    }
    ///
    /// check if you've reached last level and if not start another scan to enumerate
    /// another level...
    ///
    if(scanStatus->isPaused)
    {
        ui->buttonPause->setText("Resume");
        scanStatus->isRunning = false;
        //...
        sendStatus("[*] Scan Paused!");
        logs("[*] Scan Paused!\n");
        return;
    }
    if(!scanStatus->isStopped && (m_scanArguments->currentLevel < m_scanArguments->maxLevel) && (m_lastScanResultsCount < resultsModel->level->rowCount()))
    {
        nextLevel();
    }
    else
    {
        // set the progress bar to 100% just in case...
        if(!scanStatus->isStopped){
            ui->progressBar->setValue(ui->progressBar->maximum());
        }
        ///
        /// Reached End of the scan on all levels or the scan was stopped...
        ///
        scanStatus->isPaused = false;
        scanStatus->isStopped = false;
        scanStatus->isRunning = false;
        //...
        ui->buttonStart->setEnabled(true);
        ui->buttonStop->setDisabled(true);
        ui->buttonPause->setDisabled(true);
        //...
        sendStatus("[*] Enumeration by tldBrute Complete!");
        logs("[END] Multi-Level Subdomain Bruteforcing Complete!\n");
    }
}

void Level::on_buttonConfig_clicked(){
    ConfigDialog *configDialog = new ConfigDialog(this, scanConfig);
    configDialog->setAttribute( Qt::WA_DeleteOnClose, true );
    configDialog->show();
}

void Level::scanResult(QString subdomain, QString ipAddress){
    ///
    /// save to level model...
    ///
    resultsModel->level->appendRow(QList<QStandardItem*>() <<new QStandardItem(ipAddress) <<new QStandardItem(subdomain));
    ui->labelResultsCount->setNum(resultsModel->level->rowCount());
    ///
    /// save to project model...
    ///
    resultsModel->project->addSubdomain(QStringList()<<subdomain<<ipAddress);

}

void Level::on_buttonClearResults_clicked(){
    ///
    /// if the current tab is subdomains clear subdomains...
    ///
    if(ui->tabWidgetResults->currentIndex() == 0)
    {
        resultsModel->level->clear();
        ui->labelResultsCount->clear();
        //...
        ui->progressBar->reset();
        ui->progressBar->hide();
        //...
        resultsModel->level->setHorizontalHeaderLabels({"Subdomain Name", "IpAddress"});
    }
    ///
    /// if the current tab is logs clear logs...
    ///
    else
    {
        ui->listWidgetLogs->clear();
    }
}

void Level::on_buttonWordlist_clicked(){
    WordListDialog *wordlistDialog = new WordListDialog(this, ENGINE::SUBBRUTE);
    wordlistDialog->setAttribute( Qt::WA_DeleteOnClose, true );
    connect(wordlistDialog, SIGNAL(choosenWordlist(QString)), this, SLOT(choosenWordlist(QString)));
    wordlistDialog->show();
}

void Level::choosenWordlist(QString wordlistFilename){
    QFile file(wordlistFilename);
    ui->wordlist->add(file);
}

void Level::on_buttonAction_clicked(){
    ///
    /// check if there are results available else dont show the context menu...
    ///
    if(resultsModel->level->rowCount() < 1){
        return;
    }
    ///
    /// getting the position of the action button to place the context menu and
    /// showing the context menu right by the side of the action button...
    ///
    QPoint pos = ui->buttonAction->mapToGlobal(QPoint(0,0));
    a_Menu->exec(QPoint(pos.x()+76, pos.y()));
}

void Level::on_tableViewResults_customContextMenuRequested(const QPoint &pos){
    Q_UNUSED(pos);
    ///
    /// check if user right clicked on items else dont show the context menu...
    ///
    if(!ui->tableViewResults->selectionModel()->isSelected(ui->tableViewResults->currentIndex())){
        return;
    }
    selectionModel = ui->tableViewResults->selectionModel();
    c_Menu->exec(QCursor::pos());
}
