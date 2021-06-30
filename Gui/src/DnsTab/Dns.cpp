#include "Dns.h"
#include "ui_Dns.h"


/*************************************************************************************
                            Constructor & Destructor
*************************************************************************************/

Dns::Dns(QWidget *parent) : QDialog(parent), ui(new Ui::Dns),
      m_model(new QStandardItemModel),
      m_model_srv(new QStandardItemModel),
      m_rootItem(m_model->invisibleRootItem()),
      //...
      m_scanArguments(new ScanArguments_Records),
      m_scanResults(new ScanResults_Records)
{
    ui->setupUi(this);
    //...
    m_scanResults->rootItem = m_rootItem;
    m_scanResults->resultsCountLabel = ui->label_resultsCount;
    //...
    ui->treeView_results->setModel(m_model);
    //...
    ui->lineEdit_targets->setPlaceholderText("Enter Target domains/subdomains...");
    ui->lineEdit_srvWordlist->setPlaceholderText("Enter new SRV...");
    //...
    ui->pushButton_stop->setDisabled(true);
    ui->pushButton_pause->setDisabled(true);
    //...
    ui->frame_srvWordlist->hide();
    ui->progressBar->hide();
    //...
    m_scanArguments->targetList = ui->listWidget_targets;
    m_scanArguments->srvWordlist = ui->listWidget_srvWordlist;
    ///
    /// Setting highlight Color for items on the TreeView...
    ///
    QPalette p = palette();
    p.setColor(QPalette::Highlight, QColor(188, 188, 141));
    p.setColor(QPalette::HighlightedText, QColor(Qt::black));
    ui->treeView_results->setPalette(p);
    ui->tableView_srv->setPalette(p);
    ///
    /// setting the splitter to the middle...
    ///
    ui->splitter->setSizes(QList<int>()<<180<<1);
    //...
    loadSrvWordlist();
}
Dns::~Dns(){
    delete m_scanArguments;
    delete m_scanResults;
    delete m_model;
    //...
    delete ui;
}

/**************************************************************************************
                                      Scan
**************************************************************************************/
void Dns::on_pushButton_start_clicked(){
    if(ui->listWidget_targets->count() < 1){
        QMessageBox::warning(this, TITLE_ERROR, "Please Enter Target Subdomains For Enumeration!");
        return;
    }
    ///
    /// getting the arguments for the Scan...
    ///
    if(ui->comboBox_option->currentIndex() == 0)
    {
        m_scanArguments->choiceCount = 0;
        if(ui->checkBox_A->isChecked()){
            m_scanArguments->RecordType_a = true;
            m_scanArguments->choiceCount++;
        }
        if(ui->checkBox_AAAA->isChecked()){
            m_scanArguments->RecordType_aaaa = true;
            m_scanArguments->choiceCount++;
        }
        if(ui->checkBox_MX->isChecked()){
            m_scanArguments->RecordType_mx = true;
            m_scanArguments->choiceCount++;
        }
        if(ui->checkBox_NS->isChecked()){
            m_scanArguments->RecordType_ns = true;
            m_scanArguments->choiceCount++;
        }
        if(ui->checkBox_TXT->isChecked()){
            m_scanArguments->RecordType_txt = true;
            m_scanArguments->choiceCount++;
        }
        if(ui->checkBox_CNAME->isChecked()){
            m_scanArguments->RecordType_cname = true;
            m_scanArguments->choiceCount++;
        }
        //...
        if(m_scanArguments->choiceCount == 0){
            QMessageBox::warning(this, TITLE_ERROR, "Please Choose the DNS-RECORD you want to Enumerate!");
            return;
        }
        //...
        ui->progressBar->setMaximum(ui->listWidget_targets->count());
    }
    if(ui->comboBox_option->currentIndex() == 1)
    {
        if(ui->listWidget_srvWordlist->count() > 0)
        {
            m_scanArguments->RecordType_srv = true;
            ui->progressBar->setMaximum(ui->listWidget_targets->count()*ui->listWidget_srvWordlist->count());
        }
        else
        {
            QMessageBox::warning(this, TITLE_ERROR, "Please Enter SRV Wordlist!");
            return;
        }
    }
    ///
    /// reseting, disabling/enabling the widgets...
    ///
    ui->pushButton_start->setDisabled(true);
    ui->pushButton_stop->setEnabled(true);
    ui->progressBar->clearMask();
    ui->progressBar->reset();
    ui->progressBar->show();
    ///
    /// start Enumeration...
    ///
    m_scanArguments->currentSrvToEnumerate = 0;
    m_scanArguments->currentTargetToEnumerate = 0;
    startEnumeration();
}
void Dns::on_pushButton_stop_clicked(){
    emit stop();
}

