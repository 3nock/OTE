#include "Dns.h"
#include "ui_Dns.h"


/*************************************************************************************
                            Constructor & Destructor
*************************************************************************************/

Dns::Dns(QWidget *parent) : QDialog(parent), ui(new Ui::Dns),
      model_dnsRecords(new QStandardItemModel),
      model_lookup(new QStandardItemModel),
      rootItem_dnsRecords(model_dnsRecords->invisibleRootItem()),
      //...
      m_scanArguments_dnsRecords(new scanArguments_dnsRecords),
      m_scanArguments_lookup(new scanArguments_lookup),
      //...
      m_scanResults_dnsRecords(new scanResults_dnsRecords),
      m_scanResults_lookup(new scanResults_lookup)
{
    ui->setupUi(this);
    //...
    m_scanResults_dnsRecords->rootItem = rootItem_dnsRecords;
    m_scanResults_dnsRecords->resultsCountLabel = ui->label_resultsCount_dnsRecords;
    //...
    ui->treeView_results_dnsRecords->setModel(model_dnsRecords);
    ui->tableView_results_lookup->setModel(model_lookup);
    //...
    QStringList headerLabels = {"Subdomain Name:", "IpAddress:"};
    model_lookup->setHorizontalHeaderLabels(headerLabels);
    //...
    ui->lineEdit_wordlist_dnsRecords->setPlaceholderText("Enter Target domains/subdomains...");
    ui->lineEdit_wordlist_lookup->setPlaceholderText("Enter Host (Ip/HostName) For Lookup...");
    //...
    /*
    ui->pro->hide();
    ui->pushButton_reload_lookup->hide();
    */
    //...
    ui->pushButton_stop_dnsRecords->setDisabled(true);
    ui->pushButton_stop_lookup->setDisabled(true);
    //...
    ui->progressBar_lookup->hide();
    ui->progressBar_dnsRecords->hide();
    //...
    m_scanArguments_dnsRecords->targetWordlist = ui->listWidget_wordlist_dnsRecords;
    m_scanArguments_lookup->targetWordlist = ui->listWidget_wordlist_lookup;
    //...
    m_scanResults_lookup->model = model_lookup;
    m_scanResults_lookup->resultsCountLabel = ui->label_resultsCount_lookup;
    ///
    /// Setting highlight Color for items on the TreeView...
    ///
    QPalette p = palette();
    p.setColor(QPalette::Highlight, QColor(188, 188, 141));
    p.setColor(QPalette::HighlightedText, QColor(Qt::black));
    ui->treeView_results_dnsRecords->setPalette(p);
    ui->tableView_results_lookup->setPalette(p);
    ///
    /// setting the splitter to the middle...
    ///
    ui->splitter->setSizes(QList<int>()<<150<<1);
    ui->splitter_2->setSizes(QList<int>()<<100<<1);
}
Dns::~Dns(){
    delete m_scanArguments_dnsRecords;
    delete m_scanArguments_lookup;
    delete m_scanResults_dnsRecords;
    delete model_dnsRecords;
    delete model_lookup;
    delete ui;
}

/**************************************************************************************
                                      Scan
**************************************************************************************/
void Dns::on_pushButton_start_dnsRecords_clicked(){
    if(ui->listWidget_wordlist_dnsRecords->count() < 1){
        QMessageBox::warning(this, TITLE_ERROR, "Please Enter Target Subdomains For Enumeration!");
        return;
    }
    ///
    /// getting the arguments for the Scan...
    ///
    m_scanArguments_dnsRecords->choiceCount = 0;
    if(ui->checkBox_a_dnsRecords->isChecked()){
        m_scanArguments_dnsRecords->RecordType_a = true;
        m_scanArguments_dnsRecords->choiceCount++;
    }
    if(ui->checkBox_aaaa_dnsRecords->isChecked()){
        m_scanArguments_dnsRecords->RecordType_aaaa = true;
        m_scanArguments_dnsRecords->choiceCount++;
    }
    if(ui->checkBox_mx_dnsRecords->isChecked()){
        m_scanArguments_dnsRecords->RecordType_mx = true;
        m_scanArguments_dnsRecords->choiceCount++;
    }
    if(ui->checkBox_ns_dnsRecords->isChecked()){
        m_scanArguments_dnsRecords->RecordType_ns = true;
        m_scanArguments_dnsRecords->choiceCount++;
    }
    if(ui->checkBox_srv_dnsRecords->isChecked()){
        m_scanArguments_dnsRecords->RecordType_srv = true;
        m_scanArguments_dnsRecords->choiceCount++;
    }
    if(ui->checkBox_txt_dnsRecords->isChecked()){
        m_scanArguments_dnsRecords->RecordType_txt = true;
        m_scanArguments_dnsRecords->choiceCount++;
    }
    if(ui->checkBox_cname_dnsRecords->isChecked()){
        m_scanArguments_dnsRecords->RecordType_cname = true;
        m_scanArguments_dnsRecords->choiceCount++;
    }
    //...
    if(m_scanArguments_dnsRecords->choiceCount == 0){
        QMessageBox::warning(this, TITLE_ERROR, "Please Choose the DNS-RECORD you want to Enumerate!");
        return;
    }
    //...
    ui->pushButton_start_dnsRecords->setDisabled(true);
    ui->pushButton_stop_dnsRecords->setEnabled(true);
    ui->progressBar_dnsRecords->show();
    //...
    ui->progressBar_dnsRecords->setMaximum(ui->listWidget_wordlist_dnsRecords->count());
    //...
    startEnumeration_dnsRecords();
}
void Dns::on_pushButton_stop_dnsRecords_clicked(){
    emit stop_dnsRecords();
}

