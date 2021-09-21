#include "Brute.h"
#include "ui_Brute.h"

/*
 *have a tab to put all info/issue definitions,eg asn numbers and their respective
 * owner, ip's and their respective owners...
 *
 * change logs from listwidget to plaintext edit
 * and input from listwidget to listview
 *
 **
 fix filter from what you have learned, it shud not be asocciated with scope...

 check if connected to the internet...
 check if host is active...
 pop an alert to user if wants cont...
 Use QNetworkConfigurationManager::isOnline().

*/
/*
 create a scrapper tab: use c++...
*/


/*

  add an advanced option that resolves an item on wordlist with all nameservers..
  first obtains the server from the domain name, then uses those nameservers as default nameservers
  for the scan...

*/

/*

  add a function to first test if the domain you are testing is an active one
  to reduce un-necessary lookups

*/

/*

 create a notes tab that takes all notes for the related scans of the project...
 each new domain enumerated and its x-stics are to be recorded...
 use of wildcard in each phase...
*/

/*
 * have seperate stylesheet for each os, choose appropriate stylesheet during loading time
 * using macro which indicates what os is compiled in for better visuals...
 *
 * must be project base, on launch asks if you want a quick scan or launch a project base...
 *
 */

Brute::Brute(QWidget *parent, ResultsModel *resultsModel, Status *status) :
    BaseClass(ENGINE::BRUTE, resultsModel, status, parent),
    ui(new Ui::Brute),
    m_scanArguments(new brute::ScanArguments)
{
    ui->setupUi(this);
    ///
    /// init...
    ///
    ui->targets->init("Targets");
    ui->wordlist->init("Wordlist");
    initBaseClass(ui->targets);
    scanConfig->name = tr("ScanConfig-Brute");
    ///
    /// ...
    ///
    ui->lineEditTarget->setPlaceholderText("eg. example.com");
    ///
    /// ...
    ///
    ui->buttonStop->setDisabled(true);
    ui->buttonPause->setDisabled(true);
    //...
    ui->targets->hide();
    ui->progressBar->hide();
    ///
    /// equally seperate the widgets...
    ///
    ui->splitter->setSizes(QList<int>() << static_cast<int>((this->width() * 0.50))
                                        << static_cast<int>((this->width() * 0.50)));
    //...
    resultsModel->brute->setHorizontalHeaderLabels({"Subdomain", "IpAddress"});
    ui->tableViewResults->setModel(resultsModel->brute);
    //...
    m_scanArguments->wordlist = ui->wordlist->listWidget;
}
Brute::~Brute(){
    delete m_scanArguments;
    //...
    delete ui;
}