/**************************************** Enumerators *************************************/
void Dns::startEnumeration(){
    int maxThreads = 1;
    int wordlistCount = ui->listWidget_targets->count();
    if(wordlistCount < maxThreads){
        maxThreads = wordlistCount;
    }
    m_activeThreads = maxThreads;
    //...
    m_scanArguments->currentTargetToEnumerate = 0;
    for(int i = 0; i < maxThreads; i++){
        DnsRecordsEnumerator *Enumerator = new DnsRecordsEnumerator(m_scanArguments, m_scanResults);
        QThread *cThread = new QThread(this);
        //...
        if(ui->comboBox_option->currentIndex() == 0)
        {
            Enumerator->enumerate(cThread);
        }
        if(ui->comboBox_option->currentIndex() == 1)
        {
            Enumerator->enumerate_srv(cThread);
        }
        Enumerator->moveToThread(cThread);
        //...
        connect(Enumerator, SIGNAL(scanLog(QString)), this, SLOT(logs(QString)));
        connect(Enumerator, SIGNAL(progress(int)), ui->progressBar, SLOT(setValue(int)));
        connect(cThread, SIGNAL(finished()), this, SLOT(scanThreadEnded()));
        connect(cThread, SIGNAL(finished()), Enumerator, SLOT(deleteLater()));
        connect(cThread, SIGNAL(finished()), cThread, SLOT(deleteLater()));
        connect(this, SIGNAL(stop()), Enumerator, SLOT(onStop()));
        //...
        cThread->start();
    }
}

void Dns::loadSrvWordlist(){
    ///
    /// setup...
    ///
    m_model_srv->setHorizontalHeaderLabels({"Name", "Target", "Port"});
    ui->tableView_srv->setModel(m_model_srv);
    m_scanResults->m_model_srv = m_model_srv;
    //...
    m_scanResults->srvResultsLabel = ui->label_srvCount;
    ///
    /// ...
    ///
    QFile file(":/files/res/files/srv.txt");
    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&file);
        while (!in.atEnd()){
            ui->listWidget_srvWordlist->addItem(in.readLine());
        }
        ui->label_srvWordlistCount->setNum(ui->listWidget_srvWordlist->count());
        file.close();
    }
}

void Dns::scanThreadEnded(){
    m_endedThreads++;
    if(m_activeThreads == m_endedThreads)
    {
        ui->pushButton_stop->setDisabled(true);
        ui->pushButton_start->setEnabled(true);
        m_endedThreads = 0;
        //...
        logs("[END] Enumeration Complete!\n");
    }
}

/*******************************************************************************************
                                    Results Processing
********************************************************************************************/

void Dns::on_pushButton_clearResults_clicked(){
    if(ui->tabWidget_results->currentIndex() == 0)
    {
        ui->label_resultsCount->clear();
        m_model->clear();
        m_rootItem = m_model->invisibleRootItem();
        m_scanResults->rootItem = m_rootItem;
        m_scanResults->resultsCount = 0;
    }
    else
    {
        ui->listWidget_logs->clear();
    }
}

void Dns::on_toolButton_config_clicked(){
    //BruteConfig *bruteconfig = new BruteConfig(this, ENUMNAME_ACTIVESUBDOMAINS);
    //bruteconfig->setAttribute( Qt::WA_DeleteOnClose, true );
    //bruteconfig->show();
}

