#include "Ip.h"
#include "ui_Ip.h"

Ip::Ip(QWidget *parent, ResultsModel *resultsModel) : BaseClass(parent), ui(new Ui::Ip),
    m_resultsModel(resultsModel),
    //...
    m_scanStatus(new ScanStatus),
    m_scanConfig(new ScanConfig),
    m_scanArguments(new ScanArguments_Ip)
{
    ui->setupUi(this);
    //...
    ui->lineEdit_targets->setPlaceholderText(("Enter a new target..."));
    ui->progressBar->hide();
    //...
    ui->pushButton_stop->setDisabled(true);
    ui->pushButton_pause->setDisabled(true);
    //...
    m_resultsModel->ip->setHorizontalHeaderLabels({"IpAddress:", "HostName:"});
    ui->tableView_results->setModel(m_resultsModel->ip);
    //...
    ui->splitter_3->setSizes(QList<int>()<<160<<1);
    //...
    m_scanArguments->label_resultsCount = ui->label_resultsCount;
    m_scanArguments->targetList = ui->listWidget_targets;
    m_scanArguments->model_results = m_resultsModel->ip;
}
Ip::~Ip(){
    delete m_scanStatus;
    delete m_scanConfig;
    delete m_scanArguments;
    delete m_resultsModel->ip;
    //...
    delete ui;
}

/******************************* Start **************************************/
void Ip::on_pushButton_start_clicked(){
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
    ui->progressBar->setMaximum(ui->listWidget_targets->count());
    ///
    /// start Ip subdomain enumeration...
    ///
    startScan();
    //...
    sendStatus("[*] Testing For Ip Subdomains...");
    logs("[START] Testing For Ip Subdomains...");
}

/************************************* STOP/PAUSE *************************************/
void Ip::on_pushButton_pause_clicked(){
    ///
    /// if the scan was already paused, then this current click is to
    /// Resume the scan, just call the startScan, with the same arguments and
    /// it will continue at where it ended...
    ///
    if(m_scanStatus->isPaused)
    {
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

void Ip::on_pushButton_stop_clicked(){
    emit stop();
    m_scanStatus->isStopped = true;
}

void Ip::startScan(){
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
        IpEnumerator *Enumerator = new IpEnumerator(m_scanConfig, m_scanArguments);
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

void Ip::scanThreadEnded(){
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

void Ip::scanResult(QString subdomain, QString ipAddress){
    m_scanArguments->model_results->setItem(m_scanArguments->model_results->rowCount(), 0, new QStandardItem(ipAddress));
    m_scanArguments->model_results->setItem(m_scanArguments->model_results->rowCount()-1, 1, new QStandardItem(subdomain));
    //...
    m_scanArguments->label_resultsCount->setNum(m_scanArguments->model_results->rowCount());
}

void Ip::on_toolButton_config_clicked(){
    ConfigDialog *scanConfig = new ConfigDialog(this, m_scanConfig);
    scanConfig->setAttribute( Qt::WA_DeleteOnClose, true );
    scanConfig->show();
}

void Ip::on_pushButton_loadTargets_clicked(){
    QString filename = QFileDialog::getOpenFileName(this, INFO_LOADFILE, CURRENT_PATH);
    if(!filename.isEmpty()){
        QFile file(filename);
        if(file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QTextStream in(&file);
            while (!in.atEnd()){
                ui->listWidget_targets->addItem(in.readLine());
            }
            ui->label_targetsCount->setNum(ui->listWidget_targets->count());
            file.close();
        }
        else
        {
            QMessageBox::warning(this, TITLE_ERROR, "Failed To Open the File!");
        }
    }
}

void Ip::on_pushButton_addTargets_clicked(){
    if(ui->lineEdit_targets->text() != EMPTY)
    {
        ui->listWidget_targets->addItem(ui->lineEdit_targets->text());
        ui->lineEdit_targets->clear();
        ui->label_targetsCount->setNum(ui->listWidget_targets->count());
    }
}

void Ip::on_lineEdit_targets_returnPressed(){
    on_pushButton_addTargets_clicked();
}

void Ip::on_pushButton_clearTargets_clicked(){
    ui->listWidget_targets->clear();
    ui->label_targetsCount->clear();
}

void Ip::on_pushButton_removeTargets_clicked(){
    if(ui->listWidget_targets->selectedItems().count()){
        qDeleteAll(ui->listWidget_targets->selectedItems());
    }
    ui->label_targetsCount->setNum(ui->listWidget_targets->selectedItems().count());
}

void Ip::on_pushButton_clearResults_clicked(){
    ///
    /// if the current tab is subdomains clear subdomains...
    ///
    if(ui->tabWidget_results->currentIndex() == 0)
    {
        m_resultsModel->ip->clear();
        ui->label_resultsCount->clear();
        m_resultsModel->ip->setHorizontalHeaderLabels({"IpAddress:", "HostName:"});
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

void Ip::logs(QString log){
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

void Ip::a_receiveTargets(ENGINE engineName){
    QStandardItemModel *model;
    //...
    if(engineName == ENGINE::BRUTE){
        model = m_resultsModel->brute;
    }
    if(engineName == ENGINE::ACTIVE){
        model = m_resultsModel->active;
    }
    if(engineName == ENGINE::RECORDS){
        model = m_resultsModel->record;
    }
    if(engineName == ENGINE::IP){
        model = m_resultsModel->ip;
    }
    if(engineName == ENGINE::LEVEL){
        model = m_resultsModel->level;
    }
    //...
    for(char i = 0; i < model->rowCount(); i++){
        ui->listWidget_targets->addItem(model->item(i, 1)->text());
    }
    ui->label_targetsCount->setNum(ui->listWidget_targets->count());
}

void Ip::c_receiveTargets(QItemSelectionModel *selectionModel){
    // iterate and open each selected and append on the target's listwidget...
    foreach(const QModelIndex &index, selectionModel->selectedIndexes()){
        ui->listWidget_targets->addItem(index.data().toString());
    }
    ui->label_targetsCount->setNum(ui->listWidget_targets->count());
}

void Ip::on_radioButton_hostname_clicked(){

}
void Ip::on_radioButton_ip_clicked(){

}

void Ip::on_comboBox_option_currentIndexChanged(int index){
    Q_UNUSED(index);
}

void Ip::on_pushButton_action_clicked(){
    ///
    /// check if there are results available else dont show the context menu...
    ///
    if(m_resultsModel->ip->rowCount() < 1){
        return;
    }
    ///
    /// getting the position of the action button to place the context menu and
    /// showing the context menu right by the side of the action button...
    ///
    QPoint pos = ui->pushButton_action->mapToGlobal(QPoint(0,0));
    contextMenu_actionButton(ENGINE::IP, pos);
}

void Ip::on_tableView_results_customContextMenuRequested(const QPoint &pos){
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
    contextMenu_rightClick(ui->tableView_results->selectionModel(), localCursorPosition);
}

void Ip::on_pushButton_get_clicked(){

}
