#include "Active.h"
#include "ui_Active.h"
//...
#include <QThread>
#include <QDateTime>
#include <QClipboard>

Active::Active(QWidget *parent, ResultsModel *resultsModel, Status *status) :
    AbstractEngine(parent, resultsModel, status),
    ui(new Ui::Active),
    m_scanArguments(new active::ScanArguments)
{
    ui->setupUi(this);
    ///
    /// init...
    ///
    ui->targets->init("Targets");
    targets = ui->targets;
    scanConfig->name = tr("ScanConfig-Active");
    ///
    /// ...
    ///
    ui->progressBar->hide();
    ui->buttonAction->hide();
    //...
    ui->buttonStop->setDisabled(true);
    //ui->buttonPause->setDisabled(true);
    //...
    resultsModel->active->setHorizontalHeaderLabels({"Subdomain", "IpAddress"});
    ui->tableViewResults->setModel(resultsModel->active);
    ///
    /// equally seperate the widgets...
    ///
    ui->splitter->setSizes(QList<int>() << static_cast<int>((this->width() * 0.50))
                                        << static_cast<int>((this->width() * 0.50)));
    //...
    m_scanArguments->targetList = ui->targets->listWidget;
    ///
    /// ...
    ///
    ui->frameCustom->hide();
    ui->lineEditServiceName->setPlaceholderText("e.g SMTP");
    ui->lineEditServicePort->setPlaceholderText("e.g 889");
    ///
    /// ...
    ///
    connectActions();
}
Active::~Active(){
    delete m_scanArguments;
    delete ui;
}

void Active::onInfoLog(QString log){
    QString logTime = QDateTime::currentDateTime().toString("hh:mm:ss  ");
    ui->plainTextEditLogs->appendPlainText(logTime.append(log));
}

void Active::onErrorLog(QString log){
    QString fontedLog;
    fontedLog.append("<font color=\"red\">").append(log).append("</font>");
    QString logTime = QDateTime::currentDateTime().toString("hh:mm:ss  ");
    ui->plainTextEditLogs->appendHtml(logTime.append(fontedLog));
}

void Active::on_buttonStart_clicked(){
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
    if(ui->comboBoxOption->currentIndex() == ACTIVE::DNS){
        m_scanArguments->checkActiveService = false;
    }
    if(ui->comboBoxOption->currentIndex() == ACTIVE::HTTP){
        m_scanArguments->checkActiveService = true;
        m_scanArguments->service = 80;
    }
    if(ui->comboBoxOption->currentIndex() == ACTIVE::HTTPS){
        m_scanArguments->checkActiveService = true;
        m_scanArguments->service = 443;
    }
    if(ui->comboBoxOption->currentIndex() == ACTIVE::FTP){
        m_scanArguments->checkActiveService = true;
        m_scanArguments->service = 21;
    }
    if(ui->comboBoxOption->currentIndex() == ACTIVE::SMTP){
        m_scanArguments->checkActiveService = true;
        m_scanArguments->service = 587;
    }
    ui->progressBar->setMaximum(ui->targets->listWidget->count());
    ///
    /// start active subdomain enumeration...
    ///
    startScan();
    ui->buttonAction->show();
    //...
    sendStatus("[*] Testing For Active Subdomains...");
}

void Active::pauseScan(){
    ///
    /// if the scan was already paused, then this current click is to
    /// Resume the scan, just call the startScan, with the same arguments and
    /// it will continue at where it ended...
    ///
    if(status->active->isPaused){
        //ui->buttonPause->setText("Pause");
        status->active->isPaused = false;
        //...
        startScan();
        //...
        sendStatus("[START] Resumed Subdomain Enumeration!");
    }
    else
    {
        status->active->isPaused = true;
        emit stopScan();
    }
}

void Active::ResumeScan(){

}