/**************************************************************************************/
void Dns::on_comboBox_option_currentIndexChanged(int index){
    if(index)
    {
        ui->frame_srvWordlist->show();
        ui->frame_records->hide();
        ui->tabWidget_results->setCurrentIndex(1);
    }
    else
    {
        ui->frame_srvWordlist->hide();
        ui->frame_records->show();
        ui->tabWidget_results->setCurrentIndex(0);
    }
}

/**************************************************************************************
 *                          Wordlist Processing
 *************************************************************************************/

/************************************ Remove wordlist ************************************/
void Dns::on_pushButton_removeTargets_clicked(){
    if(ui->listWidget_targets->selectedItems().count()){
        qDeleteAll(ui->listWidget_targets->selectedItems());
    }
    ui->label_targetsCount->setNum(ui->listWidget_targets->count());
}

/************************************ Clear wordlist **************************************/
void Dns::on_pushButton_clearTargets_clicked(){
    ui->listWidget_targets->clear();
    ui->label_targetsCount->clear();
}

/************************************ Load Wordlist ***************************************/
void Dns::on_pushButton_loadTargets_clicked(){
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

/***************************** Add Item on Wordlist **************************************/
void Dns::on_pushButton_addTargets_clicked(){
    if(ui->lineEdit_targets->text() != EMPTY){
        ui->listWidget_targets->addItem(ui->lineEdit_targets->text());
        ui->lineEdit_targets->clear();
        ui->label_targetsCount->setNum(ui->listWidget_targets->count());
    }
}
void Dns::on_lineEdit_targets_returnPressed(){
    on_pushButton_addTargets_clicked();
}

/************************************* SRV WORDLIST ****************************************/
void Dns::on_pushButton_removeSrvWordlist_clicked(){
    if(ui->listWidget_srvWordlist->selectedItems().count()){
        qDeleteAll(ui->listWidget_srvWordlist->selectedItems());
    }
    ui->label_srvWordlistCount->setNum(ui->listWidget_srvWordlist->count());
}

void Dns::on_pushButton_clearSrvWordlist_clicked(){
    ui->listWidget_srvWordlist->clear();
    ui->label_srvWordlistCount->clear();
}

void Dns::on_pushButton_loadSrvWordlist_clicked(){
    QString filename = QFileDialog::getOpenFileName(this, INFO_LOADFILE, CURRENT_PATH);
    if(!filename.isEmpty()){
        QFile file(filename);
        if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
            QTextStream in(&file);
            while (!in.atEnd()){
                ui->listWidget_srvWordlist->addItem(in.readLine());
            }
            ui->label_srvWordlistCount->setNum(ui->listWidget_srvWordlist->count());
            file.close();
        }else{
            QMessageBox::warning(this, TITLE_ERROR, "Failed To Open the File!");
        }
    }
}

void Dns::on_pushButton_addSrvWordlist_clicked(){
    if(ui->lineEdit_srvWordlist->text() != EMPTY){
        ui->listWidget_srvWordlist->addItem(ui->lineEdit_srvWordlist->text());
        ui->lineEdit_srvWordlist->clear();
        ui->label_srvWordlistCount->setNum(ui->listWidget_srvWordlist->count());
    }
}

void Dns::on_lineEdit_srvWordlist_returnPressed(){
    on_pushButton_addSrvWordlist_clicked();
}


/************************************ ALL LOGS ***********************************/
void Dns::logs(QString log){
    sendLog(log);
    ui->listWidget_logs->addItem(log);
    if (log.startsWith("[ERROR]")){
        ui->listWidget_logs->item(ui->listWidget_logs->count()-1)->setForeground(Qt::red);
        return;
    }
    if(log.startsWith("[START]") || log.startsWith("[END]")){
        ui->listWidget_logs->item(ui->listWidget_logs->count()-1)->setFont(QFont("MS Shell Dlg 2", 8, QFont::Bold));
        return;
    }
}

/**********************************************************************************
 *                          Context Menu
 *********************************************************************************/