void Dns::on_pushButton_start_lookup_clicked(){
    if(ui->listWidget_wordlist_lookup->count() < 1){
        QMessageBox::warning(this, TITLE_ERROR, "Please Enter Target(IP/HostName) For Enumeration!");
        return;
    }
    ///
    /// getting the arguments for the Scan...
    ///
    ui->pushButton_start_lookup->setDisabled(true);
    ui->pushButton_stop_lookup->setEnabled(true);
    ui->progressBar_lookup->show();
    //...
    ui->progressBar_lookup->setMaximum(ui->listWidget_wordlist_lookup->count());
    //...
    startEnumeration_lookup();
}
void Dns::on_pushButton_stop_lookup_clicked(){
    emit stop_lookup();
}

/**************************************** Enumerators *************************************/
void Dns::startEnumeration_dnsRecords(){
    int maxThreads = 1;
    int wordlistCount = ui->listWidget_wordlist_dnsRecords->count();
    if(wordlistCount < maxThreads){
        maxThreads = wordlistCount;
    }
    activeThreads_dnsRecords = maxThreads;
    //...
    m_scanArguments_dnsRecords->currentTargetToEnumerate = 0;
    for(int i = 0; i < maxThreads; i++){
        Enumerator_dnsRecords *Enumerator = new Enumerator_dnsRecords(m_scanArguments_dnsRecords, m_scanResults_dnsRecords);
        QThread *cThread = new QThread(this);
        Enumerator->Enumerate(cThread);
        Enumerator->moveToThread(cThread);
        //...
        connect(Enumerator, SIGNAL(scanLog(QString)), this, SLOT(logs_dnsRecords(QString)));
        connect(Enumerator, SIGNAL(progressBarValue(int)), ui->progressBar_dnsRecords, SLOT(setValue(int)));
        connect(cThread, SIGNAL(finished()), this, SLOT(onThreadEnded_dnsRecords()));
        connect(cThread, SIGNAL(finished()), Enumerator, SLOT(deleteLater()));
        connect(cThread, SIGNAL(finished()), cThread, SLOT(deleteLater()));
        connect(this, SIGNAL(stop_dnsRecords()), Enumerator, SLOT(onStop()));
        //...
        cThread->start();
    }
}

void Dns::startEnumeration_lookup(){
    int maxThreads = 1;
    int wordlistCount = ui->listWidget_wordlist_lookup->count();
    if(wordlistCount < maxThreads){
        maxThreads = wordlistCount;
    }
    activeThreads_lookup = maxThreads;
    //...
    m_scanArguments_lookup->currentItemToEnumerate = 0;
    for(int i = 0; i < maxThreads; i++){
        Enumerator_lookup *Enumerator = new Enumerator_lookup(m_scanArguments_lookup, m_scanResults_lookup);
        QThread *cThread = new QThread(this);
        //...
        if(ui->radioButton_hostnameToIp->isChecked())
            Enumerator->Enumerate_lookup(cThread);
        if(ui->radioButton_ipToHostname->isChecked())
            Enumerator->Enumerate_reverseLookup(cThread);
        //...
        Enumerator->moveToThread(cThread);
        //...
        connect(Enumerator, SIGNAL(scanLog(QString)), this, SLOT(logs_lookup(QString)));
        connect(Enumerator, SIGNAL(progressBarValue(int)), ui->progressBar_lookup, SLOT(setValue(int)));
        connect(cThread, SIGNAL(finished()), this, SLOT(onThreadEnded_lookup()));
        connect(cThread, SIGNAL(finished()), Enumerator, SLOT(deleteLater()));
        connect(cThread, SIGNAL(finished()), cThread, SLOT(deleteLater()));
        connect(this, SIGNAL(stop_lookup()), Enumerator, SLOT(onStop()));
        //...
        cThread->start();
    }
}

