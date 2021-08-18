#include "DnsRecords.h"
#include "ui_DnsRecords.h"


/*************************************************************************************
                            Constructor & Destructor
*************************************************************************************/

DnsRecords::DnsRecords(QWidget *parent, ResultsModel *resultsModel) : BaseClass(parent, resultsModel), ui(new Ui::DnsRecords),
      m_scanArguments(new records::ScanArguments),
      m_scanResults(new records::ScanResults)
{
    ui->setupUi(this);
    ///
    /// setting up targets widgets to the base class...
    ///
    widgets->listWidget_targets = ui->listWidget_targets;
    widgets->label_targetsCount = ui->label_targetsCount;
    widgets->lineEdit_targetInput = ui->lineEdit_targets;
    widgets->listWidget_logs = ui->listWidget_logs;
    ///
    /// other initializations...
    ///
    ui->lineEdit_targets->setPlaceholderText("Enter Target eg. example.com...");
    ui->lineEdit_srvWordlist->setPlaceholderText("Enter new SRV...");
    //...
    ui->pushButton_stop->setDisabled(true);
    ui->pushButton_pause->setDisabled(true);
    //...
    ui->frame_srvWordlist->hide();
    ui->progressBar->hide();
    ///
    /// equally seperate the widgets...
    ///
    ui->splitter->setSizes(QList<int>() << static_cast<int>((this->width() * 0.50))
                                        << static_cast<int>((this->width() * 0.50)));
    //...
    resultsModel->records->model_srv->setHorizontalHeaderLabels({"Name", "Target", "Port"});
    //...
    ui->treeView_results->setModel(resultsModel->records->model_records);
    ui->tableView_srv->setModel(resultsModel->records->model_srv);
    //...
    m_scanArguments->targetList = ui->listWidget_targets;
    m_scanArguments->srvWordlist = ui->listWidget_srvWordlist;
    //...
    m_scanResults->resultsModel = resultsModel;
    m_scanResults->resultsCountLabel = ui->label_resultsCount;
    m_scanResults->srvResultsLabel = ui->label_srvResultsCount;
    //...
    loadSrvWordlist();
}
DnsRecords::~DnsRecords(){
    delete m_scanArguments;
    delete m_scanResults;
    //...
    delete ui;
}

/**************************************************************************************
                                      Scan
**************************************************************************************/
void DnsRecords::on_pushButton_start_clicked(){
    ///
    /// checking if all requirements are satisfied before scan if not prompt error
    /// then exit function...
    ///
    if(ui->listWidget_targets->count() < 1){
        QMessageBox::warning(this, TITLE_ERROR, "Please Enter Target Subdomains For Enumeration!");
        return;
    }
    if((ui->comboBox_option->currentIndex() == OPTION::ALLRECORDS) && (!ui->checkBox_A->isChecked() && !ui->checkBox_AAAA->isChecked() && !ui->checkBox_MX->isChecked() && !ui->checkBox_NS->isChecked() && !ui->checkBox_TXT->isChecked() && !ui->checkBox_CNAME->isChecked())){
        QMessageBox::warning(this, TITLE_ERROR, "Please Choose DNS Record To Enumerate!");
        return;
    }
    if((ui->comboBox_option->currentIndex() == OPTION::SRV)&& (ui->listWidget_srvWordlist->count() < 1)){
        QMessageBox::warning(this, TITLE_ERROR, "Please Enter SRV Wordlist For Enumeration!");
        return;
    }
    ///
    /// disabling & Enabling widgets...
    ///
    ui->pushButton_start->setDisabled(true);
    ui->pushButton_pause->setEnabled(true);
    ui->pushButton_stop->setEnabled(true);
    ui->progressBar->show();
    ///
    /// Resetting the scan arguments values...
    ///
    m_scanArguments->currentSrvToEnumerate = 0;
    m_scanArguments->currentTargetToEnumerate = 0;
    ui->progressBar->reset();
    m_scanArguments->progress = 0;
    ///
    /// getting the arguments for Dns Records Scan...
    ///
    if(ui->comboBox_option->currentIndex() == OPTION::ALLRECORDS)
    {
        m_scanArguments->RecordType_a = ui->checkBox_A->isChecked();
        m_scanArguments->RecordType_aaaa = ui->checkBox_AAAA->isChecked();
        m_scanArguments->RecordType_mx = ui->checkBox_MX->isChecked();
        m_scanArguments->RecordType_ns = ui->checkBox_NS->isChecked();
        m_scanArguments->RecordType_txt = ui->checkBox_TXT->isChecked();
        m_scanArguments->RecordType_cname = ui->checkBox_CNAME->isChecked();
        //...
        ui->progressBar->setMaximum(ui->listWidget_targets->count());
    }
    ///
    /// getting arguments for SRV DNS Records Scan...
    ///
    if(ui->comboBox_option->currentIndex() == OPTION::SRV)
    {
        m_scanArguments->RecordType_srv = true;
        //...
        ui->progressBar->setMaximum(ui->listWidget_targets->count()*ui->listWidget_srvWordlist->count());
    }
    ///
    /// start Enumeration...
    ///
    startScan();
    //...
    sendStatus("[START] Started Subdomain Enumeration!");
    logs("[START] Started Subdomain Enumeration!");
}