/********************************* action Button context Menu ***********************************/
void Dns::on_pushButton_action_clicked(){
    ///
    /// getting the position of the action button to place the context menu...
    ///
    QPoint pos = ui->pushButton_action->mapToGlobal(QPoint(0,0));
    ///
    /// showing the context menu right by the side of the action button...
    ///
    QMenu *menu = new QMenu(this);
    menu->setAttribute( Qt::WA_DeleteOnClose, true );
    menu->setObjectName("mainMenu");
    //...
    QAction actionSendToSave("Send To Save", this);
    QAction actionSendToMultiLevel("Send To Multi-level Scan");
    QAction actionCollectAllRecords("Send To DnsRecords");
    //...
    connect(&actionSendToSave, SIGNAL(triggered()), this, SLOT(actionSendToSave()));
    connect(&actionCollectAllRecords, SIGNAL(triggered()), this, SLOT(actionCollectAllRecords()));
    connect(&actionSendToMultiLevel, SIGNAL(triggered()), this, SLOT(actionSendToMultiLevel()));
    //...
    menu->addSeparator();
    menu->addAction(&actionCollectAllRecords);
    menu->addAction(&actionSendToSave);
    menu->addAction(&actionSendToMultiLevel);
    //...
    menu->setStyleSheet("QMenu::item::selected#mainMenu{background-color: rgb(170, 170, 127)} QMenu#mainMenu{background-color: qlineargradient(x1:0,  y1:0, x2:0, y2:1, stop: 0 white, stop: 0.8 rgb(246, 255, 199)); border-style: solid; border-color: black; border-width: 1px;}");
    menu->move(QPoint(pos.x()+76, pos.y()));
    menu->exec();
}



/********************************* right click Context Menu ***************************************/
void Dns::on_treeView_results_customContextMenuRequested(const QPoint &pos){
    Q_UNUSED(pos)
    if(!ui->treeView_results->selectionModel()->isSelected(ui->treeView_results->currentIndex())){
        return;
    }
    //...
    QMenu *menu = new QMenu(this);
    menu->setAttribute( Qt::WA_DeleteOnClose, true );
    menu->setObjectName("mainMenu");
    //...
    QAction actionSendToSave("Send Selected To Save", this);
    QAction actionSendToMultiLevel("Send Selected To Multi-Level");
    QAction actionOpenInBrowser("Open Selected in Browser");
    //...
    connect(&actionOpenInBrowser, SIGNAL(triggered()), this, SLOT(cursorOpenInBrowser()));
    connect(&actionSendToSave, SIGNAL(triggered()), this, SLOT(cursorSendToSave()));
    connect(&actionSendToMultiLevel, SIGNAL(triggered()), this, SLOT(cursorSendToMultiLevel()));
    //...
    menu->addAction(&actionOpenInBrowser);
    menu->addSeparator();
    menu->addAction(&actionSendToMultiLevel);
    menu->addAction(&actionSendToSave);
    ///
    /// getting the position of the cursor to show the context menu...
    ///
    QPoint globalCursorPos = QCursor::pos();
    QRect mouseScreenGeometry = qApp->desktop()->screen(qApp->desktop()->screenNumber(globalCursorPos))->geometry();
    QPoint localCursorPosition = globalCursorPos - mouseScreenGeometry.topLeft();
    //...
    menu->setStyleSheet("QMenu::item::selected#mainMenu{background-color: rgb(170, 170, 127)} QMenu#mainMenu{background-color: qlineargradient(x1:0,  y1:0, x2:0, y2:1, stop: 0 white, stop: 0.8 rgb(246, 255, 199)); border-style: solid; border-color: black; border-width: 1px;}");
    menu->move(localCursorPosition);
    menu->exec();
}


/************************ Implementation Of Context Menu's Actions ************************/

/*************************** For action button *****************************/
void Dns::actionSendToSave(){

}

void Dns::actionSendToMultiLevel(){

}

void Dns::actionCollectAllRecords(){

}

/******************************* For Cursor ********************************/
void Dns::cursorOpenInBrowser(){
    ///
    /// iterate and open each selected item in a browser...
    ///
    foreach(const QModelIndex &index, ui->treeView_results->selectionModel()->selectedIndexes()){
        QDesktopServices::openUrl(QUrl("https://"+index.data().toString(), QUrl::TolerantMode));
    }
}

void Dns::cursorSendToSave(){

}

void Dns::cursorSendToMultiLevel(){

}