void Dns::onThreadEnded_dnsRecords(){
    endedThreads_dnsRecords++;
    if(activeThreads_dnsRecords == endedThreads_dnsRecords){
        ui->pushButton_stop_dnsRecords->setDisabled(true);
        ui->pushButton_start_dnsRecords->setEnabled(true);
        endedThreads_dnsRecords = 0;
    }
}
void Dns::onThreadEnded_lookup(){
    endedThreads_lookup++;
    if(activeThreads_lookup == endedThreads_lookup){
        ui->pushButton_stop_lookup->setDisabled(true);
        ui->pushButton_start_lookup->setEnabled(true);
        endedThreads_lookup = 0;
    }
}

/*******************************************************************************************
                                    Results Processing
********************************************************************************************/

void Dns::on_pushButton_clearOutput_dnsRecords_clicked(){
    if(ui->tabWidget_output_dnsRecords->currentIndex() == 0){
        ui->label_resultsCount_dnsRecords->clear();
        model_dnsRecords->clear();
        rootItem_dnsRecords = model_dnsRecords->invisibleRootItem();
        m_scanResults_dnsRecords->rootItem = rootItem_dnsRecords;
        m_scanResults_dnsRecords->resultsCount = 0;
    }else{
        ui->listWidget_logs_dnsRecords->clear();
    }
}
void Dns::on_pushButton_clearResults_lookup_clicked(){
    if(ui->tabWidget_lookup->currentIndex() == 0){
        ui->label_resultsCount_lookup->clear();
        model_lookup->clear();
        //...
        QStringList headerLabels = {"HostName:", "IpAddress:"};
        model_lookup->setHorizontalHeaderLabels(headerLabels);
        //...
        m_scanResults_lookup->hostnameCount = 0;
        m_scanResults_lookup->ipAddressCount = 0;
    }else{
        ui->listWidget_logs_lookup->clear();
    }
}

void Dns::on_toolButton_config_dnsRecords_clicked(){
    //BruteConfig *bruteconfig = new BruteConfig(this, ENUMNAME_ACTIVESUBDOMAINS);
    //bruteconfig->setAttribute( Qt::WA_DeleteOnClose, true );
    //bruteconfig->show();
}
void Dns::on_toolButton_config_lookup_clicked(){

}

/**************************************************************************************
 *                          Wordlist Processing
 *************************************************************************************/

/************************************ Remove wordlist ************************************/
void Dns::on_pushButton_remove_dnsRecords_clicked(){
    int wordlistToRemoveCount = ui->listWidget_wordlist_dnsRecords->selectedItems().count();
    if(wordlistToRemoveCount){
        qDeleteAll(ui->listWidget_wordlist_dnsRecords->selectedItems());
        wordlistCount_dnsRecords = wordlistCount_dnsRecords-wordlistToRemoveCount;
    }
    ui->label_wordlistCount_dnsRecords->setNum(wordlistCount_dnsRecords);
}
void Dns::on_pushButton_removeWordlist_lookup_clicked(){
    int wordlistToRemoveCount = ui->listWidget_wordlist_lookup->selectedItems().count();
    if(wordlistToRemoveCount){
        qDeleteAll(ui->listWidget_wordlist_lookup->selectedItems());
        wordlistCount_dnsRecords = wordlistCount_lookup-wordlistToRemoveCount;
    }
    ui->label_wordlistCount_lookup->setNum(wordlistCount_lookup);
}

/************************************ Clear wordlist **************************************/
void Dns::on_pushButton_clearWordlist_dnsRecords_clicked(){
    ui->listWidget_wordlist_dnsRecords->clear();
    ui->label_wordlistCount_dnsRecords->clear();
    wordlistCount_dnsRecords = 0;
}
void Dns::on_pushButton_clearWordlist_lookup_clicked(){
    ui->listWidget_wordlist_lookup->clear();
    ui->label_wordlistCount_lookup->clear();
    wordlistCount_lookup = 0;
}

