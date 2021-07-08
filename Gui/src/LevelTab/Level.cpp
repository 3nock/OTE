#include "Level.h"
#include "ui_Level.h"

Level::Level(QWidget *parent) :QDialog(parent),ui(new Ui::Level),
    m_scanStatus(new ScanStatus),
    m_scanConfig(new ScanConfig),
    m_scanArguments(new ScanArguments_level),
    m_model_results(new QStandardItemModel)
{
    ui->setupUi(this);
    //...
    ui->lineEdit_wordlist->setPlaceholderText("Enter new wordlist item...");
    ui->lineEdit_targets->setPlaceholderText("Enter new target item...");
    //...
    ui->pushButton_stop->setDisabled(true);
    ui->pushButton_pause->setDisabled(true);
    ui->progressBar->hide();
    //...
    m_model_results->setHorizontalHeaderLabels({"Subdomain Name:", "IpAddress"});
    //...
    ui->tableView_results->setModel(m_model_results);
    //...
    m_scanArguments->wordlist = ui->listWidget_wordlist;
    m_scanArguments->targetList = ui->listWidget_targets;
    //...
    ui->splitter->setSizes(QList<int>()<<220<<2);
}

Level::~Level(){
    delete m_scanStatus;
    delete m_scanConfig;
    delete m_model_results;
    delete m_scanArguments;
    //...
    delete ui;
}

/************************************** Start and Stop Scan ********************************************/
void Level::on_pushButton_start_clicked(){
    ///
    /// checking if all requirements are satisfied before scan if not prompt error
    /// then exit function...
    ///
    if(ui->listWidget_wordlist->count() < 1){
        QMessageBox::warning(this, TITLE_ERROR, "Please Enter wordlist For Enumeration!");
        return;
    }
    if(ui->listWidget_targets->count() < 1){
        QMessageBox::warning(this, TITLE_ERROR, "Please Enter Target Subdomains For Enumeration!");
        return;
    }
    ///
    /// disabling & Enabling widgets...
    ///
    ui->pushButton_start->setDisabled(true);
    ui->pushButton_pause->setEnabled(true);
    ui->pushButton_stop->setEnabled(true);
    ui->progressBar->reset();
    ui->progressBar->show();
    ///
    /// Resetting & Getting the arguments....
    ///
    m_scanArguments->progress = 0;
    m_scanArguments->currentLevel = 0;
    m_scanArguments->currentTargetToEnumerate = 0;
    m_scanArguments->currentWordlistToEnumerate = 0;
    m_scanArguments->maxLevel = ui->spinBox_levels->value();
    lastScanResultsCount = m_model_results->rowCount();
    ui->progressBar->setMaximum(ui->listWidget_targets->count()*ui->listWidget_wordlist->count());
    ///
    /// subdomain level check...
    ///
    m_scanArguments->currentLevel++;
    ui->label_level->setNum(m_scanArguments->currentLevel);
    ///
    /// start enumeration...
    ///
    startScan();
    //...
    sendStatus("[*] Enumerating Multi-Level Subdomains...");
    logs("[START] Enumerating Multi-Level Subdomains...");
}

/********************************** PAUSE/STOP ******************************************/
void Level::on_pushButton_pause_clicked(){
    ///
    /// if the scan was already paused, then this current click is to
    /// Resume the scan, just call the startScan, with the same arguments and
    /// it will continue at where it ended...
    ///
    if(m_scanStatus->isPaused)
    {
        ui->pushButton_pause->setText("Pause");
        m_scanStatus->isPaused = false;
        //...
        startScan();
        //...
        sendStatus("[RESUME] Resumed Subdomain Enumeration!");
        logs("[RESUME] Resumed Subdomain Enumeration!");
    }
    else
    {
        m_scanStatus->isPaused = true;
        emit stop();
    }
}

void Level::on_pushButton_stop_clicked(){
    m_scanStatus->isStopped = true;
    emit stop();
}