void DnsRecords::on_pushButton_pause_clicked(){
    ///
    /// if the scan was already paused, then this current click is to
    /// Resume the scan, just call the startScan, with the same arguments and
    /// it will continue at where it ended...
    ///
    if(scanStatus->isPaused)
    {
        ui->pushButton_pause->setText("Pause");
        scanStatus->isPaused = false;
        //...
        startScan();
        //...
        sendStatus("[START] Resumed Subdomain Enumeration!");
        logs("[START] Resumed Subdomain Enumeration!");
    }
    else
    {
        scanStatus->isPaused = true;
        emit stopScan();
    }
}

void DnsRecords::on_pushButton_stop_clicked(){
    emit stopScan();
    scanStatus->isStopped = true;
}

/**************************************** Enumerators *************************************/
void DnsRecords::startScan(){
    ///
    /// if the numner of threads is greater than the number of wordlists, set the
    /// number of threads to use to the number of wordlists available to avoid
    /// creating more threads than needed...
    ///
    int wordlistCount = ui->listWidget_targets->count();
    int srvWordlistCount = ui->listWidget_srvWordlist->count();
    int threadsCount = scanConfig->threadsCount;
    if((ui->comboBox_option->currentIndex() == OPTION::ALLRECORDS) && (threadsCount > wordlistCount))
    {
        threadsCount = wordlistCount;
    }
    if((ui->comboBox_option->currentIndex() == OPTION::SRV) && (threadsCount > srvWordlistCount))
    {
        threadsCount = wordlistCount;
    }
    activeThreads = threadsCount;
    ///
    /// loop to create threads for scan...
    ///
    for(int i = 0; i < threadsCount; i++)
    {
        records::Scanner *scanner = new records::Scanner(scanConfig, m_scanArguments, m_scanResults);
        QThread *cThread = new QThread(this);
        //...
        if(ui->comboBox_option->currentIndex() == OPTION::ALLRECORDS){
            scanner->startScan(cThread);
        }
        if(ui->comboBox_option->currentIndex() == OPTION::SRV){
            scanner->startScan_srv(cThread);
        }
        scanner->moveToThread(cThread);
        //...
        connect(scanner, SIGNAL(scanLog(QString)), this, SLOT(logs(QString)));
        connect(scanner, SIGNAL(scanProgress(int)), ui->progressBar, SLOT(setValue(int)));
        connect(cThread, SIGNAL(finished()), this, SLOT(scanThreadEnded()));
        connect(cThread, SIGNAL(finished()), scanner, SLOT(deleteLater()));
        connect(cThread, SIGNAL(finished()), cThread, SLOT(deleteLater()));
        connect(this, SIGNAL(stopScan()), scanner, SLOT(stopScan()));
        //...
        cThread->start();
    }
    scanStatus->isRunning = true;
}

void DnsRecords::loadSrvWordlist(){
    QFile file(":/files/res/files/srv.txt");
    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&file);
        while (!in.atEnd()){
            ui->listWidget_srvWordlist->addItem(in.readLine());
        }
        ui->label_srvWordlistCount->setNum(ui->listWidget_srvWordlist->count());
        file.close();
    }
}

void DnsRecords::scanThreadEnded(){
    activeThreads--;
    ///
    /// if all Scan Threads have finished...
    ///
    if(activeThreads == 0)
    {
        if(scanStatus->isPaused)
        {
            ui->pushButton_pause->setText("Resume");
            scanStatus->isRunning = false;
            //...
            sendStatus("[*] Scan Paused!");
            logs("[*] Scan Paused!\n");
            return;
        }
        else
        {
            // set the progress bar to 100% just in case...
            if(!scanStatus->isStopped){
                ui->progressBar->setValue(ui->progressBar->maximum());
            }
            scanStatus->isPaused = false;
            scanStatus->isStopped = false;
            scanStatus->isRunning = false;
            //...
            ui->pushButton_start->setEnabled(true);
            ui->pushButton_pause->setDisabled(true);
            ui->pushButton_stop->setDisabled(true);
            //...
            sendStatus("[*] Enumeration Complete!");
            logs("[END] Enumeration Complete!\n");
        }
    }
}

/*******************************************************************************************
                                    Results Processing
********************************************************************************************/

