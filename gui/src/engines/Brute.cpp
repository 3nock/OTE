#include "Brute.h"
#include "ui_Brute.h"
//...
#include <QDateTime>
#include <QClipboard>

/*
 * option to resolve both A & AAAA, 3 models, one for A one for AAAA & one for A&AAAA
 *
 * {"www", "online", "webserver", "ns", "ns1", "mail", "smtp", "webmail", "shop", "dev",
            "prod", "test", "vpn", "ftp", "ssh", "secure", "whm", "admin", "webdisk", "mobile",
            "remote", "server", "cpanel", "cloud", "autodiscover", "api", "m", "blog"}

 * homoglyphs - in wordlist creation...
 * many sites are created from homoglyphs of existing ones
 * eg example.com ==> exannple.com, exampl3.com, exqmple.com, exampls.com, exampke.com
 *
 * use a hash-table instead of QSet to to save already present data
 *
 * have a tab to put subdomainIp info/issue definitions,eg asn numbers and their respective
 * owner, ip's and their respective owners...
 *
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

  add an advanced option that resolves an item on wordlist with subdomainIp nameservers..
  first obtains the server from the domain name, then uses those nameservers as default nameservers
  for the scan...

*/

/*

  add a function to first test if the domain you are testing is an active one
  to reduce un-necessary lookups

*/

/*

 create a notes tab that takes subdomainIp notes for the related scans of the project...
 each new domain enumerated and its x-stics are to be recorded...
 use of wildcard in each phase...
*/

/*
 * have seperate stylesheet for each os, choose appropriate stylesheet during loading time
 * using macro which indicates what os is compiled in for better visuals...
 *
 * must be project base, on launch asks if you want a quick scan or launch a project base...
 *
 *
    "8.8.8.8",        // Google
    "1.1.1.1",        // Cloudflare
    "9.9.9.9",        // Quad9
    "208.67.222.222", // Cisco OpenDNS
    "84.200.69.80",   // DNS.WATCH
    "64.6.64.6",      // Verisign
    "8.26.56.26",     // Comodo Secure DNS
    "64.6.64.6",      // Neustar DNS
    "195.46.39.39",   // SafeDNS
    "185.228.168.9",  // CleanBrowsing
    "76.76.19.19",    // Alternate DNS
    "77.88.8.1",      // Yandex.DNS
    "94.140.14.140",  // AdGuard
    "216.146.35.35",  // Dyn
    "192.71.245.208", // OpenNIC
    "38.132.106.139", // CyberGhost
    "109.69.8.51",    // puntCAT
    "74.82.42.42",    // Hurricane Electric
 */

Brute::Brute(QWidget *parent, ResultsModel *resultsModel, ProjectDataModel *project, Status *status) :
    AbstractEngine(parent, resultsModel, project, status),
    ui(new Ui::Brute),
    m_scanArguments(new brute::ScanArguments)
{
    ui->setupUi(this);
    ///
    /// init...
    ///
    ui->targets->init("Targets");
    ui->wordlist->init("Wordlist");
    targets = ui->targets;
    scanConfig->name = tr("ScanConfig-Brute");
    ///
    /// ...
    ///
    ui->lineEditTarget->setPlaceholderText("eg. example.com");
    ui->lineEditFilter->setPlaceholderText("Enter filter...");
    ///
    /// ...
    ///
    ui->buttonStop->setDisabled(true);
    //ui->buttonPause->setDisabled(true);
    //...
    ui->buttonAction->hide();
    ui->targets->hide();
    ui->progressBar->hide();
    //...
    ui->lineEditFilter->hide();
    ui->buttonFilter->hide();
    ui->comboBoxFilter->hide();
    ///
    /// equsubdomainIpy seperate the widgets...
    ///
    ui->splitter->setSizes(QList<int>() << static_cast<int>((this->width() * 0.50))
                                        << static_cast<int>((this->width() * 0.50)));
    //...
    result->brute->subdomainIp->setHorizontalHeaderLabels({"Subdomain", "IpAddress"});
    ui->tableViewResults->setModel(result->brute->subdomainIpProxy);
    ///
    /// ...
    ///
    this->connectActions();
    result->brute->subdomainIpProxy->setFilterCaseSensitivity(Qt::CaseInsensitive);
    result->brute->subdomainIpProxy->setRecursiveFilteringEnabled(true);
}
Brute::~Brute(){
    delete m_scanArguments;
    //...
    delete ui;
}

