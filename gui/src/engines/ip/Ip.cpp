#include "Ip.h"
#include "ui_Ip.h"
//...
#include <QDateTime>
#include <QClipboard>

Ip::Ip(QWidget *parent, ResultsModel *resultsModel, Status *status) :
    AbstractEngine(parent, resultsModel, status),
    ui(new Ui::Ip),
    m_scanArguments(new ip::ScanArguments),
    m_model(resultsModel->ip),
    m_proxyModel(resultsModel->proxy->ip)
{
    ui->setupUi(this);
    ///
    /// init...
    ///
    ui->targets->init("Target Ip");
    targets = ui->targets;
    scanConfig->name = tr("ScanConfig-Ip");
    ///
    /// ...
    ///
    ui->progressBar->hide();
    ui->buttonAction->hide();
    //...
    ui->lineEditFilter->hide();
    ui->buttonFilter->hide();
    ui->comboBoxFilter->hide();
    //...
    ui->buttonStop->setDisabled(true);
    //ui->buttonPause->setDisabled(true);
    //...
    ui->lineEditFilter->setPlaceholderText("Enter filter...");
    m_model->setHorizontalHeaderLabels({"IpAddress:", "HostName:"});
    ui->tableViewResults->setModel(m_model);
    ///
    /// equally seperate the widgets...
    ///
    ui->splitter->setSizes(QList<int>() << static_cast<int>((this->width() * 0.50))
                                        << static_cast<int>((this->width() * 0.50)));
    //...
    m_scanArguments->targetList = ui->targets->listWidget;
    m_scanArguments->model_results = m_model;
    ///
    /// ...
    ///
    this->connectActions();
    m_proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    m_proxyModel->setRecursiveFilteringEnabled(true);
}
Ip::~Ip(){
    delete m_scanArguments;
    //...
    delete ui;
}

void Ip::onInfoLog(QString log){
    QString logTime = QDateTime::currentDateTime().toString("hh:mm:ss  ");
    ui->plainTextEditLogs->appendPlainText(logTime.append(log));
}

void Ip::onErrorLog(QString log){
    QString fontedLog;
    fontedLog.append("<font color=\"red\">").append(log).append("</font>");
    QString logTime = QDateTime::currentDateTime().toString("hh:mm:ss  ");
    ui->plainTextEditLogs->appendHtml(logTime.append(fontedLog));
}

void Ip::on_buttonStart_clicked(){
    ///
    /// checking if all requirements are satisfied before scan if not prompt error
    /// then exit function...
    ///
    if(!(ui->targets->listWidget->count() > 0)){
        QMessageBox::warning(this, "Error!", "Please Enter the subdomains Wordlist for Enumeration!");
        return;
    }
    ///
    /// disabling and Enabling widgets...
    ///
    ui->buttonStart->setDisabled(true);
    //ui->buttonPause->setEnabled(true);
    ui->buttonStop->setEnabled(true);
    ui->progressBar->show();
    ///
    /// Resetting the scan arguments values...
    ///
    m_scanArguments->currentTargetToEnumerate = 0;
    m_scanArguments->progress = 0;
    ui->progressBar->reset();
    ///
    /// Getting scan arguments....
    ///
    ui->progressBar->setMaximum(ui->targets->listWidget->count());
    ///
    /// start Ip subdomain enumeration...
    ///
    startScan();
    ui->buttonAction->show();
    //...
    sendStatus("[*] Testing For Ip Subdomains...");
}

void Ip::stopScan(){

}

void Ip::pauseScan(){
    ///
    /// if the scan was already paused, then this current click is to
    /// Resume the scan, just call the startScan, with the same arguments and
    /// it will continue at where it ended...
    ///
    if(status->ip->isPaused)
    {
        //ui->buttonPause->setText("Pause");
        status->ip->isPaused = false;
        //...
        startScan();
        //...
        sendStatus("[START] Resumed Subdomain Enumeration!");
    }
    else
    {
        status->ip->isPaused = true;
        emit stopScan();
    }
}

