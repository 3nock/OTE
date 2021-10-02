#include "DnsRecords.h"
#include "ui_DnsRecords.h"
//...
#include <QDateTime>
#include <QClipboard>


DnsRecords::DnsRecords(QWidget *parent, ResultsModel *resultsModel, Status *status) :
    AbstractEngine(parent, resultsModel, status),
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
    ui->buttonAction->hide();
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
    this->loadSrvWordlist();
    this->connectActions();
    //...
    qRegisterMetaType<records::Results>("records::Results");
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
    //ui->buttonPause->setEnabled(true);
    ui->buttonStop->setEnabled(true);
    ///
    /// Resetting the scan arguments values...
    ///
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
        ui->progressBar->setMaximum(ui->targets->listWidget->count());
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
        ui->progressBarSRV->setMaximum(ui->targets->listWidget->count()*ui->srvWordlist->listWidget->count());
    }
    ///
    /// start Enumeration...
    ///
    startScan();
    ui->buttonAction->show();
    //...
    sendStatus("[START] Started Subdomain Enumeration!");
}

void DnsRecords::stopScan(){

}

void DnsRecords::pauseScan(){
    ///
    /// if the scan was already paused, then this current click is to
    /// Resume the scan, just call the startScan, with the same arguments and
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

void DnsRecords::connectActions(){
    connect(&actionClearResults, &QAction::triggered, this, [=](){this->onClearResultsSrvRecords();});
    ///
    /// SAVE...
    ///
    connect(&actionSaveSRVName, &QAction::triggered, this, [=](){this->onSaveResultsSrvRecords(CHOICE::srvName);});
    connect(&actionSaveSRVTarget, &QAction::triggered, this, [=](){this->onSaveResultsSrvRecords(CHOICE::srvTarget);});
    ///
    /// COPY...
    ///
    connect(&actionCopySRVName, &QAction::triggered, this, [=](){this->onCopyResultsSrvRecords(CHOICE::srvName);});
    connect(&actionCopySRVTarget, &QAction::triggered, this, [=](){this->onCopyResultsSrvRecords(CHOICE::srvTarget);});
    ///
    /// ...
    ///
    connect(&actionClearResults, &QAction::triggered, this, [=](){this->onClearResultsDnsRecords();});
    connect(&actionExpandResults, &QAction::triggered, this, [=](){this->onExpandResultsDnsRecords();});
    connect(&actionCollapseResults, &QAction::triggered, this, [=](){this->onCollapseResultsDnsRecords();});
    ///
    /// SAVE...
    ///
    connect(&actionSaveA, &QAction::triggered, this, [=](){this->onSaveResultsDnsRecords(CHOICE::A);});
    connect(&actionSaveAAAA, &QAction::triggered, this, [=](){this->onSaveResultsDnsRecords(CHOICE::AAAA);});
    connect(&actionSaveMX, &QAction::triggered, this, [=](){this->onSaveResultsDnsRecords(CHOICE::MX);});
    connect(&actionSaveNS, &QAction::triggered, this, [=](){this->onSaveResultsDnsRecords(CHOICE::NS);});
    connect(&actionSaveCNAME, &QAction::triggered, this, [=](){this->onSaveResultsDnsRecords(CHOICE::CNAME);});
    connect(&actionSaveTXT, &QAction::triggered, this, [=](){this->onSaveResultsDnsRecords(CHOICE::TXT);});
    ///
    /// COPY...
    ///
    connect(&actionCopyA, &QAction::triggered, this, [=](){this->onCopyResultsDnsRecords(CHOICE::A);});
    connect(&actionCopyAAAA, &QAction::triggered, this, [=](){this->onCopyResultsDnsRecords(CHOICE::AAAA);});
    connect(&actionCopyMX, &QAction::triggered, this, [=](){this->onCopyResultsDnsRecords(CHOICE::MX);});
    connect(&actionCopyNS, &QAction::triggered, this, [=](){this->onCopyResultsDnsRecords(CHOICE::NS);});
    connect(&actionCopyCNAME, &QAction::triggered, this, [=](){this->onCopyResultsDnsRecords(CHOICE::CNAME);});
    connect(&actionCopyTXT, &QAction::triggered, this, [=](){this->onCopyResultsDnsRecords(CHOICE::TXT);});
    ///
    /// SUBDOMAINS AND IPS...
    ///
    connect(&actionSendToIp, &QAction::triggered, this, [=](){emit sendIpAddressesToIp(ENGINE::RECORDS, CHOICE::ipaddress); emit changeTabToIp();});
    connect(&actionSendToOsint, &QAction::triggered, this, [=](){emit sendSubdomainsToOsint(ENGINE::RECORDS, CHOICE::susbdomains); emit changeTabToOsint();});
    connect(&actionSendToBrute, &QAction::triggered, this, [=](){emit sendSubdomainsToBrute(ENGINE::RECORDS, CHOICE::susbdomains); emit changeTabToBrute();});
    connect(&actionSendToActive, &QAction::triggered, this, [=](){emit sendSubdomainsToActive(ENGINE::RECORDS, CHOICE::susbdomains); emit changeTabToActive();});
    connect(&actionSendToRecords, &QAction::triggered, this, [=](){emit sendSubdomainsToRecord(ENGINE::RECORDS, CHOICE::susbdomains); emit changeTabToRecords();});

    /**** For Right-Click Context Menu ****/
    ///
    /// ...
    ///
    connect(&actionSave, &QAction::triggered, this, [=](){this->onSaveResultsDnsRecords(selectionModel);});
    connect(&actionCopy, &QAction::triggered, this, [=](){this->onCopyResultsDnsRecords(selectionModel);});
    //...
    connect(&actionOpenInBrowser, &QAction::triggered, this, [=](){this->openInBrowser(selectionModel);});
    //...
    connect(&actionSendToOsint_c, &QAction::triggered, this, [=](){emit sendSubdomainsToOsint(selectionModel); emit changeTabToOsint();});
    connect(&actionSendToIp_c, &QAction::triggered, this, [=](){emit sendIpAddressesToIp(selectionModel); emit changeTabToIp();});
    connect(&actionSendToBrute_c, &QAction::triggered, this, [=](){emit sendSubdomainsToBrute(selectionModel); emit changeTabToBrute();});
    connect(&actionSendToActive_c, &QAction::triggered, this, [=](){emit sendSubdomainsToActive(selectionModel); emit changeTabToActive();});
    connect(&actionSendToRecords_c, &QAction::triggered, this, [=](){emit sendSubdomainsToRecord(selectionModel); emit changeTabToRecords();});
}

void DnsRecords::onScanThreadEnded(){
    status->records->activeThreads--;
    ///
    /// if all Scan Threads have finished...
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
        resultsModel->srvrecords->appendRow(QList<QStandardItem*>() <<new QStandardItem(results.srvName) <<new QStandardItem(results.srvTarget) <<new QStandardItem(QString::number(results.srvPort)));
        resultsModel->project->addSRV(QStringList() <<results.srvName <<results.srvTarget <<results.domain);
        ui->labelResultsCountSRV->setNum(resultsModel->srvrecords->rowCount());
        return;
    }
    ///
    /// for other record types...
    ///
    QStandardItem *domainItem = new QStandardItem(results.domain);
    domainItem->setIcon(QIcon(":/img/res/icons/folder2.png"));
    domainItem->setForeground(Qt::white);
    resultsModel->dnsrecords->invisibleRootItem()->appendRow(domainItem);
    ui->labelResultsCount->setNum(resultsModel->dnsrecords->invisibleRootItem()->rowCount());
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
            resultsModel->project->addA(QStringList()<<item<<results.domain);
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
            resultsModel->project->addAAAA(QStringList()<<item<<results.domain);
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
            resultsModel->project->addNS(QStringList()<<item<<results.domain);
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
            resultsModel->project->addMX(QStringList()<<item<<results.domain);
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
            resultsModel->project->addTXT(QStringList()<<item<<results.domain);
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
            resultsModel->project->addCNAME(QStringList()<<item<<results.domain);
        }
        domainItem->appendRow(recordItem);
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
    resultsModel->dnsrecords->clear();
    ui->labelResultsCount->clear();
    ///
    /// clear the progressbar...
    ui->progressBar->clearMask();
    ui->progressBar->reset();
    ui->progressBar->hide();
    ///
    /// hide the action button...
    ///
    ui->buttonAction->hide();
}