void Brute::onInfoLog(QString log){
    QString logTime = QDateTime::currentDateTime().toString("hh:mm:ss  ");
    ui->plainTextEditLogs->appendPlainText(logTime.append(log));
}

void Brute::onErrorLog(QString log){
    QString fontedLog;
    fontedLog.append("<font color=\"red\">").append(log).append("</font>");
    QString logTime = QDateTime::currentDateTime().toString("hh:mm:ss  ");
    ui->plainTextEditLogs->appendHtml(logTime.append(fontedLog));
}

void Brute::on_buttonStart_clicked(){
    ///
    /// checking if subdomainIp requirements are satisfied before scan if not prompt error
    /// then exit function...
    ///
    if(!ui->checkBoxMultipleTargets->isChecked() && ui->lineEditTarget->text().isEmpty()){
        QMessageBox::warning(this, "Error!", "Please Enter the Target for Enumeration!");
        return;
    }
    if(ui->checkBoxMultipleTargets->isChecked() && ui->targets->listModel->rowCount() < 1){
        QMessageBox::warning(this, "Error!", "Please Enter the Targets for Enumeration!");
        return;
    }
    if(ui->wordlist->listModel->rowCount() < 1){
        QMessageBox::warning(this, "Error!", "Please Enter the Wordlist for Enumeration!");
        return;
    }
    ///
    /// disabling & Enabling widgets...
    ///
    ui->buttonStart->setDisabled(true);
    //ui->buttonPause->setEnabled(true);
    ui->buttonStop->setEnabled(true);
    ui->progressBar->show();
    ///
    /// Resetting the scan arguments values...
    ///
    m_scanArguments->wordlist = ui->wordlist->listModel->stringList();
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
            ui->progressBar->setMaximum(ui->wordlist->listModel->rowCount());
        }
        if(ui->checkBoxMultipleTargets->isChecked())
        {
            m_scanArguments->targetList = ui->targets->listModel->stringList();
            ///
            /// for multiple targets, progress equals to the total number of wordlist times the total number of targets...
            ///
            ui->progressBar->setMaximum(ui->wordlist->listModel->rowCount()*m_scanArguments->targetList.count());
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
            ui->progressBar->setMaximum(ui->wordlist->listModel->rowCount());
        }
        if(ui->checkBoxMultipleTargets->isChecked())
        {
            m_scanArguments->targetList = ui->targets->listModel->stringList();
            ///
            /// for multiple targets, progress equals to the total number of wordlist times the total number of targets...
            ///
            ui->progressBar->setMaximum(ui->wordlist->listModel->rowCount()*m_scanArguments->targetList.count());
        }
    }
    ///
    /// Starting the scan...
    ///
    startScan();
    ui->buttonAction->show();
    //...
    sendStatus("[START] Started Subdomain Enumeration!");
}
void Brute::on_lineEditTarget_returnPressed(){
    on_buttonStart_clicked();
}

void Brute::stopScan(){

}

void Brute::pauseScan(){
    ///
    /// if the scan was already paused, then this current click is to
    /// Resume the scan, just csubdomainIp the startScan, with the same arguments and
    /// it will continue at where it ended...
    ///
    if(status->brute->isPaused){
        //ui->buttonPause->setText("Pause");
        status->brute->isPaused = false;
        //...
        startScan();
        //...
        sendStatus("[START] Resumed Subdomain Enumeration!");
    }
    else
    {
        status->brute->isPaused = true;
        emit stopScan();
    }
}

void Brute::resumeScan(){

}

