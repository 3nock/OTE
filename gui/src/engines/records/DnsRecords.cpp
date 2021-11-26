#include "DnsRecords.h"
#include "ui_DnsRecords.h"
//...
#include <QDateTime>
#include <QClipboard>


DnsRecords::DnsRecords(QWidget *parent, ResultsModel *resultsModel, ProjectDataModel *project, Status *status) :
    AbstractEngine(parent, resultsModel, project, status),
    ui(new Ui::DnsRecords),
    m_scanArguments(new records::ScanArguments)
{
    ui->setupUi(this);
    ///
    /// init...
    ///
    ui->targets->init("Targets");
    ui->srvWordlist->init("SRV");
    targets = ui->targets;
    scanConfig->name = tr("ScanConfig-Records");
    ///
    /// other initializations...
    ///
    ui->buttonStop->setDisabled(true);
    //ui->buttonPause->setDisabled(true);
    //...
    ui->srvWordlist->hide();
    ui->progressBar->hide();
    ui->progressBarSRV->hide();
    //...
    ui->lineEditFilter->hide();
    ui->buttonFilter->hide();
    ui->comboBoxFilter->hide();
    ui->lineEditFilter->setPlaceholderText("Enter filter...");
    ///
    /// equsubdomainIpy seperate the widgets...
    ///
    ui->splitter->setSizes(QList<int>() << static_cast<int>((this->width() * 0.50))
                                        << static_cast<int>((this->width() * 0.50)));
    //...
    result->records->dns->setHorizontalHeaderLabels({"Name", "Target", "Port"});
    //...
    ui->treeViewResults->setModel(result->records->dns);
    ui->tableViewSRV->setModel(result->records->srv);
    //...
    this->loadSrvWordlist();
    this->connectActions();
    //...
    qRegisterMetaType<records::Results>("records::Results");
    result->records->dnsProxy->setFilterCaseSensitivity(Qt::CaseInsensitive);
    result->records->dnsProxy->setRecursiveFilteringEnabled(true);
    result->records->srvProxy->setFilterCaseSensitivity(Qt::CaseInsensitive);
    result->records->srvProxy->setRecursiveFilteringEnabled(true);
}
DnsRecords::~DnsRecords(){
    delete m_scanArguments;
    delete ui;
}

void DnsRecords::onInfoLog(QString log){
    QString logTime = QDateTime::currentDateTime().toString("hh:mm:ss  ");
    ui->plainTextEditLogs->appendPlainText(logTime.append(log));
}

void DnsRecords::onErrorLog(QString log){
    QString fontedLog;
    fontedLog.append("<font color=\"red\">").append(log).append("</font>");
    QString logTime = QDateTime::currentDateTime().toString("hh:mm:ss  ");
    ui->plainTextEditLogs->appendHtml(logTime.append(fontedLog));
}

void DnsRecords::on_buttonStart_clicked(){
    ///
    /// checking if subdomainIp requirements are satisfied before scan if not prompt error
    /// then exit function...
    ///
    if(ui->targets->listModel->rowCount() < 1){
        QMessageBox::warning(this, "Error!", "Please Enter Target Subdomains For Enumeration!");
        return;
    }
    if((ui->comboBoxOption->currentIndex() == OPTION::ALLRECORDS) && (!ui->checkBoxA->isChecked() && !ui->checkBoxAAAA->isChecked() && !ui->checkBoxMX->isChecked() && !ui->checkBoxNS->isChecked() && !ui->checkBoxTXT->isChecked() && !ui->checkBoxCNAME->isChecked())){
        QMessageBox::warning(this, "Error!", "Please Choose DNS Record To Enumerate!");
        return;
    }
    if((ui->comboBoxOption->currentIndex() == OPTION::SRV)&& (ui->srvWordlist->listModel->rowCount() < 1)){
        QMessageBox::warning(this, "Error!", "Please Enter SRV Wordlist For Enumeration!");
        return;
    }
    ///
    /// disabling & Enabling widgets...
    ///
    ui->buttonStart->setDisabled(true);
    //ui->buttonPause->setEnabled(true);
    ui->buttonStop->setEnabled(true);
    ///
    /// Resetting the scan arguments values...
    ///
    m_scanArguments->targetList = ui->targets->listModel->stringList();
    m_scanArguments->srvWordlist = ui->srvWordlist->listModel->stringList();
    m_scanArguments->currentSrvToEnumerate = 0;
    m_scanArguments->currentTargetToEnumerate = 0;
    ///
    /// getting the arguments for Dns Records Scan...
    ///
    if(ui->comboBoxOption->currentIndex() == OPTION::ALLRECORDS)
    {
        ui->progressBar->show();
        ui->progressBar->reset();
        m_scanArguments->progress = 0;
        //...
        m_scanArguments->RecordType_srv = false;
        m_scanArguments->RecordType_a = ui->checkBoxA->isChecked();
        m_scanArguments->RecordType_aaaa = ui->checkBoxAAAA->isChecked();
        m_scanArguments->RecordType_mx = ui->checkBoxMX->isChecked();
        m_scanArguments->RecordType_ns = ui->checkBoxNS->isChecked();
        m_scanArguments->RecordType_txt = ui->checkBoxTXT->isChecked();
        m_scanArguments->RecordType_cname = ui->checkBoxCNAME->isChecked();
        //...
        ui->progressBar->setMaximum(ui->targets->listModel->rowCount());
    }
    ///
    /// getting arguments for SRV DNS Records Scan...
    ///
    if(ui->comboBoxOption->currentIndex() == OPTION::SRV)
    {
        ui->progressBarSRV->show();
        ui->progressBarSRV->reset();
        m_scanArguments->progress = 0;
        m_scanArguments->RecordType_srv = true;
        ui->progressBarSRV->setMaximum(ui->targets->listModel->rowCount()*ui->srvWordlist->listModel->rowCount());
    }
    ///
    /// start Enumeration...
    ///
    startScan();
    //...
    sendStatus("[START] Started Subdomain Enumeration!");
}