void DnsRecords::onClearResultsSrvRecords(){
    ///
    /// clear the results...
    ///
    resultsModel->srvrecords->clear();
    ui->labelResultsCountSRV->clear();
    resultsModel->srvrecords->setHorizontalHeaderLabels({"Name", "Target", "Port"});
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
    pos = QPoint(pos.x()+65, pos.y());
    if(ui->tabWidgetResults->currentIndex()){
        ///
        /// creating the context menu...
        ///
        QMenu *Menu = new QMenu(this);
        QMenu *saveMenu = new QMenu(this);
        QMenu *copyMenu = new QMenu(this);
        Menu->setAttribute(Qt::WA_DeleteOnClose, true);
        //...
        saveMenu->setTitle("Save");
        copyMenu->setTitle("Copy");
        ///
        /// ADDING ACTIONS TO THE CONTEXT MENU...
        ///
        Menu->addAction(&actionClearResults);
        Menu->addSeparator();
        //...
        saveMenu->addAction(&actionSaveSRVName);
        saveMenu->addAction(&actionSaveSRVTarget);
        //...
        copyMenu->addAction(&actionCopySRVName);
        copyMenu->addAction(&actionCopySRVTarget);
        ///
        /// ....
        ///
        Menu->addMenu(copyMenu);
        Menu->addMenu(saveMenu);
        //...
        Menu->addSeparator();
        Menu->addAction(&actionSendToOsint);
        Menu->addAction(&actionSendToBrute);
        Menu->addAction(&actionSendToActive);
        Menu->addAction(&actionSendToRecords);
        ///
        /// showing the context menu...
        ///
        Menu->exec(pos);
    }
    else{
        ///
        /// creating the context menu...
        ///
        QMenu *Menu = new QMenu(this);
        QMenu *saveMenu = new QMenu(this);
        QMenu *copyMenu = new QMenu(this);
        Menu->setAttribute(Qt::WA_DeleteOnClose, true);
        //...
        saveMenu->setTitle("Save");
        copyMenu->setTitle("Copy");
        ///
        /// ADDING ACTIONS TO THE CONTEXT MENU...
        ///
        ///
        saveMenu->addAction(&actionSaveA);
        saveMenu->addAction(&actionSaveAAAA);
        saveMenu->addAction(&actionSaveNS);
        saveMenu->addAction(&actionSaveMX);
        saveMenu->addAction(&actionSaveTXT);
        saveMenu->addAction(&actionSaveCNAME);
        //...
        copyMenu->addAction(&actionCopyA);
        copyMenu->addAction(&actionCopyAAAA);
        copyMenu->addAction(&actionCopyNS);
        copyMenu->addAction(&actionCopyMX);
        copyMenu->addAction(&actionCopyTXT);
        copyMenu->addAction(&actionCopyCNAME);
        ///
        /// ....
        ///
        Menu->addAction(&actionClearResults);
        Menu->addAction(&actionExpandResults);
        Menu->addAction(&actionCollapseResults);
        Menu->addSeparator();
        //...
        Menu->addMenu(copyMenu);
        Menu->addMenu(saveMenu);
        //...
        Menu->addSeparator();
        Menu->addAction(&actionSendToIp);
        Menu->addAction(&actionSendToOsint);
        Menu->addAction(&actionSendToBrute);
        Menu->addAction(&actionSendToActive);
        Menu->addAction(&actionSendToRecords);
        ///
        /// showing the context menu...
        ///
        Menu->exec(pos);
    }
}