/************************************ Load Wordlist ***************************************/
void Dns::on_pushButton_load_dnsRecords_clicked(){
    QString filename = QFileDialog::getOpenFileName(this, INFO_LOADFILE, CURRENT_PATH);
    if(!filename.isEmpty()){
        QFile file(filename);
        if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
            QTextStream in(&file);
            while (!in.atEnd()){
                ui->listWidget_wordlist_dnsRecords->addItem(in.readLine());
                wordlistCount_dnsRecords++;
            }
            ui->label_wordlistCount_dnsRecords->setNum(wordlistCount_dnsRecords);
            file.close();
        }else{
            QMessageBox::warning(this, TITLE_ERROR, "Failed To Open the File!");
        }
    }
}
void Dns::on_pushButton_loadWordlist_lookup_clicked(){
    QString filename = QFileDialog::getOpenFileName(this, INFO_LOADFILE, CURRENT_PATH);
    if(!filename.isEmpty()){
        QFile file(filename);
        if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
            QTextStream in(&file);
            while (!in.atEnd()){
                ui->listWidget_wordlist_lookup->addItem(in.readLine());
                wordlistCount_lookup++;
            }
            ui->label_wordlistCount_lookup->setNum(wordlistCount_lookup);
            file.close();
        }else{
            QMessageBox::warning(this, TITLE_ERROR, "Failed To Open the File!");
        }
    }
}

/***************************** Add Item on Wordlist **************************************/
void Dns::on_pushButton_add_dnsRecords_clicked(){
    if(ui->lineEdit_wordlist_dnsRecords->text() != EMPTY){
        ui->listWidget_wordlist_dnsRecords->addItem(ui->lineEdit_wordlist_dnsRecords->text());
        ui->lineEdit_wordlist_dnsRecords->clear();
        wordlistCount_dnsRecords++;
        ui->label_wordlistCount_dnsRecords->setNum(wordlistCount_dnsRecords);
    }
}
void Dns::on_lineEdit_wordlist_dnsRecords_returnPressed(){
    on_pushButton_add_dnsRecords_clicked();
}

void Dns::on_pushButton_addWordlist_lookup_clicked(){
    if(ui->lineEdit_wordlist_lookup->text() != EMPTY){
        ui->listWidget_wordlist_lookup->addItem(ui->lineEdit_wordlist_lookup->text());
        ui->lineEdit_wordlist_lookup->clear();
        wordlistCount_lookup++;
        ui->label_wordlistCount_lookup->setNum(wordlistCount_lookup);
    }
}
void Dns::on_lineEdit_wordlist_lookup_returnPressed(){
    on_pushButton_addWordlist_lookup_clicked();
}

/************************************ ALL LOGS ***********************************/
void Dns::logs_dnsRecords(QString log){
    sendLog(log);
    ui->listWidget_logs_dnsRecords->addItem(log);
    if (log.startsWith("[ERROR]")){
        ui->listWidget_logs_dnsRecords->item(ui->listWidget_logs_dnsRecords->count()-1)->setForeground(Qt::red);
        return;
    }
    if(log.startsWith("[START]") || log.startsWith("[END]")){
        ui->listWidget_logs_dnsRecords->item(ui->listWidget_logs_dnsRecords->count()-1)->setFont(QFont("MS Shell Dlg 2", 8, QFont::Bold));
        return;
    }
}
void Dns::logs_lookup(QString log){
    sendLog(log);
    ui->listWidget_logs_lookup->addItem(log);
    if (log.startsWith("[ERROR]")){
        ui->listWidget_logs_lookup->item(ui->listWidget_logs_lookup->count()-1)->setForeground(Qt::red);
        return;
    }
    if(log.startsWith("[START]") || log.startsWith("[END]")){
        ui->listWidget_logs_lookup->item(ui->listWidget_logs_lookup->count()-1)->setFont(QFont("MS Shell Dlg 2", 8, QFont::Bold));
        return;
    }
}

/**********************************************************************************
 *                          Context Menu
 *********************************************************************************/
/********************************* action Button context Menu ***********************************/
void Dns::on_pushButton_action_dnsRecords_clicked(){
    // getting the position of the action button to place the context menu...
    QPoint pos = ui->pushButton_action_dnsRecords->mapToGlobal(QPoint(0,0));
    // showing the context menu right by the side of the action button...
    showContextMenu_ActionButton(QPoint(pos.x()+76, pos.y()));
}
void Dns::on_pushButton_action_lookup_clicked(){
    // getting the position of the action button to place the context menu...
    QPoint pos = ui->pushButton_action_lookup->mapToGlobal(QPoint(0,0));
    // showing the context menu right by the side of the action button...
    showContextMenu_ActionButton(QPoint(pos.x()+76, pos.y()));
}