void DnsRecords::stopScan(){

}

void DnsRecords::pauseScan(){
    ///
    /// if the scan was already paused, then this current click is to
    /// Resume the scan, just csubdomainIp the startScan, with the same arguments and
    /// it will continue at where it ended...
    ///
    if(status->records->isPaused)
    {
        //ui->buttonPause->setText("Pause");
        status->records->isPaused = false;
        //...
        startScan();
        //...
        sendStatus("[START] Resumed Subdomain Enumeration!");
    }
    else
    {
        status->records->isPaused = true;
        emit stopScan();
    }
}

void DnsRecords::ResumeScan(){

}

void DnsRecords::on_buttonStop_clicked(){
    emit stopScan();
    status->records->isStopped = true;
}

void DnsRecords::startScan(){
    ///
    /// if the numner of threads is greater than the number of wordlists, set the
    /// number of threads to use to the number of wordlists available to avoid
    /// creating more threads than needed...
    ///
    int wordlistCount = ui->targets->listModel->rowCount();
    int srvWordlistCount = ui->srvWordlist->listModel->rowCount();
    int threadsCount = scanConfig->threadsCount;
    if((ui->comboBoxOption->currentIndex() == OPTION::ALLRECORDS) && (threadsCount > wordlistCount))
    {
        threadsCount = wordlistCount;
    }
    if((ui->comboBoxOption->currentIndex() == OPTION::SRV) && (threadsCount > srvWordlistCount))
    {
        threadsCount = wordlistCount;
    }
    status->records->activeThreads = threadsCount;
    ///
    /// loop to create threads for scan...
    ///
    for(int i = 0; i < threadsCount; i++)
    {
        records::Scanner *scanner = new records::Scanner(scanConfig, m_scanArguments);
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
        if(m_scanArguments->RecordType_srv)
            connect(scanner, &records::Scanner::scanProgress, ui->progressBarSRV, &QProgressBar::setValue);
        else
            connect(scanner, &records::Scanner::scanProgress, ui->progressBar, &QProgressBar::setValue);
        connect(scanner, &records::Scanner::infoLog, this, &DnsRecords::onInfoLog);
        connect(scanner, &records::Scanner::errorLog, this, &DnsRecords::onErrorLog);
        connect(scanner, &records::Scanner::scanResult, this, &DnsRecords::onScanResult);
        connect(cThread, &QThread::finished, this, &DnsRecords::onScanThreadEnded);
        connect(cThread, &QThread::finished, scanner, &QThread::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        connect(this, &DnsRecords::stopScanThread, scanner, &records::Scanner::onStopScan);
        //...
        cThread->start();
    }
    status->records->isRunning = true;
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

void DnsRecords::onScanThreadEnded(){
    status->records->activeThreads--;
    ///
    /// if subdomainIp Scan Threads have finished...
    ///
    if(status->records->activeThreads == 0)
    {
        if(status->records->isPaused)
        {
            //ui->buttonPause->setText("Resume");
            status->records->isRunning = false;
            //...
            sendStatus("[*] Scan Paused!");
            return;
        }
        else
        {
            // set the progress bar to 100% just in case...
            if(!status->records->isStopped){
                ui->progressBar->setValue(ui->progressBar->maximum());
            }
            status->records->isPaused = false;
            status->records->isStopped = false;
            status->records->isRunning = false;
            //...
            ui->buttonStart->setEnabled(true);
            //ui->buttonPause->setDisabled(true);
            ui->buttonStop->setDisabled(true);
            //...
            sendStatus("[*] Enumeration Complete!");
        }
    }
}

void DnsRecords::onScanResult(records::Results results){
    if(m_scanArguments->RecordType_srv)
    {
        result->records->srv->appendRow(QList<QStandardItem*>() <<new QStandardItem(results.srvName) <<new QStandardItem(results.srvTarget) <<new QStandardItem(QString::number(results.srvPort)));
        project->addActiveSRV(QStringList() <<results.srvName <<results.srvTarget <<results.domain);
        ui->labelResultsCountSRV->setNum(result->records->srv->rowCount());
        return;
    }
    ///
    /// for other record types...
    ///
    QStandardItem *domainItem = new QStandardItem(results.domain);
    domainItem->setIcon(QIcon(":/img/res/icons/folder2.png"));
    domainItem->setForeground(Qt::white);
    result->records->dns->invisibleRootItem()->appendRow(domainItem);
    ui->labelResultsCount->setNum(result->records->dns->invisibleRootItem()->rowCount());
    ///
    /// ...
    ///
    if(m_scanArguments->RecordType_a && !results.A.isEmpty()){
        QStandardItem *recordItem = new QStandardItem("A");
        recordItem->setIcon(QIcon(":/img/res/icons/folder2.png"));
        recordItem->setForeground(Qt::white);
        for(const QString &item: results.A)
        {
            recordItem->appendRow(new QStandardItem(item));
            project->addActiveA(QStringList()<<item<<results.domain);
        }
        domainItem->appendRow(recordItem);
    }
    if(m_scanArguments->RecordType_aaaa && !results.AAAA.isEmpty()){
        QStandardItem *recordItem = new QStandardItem("AAAA");
        recordItem->setIcon(QIcon(":/img/res/icons/folder2.png"));
        recordItem->setForeground(Qt::white);
        for(QString item: results.AAAA)
        {
            recordItem->appendRow(new QStandardItem(item));
            project->addActiveAAAA(QStringList()<<item<<results.domain);
        }
        domainItem->appendRow(recordItem);
    }
    if(m_scanArguments->RecordType_ns  && !results.NS.isEmpty()){
        QStandardItem *recordItem = new QStandardItem("NS");
        recordItem->setIcon(QIcon(":/img/res/icons/folder2.png"));
        recordItem->setForeground(Qt::white);
        for(QString item: results.NS)
        {
            recordItem->appendRow(new QStandardItem(item));
            project->addActiveNS(QStringList()<<item<<results.domain);
        }
        domainItem->appendRow(recordItem);
    }
    if(m_scanArguments->RecordType_mx && !results.MX.isEmpty()){
        QStandardItem *recordItem = new QStandardItem("MX");
        recordItem->setIcon(QIcon(":/img/res/icons/folder2.png"));
        recordItem->setForeground(Qt::white);
        for(QString item: results.MX)
        {
            recordItem->appendRow(new QStandardItem(item));
            project->addActiveMX(QStringList()<<item<<results.domain);
        }
        domainItem->appendRow(recordItem);
    }
    if(m_scanArguments->RecordType_txt && !results.TXT.isEmpty()){
        QStandardItem *recordItem = new QStandardItem("TXT");
        recordItem->setIcon(QIcon(":/img/res/icons/folder2.png"));
        recordItem->setForeground(Qt::white);
        for(QString item: results.TXT)
        {
            recordItem->appendRow(new QStandardItem(item));
            project->addActiveTXT(QStringList()<<item<<results.domain);
        }
        domainItem->appendRow(recordItem);
    }
    if(m_scanArguments->RecordType_cname  && !results.CNAME.isEmpty()){
        QStandardItem *recordItem = new QStandardItem("CNAME");
        recordItem->setIcon(QIcon(":/img/res/icons/folder2.png"));
        recordItem->setForeground(Qt::white);
        for(QString item: results.CNAME)
        {
            recordItem->appendRow(new QStandardItem(item));
            project->addActiveCNAME(QStringList()<<item<<results.domain);
        }
        domainItem->appendRow(recordItem);
    }
}

void DnsRecords::onShowFilter(bool show){
    if(show){
        ui->buttonFilter->show();
        ui->lineEditFilter->show();
        ui->comboBoxFilter->show();
    }
    else{
        ui->buttonFilter->hide();
        ui->lineEditFilter->hide();
        ui->comboBoxFilter->hide();
    }
}

void DnsRecords::onExpandResultsDnsRecords(){
    ui->treeViewResults->expandAll();
}

void DnsRecords::onCollapseResultsDnsRecords(){
    ui->treeViewResults->collapseAll();
}

void DnsRecords::onClearResultsDnsRecords(){
    ///
    /// clear the results...
    ///
    result->records->dns->clear();
    ui->labelResultsCount->clear();
    ///
    /// clear the progressbar...
    ui->progressBar->clearMask();
    ui->progressBar->reset();
    ui->progressBar->hide();
}

void DnsRecords::onClearResultsSrvRecords(){
    ///
    /// clear the results...
    ///
    result->records->srv->clear();
    ui->labelResultsCountSRV->clear();
    result->records->srv->setHorizontalHeaderLabels({"Name", "Target", "Port"});
    ///
    /// hide the action button...
    ///
    ui->progressBarSRV->hide();
}

void DnsRecords::on_buttonConfig_clicked(){
    /*
    ScanConfig *bruteconfig = new ScanConfig(this, m_scanConfig, ENGINE::RECORDS);
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

void DnsRecords::on_buttonFilter_clicked(){
    QString keyword = ui->lineEditFilter->text();
}