void Brute::connectActions(){
    connect(&actionClearResults, &QAction::triggered, this, [=](){this->onClearResults();});
    connect(&actionShowFilter, &QAction::triggered, this, [=](){this->onShowFilter(true);});
    connect(&actionHideFilter, &QAction::triggered, this, [=](){this->onShowFilter(false);});
    ///
    /// SAVE...
    ///
    connect(&actionSaveSubdomains, &QAction::triggered, this, [=](){this->onSaveResults(CHOICE::subdomain);});
    connect(&actionSaveIpAddresses, &QAction::triggered, this, [=](){this->onSaveResults(CHOICE::ip);});
    connect(&actionSaveAll, &QAction::triggered, this, [=](){this->onSaveResults(CHOICE::subdomainIp);});
    ///
    /// COPY...
    ///
    connect(&actionCopySubdomains, &QAction::triggered, this, [=](){this->onCopyResults(CHOICE::subdomain);});
    connect(&actionCopyIpAddresses, &QAction::triggered, this, [=](){this->onCopyResults(CHOICE::ip);});
    connect(&actionCopyAll, &QAction::triggered, this, [=](){this->onCopyResults(CHOICE::subdomainIp);});
    ///
    /// SUBDOMAINS AND IPS...
    ///
    connect(&actionSendToIp, &QAction::triggered, this, [=](){emit sendIpAddressesToIp(ENGINE::BRUTE, CHOICE::ip, PROXYMODEL_TYPE::subdomainIpProxy); emit changeTabToIp();});
    connect(&actionSendToOsint, &QAction::triggered, this, [=](){emit sendSubdomainsToOsint(ENGINE::BRUTE, CHOICE::subdomain, PROXYMODEL_TYPE::subdomainIpProxy); emit changeTabToOsint();});
    connect(&actionSendToBrute, &QAction::triggered, this, [=](){emit sendSubdomainsToBrute(ENGINE::BRUTE, CHOICE::subdomain, PROXYMODEL_TYPE::subdomainIpProxy); emit changeTabToBrute();});
    connect(&actionSendToActive, &QAction::triggered, this, [=](){emit sendSubdomainsToActive(ENGINE::BRUTE, CHOICE::subdomain, PROXYMODEL_TYPE::subdomainIpProxy); emit changeTabToActive();});
    connect(&actionSendToRecords, &QAction::triggered, this, [=](){emit sendSubdomainsToRecord(ENGINE::BRUTE, CHOICE::subdomain, PROXYMODEL_TYPE::subdomainIpProxy); emit changeTabToRecords();});

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
        //ui->buttonPause->setDisabled(true);
        ui->buttonStop->setDisabled(true);
        //...
        sendStatus("[*] Enumeration Complete!");
    }
    status->brute->isStopped = true;
}

void Brute::startScan(){
    ///
    /// if the numner of threads is greater than the number of wordlists, set the
    /// number of threads to use to the number of wordlists available to avoid
    /// creating more threads than needed...
    ///
    int wordlistCount = ui->wordlist->listModel->rowCount();
    int threadsCount = scanConfig->threadsCount;
    if(threadsCount > wordlistCount)
    {
        threadsCount = wordlistCount;
    }
    status->brute->activeThreads = threadsCount;
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
        connect(scanner, &brute::Scanner::scanResult, this, &Brute::onScanResult);
        connect(scanner, &brute::Scanner::scanProgress, ui->progressBar, &QProgressBar::setValue);
        connect(scanner, &brute::Scanner::infoLog, this, &Brute::onInfoLog);
        connect(scanner, &brute::Scanner::errorLog, this, &Brute::onErrorLog);
        connect(cThread, &QThread::finished, this, &Brute::onScanThreadEnded);
        connect(cThread, &QThread::finished, scanner, &brute::Scanner::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        connect(this, &Brute::stopScanThread, scanner, &brute::Scanner::onStopScan);
        //...
        cThread->start();
    }
    status->brute->isRunning = true;
}

void Brute::onScanResult(QString subdomain, QString ipAddress, QString target){
    if(m_subdomainsSet.contains(subdomain)){
        return;
    }
    m_subdomainsSet.insert(subdomain);
    ///
    /// save to brute model...
    ///
    result->brute->subdomainIp->appendRow(QList<QStandardItem*>() <<new QStandardItem(subdomain) <<new QStandardItem(ipAddress));
    ui->labelResultsCount->setNum(result->brute->subdomainIp->rowCount());
    ///
    /// save to Project model...
    ///
    if(m_scanArguments->tldBrute){
        project->addActiveTLD(QStringList()<<subdomain<<ipAddress<<target);
    }
    if(m_scanArguments->subBrute){
        project->addActiveSubdomain(QStringList()<<subdomain<<ipAddress<<target);
    }
}

void Brute::onScanThreadEnded(){
    status->brute->activeThreads--;
    ///
    /// if subdomainIp Scan Threads have finished...
    ///
    if(status->brute->activeThreads == 0)
    {
        if(status->brute->isPaused)
        {
            //ui->buttonPause->setText("Resume");
            status->brute->isRunning = false;
            //...
            sendStatus("[*] Scan Paused!");
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
            //ui->buttonPause->setDisabled(true);
            ui->buttonStop->setDisabled(true);
            //...
            sendStatus("[*] Enumeration Complete!");
        }
    }
}

