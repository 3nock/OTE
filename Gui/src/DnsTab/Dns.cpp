#include "Dns.h"
#include "ui_Dns.h"

/*************************************************************************************
 *                          Constructor & Destructor
 *************************************************************************************/
Dns::Dns(QWidget *parent) : QDialog(parent), ui(new Ui::Dns),
      resultsRootModel(new QStandardItemModel),
      root_item(resultsRootModel->invisibleRootItem()),
      //...
      scanArguments(new scanArguments_dnsRecords),
      scanResults(new scanResults_dnsRecords)
{
    ui->setupUi(this);
    //...
    scanResults->root_item = root_item;
    scanResults->resultsCountLabel = ui->label_resultsCount_dnsRecords;
    //...
    ui->treeView_results_dnsRecords->setModel(resultsRootModel);
    ui->treeView_results_dnsRecords->expandAll();
    //...
    ui->lineEdit_wordlist_dnsRecords->setPlaceholderText("Enter Target domains/subdomains..");
    ui->pushButton_reload_dnsRecords->hide();
    //...
    ui->pushButton_stop_dnsRecords->setDisabled(true);
    ///
    /// Setting highlight Color for items on the TreeView...
    ///
    QPalette p = palette();
    p.setColor(QPalette::Highlight, QColor(188, 188, 141));
    p.setColor(QPalette::HighlightedText, QColor(Qt::black));
    ui->treeView_results_dnsRecords->setPalette(p);
    ///
    /// setting the splitter to the middle...
    ///
    ui->splitter->setSizes(QList<int>()<<150<<1);
}
Dns::~Dns(){
    delete scanArguments;
    delete scanResults;
    delete resultsRootModel;
    delete ui;
}

/**************************************************************************************
                                  Scan
**************************************************************************************/
void Dns::on_pushButton_start_dnsRecords_clicked(){
    if(ui->listWidget_wordlist_dnsRecords->count() < 1){
        QMessageBox::warning(this, TITLE_ERROR, "Please Enter Target Subdomains For Enumeration");
        return;
    }
    ///
    /// getting the arguments for the Scan...
    ///
    scanArguments->targetWordlist = ui->listWidget_wordlist_dnsRecords;
    scanArguments->choiceCount = 0;
    if(ui->checkBox_mx_dnsRecords->isChecked()){
        scanArguments->RecordType_mx = ui->checkBox_mx_dnsRecords->isChecked();
        scanArguments->choiceCount++;
    }
    if(ui->checkBox_ns_dnsRecords->isChecked()){
        scanArguments->RecordType_ns = ui->checkBox_ns_dnsRecords->isChecked();
        scanArguments->choiceCount++;
    }
    if(ui->checkBox_srv_dnsRecords->isChecked()){
        scanArguments->RecordType_srv = ui->checkBox_srv_dnsRecords->isChecked();
        scanArguments->choiceCount++;
    }
    if(ui->checkBox_txt_dnsRecords->isChecked()){
        scanArguments->RecordType_txt = ui->checkBox_txt_dnsRecords->isChecked();
        scanArguments->choiceCount++;
    }
    if(ui->checkBox_cname_dnsRecords->isChecked()){
        scanArguments->RecordType_cname = ui->checkBox_cname_dnsRecords->isChecked();
        scanArguments->choiceCount++;
    }
    //...
    if(scanArguments->choiceCount == 0){
        QMessageBox::warning(this, TITLE_ERROR, "Please Choose the DNS-RECORD you want to Enumerate!");
        return;
    }
    //...
    ui->pushButton_start_dnsRecords->setDisabled(true);
    ui->pushButton_stop_dnsRecords->setEnabled(true);
    ui->pushButton_reload_dnsRecords->show();
    //...
    startEnumeration_dnsRecords();
}
void Dns::on_pushButton_stop_dnsRecords_clicked(){
    emit stop_dnsRecords();
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
    scanArguments->currentTargetToEnumerate = 0;
    for(int i = 0; i < maxThreads; i++){
        Enumerator_dnsRecords *Enumerator = new Enumerator_dnsRecords(scanArguments, scanResults);
        QThread *cThread = new QThread(this);
        Enumerator->Enumerate(cThread);
        Enumerator->moveToThread(cThread);
        //...
        connect(Enumerator, SIGNAL(scanlogs(QString)), this, SLOT(logs_dnsRecords(QString)));
        connect(cThread, SIGNAL(finished()), this, SLOT(onThreadEnded()));
        connect(cThread, SIGNAL(finished()), Enumerator, SLOT(deleteLater()));
        connect(cThread, SIGNAL(finished()), cThread, SLOT(deleteLater()));
        connect(this, SIGNAL(stop_dnsRecords()), Enumerator, SLOT(onStop()));
        //...
        cThread->start();
    }
}

