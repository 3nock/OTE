#include "Brute.h"
#include "ui_Brute.h"

/*

  add an advanced option that resolves an item on wordlist with all nameservers..
  first obtains the server from the domain name, then uses those nameservers as default nameservers
  for the scan...

*/

/*

  add a function to first test if the domain you are testing is an active one
  to reduce un-necessary lookups

*/

/*

 create a notes tab that takes all notes for the related scans of the project...
 each new domain enumerated and its x-stics are to be recorded...
 use of wildcard in each phase...
*/

/*
 *
 * must be project base, on launch asks if you want a quick scan or launch a project base...
 *
 */

// initiate everything in the constructor...

/*************************** Class Constructor & Deconstructor *************************/
Brute::Brute(QWidget *parent) : QWidget(parent), ui(new Ui::Brute),
      //...
      m_model(new QStandardItemModel),
      m_scanArguments(new ScanArguments_Brute)
{
    ui->setupUi(this);
    //...
    ui->lineEdit_targetDomain->setPlaceholderText("eg. example.com");
    ui->lineEdit_wordlist->setPlaceholderText("Enter a new item...");
    //...
    ui->pushButton_stop->setDisabled(true);
    //...
    ui->splitter->setSizes(QList<int>()<<150<<1);
    //...
    QStringList headerLabels = {"Subdomain Name:", "IpAddress"};
    m_model->setHorizontalHeaderLabels(headerLabels);
    //...
    ui->tableView_results->setModel(m_model);
    //...
    m_scanArguments->wordlist = ui->listWidget_wordlist;
    ///
    /// Setting highlight Color for items on the listView...
    ///
    QPalette p = palette();
    p.setColor(QPalette::Highlight, QColor(188, 188, 141));
    p.setColor(QPalette::HighlightedText, QColor(Qt::black));
    ui->tableView_results->setPalette(p);
    //...
    currentPath = QDir::currentPath();
    //...
}
Brute::~Brute(){
    delete m_model;
    //...
    delete ui;
}

/*********************************** START Enumeration **********************************/
void Brute::on_pushButton_start_clicked(){
    if(!ui->lineEdit_targetDomain->isModified() || !(ui->listWidget_wordlist->count() > 0)){
        QMessageBox::warning(this, TITLE_ERROR, "Please Enter the Target-Domain and/or Wordlist for Enumeration!");
        return;
    }
    ui->pushButton_start->setDisabled(true);
    ui->pushButton_stop->setEnabled(true);
    ui->progressBar->show();
    //...
    ui->progressBar->setMaximum(ui->listWidget_wordlist->count());
    //...
    if(ui->radioButton_subBrute->isChecked()){
        m_scanArguments->targetDomain = TargetNameFilter(ui->lineEdit_targetDomain->text(), ENUMNAME_SUBBRUTE);
        if(m_scanArguments->checkWildcardSubdomains){
            checkWildcards();
        }else{
            startEnumeration_subBrute();
        }
        // logs...
        sendStatus("[*] BruteForcing Subdomains For Domain"+m_scanArguments->targetDomain);
        logs("[START] BruteForcing Subdomains For Domain: "+m_scanArguments->targetDomain);
    }
    if(ui->radioButton_tldBrute->isChecked()){
        m_scanArguments->targetDomain = TargetNameFilter(ui->lineEdit_targetDomain->text(), ENUMNAME_TLDBRUTE);
        startEnumeration_tldBrute();
        // logs...
        sendStatus("[*] BruteForcing TLDs For Domain: "+m_scanArguments->targetDomain);
        logs("[START] BruteForcing TLDs For Domain: "+m_scanArguments->targetDomain);
    }
}
void Brute::on_lineEdit_targetDomain_returnPressed(){
    on_pushButton_start_clicked();
}

/************************************* stop Enumeration *************************************/
void Brute::on_pushButton_stop_clicked(){
    emit stop();
}

/************************************ Enumeration Functions *********************************/
void Brute::checkWildcards(){
    BruteEnumerator_Wildcards *wildcardsEnumerator = new BruteEnumerator_Wildcards(m_scanArguments);
    QThread *cThread = new QThread;
    wildcardsEnumerator->Enumerate(cThread);
    wildcardsEnumerator->moveToThread(cThread);
    //...
    connect(cThread, SIGNAL(finished()), this, SLOT(startEnumeration()));
    connect(cThread, SIGNAL(finished()), wildcardsEnumerator, SLOT(deleteLater()));
    connect(cThread, SIGNAL(finished()), cThread, SLOT(deleteLater()));
    //...
    cThread->start();
}