void Brute::on_buttonConfig_clicked(){
    ConfigDialog *configDialog = new ConfigDialog(this, scanConfig);
    configDialog->setAttribute( Qt::WA_DeleteOnClose, true );
    configDialog->show();
}

void Brute::onClearResults(){
    ///
    /// clear the results...
    ///
    m_subdomainsSet.clear();
    result->brute->subdomainIp->clear();
    ui->labelResultsCount->clear();
    result->brute->subdomainIp->setHorizontalHeaderLabels({"Subdomain", "IpAddress"});
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

void Brute::onShowFilter(bool show){
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
    connect(wordlistDialog, &WordListDialog::choosenWordlist, this, &Brute::onChoosenWordlist);
    wordlistDialog->show();
}

void Brute::onChoosenWordlist(QString wordlistFilename){
    QFile file(wordlistFilename);
    ui->wordlist->add(file);
}

void Brute::on_buttonAction_clicked(){
    ///
    /// check if there are results available else dont show the context menu...
    ///
    if(result->brute->subdomainIp->rowCount() < 1){
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

void Brute::on_tableViewResults_customContextMenuRequested(const QPoint &pos){
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

void Brute::on_checkBoxMultipleTargets_clicked(bool checked){
    if(checked)
        ui->targets->show();
    else
        ui->targets->hide();
}

void Brute::onSaveResults(CHOICE choice){
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
    case CHOICE::subdomain:
        for(int i = 0; i != result->brute->subdomainIpProxy->rowCount(); ++i)
        {
            item = result->brute->subdomainIpProxy->data(result->brute->subdomainIpProxy->index(i, 0)).toString().append(NEWLINE);
            if(!itemSet.contains(item)){
                itemSet.insert(item);
                file.write(item.toUtf8());
            }
        }
        break;
    case CHOICE::ip:
        for(int i = 0; i != result->brute->subdomainIpProxy->rowCount(); ++i)
        {
            item = result->brute->subdomainIpProxy->data(result->brute->subdomainIpProxy->index(i, 1)).toString().append(NEWLINE);
            if(!itemSet.contains(item)){
                itemSet.insert(item);
                file.write(item.toUtf8());
            }
        }
        break;
    case CHOICE::subdomainIp:
        for(int i = 0; i != result->brute->subdomainIpProxy->rowCount(); ++i)
        {
            item = result->brute->subdomainIpProxy->data(result->brute->subdomainIpProxy->index(i, 0)).toString()+":"+result->brute->subdomainIpProxy->data(result->brute->subdomainIpProxy->index(i, 0)).toString().append(NEWLINE);
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

void Brute::onSaveResults(QItemSelectionModel *selectionModel){
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

void Brute::onCopyResults(CHOICE choice){
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
    case CHOICE::subdomain:
        for(int i = 0; i != result->brute->subdomainIpProxy->rowCount(); ++i)
        {
            item = result->brute->subdomainIpProxy->data(result->brute->subdomainIpProxy->index(i, 0)).toString().append(NEWLINE);
            if(!itemSet.contains(item)){
                itemSet.insert(item);
                clipboardData.append(item);
            }
        }
        break;
    case CHOICE::ip:
        for(int i = 0; i != result->brute->subdomainIpProxy->rowCount(); ++i)
        {
            item = result->brute->subdomainIpProxy->data(result->brute->subdomainIpProxy->index(i, 1)).toString().append(NEWLINE);
            if(!itemSet.contains(item)){
                itemSet.insert(item);
                clipboardData.append(item);
            }
        }
        break;
    case CHOICE::subdomainIp:
        for(int i = 0; i != result->brute->subdomainIpProxy->rowCount(); ++i)
        {
            item = result->brute->subdomainIpProxy->data(result->brute->subdomainIpProxy->index(i, 0)).toString()+"|"+result->brute->subdomainIpProxy->data(result->brute->subdomainIpProxy->index(i, 1)).toString().append(NEWLINE);
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

void Brute::onCopyResults(QItemSelectionModel *selectionModel){
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

void Brute::on_buttonFilter_clicked(){
    QString filterKeyword = ui->lineEditFilter->text();
    result->brute->subdomainIpProxy->setFilterKeyColumn(ui->comboBoxFilter->currentIndex());
    result->brute->subdomainIpProxy->setFilterRegExp(filterKeyword);
    ui->tableViewResults->setModel(result->brute->subdomainIpProxy);
}
