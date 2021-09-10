#include "Ip.h"
#include "ui_Ip.h"

Ip::Ip(QWidget *parent, ResultsModel *resultsModel) : BaseClass(ENGINE::IP, resultsModel, parent), ui(new Ui::Ip),
    m_scanArguments(new ip::ScanArguments)
{
    ui->setupUi(this);
    ///
    /// init...
    ///
    ui->targets->init("Target Ip");
    initBaseClass(ui->targets);
    scanConfig->name = tr("ScanConfig-Ip");
    ///
    /// ...
    ///
    ui->progressBar->hide();
    //...
    ui->buttonStop->setDisabled(true);
    ui->buttonPause->setDisabled(true);
    //...
    resultsModel->ip->setHorizontalHeaderLabels({"IpAddress:", "HostName:"});
    ui->tableViewResults->setModel(resultsModel->ip);
    ///
    /// equally seperate the widgets...
    ///
    ui->splitter->setSizes(QList<int>() << static_cast<int>((this->width() * 0.50))
                                        << static_cast<int>((this->width() * 0.50)));
    //...
    m_scanArguments->label_resultsCount = ui->labelResultsCount;
    m_scanArguments->targetList = ui->targets->listWidget;
    m_scanArguments->model_results = resultsModel->ip;
}
Ip::~Ip(){
    delete m_scanArguments;
    //...
    delete ui;
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
    ui->buttonPause->setEnabled(true);
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
    //...
    sendStatus("[*] Testing For Ip Subdomains...");
    logs("[START] Testing For Ip Subdomains...");
}

void Ip::on_buttonPause_clicked(){
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

void Ip::on_buttonStop_clicked(){
    emit stopScan();
    scanStatus->isStopped = true;
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
    activeThreads = threadsCount;
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
        connect(scanner, SIGNAL(scanResult(QString, QString)), this, SLOT(scanResult(QString, QString)));
        connect(scanner, SIGNAL(scanProgress(int)), ui->progressBar, SLOT(setValue(int)));
        connect(scanner, SIGNAL(scanLog(QString)), this, SLOT(logs(QString)));
        connect(cThread, SIGNAL(finished()), this, SLOT(scanThreadEnded()));
        connect(cThread, SIGNAL(finished()), scanner, SLOT(deleteLater()));
        connect(cThread, SIGNAL(finished()), cThread, SLOT(deleteLater()));
        connect(this, SIGNAL(stopScan()), scanner, SLOT(stopScan()));
        //...
        cThread->start();
    }
    scanStatus->isRunning = true;
}

void Ip::scanThreadEnded(){
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

void Ip::scanResult(QString subdomain, QString ipAddress){
    ///
    /// save to ip model model...
    ///
    resultsModel->ip->appendRow(QList<QStandardItem*>() <<new QStandardItem(ipAddress) <<new QStandardItem(subdomain));
    m_scanArguments->label_resultsCount->setNum(m_scanArguments->model_results->rowCount());
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

void Ip::on_buttonClear_clicked(){
    ///
    /// if the current tab is subdomains clear subdomains...
    ///
    if(ui->tabWidgetResults->currentIndex() == 0)
    {
        resultsModel->ip->clear();
        ui->labelResultsCount->clear();
        resultsModel->ip->setHorizontalHeaderLabels({"IpAddress:", "HostName:"});
        //...
        ui->progressBar->clearMask();
        ui->progressBar->reset();
        ui->progressBar->hide();
    }
    ///
    /// if current tab is logs-tab clear logs...
    ///
    else
    {
        ui->listWidgetLogs->clear();
    }
}

void Ip::on_comboBoxOption_currentIndexChanged(int index){
    Q_UNUSED(index);
}

void Ip::on_buttonAction_clicked(){
    ///
    /// check if there are results available else dont show the context menu...
    ///
    if(resultsModel->ip->rowCount() < 1){
        return;
    }
    ///
    /// getting the position of the action button to place the context menu and
    /// showing the context menu right by the side of the action button...
    ///
    QPoint pos = ui->buttonAction->mapToGlobal(QPoint(0,0));
    a_Menu->exec(QPoint(pos.x()+76, pos.y()));
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
    c_Menu->exec(QCursor::pos());
}