void Ip::ResumeScan(){

}

void Ip::connectActions(){
    connect(&actionClearResults, &QAction::triggered, this, [=](){this->onClearResults();});
    connect(&actionShowFilter, &QAction::triggered, this, [=](){this->onShowFilter(true);});
    connect(&actionHideFilter, &QAction::triggered, this, [=](){this->onShowFilter(false);});
    ///
    /// SAVE...
    ///
    connect(&actionSaveSubdomains, &QAction::triggered, this, [=](){this->onSaveResults(CHOICE::susbdomains);});
    connect(&actionSaveIpAddresses, &QAction::triggered, this, [=](){this->onSaveResults(CHOICE::ipaddress);});
    connect(&actionSaveAll, &QAction::triggered, this, [=](){this->onSaveResults(CHOICE::all);});
    ///
    /// COPY...
    ///
    connect(&actionCopySubdomains, &QAction::triggered, this, [=](){this->onCopyResults(CHOICE::susbdomains);});
    connect(&actionCopyIpAddresses, &QAction::triggered, this, [=](){this->onCopyResults(CHOICE::ipaddress);});
    connect(&actionCopyAll, &QAction::triggered, this, [=](){this->onCopyResults(CHOICE::all);});
    ///
    /// SUBDOMAINS AND IPS...
    ///
    connect(&actionSendToIp, &QAction::triggered, this, [=](){emit sendIpAddressesToIp(ENGINE::IP, CHOICE::ipaddress); emit changeTabToIp();});
    connect(&actionSendToOsint, &QAction::triggered, this, [=](){emit sendSubdomainsToOsint(ENGINE::IP, CHOICE::susbdomains); emit changeTabToOsint();});
    connect(&actionSendToBrute, &QAction::triggered, this, [=](){emit sendSubdomainsToBrute(ENGINE::IP, CHOICE::susbdomains); emit changeTabToBrute();});
    connect(&actionSendToActive, &QAction::triggered, this, [=](){emit sendSubdomainsToActive(ENGINE::IP, CHOICE::susbdomains); emit changeTabToActive();});
    connect(&actionSendToRecords, &QAction::triggered, this, [=](){emit sendSubdomainsToRecord(ENGINE::IP, CHOICE::susbdomains); emit changeTabToRecords();});

    /**** For Right-Click Context-Menu ****/
    connect(&actionSave, &QAction::triggered, this, [=](){this->onSaveResults(selectionModel);});
    connect(&actionCopy, &QAction::triggered, this, [=](){this->onCopyResults(selectionModel);});
    //...
    connect(&actionOpenInBrowser, &QAction::triggered, this, [=](){this->openInBrowser(selectionModel);});
    //...
    connect(&actionSendToOsint_c, &QAction::triggered, this, [=](){emit sendSubdomainsToOsint(selectionModel); emit changeTabToOsint();});
    connect(&actionSendToIp_c, &QAction::triggered, this, [=](){emit sendIpAddressesToIp(selectionModel); emit changeTabToIp();});
    connect(&actionSendToBrute_c, &QAction::triggered, this, [=](){emit sendSubdomainsToBrute(selectionModel); emit changeTabToBrute();});
    connect(&actionSendToActive_c, &QAction::triggered, this, [=](){emit sendSubdomainsToActive(selectionModel); emit changeTabToActive();});
    connect(&actionSendToRecords_c, &QAction::triggered, this, [=](){emit sendSubdomainsToRecord(selectionModel); emit changeTabToRecords();});
}

void Ip::on_buttonStop_clicked(){
    emit stopScan();
    status->ip->isStopped = true;
}

