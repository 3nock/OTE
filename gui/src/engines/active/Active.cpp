#include "Active.h"
#include "ui_Active.h"
//...
#include <QThread>


Active::Active(QWidget *parent, ResultsModel *resultsModel, Status *status) :
    BaseClass(ENGINE::ACTIVE, resultsModel, status, parent),
    ui(new Ui::Active),
    m_scanArguments(new active::ScanArguments)
{
    ui->setupUi(this);
    ///
    /// init...
    ///
    ui->targets->init("Targets");
    initBaseClass(ui->targets);
    scanConfig->name = tr("ScanConfig-Active");
    ///
    /// ...
    ///
    ui->progressBar->hide();
    //...
    ui->buttonStop->setDisabled(true);
    ui->buttonPause->setDisabled(true);
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
}
Active::~Active(){
    delete m_scanArguments;
    delete ui;
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
    //...
    sendStatus("[*] Testing For Active Subdomains...");
    logs("[START] Testing For Active Subdomains...");
}

void Active::on_buttonPause_clicked(){
    ///
    /// if the scan was already paused, then this current click is to
    /// Resume the scan, just call the startScan, with the same arguments and
    /// it will continue at where it ended...
    ///
    if(status->active->isPaused){
        ui->buttonPause->setText("Pause");
        status->active->isPaused = false;
        //...
        startScan();
        //...
        sendStatus("[START] Resumed Subdomain Enumeration!");
        logs("[START] Resumed Subdomain Enumeration!");
    }
    else
    {
        status->active->isPaused = true;
        emit stopScan();
    }
}

void Active::on_buttonStop_clicked(){
    emit stopScan();
    status->active->isStopped = true;
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
    activeThreads = threadsCount;
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
    status->active->isRunning = true;
}

void Active::scanResult(QString subdomain, QString ipAddress){
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

void Active::scanThreadEnded(){
    activeThreads--;
    ///
    /// if all Scan Threads have finished...
    ///
    if(activeThreads == 0)
    {
        if(status->active->isPaused)
        {
            ui->buttonPause->setText("Resume");
            status->active->isRunning = false;
            //...
            sendStatus("[*] Scan Paused!");
            logs("[*] Scan Paused!\n");
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
            ui->buttonPause->setDisabled(true);
            ui->buttonStop->setDisabled(true);
            //...
            sendStatus("[*] Enumeration Complete!");
            logs("[END] Enumeration Complete!\n");
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

void Active::on_buttonClearResults_clicked(){
    ///
    /// if the current tab is subdomains clear subdomains...
    ///
    if(ui->tabWidgetResults->currentIndex() == 0)
    {
        resultsModel->active->clear();
        ui->labelResultsCount->clear();
        resultsModel->active->setHorizontalHeaderLabels({"Subdomain", "IpAddress"});
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
    a_Menu->exec(QPoint(pos.x()+76, pos.y()));
}

void Active::on_tableViewResults_customContextMenuRequested(const QPoint &pos){
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

void Active::on_checkBoxCustomActive_clicked(bool checked){
    if(checked){
        ui->frameDefault->hide();
        ui->frameCustom->show();
    }else{
        ui->frameCustom->hide();
        ui->frameDefault->show();
    }
}