/***************************************************************************************
                                    Enumeration
****************************************************************************************/
void Level::startScan(){
    ///
    /// if the numner of threads is greater than the number of wordlists, set the
    /// number of threads to use to the number of wordlists available to avoid
    /// creating more threads than needed...
    ///
    int wordlistCount = ui->listWidget_wordlist->count();
    int threadsCount = m_scanConfig->threadsCount;
    if(threadsCount > wordlistCount)
    {
        threadsCount = wordlistCount;
    }
    m_activeThreads = threadsCount;
    ///
    /// starting the loop to enumerate subdmains according to the number of threads...
    ///
    for(int i = 0; i < threadsCount; i++)
    {
        LevelEnumerator *Enumerator = new LevelEnumerator(m_scanConfig, m_scanArguments);
        QThread *cThread = new QThread;
        Enumerator->enumerate(cThread);
        Enumerator->moveToThread(cThread);
        //...
        connect(Enumerator, SIGNAL(scanResult(QString, QString)), this, SLOT(scanResult(QString, QString)));
        connect(Enumerator, SIGNAL(progress(int)), ui->progressBar, SLOT(setValue(int)));
        connect(Enumerator, SIGNAL(scanLog(QString)), this, SLOT(logs(QString)));
        connect(cThread, SIGNAL(finished()), this, SLOT(scanThreadEnd()));
        connect(cThread, SIGNAL(finished()), Enumerator, SLOT(deleteLater()));
        connect(cThread, SIGNAL(finished()), cThread, SLOT(deleteLater()));
        connect(this, SIGNAL(stop()), Enumerator, SLOT(onStop()));
        //...
        cThread->start();
    }
    m_scanStatus->isRunning = true;
}

void Level::nextLevel(){
    ///
    /// first Clear the last level targetList of all previous subdomains...
    ///
    ui->listWidget_targets->clear();
    ///
    /// then copy the newly enumerated subdomains from results model to the
    /// new targetList...
    ///
    while(lastScanResultsCount < m_model_results->rowCount())
    {
        ui->listWidget_targets->addItem(m_model_results->item(lastScanResultsCount, 0)->text());
        lastScanResultsCount++;
    }
    ui->label_targetsCount->setNum(ui->listWidget_targets->count());
    ///
    /// Resetting the arguments....
    ///
    ui->progressBar->reset();
    m_scanArguments->progress = 0;
    m_scanArguments->currentTargetToEnumerate = 0;
    m_scanArguments->currentWordlistToEnumerate = 0;
    lastScanResultsCount = m_model_results->rowCount();
    ui->progressBar->setMaximum(ui->listWidget_targets->count()*ui->listWidget_wordlist->count());
    ///
    /// subdomain level check...
    ///
    m_scanArguments->currentLevel++;
    ui->label_level->setNum(m_scanArguments->currentLevel);
    ///
    /// start another scan on a higher level than previous scan...
    ///
    startScan();
    //...
    sendStatus("[*] Enumerating subdomains on Level: "+QString::number(m_scanArguments->currentLevel));
    logs("[*] Enumerating subdomains on Level: "+QString::number(m_scanArguments->currentLevel));
}

void Level::scanThreadEnd(){
    m_activeThreads--;
    if(m_activeThreads != 0)
    {
        return;
    }
    ///
    /// check if you've reached last level and if not start another scan to enumerate
    /// another level...
    ///
    if(m_scanStatus->isPaused)
    {
        ui->pushButton_pause->setText("Resume");
        m_scanStatus->isRunning = false;
        //...
        sendStatus("[*] Scan Paused!");
        logs("[*] Scan Paused!\n");
        return;
    }
    if(!m_scanStatus->isStopped && (m_scanArguments->currentLevel < m_scanArguments->maxLevel) && (lastScanResultsCount < m_model_results->rowCount()))
    {
        nextLevel();
    }
    else
    {
        ///
        /// Reached End of the scan on all levels or the scan was stopped...
        ///
        m_scanStatus->isPaused = false;
        m_scanStatus->isStopped = false;
        m_scanStatus->isRunning = false;
        //...
        ui->pushButton_start->setEnabled(true);
        ui->pushButton_stop->setDisabled(true);
        ui->pushButton_pause->setDisabled(true);
        //...
        sendStatus("[*] Enumeration by tldBrute Complete!");
        logs("[END] Multi-Level Subdomain Bruteforcing Complete!\n");
    }
}

void Level::on_toolButton_config_clicked(){
    ConfigDialog *scanConfig = new ConfigDialog(this, m_scanConfig);
    scanConfig->setAttribute( Qt::WA_DeleteOnClose, true );
    scanConfig->show();
}

/************************************** Results *****************************************/
void Level::scanResult(QString subdomain, QString ipAddress){
    m_model_results->setItem(m_model_results->rowCount(), 0, new QStandardItem(subdomain));
    m_model_results->setItem(m_model_results->rowCount()-1, 1, new QStandardItem(ipAddress));
    ui->label_resultsCount->setNum(m_model_results->rowCount());
}

void Level::on_pushButton_clearResults_clicked(){
    ///
    /// if the current tab is subdomains clear subdomains...
    ///
    if(ui->tabWidget_results->currentIndex() == 0)
    {
        m_model_results->clear();
        ui->label_resultsCount->clear();
        //...
        ui->progressBar->reset();
        ui->progressBar->hide();
        //...
        m_model_results->setHorizontalHeaderLabels({"Subdomain Name", "IpAddress"});
    }
    ///
    /// if the current tab is logs clear logs...
    ///
    else
    {
        ui->listWidget_logs->clear();
    }
}

