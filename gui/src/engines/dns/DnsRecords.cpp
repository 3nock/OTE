#include "DnsRecords.h"
#include "ui_DnsRecords.h"


DnsRecords::DnsRecords(QWidget *parent, ResultsModel *resultsModel) : BaseClass(ENGINE::RECORDS, resultsModel, parent), ui(new Ui::DnsRecords),
      m_scanArguments(new records::ScanArguments),
      m_scanResults(new records::ScanResults)
{
    ui->setupUi(this);
    ///
    /// init...
    ///
    ui->targets->init("Targets");
    ui->srvWordlist->init("SRV");
    initBaseClass(ui->targets);
    scanConfig->name = tr("ScanConfig-Records");
    ///
    /// other initializations...
    ///
    ui->buttonStop->setDisabled(true);
    ui->buttonPause->setDisabled(true);
    //...
    ui->srvWordlist->hide();
    ui->progressBar->hide();
    ///
    /// equally seperate the widgets...
    ///
    ui->splitter->setSizes(QList<int>() << static_cast<int>((this->width() * 0.50))
                                        << static_cast<int>((this->width() * 0.50)));
    //...
    resultsModel->srvrecords->setHorizontalHeaderLabels({"Name", "Target", "Port"});
    //...
    ui->treeViewResults->setModel(resultsModel->dnsrecords);
    ui->tableViewSRV->setModel(resultsModel->srvrecords);
    //...
    m_scanArguments->targetList = ui->targets->listWidget;
    m_scanArguments->srvWordlist = ui->srvWordlist->listWidget;
    //...
    m_scanResults->resultsModel = resultsModel;
    m_scanResults->resultsCountLabel = ui->labelResultsCount;
    m_scanResults->srvResultsLabel = ui->labelResultsCountSRV;
    //...
    loadSrvWordlist();
}
DnsRecords::~DnsRecords(){
    delete m_scanArguments;
    delete m_scanResults;
    //...
    delete ui;
}

void DnsRecords::on_buttonStart_clicked(){
    ///
    /// checking if all requirements are satisfied before scan if not prompt error
    /// then exit function...
    ///
    if(ui->targets->listWidget->count() < 1){
        QMessageBox::warning(this, "Error!", "Please Enter Target Subdomains For Enumeration!");
        return;
    }
    if((ui->comboBoxOption->currentIndex() == OPTION::ALLRECORDS) && (!ui->checkBoxA->isChecked() && !ui->checkBoxAAAA->isChecked() && !ui->checkBoxMX->isChecked() && !ui->checkBoxNS->isChecked() && !ui->checkBoxTXT->isChecked() && !ui->checkBoxCNAME->isChecked())){
        QMessageBox::warning(this, "Error!", "Please Choose DNS Record To Enumerate!");
        return;
    }
    if((ui->comboBoxOption->currentIndex() == OPTION::SRV)&& (ui->srvWordlist->listWidget->count() < 1)){
        QMessageBox::warning(this, "Error!", "Please Enter SRV Wordlist For Enumeration!");
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
    m_scanArguments->currentSrvToEnumerate = 0;
    m_scanArguments->currentTargetToEnumerate = 0;
    ui->progressBar->reset();
    m_scanArguments->progress = 0;
    ///
    /// getting the arguments for Dns Records Scan...
    ///
    if(ui->comboBoxOption->currentIndex() == OPTION::ALLRECORDS)
    {
        m_scanArguments->RecordType_a = ui->checkBoxA->isChecked();
        m_scanArguments->RecordType_aaaa = ui->checkBoxAAAA->isChecked();
        m_scanArguments->RecordType_mx = ui->checkBoxMX->isChecked();
        m_scanArguments->RecordType_ns = ui->checkBoxNS->isChecked();
        m_scanArguments->RecordType_txt = ui->checkBoxTXT->isChecked();
        m_scanArguments->RecordType_cname = ui->checkBoxCNAME->isChecked();
        //...
        ui->progressBar->setMaximum(ui->targets->listWidget->count());
    }
    ///
    /// getting arguments for SRV DNS Records Scan...
    ///
    if(ui->comboBoxOption->currentIndex() == OPTION::SRV)
    {
        m_scanArguments->RecordType_srv = true;
        //...
        ui->progressBar->setMaximum(ui->targets->listWidget->count()*ui->srvWordlist->listWidget->count());
    }
    ///
    /// start Enumeration...
    ///
    startScan();
    //...
    sendStatus("[START] Started Subdomain Enumeration!");
    logs("[START] Started Subdomain Enumeration!");
}

void DnsRecords::on_buttonPause_clicked(){
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
        sendStatus("[START] Resumed Subdomain Enumeration!");
        logs("[START] Resumed Subdomain Enumeration!");
    }
    else
    {
        scanStatus->isPaused = true;
        emit stopScan();
    }
}