void Brute::on_buttonStart_clicked(){
    ///
    /// checking if all requirements are satisfied before scan if not prompt error
    /// then exit function...
    ///
    if(!ui->checkBoxMultipleTargets->isChecked() && ui->lineEditTarget->text().isEmpty()){
        QMessageBox::warning(this, "Error!", "Please Enter the Target for Enumeration!");
        return;
    }
    if(ui->checkBoxMultipleTargets->isChecked() && ui->targets->listWidget->count() < 1){
        QMessageBox::warning(this, "Error!", "Please Enter the Targets for Enumeration!");
        return;
    }
    if(ui->wordlist->listWidget->count() < 1){
        QMessageBox::warning(this, "Error!", "Please Enter the Wordlist for Enumeration!");
        return;
    }
    ///
    /// disabling & Enabling widgets...
    ///
    ui->buttonStart->setDisabled(true);
    ui->buttonPause->setEnabled(true);
    ui->buttonStop->setEnabled(true);
    ui->progressBar->show();
    ///
    /// Resetting the scan arguments values...
    ///
    m_scanArguments->currentWordlistToEnumerate = 0;
    m_scanArguments->currentTargetToEnumerate = 0;
    m_scanArguments->targetList.clear();
    ui->progressBar->reset();
    m_scanArguments->progress = 0;

    /****** Processing targets if user chooses subdomain bruteForcing *******/
    if(ui->comboBoxBruteType->currentIndex() == 0)
    {
        m_scanArguments->subBrute = true;
        m_scanArguments->tldBrute = false;

        if(!ui->checkBoxMultipleTargets->isChecked())
        {
            m_scanArguments->targetList.append(TargetNameFilter(ui->lineEditTarget->text(), ENGINE::SUBBRUTE));
            ///
            /// for a single target, progress equals to the total number of wordlist...
            ///
            ui->progressBar->setMaximum(ui->wordlist->listWidget->count());
        }
        if(ui->checkBoxMultipleTargets->isChecked())
        {
            for(int i = 0; i < ui->targets->listWidget->count(); i++)
            {
                m_scanArguments->targetList.append(TargetNameFilter(ui->targets->listWidget->item(i)->text(), ENGINE::SUBBRUTE));
            }
            ///
            /// for multiple targets, progress equals to the total number of wordlist times the total number of targets...
            ///
            ui->progressBar->setMaximum(ui->wordlist->listWidget->count()*m_scanArguments->targetList.count());
        }
    }

    /****** Processing targets if user chooses TLD bruteForcing *******/
    if(ui->comboBoxBruteType->currentIndex() == 1)
    {
        m_scanArguments->tldBrute = true;
        m_scanArguments->subBrute = false;

        if(!ui->checkBoxMultipleTargets->isChecked())
        {
            m_scanArguments->targetList.append(TargetNameFilter(ui->lineEditTarget->text(), ENGINE::TLDBRUTE));
            ///
            /// for a single target, progress equals to the total number of wordlist...
            ///
            ui->progressBar->setMaximum(ui->wordlist->listWidget->count());
        }
        if(ui->checkBoxMultipleTargets->isChecked())
        {
            for(int i = 0; i < ui->targets->listWidget->count(); i++)
            {
                m_scanArguments->targetList.append(TargetNameFilter(ui->targets->listWidget->item(i)->text(), ENGINE::TLDBRUTE));
            }
            ///
            /// for multiple targets, progress equals to the total number of wordlist times the total number of targets...
            ///
            ui->progressBar->setMaximum(ui->wordlist->listWidget->count()*m_scanArguments->targetList.count());
        }
    }
    ///
    /// Starting the scan...
    ///
    startScan();
    //...
    sendStatus("[START] Started Subdomain Enumeration!");
    logs("[START] Started Subdomain Enumeration!");
}
void Brute::on_lineEditTarget_returnPressed(){
    on_buttonStart_clicked();
}

void Brute::on_buttonPause_clicked(){
    ///
    /// if the scan was already paused, then this current click is to
    /// Resume the scan, just call the startScan, with the same arguments and
    /// it will continue at where it ended...
    ///
    if(status->brute->isPaused){
        ui->buttonPause->setText("Pause");
        status->brute->isPaused = false;
        //...
        startScan();
        //...
        sendStatus("[START] Resumed Subdomain Enumeration!");
        logs("[START] Resumed Subdomain Enumeration!");
    }
    else
    {
        status->brute->isPaused = true;
        emit stopScan();
    }
}

void Brute::on_buttonStop_clicked(){
    emit stopScan();
    if(status->brute->isPaused)
    {
        m_scanArguments->targetList.clear();
        status->brute->isPaused = false;
        status->brute->isStopped = false;
        status->brute->isRunning = false;
        //...
        ui->buttonStart->setEnabled(true);
        ui->buttonPause->setDisabled(true);
        ui->buttonStop->setDisabled(true);
        //...
        sendStatus("[*] Enumeration Complete!");
        logs("[END] Enumeration Complete!\n");
    }
    status->brute->isStopped = true;
}

void Brute::startScan(){
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
    /// loop to create threads for scan...
    ///
    for(int i = 0; i < threadsCount; i++)
    {
        QThread *cThread = new QThread;
        brute::Scanner *scanner = new brute::Scanner(scanConfig, m_scanArguments);
        scanner->startScan(cThread);
        scanner->moveToThread(cThread);
        //...
        connect(scanner, SIGNAL(scanResult(QString, QString, QString)), this, SLOT(scanResult(QString, QString, QString)));
        connect(scanner, SIGNAL(scanProgress(int)), ui->progressBar, SLOT(setValue(int)));
        connect(scanner, SIGNAL(scanLog(QString)), this, SLOT(logs(QString)));
        connect(cThread, SIGNAL(finished()), this, SLOT(scanThreadEnded()));
        connect(cThread, SIGNAL(finished()), scanner, SLOT(deleteLater()));
        connect(cThread, SIGNAL(finished()), cThread, SLOT(deleteLater()));
        connect(this, SIGNAL(stopScan()), scanner, SLOT(stopScan()));
        //...
        cThread->start();
    }
    status->brute->isRunning = true;
}