void Dns::onThreadEnded(){
    endedThreads_dnsRecords++;
    if(activeThreads_dnsRecords == endedThreads_dnsRecords){
        ui->pushButton_stop_dnsRecords->setDisabled(true);
        ui->pushButton_start_dnsRecords->setEnabled(true);
        endedThreads_dnsRecords = 0;
    }
}

/*******************************************************************************************
                                    Results Processing
********************************************************************************************/

void Dns::on_pushButton_clearOutput_dnsRecords_clicked(){
    if(ui->tabWidget_output_dnsRecords->currentIndex() == 0){
        ui->label_resultsCount_dnsRecords->clear();
        resultsRootModel->clear();
        root_item = resultsRootModel->invisibleRootItem();
        scanResults->root_item = root_item;
        scanResults->resultsCount = 0;
    }else{
        ui->listWidget_logs_dnsRecords->clear();
    }
}


/**************************************************************************************
 *                          Wordlist Processing
 *************************************************************************************/
void Dns::on_toolButton_config_dnsRecords_clicked(){
    //BruteConfig *bruteconfig = new BruteConfig(this, ENUMNAME_ACTIVESUBDOMAINS);
    //bruteconfig->setAttribute( Qt::WA_DeleteOnClose, true );
    //bruteconfig->show();
}

void Dns::on_pushButton_remove_dnsRecords_clicked(){
    int wordlistToRemoveCount = ui->listWidget_wordlist_dnsRecords->selectedItems().count();
    if(wordlistToRemoveCount){
        qDeleteAll(ui->listWidget_wordlist_dnsRecords->selectedItems());
        wordlistCount_dnsRecords = wordlistCount_dnsRecords-wordlistToRemoveCount;
    }
    ui->label_wordlistCount_dnsRecords->setNum(wordlistCount_dnsRecords);
}

void Dns::on_pushButton_clearWordlist_dnsRecords_clicked(){
    ui->listWidget_wordlist_dnsRecords->clear();
    ui->label_wordlistCount_dnsRecords->clear();
    wordlistCount_dnsRecords = 0;
}

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

void Dns::on_pushButton_reload_dnsRecords_clicked(){
    int count = ui->listWidget_wordlist_dnsRecords->count();
    for(int i = 0; i < count; i++){
        ui->listWidget_wordlist_dnsRecords->item(i)->setForeground(Qt::black);
    }
    ui->pushButton_reload_dnsRecords->hide();
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

/**********************************************************************************
 *                          Context Menu
 *********************************************************************************/
void Dns::on_pushButton_action_dnsRecords_clicked(){
    // getting the position of the action button to place the context menu...
    QPoint pos = ui->pushButton_action_dnsRecords->mapToGlobal(QPoint(0,0));
    // showing the context menu right by the side of the action button...
    showContextMenu_ActionButton(QPoint(pos.x()+76, pos.y()));
}

void Dns::on_treeView_results_dnsRecords_customContextMenuRequested(const QPoint &pos){
    Q_UNUSED(pos)
    if(!ui->treeView_results_dnsRecords->selectionModel()->isSelected(ui->treeView_results_dnsRecords->currentIndex())){
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

void Dns::actionSendToMultiLevel_dnsRecords(){

}

void Dns::actionCollectAllRecords_dnsRecords(){

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

void Dns::cursorSendToSave_dnsRecords(){

}

void Dns::cursorSendToMultiLevel_dnsRecords(){

}
