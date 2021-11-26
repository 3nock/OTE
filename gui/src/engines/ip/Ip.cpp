#include "Ip.h"
#include "ui_Ip.h"
//...
#include <QDateTime>
#include <QClipboard>

Ip::Ip(QWidget *parent, ResultsModel *resultsModel, ProjectDataModel *project, Status *status) :
    AbstractEngine(parent, resultsModel, project, status),
    ui(new Ui::Ip),
    m_scanArguments(new ipEngine::ScanArguments)
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
    //...
    ui->lineEditFilter->hide();
    ui->buttonFilter->hide();
    ui->comboBoxFilter->hide();
    //...
    ui->buttonStop->setDisabled(true);
    //ui->buttonPause->setDisabled(true);
    //...
    ui->lineEditFilter->setPlaceholderText("Enter filter...");
    result->ip->subdomainIp->setHorizontalHeaderLabels({"IpAddress:", "HostName:"});
    ui->tableViewResults->setModel(result->ip->subdomainIp);
    ///
    /// equsubdomainIpy seperate the widgets...
    ///
    ui->splitter->setSizes(QList<int>() << static_cast<int>((this->width() * 0.50))
                                        << static_cast<int>((this->width() * 0.50)));
    ///
    /// ...
    ///
    this->connectActions();
    result->ip->subdomainIpProxy->setFilterCaseSensitivity(Qt::CaseInsensitive);
    result->ip->subdomainIpProxy->setRecursiveFilteringEnabled(true);
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
    /// checking if subdomainIp requirements are satisfied before scan if not prompt error
    /// then exit function...
    ///
    if(!(ui->targets->listModel->rowCount() > 0)){
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
    m_scanArguments->targetList = ui->targets->listModel->stringList();
    m_scanArguments->currentTargetToEnumerate = 0;
    m_scanArguments->progress = 0;
    ui->progressBar->reset();
    ///
    /// Getting scan arguments....
    ///
    ui->progressBar->setMaximum(ui->targets->listModel->rowCount());
    ///
    /// start Ip subdomain enumeration...
    ///
    startScan();
    //...
    sendStatus("[*] Testing For Ip Subdomains...");
}

void Ip::stopScan(){

}

void Ip::pauseScan(){
    ///
    /// if the scan was already paused, then this current click is to
    /// Resume the scan, just csubdomainIp the startScan, with the same arguments and
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
    int wordlistCount = ui->targets->listModel->rowCount();
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
        ipEngine::Scanner *scanner = new ipEngine::Scanner(scanConfig, m_scanArguments);
        QThread *cThread = new QThread;
        scanner->startScan(cThread);
        scanner->moveToThread(cThread);
        //...
        connect(scanner, &ipEngine::Scanner::scanResult, this, &Ip::onScanResult);
        connect(scanner, &ipEngine::Scanner::scanProgress, ui->progressBar, &QProgressBar::setValue);
        connect(scanner, &ipEngine::Scanner::infoLog, this, &Ip::onInfoLog);
        connect(scanner, &ipEngine::Scanner::errorLog, this, &Ip::onErrorLog);
        connect(cThread, &QThread::finished, this, &Ip::onScanThreadEnded);
        connect(cThread, &QThread::finished, scanner, &ipEngine::Scanner::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        connect(this, &Ip::stopScanThread, scanner, &ipEngine::Scanner::onStopScan);
        //...
        cThread->start();
    }
    status->ip->isRunning = true;
}

void Ip::onScanThreadEnded(){
    status->ip->activeThreads--;
    ///
    /// if subdomainIp Scan Threads have finished...
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
    result->ip->subdomainIp->appendRow(QList<QStandardItem*>() <<new QStandardItem(ipAddress) <<new QStandardItem(subdomain));
    ui->labelResultsCount->setNum(result->ip->subdomainIp->rowCount());
    ///
    /// save to project model...
    ///
    project->addActiveSubdomain(QStringList()<<subdomain<<ipAddress<<subdomain);
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
    result->ip->subdomainIp->clear();
    ui->labelResultsCount->clear();
    result->ip->subdomainIp->setHorizontalHeaderLabels({"IpAddress", "HostName"});
    ///
    /// clear the progressbar...
    ui->progressBar->clearMask();
    ui->progressBar->reset();
    ui->progressBar->hide();
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

void Ip::on_buttonFilter_clicked(){
    QString filterKeyword = ui->lineEditFilter->text();
    result->ip->subdomainIpProxy->setFilterKeyColumn(ui->comboBoxFilter->currentIndex());
    result->ip->subdomainIpProxy->setFilterRegExp(filterKeyword);
    ui->tableViewResults->setModel(result->ip->subdomainIpProxy);
}