/********************************* right click Context Menu ***************************************/
void Dns::on_treeView_results_dnsRecords_customContextMenuRequested(const QPoint &pos){
    Q_UNUSED(pos)
    if(!ui->treeView_results_dnsRecords->selectionModel()->isSelected(ui->treeView_results_dnsRecords->currentIndex())){
        return;
    }
    showContextMenu_RightClick();
}
void Dns::on_tableView_results_lookup_customContextMenuRequested(const QPoint &pos){
    Q_UNUSED(pos)
    if(!ui->tableView_results_lookup->selectionModel()->isSelected(ui->tableView_results_lookup->currentIndex())){
        return;
    }
    showContextMenu_RightClick();
}

void Dns::showContextMenu_ActionButton(QPoint position){
    QMenu *menu = new QMenu(this);
    menu->setAttribute( Qt::WA_DeleteOnClose, true );
    menu->setObjectName("mainMenu");
    //...
    QAction actionSendToSave("Send To Save", this);
    QAction actionSendToMultiLevel("Send To Multi-level Scan");
    QAction actionCollectAllRecords("Send To DnsRecords");
    //...
    connect(&actionSendToSave, SIGNAL(triggered()), this, SLOT(actionSendToSave_dnsRecords()));
    connect(&actionCollectAllRecords, SIGNAL(triggered()), this, SLOT(actionCollectAllRecords_dnsRecords()));
    connect(&actionSendToMultiLevel, SIGNAL(triggered()), this, SLOT(actionSendToMultiLevel_dnsRecords()));
    //...
    menu->addSeparator();
    menu->addAction(&actionCollectAllRecords);
    menu->addAction(&actionSendToSave);
    menu->addAction(&actionSendToMultiLevel);
    //...
    menu->setStyleSheet("QMenu::item::selected#mainMenu{background-color: rgb(170, 170, 127)} QMenu#mainMenu{background-color: qlineargradient(x1:0,  y1:0, x2:0, y2:1, stop: 0 white, stop: 0.8 rgb(246, 255, 199)); border-style: solid; border-color: black; border-width: 1px;}");
    menu->move(position);
    menu->exec();
}

void Dns::showContextMenu_RightClick(){
    QMenu *menu = new QMenu(this);
    menu->setAttribute( Qt::WA_DeleteOnClose, true );
    menu->setObjectName("mainMenu");
    //...
    QAction actionSendToSave("Send Selected To Save", this);
    QAction actionSendToMultiLevel("Send Selected To Multi-Level");
    QAction actionOpenInBrowser("Open Selected in Browser");
    //...
    connect(&actionOpenInBrowser, SIGNAL(triggered()), this, SLOT(cursorOpenInBrowser_dnsRecords()));
    connect(&actionSendToSave, SIGNAL(triggered()), this, SLOT(cursorSendToSave_dnsRecords()));
    connect(&actionSendToMultiLevel, SIGNAL(triggered()), this, SLOT(cursorSendToMultiLevel_dnsRecords()));
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
void Dns::actionSendToSave_dnsRecords(){

}

void Dns::actionSendToSave_lookup(){

}

void Dns::actionSendToMultiLevel_dnsRecords(){

}

void Dns::actionSendToMultiLevel_lookup(){

}

void Dns::actionCollectAllRecords_dnsRecords(){

}

void Dns::actionCollectAllRecords_lookup(){

}

/******************************* For Cursor ********************************/
void Dns::cursorOpenInBrowser_dnsRecords(){
    ///
    /// iterate and open each selected item in a browser...
    ///
    foreach(const QModelIndex &index, ui->treeView_results_dnsRecords->selectionModel()->selectedIndexes()){
        QDesktopServices::openUrl(QUrl("https://"+index.data().toString(), QUrl::TolerantMode));
    }
}

void Dns::cursorOpenInBrowser_lookup(){
    ///
    /// iterate and open each selected item in a browser...
    ///
    foreach(const QModelIndex &index, ui->tableView_results_lookup->selectionModel()->selectedIndexes()){
        QDesktopServices::openUrl(QUrl("https://"+index.data().toString(), QUrl::TolerantMode));
    }
}

void Dns::cursorSendToSave_dnsRecords(){

}

void Dns::cursorSendToSave_lookup(){

}

void Dns::cursorSendToMultiLevel_dnsRecords(){

}

void Dns::cursorSendToMultiLevel_lookup(){

}