void Brute::startEnumeration_subBrute(){
    int maxThreads = m_scanArguments->maxThreads;
    int wordlistCount = ui->listWidget_wordlist->count();
    if(maxThreads > wordlistCount){
        maxThreads = wordlistCount;
    }
    activeThreads = maxThreads;
    m_scanArguments->currentItemToEnumerate = 0;
    for(int i = 0; i != maxThreads; i++){
        //...
        BruteEnumerator_subBrute *Enumerator = new BruteEnumerator_subBrute(m_scanArguments);
        QThread *cThread = new QThread;
        Enumerator->Enumerate(cThread);
        Enumerator->moveToThread(cThread);
        //...
        connect(Enumerator, SIGNAL(resolvedSubdomain(QString, QString)), this, SLOT(resolvedSubdomain(QString, QString)));
        connect(Enumerator, SIGNAL(progressBarValue(int)), ui->progressBar, SLOT(setValue(int)));
        connect(Enumerator, SIGNAL(scanLog(QString)), this, SLOT(logs(QString)));
        connect(cThread, SIGNAL(finished()), this, SLOT(onThreadEnd()));
        connect(cThread, SIGNAL(finished()), Enumerator, SLOT(deleteLater()));
        connect(cThread, SIGNAL(finished()), cThread, SLOT(deleteLater()));
        connect(this, SIGNAL(stop()), Enumerator, SLOT(onStop()));
        //...
        cThread->start();
    }
}

void Brute::startEnumeration_tldBrute(){
    int maxThreads = m_scanArguments->maxThreads;
    int totalWordlist = ui->listWidget_wordlist->count();
    if(maxThreads > totalWordlist){
        maxThreads = totalWordlist;
    }
    activeThreads = maxThreads;
    m_scanArguments->currentItemToEnumerate = 0;
    for(int i = 0; i != maxThreads; i++){
        //...
        BruteEnumerator_tldBrute *Enumerator = new BruteEnumerator_tldBrute(m_scanArguments);
        QThread *cThread = new QThread;
        Enumerator->Enumerate(cThread);
        Enumerator->moveToThread(cThread);
        //...
        connect(Enumerator, SIGNAL(resolvedSubdomain(QString, QString)), this, SLOT(resolvedSubdomain(QString, QString)));
        connect(Enumerator, SIGNAL(progressBarValue(int)), ui->progressBar, SLOT(setValue(int)));
        connect(Enumerator, SIGNAL(scanLog(QString)), this, SLOT(logs(QString)));
        connect(cThread, SIGNAL(finished()), this, SLOT(onThreadEnd()));
        connect(cThread, SIGNAL(finished()), Enumerator, SLOT(deleteLater()));
        connect(cThread, SIGNAL(finished()), cThread, SLOT(deleteLater()));
        connect(this, SIGNAL(stop()), Enumerator, SLOT(onStop()));
        //...
        cThread->start();
    }
}

/************************************ Receiving Results ***********************************/
void Brute::resolvedSubdomain(QString subdomain, QString ipAddress){
    m_model->setItem(subdomainCount, 0, new QStandardItem(subdomain));
    m_model->setItem(subdomainCount, 1, new QStandardItem(ipAddress));
    subdomainCount++;
    ui->label_subdomainsCount->setNum(subdomainCount);
}

/************************************* End Of Enumeration **********************************/
void Brute::onThreadEnd(){
    endedThreads++;
    if(endedThreads == activeThreads){
        endedThreads = 0;
        ui->pushButton_start->setEnabled(true);
        ui->pushButton_stop->setDisabled(true);
        //...
        sendStatus("[*] Enumeration Complete!");
        logs("[END] Enumeration Complete!\n");
    }
}

/******************************** Scan Config Dialog ***************************************/
void Brute::on_toolButton_config_clicked(){
    BruteConfigDialog *scanConfig = new BruteConfigDialog(this, m_scanArguments);
    scanConfig->setAttribute( Qt::WA_DeleteOnClose, true );
    scanConfig->show();
}

/******************************* Generate Wordlist (Dialog) ******************************/
void Brute::on_pushButton_generate_clicked(){
    WordListGeneratorDialog *wordlistgenerator = new WordListGeneratorDialog(this);
    wordlistgenerator->setAttribute( Qt::WA_DeleteOnClose, true );
    wordlistgenerator->show();
}

