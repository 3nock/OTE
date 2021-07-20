#include "Active.h"
#include "ui_Active.h"

Active::Active(QWidget *parent, ResultsModel *resultsModel) : BaseClass(parent), ui(new Ui::Active),
    m_resultsModel(resultsModel),
    //...
    m_scanStatus(new ScanStatus),
    m_scanConfig(new ScanConfig),
    m_scanArguments(new ScanArguments_Active)
{
    ui->setupUi(this);
    //...
    ui->lineEdit_targets->setPlaceholderText(("Enter a new target..."));
    ui->progressBar->hide();
    //...
    ui->pushButton_stop->setDisabled(true);
    ui->pushButton_pause->setDisabled(true);
    //...
    m_resultsModel->active->setHorizontalHeaderLabels({"Subdomain Name:", "IpAddress"});
    ui->tableView_results->setModel(m_resultsModel->active);
    //...
    ui->splitter_3->setSizes(QList<int>()<<160<<1);
    //...
    m_scanArguments->targetList = ui->listWidget_targets;
}
Active::~Active(){
    delete m_scanStatus;
    delete m_scanConfig;
    delete m_scanArguments;
    //...
    delete ui;
}

/******************************* Start **************************************/
void Active::on_pushButton_start_clicked(){
    ///
    /// checking if all requirements are satisfied before scan if not prompt error
    /// then exit function...
    ///
    if(!(ui->listWidget_targets->count() > 0)){
        QMessageBox::warning(this, TITLE_ERROR, "Please Enter the subdomains Wordlist for Enumeration!");
        return;
    }
    ///
    /// disabling and Enabling widgets...
    ///
    ui->pushButton_start->setDisabled(true);
    ui->pushButton_pause->setEnabled(true);
    ui->pushButton_stop->setEnabled(true);
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
    if(ui->comboBox_option->currentIndex() == ACTIVE::DNS){
        m_scanArguments->checkActiveService = false;
    }
    if(ui->comboBox_option->currentIndex() == ACTIVE::HTTP){
        m_scanArguments->checkActiveService = true;
        m_scanArguments->service = 80;
    }
    if(ui->comboBox_option->currentIndex() == ACTIVE::HTTPS){
        m_scanArguments->checkActiveService = true;
        m_scanArguments->service = 443;
    }
    if(ui->comboBox_option->currentIndex() == ACTIVE::FTP){
        m_scanArguments->checkActiveService = true;
        m_scanArguments->service = 21;
    }
    if(ui->comboBox_option->currentIndex() == ACTIVE::SMTP){
        m_scanArguments->checkActiveService = true;
        m_scanArguments->service = 587;
    }
    ui->progressBar->setMaximum(ui->listWidget_targets->count());
    ///
    /// start active subdomain enumeration...
    ///
    startScan();
    //...
    sendStatus("[*] Testing For Active Subdomains...");
    logs("[START] Testing For Active Subdomains...");
}

/************************************* STOP/PAUSE *************************************/
void Active::on_pushButton_pause_clicked(){
    ///
    /// if the scan was already paused, then this current click is to
    /// Resume the scan, just call the startScan, with the same arguments and
    /// it will continue at where it ended...
    ///
    if(m_scanStatus->isPaused){
        ui->pushButton_pause->setText("Pause");
        m_scanStatus->isPaused = false;
        //...
        startScan();
        //...
        sendStatus("[START] Resumed Subdomain Enumeration!");
        logs("[START] Resumed Subdomain Enumeration!");
    }
    else
    {
        m_scanStatus->isPaused = true;
        emit stop();
    }
}

void Active::on_pushButton_stop_clicked(){
    emit stop();
    m_scanStatus->isStopped = true;
}

void Active::startScan(){
    ///
    /// if the numner of threads is greater than the number of wordlists, set the
    /// number of threads to use to the number of wordlists available to avoid
    /// creating more threads than needed...
    ///
    int wordlistCount = ui->listWidget_targets->count();
    int threadsCount = m_scanConfig->threadsCount;
    if(threadsCount > wordlistCount)
    {
        threadsCount = wordlistCount;
    }
    m_activeThreads = threadsCount;
    ///
    /// loop to create threads for enumeration...
    ///
    for(int i = 0; i < threadsCount; i++)
    {
        ActiveEnumerator *Enumerator = new ActiveEnumerator(m_scanConfig, m_scanArguments);
        QThread *cThread = new QThread;
        Enumerator->enumerate(cThread);
        Enumerator->moveToThread(cThread);
        //...
        connect(Enumerator, SIGNAL(scanResult(QString, QString)), this, SLOT(scanResult(QString, QString)));
        connect(Enumerator, SIGNAL(progress(int)), ui->progressBar, SLOT(setValue(int)));
        connect(Enumerator, SIGNAL(scanLog(QString)), this, SLOT(logs(QString)));
        connect(cThread, SIGNAL(finished()), this, SLOT(scanThreadEnded()));
        connect(cThread, SIGNAL(finished()), Enumerator, SLOT(deleteLater()));
        connect(cThread, SIGNAL(finished()), cThread, SLOT(deleteLater()));
        connect(this, SIGNAL(stop()), Enumerator, SLOT(onStop()));
        //...
        cThread->start();
    }
    m_scanStatus->isRunning = true;
}