void Brute::scanResult(QString subdomain, QString ipAddress, QString target){
    if(m_subdomainsSet.contains(subdomain)){
        return;
    }
    m_subdomainsSet.insert(subdomain);
    ///
    /// save to brute model...
    ///
    resultsModel->brute->appendRow(QList<QStandardItem*>() <<new QStandardItem(subdomain) <<new QStandardItem(ipAddress));
    ui->labelResultsCount->setNum(resultsModel->brute->rowCount());
    ///
    /// save to Project model...
    ///
    if(m_scanArguments->tldBrute){
        resultsModel->project->addTLD(QStringList()<<subdomain<<ipAddress<<target);
    }
    if(m_scanArguments->subBrute){
        resultsModel->project->addSubdomain(QStringList()<<subdomain<<ipAddress<<target);
    }
}

void Brute::scanThreadEnded(){
    activeThreads--;
    ///
    /// if all Scan Threads have finished...
    ///
    if(activeThreads == 0)
    {
        if(status->brute->isPaused)
        {
            ui->buttonPause->setText("Resume");
            status->brute->isRunning = false;
            //...
            sendStatus("[*] Scan Paused!");
            logs("[*] Scan Paused!\n");
            return;
        }
        else
        {
            // set the progress bar to 100% just in case...
            if(!status->brute->isStopped){
                ui->progressBar->setValue(ui->progressBar->maximum());
            }
            m_scanArguments->targetList.clear();
            status->brute->isPaused = false;
            status->brute->isStopped = false;
            status->brute->isRunning = false;
            //...
            ui->buttonStart->setEnabled(true);
            ui->buttonPause->setDisabled(true);
            ui->buttonStop->setDisabled(true);
            //...
            sendStatus("[*] Enumeration Complete!");
            logs("[END] Enumeration Complete!\n");
        }
    }
}

void Brute::on_buttonConfig_clicked(){
    ConfigDialog *configDialog = new ConfigDialog(this, scanConfig);
    configDialog->setAttribute( Qt::WA_DeleteOnClose, true );
    configDialog->show();
}

void Brute::on_buttonClearResults_clicked(){
    ///
    /// if the current tab is subdomains clear subdomains...
    ///
    if(ui->tabWidgetResults->currentIndex() == 0)
    {
        m_subdomainsSet.clear();
        resultsModel->brute->clear();
        ui->labelResultsCount->clear();
        //...
        ui->progressBar->reset();
        ui->progressBar->hide();
        //...
        resultsModel->brute->setHorizontalHeaderLabels({"Subdomain Name", "IpAddress"});
    }
    ///
    /// if the current tab is logs clear logs...
    ///
    else
    {
        ui->listWidgetLogs->clear();
    }
}

void Brute::on_buttonWordlist_clicked(){
    WordListDialog *wordlistDialog = nullptr;
    if(ui->comboBoxBruteType->currentIndex() == 0){
        wordlistDialog = new WordListDialog(this, ENGINE::SUBBRUTE);
    }
    if(ui->comboBoxBruteType->currentIndex() == 1){
        wordlistDialog = new WordListDialog(this, ENGINE::TLDBRUTE);
    }
    wordlistDialog->setAttribute( Qt::WA_DeleteOnClose, true );
    //...
    connect(wordlistDialog, SIGNAL(choosenWordlist(QString)), this, SLOT(choosenWordlist(QString)));
    wordlistDialog->show();
}

void Brute::choosenWordlist(QString wordlistFilename){
    QFile file(wordlistFilename);
    ui->wordlist->add(file);
}

void Brute::on_buttonAction_clicked(){
    ///
    /// check if there are results available else dont show the context menu...
    ///
    if(resultsModel->brute->rowCount() < 1){
        return;
    }
    ///
    /// getting the position of the action button to place the context menu and
    /// showing the context menu right by the side of the action button...
    ///
    QPoint pos = ui->buttonAction->mapToGlobal(QPoint(0,0));
    a_Menu->exec(QPoint(pos.x()+76, pos.y()));
}

void Brute::on_tableViewResults_customContextMenuRequested(const QPoint &pos){
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

void Brute::on_checkBoxMultipleTargets_clicked(bool checked){
    if(checked)
        ui->targets->show();
    else
        ui->targets->hide();
}