/**************************** Choose Wordlist To Use (Dialog) ****************************/
void Brute::on_toolButton_wordlist_clicked(){
    WordListDialog *wordlistDialog;
    if(ui->radioButton_subBrute->isChecked()){
        wordlistDialog = new WordListDialog(this, ENUMNAME_SUBBRUTE);
    }
    if(ui->radioButton_tldBrute->isChecked()){
        wordlistDialog = new WordListDialog(this, ENUMNAME_TLDBRUTE);
    }
    wordlistDialog->setAttribute( Qt::WA_DeleteOnClose, true );
    connect(wordlistDialog, SIGNAL(wordlistFilename(QString)), this, SLOT(onWordlistFilename(QString)));
    wordlistDialog->show();
}

/********************* Receiving the Wordlist that the user choose ***************************/
void Brute::onWordlistFilename(QString wordlistFilename){
    QFile file(wordlistFilename);
    if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
        QTextStream in(&file);
        while (!in.atEnd()){
            ui->listWidget_wordlist->addItem(in.readLine());
            wordlistCount++;
        }
        file.close();
    }
    ui->label_wordlistCount->setNum(wordlistCount);
}

/************************************ Loading Wordlist ***********************************/
void Brute::on_pushButton_load_clicked(){
    QString filename = QFileDialog::getOpenFileName(this, INFO_LOADFILE, CURRENT_PATH);
    if(!filename.isEmpty()){
        QFile file(filename);
        if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
            QTextStream in(&file);
            while (!in.atEnd()){
                ui->listWidget_wordlist->addItem(in.readLine());
                wordlistCount++;
            }
            ui->label_wordlistCount->setNum(wordlistCount);
            file.close();
        }else{
            QMessageBox::warning(this, TITLE_ERROR, "Failed To Open the File!");
        }
    }
}

/******************************** Add Item on the Wordlist *********************************/
void Brute::on_pushButton_add_clicked(){
    if(ui->lineEdit_wordlist->text() != EMPTY){
        ui->listWidget_wordlist->addItem(ui->lineEdit_wordlist->text());
        ui->lineEdit_wordlist->clear();
        wordlistCount++;
        ui->label_wordlistCount->setNum(wordlistCount);
    }
}
void Brute::on_lineEdit_wordlist_returnPressed(){
    on_pushButton_add_clicked();
}

/******************************** Clear The Wordlist **************************************/
void Brute::on_pushButton_clearWordlist_clicked(){
    ui->listWidget_wordlist->clear();
    ui->label_wordlistCount->clear();
    wordlistCount = 0;
    //...
    m_model->setHorizontalHeaderLabels({"Subdomain Name", "IpAddress"});
}

/***************************** Removing Item From Wordlist *********************************/
void Brute::on_pushButton_remove_clicked(){
    int selectionCount = ui->listWidget_wordlist->selectedItems().count();
    if(selectionCount){
        qDeleteAll(ui->listWidget_wordlist->selectedItems());
        wordlistCount = wordlistCount-selectionCount;
    }
    ui->label_wordlistCount->setNum(wordlistCount);
}

/**************************** Clear the Enumerated Subdomains *****************************/
void Brute::on_pushButton_clearResults_clicked(){
    // if the current tab is subdomains clear subdomains if logs clear logs...
    if(ui->tabWidget_results->currentIndex() == 0){
        m_model->clear();
        ui->label_subdomainsCount->clear();
        subdomainCount = 0;
    }else{
        ui->listWidget_logs->clear();
    }
}