void DnsRecords::on_pushButton_clearResults_clicked(){
    switch (ui->tabWidget_results->currentIndex()){
        case 0:
            resultsModel->records->model_records->clear();
            ui->label_resultsCount->clear();
            resultsModel->records->rootItem = resultsModel->records->model_records->invisibleRootItem();
            //...
            ui->progressBar->hide();
            break;
        //...
        case 1:
            resultsModel->records->model_srv->clear();
            ui->label_srvResultsCount->clear();
            resultsModel->records->model_srv->setHorizontalHeaderLabels({"Name", "Target", "Port"});
            //...
            ui->progressBar->hide();
            break;
        //...
        case 2:
            ui->listWidget_logs->clear();
            break;
    }
}

void DnsRecords::on_toolButton_config_clicked(){
    /*
    ScanConfig *bruteconfig = new ScanConfig(this, m_scanConfig, ENGINE::ACTIVE);
    bruteconfig->setAttribute( Qt::WA_DeleteOnClose, true );
    bruteconfig->show();
    */
}

/**************************************************************************************/
void DnsRecords::on_comboBox_option_currentIndexChanged(int index){
    if(index)
    {
        ui->frame_srvWordlist->show();
        ui->frame_records->hide();
        ui->tabWidget_results->setCurrentIndex(1);
    }
    else
    {
        ui->frame_srvWordlist->hide();
        ui->frame_records->show();
        ui->tabWidget_results->setCurrentIndex(0);
    }
}

/**************************************************************************************
                          Targets for Scan
***************************************************************************************/

void DnsRecords::on_pushButton_removeTargets_clicked(){
    removeTargets();
}

void DnsRecords::on_pushButton_clearTargets_clicked(){
    clearTargets();
}

void DnsRecords::on_pushButton_loadTargets_clicked(){
    loadTargetsFromFile();
}

void DnsRecords::on_pushButton_addTargets_clicked(){
    addTargets();
}
void DnsRecords::on_lineEdit_targets_returnPressed(){
    addTargets();
}

/************************************* SRV WORDLIST ****************************************/
void DnsRecords::on_pushButton_removeSrvWordlist_clicked(){
    if(ui->listWidget_srvWordlist->selectedItems().count()){
        qDeleteAll(ui->listWidget_srvWordlist->selectedItems());
    }
    ui->label_srvWordlistCount->setNum(ui->listWidget_srvWordlist->count());
}

void DnsRecords::on_pushButton_clearSrvWordlist_clicked(){
    ui->listWidget_srvWordlist->clear();
    ui->label_srvWordlistCount->clear();
}

void DnsRecords::on_pushButton_loadSrvWordlist_clicked(){
    QString filename = QFileDialog::getOpenFileName(this, INFO_LOADFILE, CURRENT_PATH);
    if(!filename.isEmpty()){
        QFile file(filename);
        if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
            QTextStream in(&file);
            while (!in.atEnd()){
                ui->listWidget_srvWordlist->addItem(in.readLine());
            }
            ui->label_srvWordlistCount->setNum(ui->listWidget_srvWordlist->count());
            file.close();
        }else{
            QMessageBox::warning(this, TITLE_ERROR, "Failed To Open the File!");
        }
    }
}

void DnsRecords::on_pushButton_addSrvWordlist_clicked(){
    if(ui->lineEdit_srvWordlist->text() != EMPTY){
        ui->listWidget_srvWordlist->addItem(ui->lineEdit_srvWordlist->text());
        ui->lineEdit_srvWordlist->clear();
        ui->label_srvWordlistCount->setNum(ui->listWidget_srvWordlist->count());
    }
}

void DnsRecords::on_lineEdit_srvWordlist_returnPressed(){
    on_pushButton_addSrvWordlist_clicked();
}

/********************************* action Button context Menu ***********************************/
void DnsRecords::on_pushButton_action_clicked(){
    ///
    /// check if there are results available else dont show the context menu...
    ///
    if(resultsModel->records->model_records->rowCount() < 1 && resultsModel->records->model_srv->rowCount() < 1){
        return;
    }
    ///
    /// getting the position of the action button to place the context menu and
    /// showing the context menu right by the side of the action button...
    ///
    QPoint pos = ui->pushButton_action->mapToGlobal(QPoint(0,0));
    if(ui->tabWidget_results->currentIndex()){
        contextMenu_actionButton(ENGINE::SRVRECORDS, pos);
    }
    else{
        contextMenu_actionButton(ENGINE::DNSRECORDS, pos);
    }
}

/********************************* right click Context Menu ***************************************/
void DnsRecords::on_treeView_results_customContextMenuRequested(const QPoint &pos){
    Q_UNUSED(pos)
    if(!ui->treeView_results->selectionModel()->isSelected(ui->treeView_results->currentIndex())){
        return;
    }
    contextMenu_rightClick(ui->treeView_results->selectionModel());
}

void DnsRecords::on_tableView_srv_customContextMenuRequested(const QPoint &pos){
    Q_UNUSED(pos)
    if(!ui->tableView_srv->selectionModel()->isSelected(ui->tableView_srv->currentIndex())){
        return;
    }
    contextMenu_rightClick(ui->tableView_srv->selectionModel());
}