void Active::scanResult(QString subdomain, QString ipAddress){
    m_resultsModel->active->setItem(m_resultsModel->active->rowCount(), 0, new QStandardItem(subdomain));
    m_resultsModel->active->setItem(m_resultsModel->active->rowCount()-1, 1, new QStandardItem(ipAddress));
    ui->label_resultsCount->setNum(m_resultsModel->active->rowCount());
}

void Active::scanThreadEnded(){
    m_activeThreads--;
    ///
    /// if all Scan Threads have finished...
    ///
    if(m_activeThreads == 0)
    {
        if(m_scanStatus->isPaused)
        {
            ui->pushButton_pause->setText("Resume");
            m_scanStatus->isRunning = false;
            //...
            sendStatus("[*] Scan Paused!");
            logs("[*] Scan Paused!\n");
            return;
        }
        else
        {
            m_scanStatus->isPaused = false;
            m_scanStatus->isStopped = false;
            m_scanStatus->isRunning = false;
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

/********************* When User Changes options For Active Scan *************************/
void Active::on_comboBox_option_currentIndexChanged(int index){
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

void Active::on_toolButton_config_clicked(){
    ConfigDialog *scanConfig = new ConfigDialog(this, m_scanConfig);
    scanConfig->setAttribute( Qt::WA_DeleteOnClose, true );
    scanConfig->show();
}

void Active::on_pushButton_get_clicked(){

}

void Active::on_pushButton_loadTargets_clicked(){
    QString filename = QFileDialog::getOpenFileName(this, INFO_LOADFILE, CURRENT_PATH);
    if(!filename.isEmpty()){
        QFile file(filename);
        if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
            QTextStream in(&file);
            while (!in.atEnd()){
                ui->listWidget_targets->addItem(in.readLine());
            }
            ui->label_targetsCount->setNum(ui->listWidget_targets->count());
            file.close();
        }else{
            QMessageBox::warning(this, TITLE_ERROR, "Failed To Open the File!");
        }
    }
}

void Active::on_pushButton_addTargets_clicked(){
    if(ui->lineEdit_targets->text() != EMPTY){
        ui->listWidget_targets->addItem(ui->lineEdit_targets->text());
        ui->lineEdit_targets->clear();
        ui->label_targetsCount->setNum(ui->listWidget_targets->count());
    }
}

void Active::on_lineEdit_targets_returnPressed(){
    on_pushButton_addTargets_clicked();
}

void Active::on_pushButton_clearTargets_clicked(){
    ui->listWidget_targets->clear();
    ui->label_targetsCount->clear();
}

void Active::on_pushButton_removeTargets_clicked(){
    if(ui->listWidget_targets->selectedItems().count()){
        qDeleteAll(ui->listWidget_targets->selectedItems());
    }
    ui->label_targetsCount->setNum(ui->listWidget_targets->selectedItems().count());
}

void Active::on_pushButton_clearResults_clicked(){
    ///
    /// if the current tab is subdomains clear subdomains...
    ///
    if(ui->tabWidget_results->currentIndex() == 0)
    {
        m_resultsModel->active->clear();
        ui->label_resultsCount->clear();
        m_resultsModel->active->setHorizontalHeaderLabels({"Subdomain Name:", "IpAddress"});
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
        ui->listWidget_logs->clear();
    }
}

void Active::logs(QString log){
    sendLog(log);
    ui->listWidget_logs->addItem(log);
    if(log.startsWith("[ERROR]"))
    {
        ui->listWidget_logs->item(ui->listWidget_logs->count()-1)->setForeground(Qt::red);
        return;
    }
    if(log.startsWith("[START]") || log.startsWith("[END]"))
    {
        ui->listWidget_logs->item(ui->listWidget_logs->count()-1)->setFont(QFont("MS Shell Dlg 2", 8, QFont::Bold));
        return;
    }
}

void Active::receiveTargets(ENGINE engineName){
    if(engineName == ENGINE::BRUTE){
        for(char i = 0; i < m_resultsModel->brute->rowCount(); i++)
        {
            ui->listWidget_targets->addItem(m_resultsModel->brute->item(i, 0)->text());
        }
    }
    ui->label_targetsCount->setNum(ui->listWidget_targets->count());
}

void Active::on_radioButton_hostname_clicked(){

}
void Active::on_radioButton_ip_clicked(){

}

void Active::on_pushButton_action_clicked(){
    ///
    /// getting the position of the action button to place the context menu and
    /// showing the context menu right by the side of the action button...
    ///
    QPoint pos = ui->pushButton_action->mapToGlobal(QPoint(0,0));
    //...
    QMenu *contextMenu_actionButton = new QMenu(this);
    contextMenu_actionButton->setAttribute( Qt::WA_DeleteOnClose, true );
    contextMenu_actionButton->setObjectName("actionButtonMenu");
    //...
    QAction actionSendToIp("Send IpAddresses To Ip");
    QAction actionSendToActive("Send Subdomains To Active");
    QAction actionSendToBrute("Send Subdomains To Brute");
    QAction actionSendToSave("Send Subdomains To Save");
    QAction actionSendToLevel("Send Subdomains To Level");
    QAction actionSendToRecords("Send Subdomains To Records");
    //...
    connect(&actionSendToIp, SIGNAL(triggered()), this, SLOT(actionSendToIp(ENGINE::ACTION)));
    connect(&actionSendToSave, SIGNAL(triggered()), this, SLOT(actionSendToSave(ENGINE::ACTION)));
    connect(&actionSendToBrute, SIGNAL(triggered()), this, SLOT(actionSendToBrute(ENGINE::ACTION)));
    connect(&actionSendToActive, SIGNAL(triggered()), this, SLOT(actionSendToActive(ENGINE::ACTION)));
    connect(&actionSendToRecords, SIGNAL(triggered()), this, SLOT(actionSendToRecords(ENGINE::ACTION)));
    connect(&actionSendToLevel, SIGNAL(triggered()), this, SLOT(actionSendToLevel(ENGINE::ACTION)));
    //...
    contextMenu_actionButton->addAction(&actionSendToIp);
    contextMenu_actionButton->addAction(&actionSendToBrute);
    contextMenu_actionButton->addAction(&actionSendToActive);
    contextMenu_actionButton->addAction(&actionSendToRecords);
    contextMenu_actionButton->addAction(&actionSendToLevel);
    contextMenu_actionButton->addAction(&actionSendToSave);
    //...
    contextMenu_actionButton->move(QPoint(pos.x()+76, pos.y()));
    contextMenu_actionButton->exec();;
}

void Active::on_tableView_results_customContextMenuRequested(const QPoint &pos){
    Q_UNUSED(pos);
    ///
    /// check if user right clicked on items else dont show the context menu...
    ///
    if(!ui->tableView_results->selectionModel()->isSelected(ui->tableView_results->currentIndex())){
        return;
    }
    ///
    /// getting the position of the cursor to place the context menu...
    ///
    QPoint globalCursorPos = QCursor::pos();
    QRect mouseScreenGeometry = qApp->desktop()->screen(qApp->desktop()->screenNumber(globalCursorPos))->geometry();
    QPoint localCursorPosition = globalCursorPos - mouseScreenGeometry.topLeft();
    //...
    QMenu *contextMenu_rightClick = new QMenu(this);
    contextMenu_rightClick->setAttribute( Qt::WA_DeleteOnClose, true );
    contextMenu_rightClick->setObjectName("rightClickMenu");
    //...
    QAction actionSendToSave("Send Selected To Save", this);
    QAction actionSendToRecords("Send Selected To Records");
    QAction actionOpenInBrowser("Open Selected in Browser");
    //...
    connect(&actionOpenInBrowser, SIGNAL(triggered()), this, SLOT(cursorOpenInBrowser()));
    connect(&actionSendToSave, SIGNAL(triggered()), this, SLOT(cursorSendToSave()));
    connect(&actionSendToRecords, SIGNAL(triggered()), this, SLOT(cursorSendToRecords()));
    //...
    contextMenu_rightClick->addAction(&actionOpenInBrowser);
    contextMenu_rightClick->addSeparator();
    contextMenu_rightClick->addAction(&actionSendToRecords);
    contextMenu_rightClick->addAction(&actionSendToSave);
    //...
    contextMenu_rightClick->move(localCursorPosition);
    contextMenu_rightClick->exec();
}

/*********************************************************************************/
void Active::cursorOpenInBrowser(){
    foreach(const QModelIndex &index, ui->tableView_results->selectionModel()->selectedIndexes()){
        QDesktopServices::openUrl(QUrl("https://"+index.data().toString(), QUrl::TolerantMode));
    }
}