void DnsRecords::on_treeViewResults_customContextMenuRequested(const QPoint &pos){
    Q_UNUSED(pos)
    if(!ui->treeViewResults->selectionModel()->isSelected(ui->treeViewResults->currentIndex())){
        return;
    }
    selectionModel = ui->treeViewResults->selectionModel();
    ///
    /// creating the context menu...
    ///
    QMenu *Menu = new QMenu(this);
    Menu->setAttribute(Qt::WA_DeleteOnClose, true);
    ///
    /// ...
    ///
    Menu->addAction(&actionCopy);
    Menu->addAction(&actionSave);
    Menu->addSeparator();
    Menu->addAction(&actionOpenInBrowser);
    Menu->addSeparator();
    Menu->addAction(&actionSendToIp_c);
    Menu->addAction(&actionSendToOsint_c);
    Menu->addAction(&actionSendToBrute_c);
    Menu->addAction(&actionSendToActive_c);
    Menu->addAction(&actionSendToRecords_c);
    ///
    /// showing the menu...
    ///
    Menu->exec(QCursor::pos());
}

void DnsRecords::on_tableViewSRV_customContextMenuRequested(const QPoint &pos){
    Q_UNUSED(pos)
    if(!ui->tableViewSRV->selectionModel()->isSelected(ui->tableViewSRV->currentIndex())){
        return;
    }
    selectionModel = ui->tableViewSRV->selectionModel();
    ///
    /// creating the context menu...
    ///
    QMenu *Menu = new QMenu(this);
    Menu->setAttribute(Qt::WA_DeleteOnClose, true);
    ///
    /// ...
    ///
    Menu->addAction(&actionCopy);
    Menu->addAction(&actionSave);
    Menu->addSeparator();
    Menu->addAction(&actionOpenInBrowser);
    Menu->addSeparator();
    Menu->addAction(&actionSendToOsint_c);
    Menu->addAction(&actionSendToBrute_c);
    Menu->addAction(&actionSendToActive_c);
    Menu->addAction(&actionSendToRecords_c);
    ///
    /// showing the menu...
    ///
    Menu->exec(QCursor::pos());
}