void DnsRecords::on_buttonStop_clicked(){
    emit stopScan();
    scanStatus->isStopped = true;
}

void DnsRecords::startScan(){
    ///
    /// if the numner of threads is greater than the number of wordlists, set the
    /// number of threads to use to the number of wordlists available to avoid
    /// creating more threads than needed...
    ///
    int wordlistCount = ui->targets->listWidget->count();
    int srvWordlistCount = ui->srvWordlist->listWidget->count();
    int threadsCount = scanConfig->threadsCount;
    if((ui->comboBoxOption->currentIndex() == OPTION::ALLRECORDS) && (threadsCount > wordlistCount))
    {
        threadsCount = wordlistCount;
    }
    if((ui->comboBoxOption->currentIndex() == OPTION::SRV) && (threadsCount > srvWordlistCount))
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
        if(ui->comboBoxOption->currentIndex() == OPTION::ALLRECORDS){
            scanner->startScan(cThread);
        }
        if(ui->comboBoxOption->currentIndex() == OPTION::SRV){
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
            ui->srvWordlist->add(in.readLine());
        }
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
            ui->buttonPause->setText("Resume");
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
            ui->buttonStart->setEnabled(true);
            ui->buttonPause->setDisabled(true);
            ui->buttonStop->setDisabled(true);
            //...
            sendStatus("[*] Enumeration Complete!");
            logs("[END] Enumeration Complete!\n");
        }
    }
}

void DnsRecords::on_buttonClear_clicked(){
    switch (ui->tabWidgetResults->currentIndex()){
        case 0:
            resultsModel->dnsrecords->clear();
            ui->labelResultsCount->clear();
            //...
            ui->progressBar->hide();
            break;
        //...
        case 1:
            resultsModel->srvrecords->clear();
            ui->labelResultsCountSRV->clear();
            resultsModel->srvrecords->setHorizontalHeaderLabels({"Name", "Target", "Port"});
            //...
            ui->progressBar->hide();
            break;
        //...
        case 2:
            ui->listWidgetLogs->clear();
            break;
    }
}

void DnsRecords::on_buttonConfig_clicked(){
    /*
    ScanConfig *bruteconfig = new ScanConfig(this, m_scanConfig, ENGINE::ACTIVE);
    bruteconfig->setAttribute( Qt::WA_DeleteOnClose, true );
    bruteconfig->show();
    */
}

void DnsRecords::on_comboBoxOption_currentIndexChanged(int index){
    if(index)
    {
        ui->srvWordlist->show();
        ui->frame_records->hide();
        ui->tabWidgetResults->setCurrentIndex(1);
    }
    else
    {
        ui->srvWordlist->hide();
        ui->frame_records->show();
        ui->tabWidgetResults->setCurrentIndex(0);
    }
}

void DnsRecords::on_buttonAction_clicked(){
    ///
    /// check if there are results available else dont show the context menu...
    ///
    if(resultsModel->dnsrecords->rowCount() < 1 && resultsModel->srvrecords->rowCount() < 1){
        return;
    }
    ///
    /// getting the position of the action button to place the context menu and
    /// showing the context menu right by the side of the action button...
    ///
    QPoint pos = ui->buttonAction->mapToGlobal(QPoint(0,0));
    if(ui->tabWidgetResults->currentIndex()){
        a_Menu->exec(QPoint(pos.x()+76, pos.y()));
    }
    else{
        a_Menu->exec(QPoint(pos.x()+76, pos.y()));
    }
}

void DnsRecords::on_treeViewResults_customContextMenuRequested(const QPoint &pos){
    Q_UNUSED(pos)
    if(!ui->treeViewResults->selectionModel()->isSelected(ui->treeViewResults->currentIndex())){
        return;
    }
    selectionModel = ui->treeViewResults->selectionModel();
    c_Menu->exec(QCursor::pos());
}

void DnsRecords::on_tableViewSRV_customContextMenuRequested(const QPoint &pos){
    Q_UNUSED(pos)
    if(!ui->tableViewSRV->selectionModel()->isSelected(ui->tableViewSRV->currentIndex())){
        return;
    }
    selectionModel = ui->tableViewSRV->selectionModel();
    c_Menu->exec(QCursor::pos());
}