/************************************** Subdomains **************************************/
void Level::on_pushButton_removeTargets_clicked(){
    int selectionCount = ui->listWidget_targets->selectedItems().count();
    if(selectionCount){
        qDeleteAll(ui->listWidget_targets->selectedItems());
    }
    ui->label_targetsCount->setNum(ui->listWidget_targets->count());
}

void Level::on_pushButton_clearTargets_clicked(){
    ui->listWidget_targets->clear();
    ui->label_targetsCount->clear();
}

void Level::on_pushButton_loadTargets_clicked(){
    QString filename = QFileDialog::getOpenFileName(this, INFO_LOADFILE, CURRENT_PATH);
    if(filename.isEmpty()){
        return;
    }
    QFile file(filename);
    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&file);
        while (!in.atEnd()){
            ui->listWidget_targets->addItem(in.readLine());
        }
        ui->label_targetsCount->setNum(ui->listWidget_targets->count());
        file.close();
    }
    else
    {
        QMessageBox::warning(this, TITLE_ERROR, "Failed To Open the File!");
    }
}

void Level::on_pushButton_addTargets_clicked(){
    if(ui->lineEdit_targets->text() != EMPTY)
    {
        ui->listWidget_targets->addItem(ui->lineEdit_targets->text());
        ui->lineEdit_targets->clear();
        ui->label_targetsCount->setNum(ui->listWidget_targets->count());
    }
}
void Level::on_lineEdit_targets_returnPressed(){
    on_pushButton_addTargets_clicked();
}

/************************************* Wordlist *****************************************/
void Level::on_pushButton_removeWordlist_clicked(){
    if(ui->listWidget_wordlist->selectedItems().count())
    {
        qDeleteAll(ui->listWidget_wordlist->selectedItems());
    }
    ui->label_wordlistCount->setNum(ui->listWidget_wordlist->count());
}

void Level::on_pushButton_clearWordlist_clicked(){
    ui->listWidget_wordlist->clear();
    ui->label_wordlistCount->clear();
}

void Level::on_pushButton_loadWordlist_clicked(){
    QString filename = QFileDialog::getOpenFileName(this, INFO_LOADFILE, CURRENT_PATH);
    if(!filename.isEmpty())
    {
        QFile file(filename);
        if(file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QTextStream in(&file);
            while (!in.atEnd())
            {
                ui->listWidget_wordlist->addItem(in.readLine());
            }
            ui->label_wordlistCount->setNum(ui->listWidget_wordlist->count());
            file.close();
        }else
        {
            QMessageBox::warning(this, TITLE_ERROR, "Failed To Open the File!");
        }
    }
}

void Level::on_pushButton_addWordlist_clicked(){
    if(ui->lineEdit_wordlist->text() != EMPTY)
    {
        ui->listWidget_wordlist->addItem(ui->lineEdit_wordlist->text());
        ui->lineEdit_wordlist->clear();
        ui->label_wordlistCount->setNum(ui->listWidget_wordlist->count());
    }
}
void Level::on_lineEdit_wordlist_returnPressed(){
    on_pushButton_addWordlist_clicked();
}

void Level::on_toolButton_wordlist_clicked(){
    WordListDialog *wordlistDialog = new WordListDialog(this, ENGINE::SUBBRUTE);
    wordlistDialog->setAttribute( Qt::WA_DeleteOnClose, true );
    connect(wordlistDialog, SIGNAL(choosenWordlist(QString)), this, SLOT(choosenWordlist(QString)));
    wordlistDialog->show();
}

void Level::choosenWordlist(QString wordlistFilename){
    QFile file(wordlistFilename);
    if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
        QTextStream in(&file);
        while (!in.atEnd()){
            ui->listWidget_wordlist->addItem(in.readLine());
        }
        file.close();
    }
    ui->label_wordlistCount->setNum(ui->listWidget_wordlist->count());
}

/******************************************** logs ******************************************/
void Level::logs(QString log){
    sendLog(log);
    ui->listWidget_logs->addItem(log);
    if (log.startsWith("[ERROR]"))
    {
        ui->listWidget_logs->item(ui->listWidget_logs->count()-1)->setForeground(Qt::red);
        return;
    }
    if(log.startsWith("[START]") || log.startsWith("[END]"))
    {
        ui->listWidget_logs->item(ui->listWidget_logs->count()-1)->setFont(QFont("MS Shell Dlg 2", 8, QFont::Bold));
        return;
    }
}