void Ip::startScan(){
    ///
    /// if the numner of threads is greater than the number of wordlists, set the
    /// number of threads to use to the number of wordlists available to avoid
    /// creating more threads than needed...
    ///
    int wordlistCount = ui->targets->listWidget->count();
    int threadsCount = scanConfig->threadsCount;
    if(threadsCount > wordlistCount)
    {
        threadsCount = wordlistCount;
    }
    status->ip->activeThreads = threadsCount;
    ///
    /// loop to create threads for enumeration...
    ///
    for(int i = 0; i < threadsCount; i++)
    {
        ip::Scanner *scanner = new ip::Scanner(scanConfig, m_scanArguments);
        QThread *cThread = new QThread;
        scanner->startScan(cThread);
        scanner->moveToThread(cThread);
        //...
        connect(scanner, &ip::Scanner::scanResult, this, &Ip::onScanResult);
        connect(scanner, &ip::Scanner::scanProgress, ui->progressBar, &QProgressBar::setValue);
        connect(scanner, &ip::Scanner::infoLog, this, &Ip::onInfoLog);
        connect(scanner, &ip::Scanner::errorLog, this, &Ip::onErrorLog);
        connect(cThread, &QThread::finished, this, &Ip::onScanThreadEnded);
        connect(cThread, &QThread::finished, scanner, &ip::Scanner::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        connect(this, &Ip::stopScanThread, scanner, &ip::Scanner::onStopScan);
        //...
        cThread->start();
    }
    status->ip->isRunning = true;
}

void Ip::onScanThreadEnded(){
    status->ip->activeThreads--;
    ///
    /// if all Scan Threads have finished...
    ///
    if(status->ip->activeThreads == 0)
    {
        if(status->ip->isPaused)
        {
            //ui->buttonPause->setText("Resume");
            status->ip->isRunning = false;
            //...
            sendStatus("[*] Scan Paused!");
            return;
        }
        else
        {
            // set the progress bar to 100% just in case...
            if(!status->ip->isStopped){
                ui->progressBar->setValue(ui->progressBar->maximum());
            }
            status->ip->isPaused = false;
            status->ip->isStopped = false;
            status->ip->isRunning = false;
            //...
            ui->buttonStart->setEnabled(true);
            //ui->buttonPause->setDisabled(true);
            ui->buttonStop->setDisabled(true);
            //...
            sendStatus("[*] Enumeration Complete!");
        }
    }
}

void Ip::onScanResult(QString subdomain, QString ipAddress){
    ///
    /// save to ip model model...
    ///
    m_model->appendRow(QList<QStandardItem*>() <<new QStandardItem(ipAddress) <<new QStandardItem(subdomain));
    ui->labelResultsCount->setNum(m_scanArguments->model_results->rowCount());
    ///
    /// save to project model...
    ///
    resultsModel->project->addSubdomain(QStringList()<<subdomain<<ipAddress<<subdomain);
}

void Ip::on_buttonConfig_clicked(){
    ConfigDialog *configDialog = new ConfigDialog(this, scanConfig);
    configDialog->setAttribute( Qt::WA_DeleteOnClose, true );
    configDialog->show();
}

void Ip::onClearResults(){
    ///
    /// clear the results...
    ///
    m_model->clear();
    ui->labelResultsCount->clear();
    m_model->setHorizontalHeaderLabels({"IpAddress", "HostName"});
    ///
    /// clear the progressbar...
    ui->progressBar->clearMask();
    ui->progressBar->reset();
    ui->progressBar->hide();
    ///
    /// hide the action button...
    ///
    ui->buttonAction->hide();
    ///
    /// hide the filter...
    ///
    ui->buttonFilter->hide();
    ui->lineEditFilter->hide();
    ui->comboBoxFilter->hide();
}

void Ip::onShowFilter(bool show){
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

void Ip::on_comboBoxOption_currentIndexChanged(int index){
    Q_UNUSED(index);
}

void Ip::on_buttonAction_clicked(){
    ///
    /// check if there are results available else dont show the context menu...
    ///
    if(m_model->rowCount() < 1){
        return;
    }
    ///
    /// getting the position of the action button to place the context menu and
    /// showing the context menu right by the side of the action button...
    ///
    QPoint pos = ui->buttonAction->mapToGlobal(QPoint(0,0));
    pos = QPoint(pos.x()+65, pos.y());
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
    saveMenu->addAction(&actionSaveSubdomains);
    saveMenu->addAction(&actionSaveIpAddresses);
    saveMenu->addAction(&actionSaveAll);
    //...
    copyMenu->addAction(&actionCopySubdomains);
    copyMenu->addAction(&actionCopyIpAddresses);
    copyMenu->addAction(&actionCopyAll);
    ///
    /// ....
    ///
    Menu->addAction(&actionClearResults);
    if(ui->lineEditFilter->isHidden() && ui->buttonFilter->isHidden()){
        Menu->addAction(&actionShowFilter);
    }else{
        Menu->addAction(&actionHideFilter);
    }
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

void Ip::on_tableViewResults_customContextMenuRequested(const QPoint &pos){
    Q_UNUSED(pos);
    ///
    /// check if user right clicked on items else dont show the context menu...
    ///
    if(!ui->tableViewResults->selectionModel()->isSelected(ui->tableViewResults->currentIndex())){
        return;
    }
    selectionModel = ui->tableViewResults->selectionModel();
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

void Ip::onSaveResults(CHOICE choice){
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
    case CHOICE::susbdomains:
        for(int i = 0; i != m_proxyModel->rowCount(); ++i)
        {
            item = m_proxyModel->data(m_proxyModel->index(i, 0)).toString().append(NEWLINE);
            if(!itemSet.contains(item)){
                itemSet.insert(item);
                file.write(item.toUtf8());
            }
        }
        break;
    case CHOICE::ipaddress:
        for(int i = 0; i != m_proxyModel->rowCount(); ++i)
        {
            item = m_proxyModel->data(m_proxyModel->index(i, 1)).toString().append(NEWLINE);
            if(!itemSet.contains(item)){
                itemSet.insert(item);
                file.write(item.toUtf8());
            }
        }
        break;
    case CHOICE::all:
        for(int i = 0; i != m_proxyModel->rowCount(); ++i)
        {
            item = m_proxyModel->data(m_proxyModel->index(i, 0)).toString()+":"+m_proxyModel->data(m_proxyModel->index(i, 1)).toString().append(NEWLINE);
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

void Ip::onSaveResults(QItemSelectionModel *selectionModel){
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

void Ip::onCopyResults(CHOICE choice){
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
    case CHOICE::susbdomains:
        for(int i = 0; i != m_proxyModel->rowCount(); ++i)
        {
            item = m_proxyModel->data(m_proxyModel->index(i, 0)).toString().append(NEWLINE);
            if(!itemSet.contains(item)){
                itemSet.insert(item);
                clipboardData.append(item);
            }
        }
        break;
    case CHOICE::ipaddress:
        for(int i = 0; i != m_proxyModel->rowCount(); ++i)
        {
            item = m_proxyModel->data(m_proxyModel->index(i, 1)).toString().append(NEWLINE);
            if(!itemSet.contains(item)){
                itemSet.insert(item);
                clipboardData.append(item);
            }
        }
        break;
    case CHOICE::all:
        for(int i = 0; i != m_proxyModel->rowCount(); ++i)
        {
            item = m_proxyModel->data(m_proxyModel->index(i, 0)).toString()+"|"+m_proxyModel->data(m_proxyModel->index(i, 1)).toString().append(NEWLINE);
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

void Ip::onCopyResults(QItemSelectionModel *selectionModel){
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

void Ip::on_buttonFilter_clicked(){
    QString filterKeyword = ui->lineEditFilter->text();
    m_proxyModel->setFilterKeyColumn(ui->comboBoxFilter->currentIndex());
    m_proxyModel->setFilterRegExp(filterKeyword);
    ui->tableViewResults->setModel(m_proxyModel);
}