void DnsRecords::onSaveResultsDnsRecords(CHOICE choice){
    ///
    /// checks...
    ///
    QString filename = QFileDialog::getSaveFileName(this, "Save To File", "./");
    if(filename.isEmpty()){
        return;
    }
    QFile file(filename);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    if(!file.isOpen()){
        return;
    }
    ///
    /// variable declarations...
    ///
    QSet<QString> itemSet;
    QString item;
    ///
    /// choice of item to save...
    ///
    switch(choice){
    case CHOICE::MX:
        for(int i = 0; i < resultsModel->dnsrecords->rowCount(); i++)
        {
            for(int j = 0; j < resultsModel->dnsrecords->item(i)->rowCount(); j++)
            {
                if(resultsModel->dnsrecords->item(i)->child(j)->text() == "MX"){
                    for(int k = 0; k < resultsModel->dnsrecords->item(i)->child(j)->rowCount(); k++)
                    {
                        item = resultsModel->dnsrecords->item(i)->child(j)->child(k)->text().append(NEWLINE);
                        if(!itemSet.contains(item)){
                            itemSet.insert(item);
                            file.write(item.toUtf8());
                        }
                    }
                }
            }
        }
        break;
    case CHOICE::NS:
        for(int i = 0; i < resultsModel->dnsrecords->rowCount(); i++)
        {
            for(int j = 0; j < resultsModel->dnsrecords->item(i)->rowCount(); j++)
            {
                if(resultsModel->dnsrecords->item(i)->child(j)->text() == "NS"){
                    for(int k = 0; k < resultsModel->dnsrecords->item(i)->child(j)->rowCount(); k++)
                    {
                        item = resultsModel->dnsrecords->item(i)->child(j)->child(k)->text().append(NEWLINE);
                        if(!itemSet.contains(item)){
                            itemSet.insert(item);
                            file.write(item.toUtf8());
                        }
                    }
                }
            }
        }
        break;
    case CHOICE::TXT:
        for(int i = 0; i < resultsModel->dnsrecords->rowCount(); i++)
        {
            for(int j = 0; j < resultsModel->dnsrecords->item(i)->rowCount(); j++)
            {
                if(resultsModel->dnsrecords->item(i)->child(j)->text() == "TXT"){
                    for(int k = 0; k < resultsModel->dnsrecords->item(i)->child(j)->rowCount(); k++)
                    {
                        item = resultsModel->dnsrecords->item(i)->child(j)->child(k)->text().append(NEWLINE);
                        if(!itemSet.contains(item)){
                            itemSet.insert(item);
                            file.write(item.toUtf8());
                        }
                    }
                }
            }
        }
        break;
    case CHOICE::CNAME:
        for(int i = 0; i < resultsModel->dnsrecords->rowCount(); i++)
        {
            for(int j = 0; j < resultsModel->dnsrecords->item(i)->rowCount(); j++)
            {
                if(resultsModel->dnsrecords->item(i)->child(j)->text() == "CNAME"){
                    for(int k = 0; k < resultsModel->dnsrecords->item(i)->child(j)->rowCount(); k++)
                    {
                        item = resultsModel->dnsrecords->item(i)->child(j)->child(k)->text().append(NEWLINE);
                        if(!itemSet.contains(item)){
                            itemSet.insert(item);
                            file.write(item.toUtf8());
                        }
                    }
                }
            }
        }
        break;
    case CHOICE::A:
        for(int i = 0; i < resultsModel->dnsrecords->rowCount(); i++)
        {
            for(int j = 0; j < resultsModel->dnsrecords->item(i)->rowCount(); j++)
            {
                if(resultsModel->dnsrecords->item(i)->child(j)->text() == "A"){
                    for(int k = 0; k < resultsModel->dnsrecords->item(i)->child(j)->rowCount(); k++)
                    {
                        item = resultsModel->dnsrecords->item(i)->child(j)->child(k)->text().append(NEWLINE);
                        if(!itemSet.contains(item)){
                            itemSet.insert(item);
                            file.write(item.toUtf8());
                        }
                    }
                }
            }
        }
        break;
    case CHOICE::AAAA:
        for(int i = 0; i < resultsModel->dnsrecords->rowCount(); i++)
        {
            for(int j = 0; j < resultsModel->dnsrecords->item(i)->rowCount(); j++)
            {
                if(resultsModel->dnsrecords->item(i)->child(j)->text() == "AAAA"){
                    for(int k = 0; k < resultsModel->dnsrecords->item(i)->child(j)->rowCount(); k++)
                    {
                        item = resultsModel->dnsrecords->item(i)->child(j)->child(k)->text().append(NEWLINE);
                        if(!itemSet.contains(item)){
                            itemSet.insert(item);
                            file.write(item.toUtf8());
                        }
                    }
                }
            }
        }
        break;
    default:
        break;
    }
    file.close();
}