/********************************************************************************************
                                    Custom-Menus
*********************************************************************************************/
void Level::on_pushButton_action_clicked(){
    ///
    /// getting the position of the action button to place the context menu and
    /// showing the context menu right by the side of the action button...
    ///
    QPoint pos = ui->pushButton_action->mapToGlobal(QPoint(0,0));
    //...
    QMenu *contextMenu_actionButton = new QMenu(this);
    contextMenu_actionButton->setAttribute( Qt::WA_DeleteOnClose, true );
    contextMenu_actionButton->setObjectName("actionButtonMenu");
    //...
    QAction actionSendToSave("Send To Save", this);
    QAction actionSendToMultiLevel("Send To Multi-level Scan");
    QAction actionSendToDnsRecords("Send To DnsRecords");
    //...
    connect(&actionSendToSave, SIGNAL(triggered()), this, SLOT(actionSendToSave()));
    connect(&actionSendToDnsRecords, SIGNAL(triggered()), this, SLOT(actionSendToDnsRecords()));
    connect(&actionSendToMultiLevel, SIGNAL(triggered()), this, SLOT(actionSendToMultiLevel()));
    //...
    contextMenu_actionButton->addSeparator();
    contextMenu_actionButton->addAction(&actionSendToDnsRecords);
    contextMenu_actionButton->addAction(&actionSendToSave);
    contextMenu_actionButton->addAction(&actionSendToMultiLevel);
    //...
    contextMenu_actionButton->move(QPoint(pos.x()+76, pos.y()));
    contextMenu_actionButton->exec();
}

void Level::on_tableView_results_customContextMenuRequested(const QPoint &pos){
    Q_UNUSED(pos);
    ///
    /// check if user right clicked on items else dont show the context menu...
    ///
    if(!ui->tableView_results->selectionModel()->isSelected(ui->tableView_results->currentIndex())){
        return;
    }
    ///
    /// getting the position of the cursor to place the context menu...
    ///
    QPoint globalCursorPos = QCursor::pos();
    QRect mouseScreenGeometry = qApp->desktop()->screen(qApp->desktop()->screenNumber(globalCursorPos))->geometry();
    QPoint localCursorPosition = globalCursorPos - mouseScreenGeometry.topLeft();
    //...
    QMenu *contextMenu_rightClick = new QMenu(this);
    contextMenu_rightClick->setAttribute( Qt::WA_DeleteOnClose, true );
    contextMenu_rightClick->setObjectName("rightClickMenu");
    //...
    QAction actionSendToSave("Send Selected To Save", this);
    QAction actionSendToDnsRecords("Send Selected To DnsRecords");
    QAction actionOpenInBrowser("Open Selected in Browser");
    //...
    connect(&actionOpenInBrowser, SIGNAL(triggered()), this, SLOT(cursorOpenInBrowser()));
    connect(&actionSendToSave, SIGNAL(triggered()), this, SLOT(cursorSendToSave()));
    connect(&actionSendToDnsRecords, SIGNAL(triggered()), this, SLOT(cursorSendToDnsRecords()));
    //...
    contextMenu_rightClick->addAction(&actionOpenInBrowser);
    contextMenu_rightClick->addSeparator();
    contextMenu_rightClick->addAction(&actionSendToDnsRecords);
    contextMenu_rightClick->addAction(&actionSendToSave);
    //...
    contextMenu_rightClick->move(localCursorPosition);
    contextMenu_rightClick->exec();
}

/****************************** Action Context Menu Methods ***************************/
void Level::actionSendToSave(){
    /*
    int resultsCount = ui->listWidget_subdomains->count();
    for(int i = 0; i != resultsCount; ++i){
        emit sendResultsToSave(ui->listWidget_subdomains->item(i)->text());
    }
    logs("[*] Sent "+QString::number(resultsCount)+" subBrute Enumerated Subdomains To Save Tab...");
    emit changeTabToSave();
    */
}

/*********************************************************************************/
void Level::actionSendToDnsRecords(){

}

/**********************************************************************************/
void Level::actionSendToMultiLevel(){

}

/*************************** Cursor Right Click Context Menu ***********************/
void Level::cursorSendToSave(){
    /*foreach(const QModelIndex &index, ui->tableView_results->selectionModel()->selectedIndexes()){
        // send selection
    }
    logs("[*] Sent "+QString::number(ui->listWidget_subdomains->count())+" subBrute Enumerated Subdomains To Save Tab...");
    emit changeTabToSave();*/
}

/************************************************************************************/
void Level::cursorOpenInBrowser(){
    // iterate and open each selected item in a browser...
    foreach(const QModelIndex &index, ui->tableView_results->selectionModel()->selectedIndexes()){
        QDesktopServices::openUrl(QUrl("https://"+index.data().toString(), QUrl::TolerantMode));
    }
}


/*************************************************************************************/
void Level::cursorSendToDnsRecords(){

}