void Active::connectActions(){
    connect(&actionClearResults, &QAction::triggered, this, [=](){this->onClearResults();});
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
    connect(&actionSendToIp, &QAction::triggered, this, [=](){emit sendIpAddressesToIp(ENGINE::BRUTE, CHOICE::ipaddress); emit changeTabToIp();});
    connect(&actionSendToOsint, &QAction::triggered, this, [=](){emit sendSubdomainsToOsint(ENGINE::BRUTE, CHOICE::susbdomains); emit changeTabToOsint();});
    connect(&actionSendToBrute, &QAction::triggered, this, [=](){emit sendSubdomainsToBrute(ENGINE::BRUTE, CHOICE::susbdomains); emit changeTabToBrute();});
    connect(&actionSendToActive, &QAction::triggered, this, [=](){emit sendSubdomainsToActive(ENGINE::BRUTE, CHOICE::susbdomains); emit changeTabToActive();});
    connect(&actionSendToRecords, &QAction::triggered, this, [=](){emit sendSubdomainsToRecord(ENGINE::BRUTE, CHOICE::susbdomains); emit changeTabToRecords();});

    /***** For Right CLick *****/
    ///
    /// ...
    ///
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

void Active::on_buttonStop_clicked(){
    emit stopScan();
    status->active->isStopped = true;
}

void Active::stopScan(){

}

void Active::startScan(){
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
    status->active->activeThreads = threadsCount;
    ///
    /// loop to create threads for enumeration...
    ///
    for(int i = 0; i < threadsCount; i++)
    {
        active::Scanner *scanner = new active::Scanner(scanConfig, m_scanArguments);
        QThread *cThread = new QThread;
        scanner->startScan(cThread);
        scanner->moveToThread(cThread);
        //...
        connect(scanner, &active::Scanner::scanResult, this, &Active::onScanResult);
        connect(scanner, &active::Scanner::scanProgress, ui->progressBar, &QProgressBar::setValue);
        connect(scanner, &active::Scanner::infoLog, this, &Active::onInfoLog);
        connect(scanner, &active::Scanner::errorLog, this, &Active::onErrorLog);
        connect(cThread, &QThread::finished, this, &Active::onScanThreadEnded);
        connect(cThread, &QThread::finished, scanner, &active::Scanner::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        connect(this, &Active::stopScanThread, scanner, &active::Scanner::onStopScan);
        //...
        cThread->start();
    }
    status->active->isRunning = true;
}

void Active::onScanResult(QString subdomain, QString ipAddress){
    ///
    /// save to active model...
    ///
    resultsModel->active->appendRow(QList<QStandardItem*>() << new QStandardItem(subdomain) << new QStandardItem(ipAddress));
    ui->labelResultsCount->setNum(resultsModel->active->rowCount());
    ///
    /// save to project model...
    ///
    resultsModel->project->addSubdomain(QStringList()<<subdomain<<ipAddress<<subdomain);
}

void Active::onScanThreadEnded(){
    status->active->activeThreads--;
    ///
    /// if all Scan Threads have finished...
    ///
    if(status->active->activeThreads == 0)
    {
        if(status->active->isPaused)
        {
            //ui->buttonPause->setText("Resume");
            status->active->isRunning = false;
            //...
            sendStatus("[*] Scan Paused!");
            return;
        }
        else
        {
            // set the progress bar to 100% just in case...
            if(!status->active->isStopped){
                ui->progressBar->setValue(ui->progressBar->maximum());
            }
            status->active->isPaused = false;
            status->active->isStopped = false;
            status->active->isRunning = false;
            //...
            ui->buttonStart->setEnabled(true);
            //ui->buttonPause->setDisabled(true);
            ui->buttonStop->setDisabled(true);
            //...
            sendStatus("[*] Enumeration Complete!");
        }
    }
}

void Active::on_comboBoxOption_currentIndexChanged(int index){
    if(index == ACTIVE::DNS){
        ui->label_details->setText("Resolves the target hostname To it's IpAddress");
    }
    if(index == ACTIVE::HTTP){
        ui->label_details->setText("Resolves the target, if Resolved, Then tests for connection To port 80");
    }
    if(index == ACTIVE::HTTPS){
        ui->label_details->setText("Resolves the target, if Resolved Then tests for connection To port 443");
    }
    if(index == ACTIVE::FTP){
        ui->label_details->setText("Resolves the target, if Resolved Then tests for connection To port 20");
    }
    if(index == ACTIVE::SMTP){
        ui->label_details->setText("Resolves the target, if Resolved Then tests for connection To port 587");
    }
}

void Active::on_buttonConfig_clicked(){
    ConfigDialog *configDialog = new ConfigDialog(this, scanConfig);
    configDialog->setAttribute( Qt::WA_DeleteOnClose, true );
    configDialog->show();
}

void Active::onClearResults(){
    ///
    /// clear the results...
    ///
    resultsModel->active->clear();
    ui->labelResultsCount->clear();
    resultsModel->active->setHorizontalHeaderLabels({"Subdomain", "IpAddress"});
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

void Active::on_buttonAction_clicked(){
    ///
    /// check if there are results available else dont show the context menu...
    ///
    if(resultsModel->active->rowCount() < 1){
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

void Active::on_tableViewResults_customContextMenuRequested(const QPoint &pos){
    Q_UNUSED(pos);
    ///
    /// check if user right clicked on items else dont show the context menu...
    ///
    if(!ui->tableViewResults->selectionModel()->isSelected(ui->tableViewResults->currentIndex())){
        return;
    }
    ///
    /// getting the selected items...
    ///
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

void Active::on_checkBoxCustomActive_clicked(bool checked){
    if(checked){
        ui->frameDefault->hide();
        ui->frameCustom->show();
    }else{
        ui->frameCustom->hide();
        ui->frameDefault->show();
    }
}

void Active::onSaveResults(CHOICE choice){
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
        for(int i = 0; i != resultsModel->active->rowCount(); ++i)
        {
            item = resultsModel->active->item(i, 0)->text().append(NEWLINE);
            if(!itemSet.contains(item)){
                itemSet.insert(item);
                file.write(item.toUtf8());
            }
        }
        break;
    case CHOICE::ipaddress:
        for(int i = 0; i != resultsModel->active->rowCount(); ++i)
        {
            item = resultsModel->active->item(i, 1)->text().append(NEWLINE);
            if(!itemSet.contains(item)){
                itemSet.insert(item);
                file.write(item.toUtf8());
            }
        }
        break;
    case CHOICE::all:
        for(int i = 0; i != resultsModel->active->rowCount(); ++i)
        {
            item = resultsModel->active->item(i, 0)->text()+":"+resultsModel->active->item(i, 1)->text().append(NEWLINE);
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

void Active::onSaveResults(QItemSelectionModel *selectionModel){
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

void Active::onCopyResults(CHOICE choice){
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
        for(int i = 0; i != resultsModel->active->rowCount(); ++i)
        {
            item = resultsModel->active->item(i, 0)->text().append(NEWLINE);
            if(!itemSet.contains(item)){
                itemSet.insert(item);
                clipboardData.append(item);
            }
        }
        break;
    case CHOICE::ipaddress:
        for(int i = 0; i != resultsModel->active->rowCount(); ++i)
        {
            item = resultsModel->active->item(i, 1)->text().append(NEWLINE);
            if(!itemSet.contains(item)){
                itemSet.insert(item);
                clipboardData.append(item);
            }
        }
        break;
    case CHOICE::all:
        for(int i = 0; i != resultsModel->active->rowCount(); ++i)
        {
            item = resultsModel->active->item(i, 0)->text()+"|"+resultsModel->active->item(i, 1)->text().append(NEWLINE);
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

void Active::onCopyResults(QItemSelectionModel *selectionModel){
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