void DnsRecords::onSaveResultsDnsRecords(QItemSelectionModel* selectionModel){
    QString filename = QFileDialog::getSaveFileName(this, "Save To File", "./");
    if(filename.isEmpty()){
        return;
    }
    QSet<QString> itemSet;
    QString data;
    QString item;
    ///
    /// ...
    ///
    QFile file(filename);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    if(file.isOpen())
    {
        foreach(const QModelIndex &index, selectionModel->selectedIndexes()){
            item = index.data().toString();
            if(!itemSet.contains(item) && item != "A" && item != "AAAA" && item != "NS" && item != "MX" && item != "CNAME" && item != "TXT"){
                itemSet.insert(item);
                data.append(item.append(NEWLINE));
            }
        }
        file.write(data.toUtf8());
        file.close();
    }
}

void DnsRecords::onCopyResultsDnsRecords(CHOICE choice){
    ///
    /// variable declaration...
    ///
    QClipboard *clipboard = QGuiApplication::clipboard();
    QString clipboardData;
    QSet<QString> itemSet;
    QString item;
    ///
    /// type of item to save...
    ///
    switch(choice){
    case CHOICE::MX:
        for(int i = 0; i < resultsModel->dnsrecords->rowCount(); i++)
        {
            for(int j = 0; j < resultsModel->dnsrecords->item(i)->rowCount(); j++)
            {
                if(resultsModel->dnsrecords->item(i)->child(j)->text() == "MX"){
                    for(int k = 0; k < resultsModel->dnsrecords->item(i)->child(j)->rowCount(); k++)
                    {
                        item = resultsModel->dnsrecords->item(i)->child(j)->child(k)->text().append(NEWLINE);
                        if(!itemSet.contains(item)){
                            itemSet.insert(item);
                            clipboardData.append(item);
                        }
                    }
                }
            }
        }
        break;
    case CHOICE::NS:
        for(int i = 0; i < resultsModel->dnsrecords->rowCount(); i++)
        {
            for(int j = 0; j < resultsModel->dnsrecords->item(i)->rowCount(); j++)
            {
                if(resultsModel->dnsrecords->item(i)->child(j)->text() == "NS"){
                    for(int k = 0; k < resultsModel->dnsrecords->item(i)->child(j)->rowCount(); k++)
                    {
                        item = resultsModel->dnsrecords->item(i)->child(j)->child(k)->text().append(NEWLINE);
                        if(!itemSet.contains(item)){
                            itemSet.insert(item);
                            clipboardData.append(item);
                        }
                    }
                }
            }
        }
        break;
    case CHOICE::TXT:
        for(int i = 0; i < resultsModel->dnsrecords->rowCount(); i++)
        {
            for(int j = 0; j < resultsModel->dnsrecords->item(i)->rowCount(); j++)
            {
                if(resultsModel->dnsrecords->item(i)->child(j)->text() == "TXT"){
                    for(int k = 0; k < resultsModel->dnsrecords->item(i)->child(j)->rowCount(); k++)
                    {
                        item = resultsModel->dnsrecords->item(i)->child(j)->child(k)->text().append(NEWLINE);
                        if(!itemSet.contains(item)){
                            itemSet.insert(item);
                            clipboardData.append(item);
                        }
                    }
                }
            }
        }
        break;
    case CHOICE::CNAME:
        for(int i = 0; i < resultsModel->dnsrecords->rowCount(); i++)
        {
            for(int j = 0; j < resultsModel->dnsrecords->item(i)->rowCount(); j++)
            {
                if(resultsModel->dnsrecords->item(i)->child(j)->text() == "CNAME"){
                    for(int k = 0; k < resultsModel->dnsrecords->item(i)->child(j)->rowCount(); k++)
                    {
                        item = resultsModel->dnsrecords->item(i)->child(j)->child(k)->text().append(NEWLINE);
                        if(!itemSet.contains(item)){
                            itemSet.insert(item);
                            clipboardData.append(item);
                        }
                    }
                }
            }
        }
        break;
    case CHOICE::A:
        for(int i = 0; i < resultsModel->dnsrecords->rowCount(); i++)
        {
            for(int j = 0; j < resultsModel->dnsrecords->item(i)->rowCount(); j++)
            {
                if(resultsModel->dnsrecords->item(i)->child(j)->text() == "A"){
                    for(int k = 0; k < resultsModel->dnsrecords->item(i)->child(j)->rowCount(); k++)
                    {
                        item = resultsModel->dnsrecords->item(i)->child(j)->child(k)->text().append(NEWLINE);
                        if(!itemSet.contains(item)){
                            itemSet.insert(item);
                            clipboardData.append(item);
                        }
                    }
                }
            }
        }
        break;
    case CHOICE::AAAA:
        for(int i = 0; i < resultsModel->dnsrecords->rowCount(); i++)
        {
            for(int j = 0; j < resultsModel->dnsrecords->item(i)->rowCount(); j++)
            {
                if(resultsModel->dnsrecords->item(i)->child(j)->text() == "AAAA"){
                    for(int k = 0; k < resultsModel->dnsrecords->item(i)->child(j)->rowCount(); k++)
                    {
                        item = resultsModel->dnsrecords->item(i)->child(j)->child(k)->text().append(NEWLINE);
                        if(!itemSet.contains(item)){
                            itemSet.insert(item);
                            clipboardData.append(item);
                        }
                    }
                }
            }
        }
        break;
    default:
        break;
    }
    clipboard->setText(clipboardData);
}