/****************************** Save And Display logs *************************************/
void Brute::logs(QString log){
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


/***************************** Context Menu For Action Button *****************************/
void Brute::on_pushButton_action_clicked(){
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
    QAction actionSendToDnsRecords("Send To DnsRecords");
    //...
    connect(&actionSendToSave, SIGNAL(triggered()), this, SLOT(actionSendToSave()));
    connect(&actionSendToDnsRecords, SIGNAL(triggered()), this, SLOT(actionSendToDnsRecords()));
    connect(&actionSendToMultiLevel, SIGNAL(triggered()), this, SLOT(actionSendToMultiLevel()));
    //...
    menu->addSeparator();
    menu->addAction(&actionSendToDnsRecords);
    menu->addAction(&actionSendToSave);
    menu->addAction(&actionSendToMultiLevel);
    //...
    menu->setStyleSheet("QMenu::item::selected#mainMenu{background-color: rgb(170, 170, 127)} QMenu#mainMenu{background-color: qlineargradient(x1:0,  y1:0, x2:0, y2:1, stop: 0 white, stop: 0.8 rgb(246, 255, 199)); border-style: solid; border-color: black; border-width: 1px;}");
    menu->move(QPoint(pos.x()+76, pos.y()));
    menu->exec();
}

/****************************** Cursor right-click Context Menu ******************************/
void Brute::on_tableView_results_customContextMenuRequested(const QPoint &pos){
    Q_UNUSED(pos);
    ///
    /// check if user right clicked on items else dont show the context menu...
    ///
    if(!ui->tableView_results->selectionModel()->isSelected(ui->tableView_results->currentIndex())){
        return;
    }
    //...
    QMenu *menu = new QMenu(this);
    menu->setAttribute( Qt::WA_DeleteOnClose, true );
    menu->setObjectName("mainMenu");
    QAction actionSendToSave("Send Selected To Save", this);
    QAction actionSendToDnsRecords("Send Selected To DnsRecords");
    QAction actionOpenInBrowser("Open Selected in Browser");
    //...
    connect(&actionOpenInBrowser, SIGNAL(triggered()), this, SLOT(cursorOpenInBrowser()));
    connect(&actionSendToSave, SIGNAL(triggered()), this, SLOT(cursorSendToSave()));
    connect(&actionSendToDnsRecords, SIGNAL(triggered()), this, SLOT(cursorSendToDnsRecords()));
    //...
    menu->addAction(&actionOpenInBrowser);
    menu->addSeparator();
    menu->addAction(&actionSendToDnsRecords);
    menu->addAction(&actionSendToSave);
    //...
    QPoint globalCursorPos = QCursor::pos();
    QRect mouseScreenGeometry = qApp->desktop()->screen(qApp->desktop()->screenNumber(globalCursorPos))->geometry();
    QPoint localCursorPosition = globalCursorPos - mouseScreenGeometry.topLeft();
    //...
    menu->setStyleSheet("QMenu::item::selected#mainMenu{background-color: rgb(170, 170, 127)} QMenu#mainMenu{background-color: qlineargradient(x1:0,  y1:0, x2:0, y2:1, stop: 0 white, stop: 0.8 rgb(246, 255, 199)); border-style: solid; border-color: black; border-width: 1px;}");
    menu->move(localCursorPosition);
    menu->exec();
}


/****************************** Action Context Menu Methods ***************************/
void Brute::actionSendToSave(){
    /*
    int resultsCount = ui->listWidget_subdomains->count();
    for(int i = 0; i != resultsCount; ++i){
        emit sendResultsToSave(ui->listWidget_subdomains->item(i)->text());
    }
    logs("[*] Sent "+QString::number(resultsCount)+" subBrute Enumerated Subdomains To Save Tab...");
    emit changeTabToSave();
    */
}

/*********************************************************************************/
void Brute::actionSendToDnsRecords(){

}

/**********************************************************************************/
void Brute::actionSendToMultiLevel(){

}

/*************************** Cursor Right Click Context Menu ***********************/
void Brute::cursorSendToSave(){
    /*foreach(const QModelIndex &index, ui->tableView_results->selectionModel()->selectedIndexes()){
        // send selection
    }
    logs("[*] Sent "+QString::number(ui->listWidget_subdomains->count())+" subBrute Enumerated Subdomains To Save Tab...");
    emit changeTabToSave();*/
}

/************************************************************************************/
void Brute::cursorOpenInBrowser(){
    // iterate and open each selected item in a browser...
    foreach(const QModelIndex &index, ui->tableView_results->selectionModel()->selectedIndexes()){
        QDesktopServices::openUrl(QUrl("https://"+index.data().toString(), QUrl::TolerantMode));
    }
}


/*************************************************************************************/
void Brute::cursorSendToDnsRecords(){

}

/********** on receiving signal to send the enumeration results to the save tab ********/
void Brute::onSendResultsToSave(){
    /*
    int resultsCount = ui->listWidget_subdomains->count();
    for(int i = 0; i != resultsCount; ++i){
        emit sendResultsToSave(ui->listWidget_subdomains->item(i)->text());
    }
    logs("[*] Sent "+QString::number(resultsCount)+" subBrute Enumerated Subdomains To Save Tab...");
    //...
    resultsCount = ui->listWidget_subdomains_tldBrute->count();
    for(int i = 0; i != resultsCount; ++i){
        emit sendResultsToSave(ui->listWidget_subdomains_tldBrute->item(i)->text());
    }
    logs_tldBrute("[*] Sent "+QString::number(resultsCount)+" tldBrute Enumerated Subdomains To Save Tab...");
    //...
    resultsCount = ui->listWidget_subdomains_activeSubdomains->count();
    for(int i = 0; i != resultsCount; ++i){
        emit sendResultsToSave(ui->listWidget_subdomains_activeSubdomains->item(i)->text());
    }
    logs_activeSubdomains("[*] Sent "+QString::number(resultsCount)+" activeSubdomains Enumerated Subdomains To Save Tab...");
    */
}