void DnsRecords::onCopyResultsDnsRecords(QItemSelectionModel* selectionModel){
    QClipboard *clipboard = QGuiApplication::clipboard();
    QSet<QString> itemSet;
    QString data;
    QString item;
    ///
    /// ...
    ///
    foreach(const QModelIndex &index, selectionModel->selectedIndexes())
    {
        item = index.data().toString();
        if(!itemSet.contains(item) && item != "A" && item != "AAAA" && item != "NS" && item != "MX" && item != "CNAME" && item != "TXT"){
            itemSet.insert(item);
            data.append(item.append(NEWLINE));
        }
    }
    clipboard->setText(data);
}

void DnsRecords::onSaveResultsSrvRecords(CHOICE choice){
    ///
    /// checks...
    ///
    QString filename = QFileDialog::getSaveFileName(this, "Save To File", "./");
    if(filename.isEmpty()){
        return;
    }
    QFile file(filename);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    if(!file.isOpen()){
        return;
    }
    ///
    /// variable declarations...
    ///
    QSet<QString> itemSet;
    QString item;
    ///
    /// choice of item to save...
    ///
    switch(choice){
    case CHOICE::srvName:
        for(int i = 0; i != resultsModel->srvrecords->rowCount(); ++i)
        {
            item = resultsModel->srvrecords->item(i, 0)->text().append(NEWLINE);
            if(!itemSet.contains(item)){
                itemSet.insert(item);
                file.write(item.toUtf8());
            }
        }
        break;
    case CHOICE::srvTarget:
        for(int i = 0; i != resultsModel->srvrecords->rowCount(); ++i)
        {
            item = resultsModel->srvrecords->item(i, 1)->text().append(NEWLINE);
            if(!itemSet.contains(item)){
                itemSet.insert(item);
                file.write(item.toUtf8());
            }
        }
        break;
    default:
        break;
    }
    file.close();
}

void DnsRecords::onSaveResultsSrvRecords(QItemSelectionModel *selectionModel){
    QString filename = QFileDialog::getSaveFileName(this, "Save To File", "./");
    if(filename.isEmpty()){
        return;
    }
    QSet<QString> itemSet;
    QString data;
    QString item;
    ///
    /// ...
    ///
    QFile file(filename);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    if(file.isOpen())
    {
        foreach(const QModelIndex &index, selectionModel->selectedIndexes()){
            item = index.data().toString();
            if(!itemSet.contains(item)){
                itemSet.insert(item);
                data.append(item.append(NEWLINE));
            }
        }
        file.write(data.toUtf8());
        file.close();
    }
}

void DnsRecords::onCopyResultsSrvRecords(CHOICE choice){
    ///
    /// variable declaration...
    ///
    QClipboard *clipboard = QGuiApplication::clipboard();
    QString clipboardData;
    QSet<QString> itemSet;
    QString item;
    ///
    /// type of item to save...
    ///
    switch(choice){
    case CHOICE::srvName:
        for(int i = 0; i != resultsModel->srvrecords->rowCount(); ++i)
        {
            item = resultsModel->srvrecords->item(i, 0)->text().append(NEWLINE);
            if(!itemSet.contains(item)){
                itemSet.insert(item);
                clipboardData.append(item);
            }
        }
        break;
    case CHOICE::srvTarget:
        for(int i = 0; i != resultsModel->srvrecords->rowCount(); ++i)
        {
            item = resultsModel->srvrecords->item(i, 1)->text().append(NEWLINE);
            if(!itemSet.contains(item)){
                itemSet.insert(item);
                clipboardData.append(item);
            }
        }
        break;
    default:
        break;
    }
    clipboard->setText(clipboardData);
}

void DnsRecords::onCopyResultsSrvRecords(QItemSelectionModel *selectionModel){
    QClipboard *clipboard = QGuiApplication::clipboard();
    QSet<QString> itemSet;
    QString data;
    QString item;
    ///
    /// ...
    ///
    foreach(const QModelIndex &index, selectionModel->selectedIndexes())
    {
        item = index.data().toString();
        if(!itemSet.contains(item)){
            itemSet.insert(item);
            data.append(item.append(NEWLINE));
        }
    }
    clipboard->setText(data);
}

