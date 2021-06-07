#include "Brute.h"
#include "ui_Brute.h"

/*************************** Class Constructor & Deconstructor *************************/
Brute::Brute(QWidget *parent) :QWidget(parent),ui(new Ui::Brute){
    ui->setupUi(this);
    //...
    ui->lineEdit_targetDomain_subBrute->setPlaceholderText("eg. example.com");
    ui->lineEdit_targetDomain_tldBrute->setPlaceholderText("eg. example.com");
    ui->lineEdit_wordlist_subBrute->setPlaceholderText("Enter a new item...");
    ui->lineEdit_manualWordlist_tldBrute->setPlaceholderText("Enter a new item...");
    ui->lineEdit_manualWordlist_activeSubdomains->setPlaceholderText(("Enter a new item..."));
    //...
    ui->pushButton_stop_subBrute->setDisabled(true);
    ui->pushButton_stop_tldBrute->setDisabled(true);
    ui->pushButton_stop_activeSubdomains->setDisabled(true);
    //...
    ui->splitter->setSizes(QList<int>()<<150<<1);
    ui->splitter_2->setSizes(QList<int>()<<150<<1);
    ui->splitter_3->setSizes(QList<int>()<<150<<1);
    //...
    ui->pushButton_reloadEnumeratedWordlist_subBrute->hide();
    ui->pushButton_reloadEnumeratedWordlist_tldBrute->hide();
    ui->pushButton_reloadEnumeratedWordlist_activeSubdomains->hide();
    //...
    currentPath = QDir::currentPath();
}
Brute::~Brute(){
    delete ui;
}

/*********************************** START Enumeration **********************************/
void Brute::on_pushButton_start_subBrute_clicked(){
    if(!ui->lineEdit_targetDomain_subBrute->isModified() || !(ui->listWidget_wordlist_subBrute->count() > 0)){
        QMessageBox::warning(this, TITLE_ERROR, "Please Enter the Target-Domain and/or Wordlist for Enumeration!");
        return;
    }
    ui->pushButton_reloadEnumeratedWordlist_subBrute->show();
    ui->pushButton_start_subBrute->setDisabled(true);
    ui->pushButton_stop_subBrute->setEnabled(true);
    //...
    scanArguments_subBrute.targetDomain = TargetNameFilter(ui->lineEdit_targetDomain_subBrute->text(), ENUMNAME_SUBBRUTE);
    scanArguments_subBrute.wordlist = ui->listWidget_wordlist_subBrute;
    if(scanArguments_subBrute.checkWildcardSubdomains){
        checkWildcards();
    }else{
        startEnumeration_subBrute();
    }
    //...
    sendStatus("[*] BruteForcing Subdomains For Domain"+scanArguments_subBrute.targetDomain);
    logs_subBrute("[START] BruteForcing Subdomains For Domain: "+scanArguments_subBrute.targetDomain);
}
void Brute::on_lineEdit_targetDomain_subBrute_returnPressed(){
    on_pushButton_start_subBrute_clicked();
}

void Brute::on_pushButton_start_tldBrute_clicked(){
    if(!ui->lineEdit_targetDomain_tldBrute->isModified() || !(ui->listWidget_wordlist_tldBrute->count() > 0)){
        QMessageBox::warning(this, TITLE_ERROR, "Please Enter the Target-Domain and/or Wordlist for Enumeration!");
        return;
    }
    ui->pushButton_start_tldBrute->setDisabled(true);
    ui->pushButton_stop_tldBrute->setEnabled(true);
    ui->pushButton_reloadEnumeratedWordlist_tldBrute->show();
    //...
    scanArguments_tldBrute.targetDomain = TargetNameFilter(ui->lineEdit_targetDomain_tldBrute->text(), ENUMNAME_TLDBRUTE);
    scanArguments_tldBrute.wordlist = ui->listWidget_wordlist_tldBrute;
    startEnumeration_tldBrute();
    //...
    sendStatus("[*] BruteForcing TLDs For Domain: "+scanArguments_tldBrute.targetDomain);
    logs_tldBrute("[START] BruteForcing TLDs For Domain: "+scanArguments_tldBrute.targetDomain);
}
void Brute::on_lineEdit_targetDomain_tldBrute_returnPressed(){
    on_pushButton_start_tldBrute_clicked();
}

void Brute::on_pushButton_start_activeSubdomains_clicked(){
    if(!(ui->listWidget_wordlist_activeSubdomains->count() > 0)){
        QMessageBox::warning(this, TITLE_ERROR, "Please Enter the subdomains Wordlist for Enumeration!");
        return;
    }
    ui->pushButton_reloadEnumeratedWordlist_activeSubdomains->show();
    ui->pushButton_start_activeSubdomains->setDisabled(true);
    ui->pushButton_stop_activeSubdomains->setEnabled(true);
    //...
    scanArguments_activeSubdomains.wordlist = ui->listWidget_wordlist_activeSubdomains;
    //...
    startEnumeration_activeSubdomains();
    //...
    sendStatus("[*] Testing For Active Subdomains...");
    logs_activeSubdomains("[START] Testing For Active Subdomains...");
}

/************************************* stop Enumeration *************************************/
void Brute::on_pushButton_stop_subBrute_clicked(){
    emit stop_subBrute();
}
void Brute::on_pushButton_stop_tldBrute_clicked(){
    emit stop_tldBrute();
}
void Brute::on_pushButton_stop_activeSubdomains_clicked(){
    emit stop_activeSubdomains();
}

/************************************ Enumeration Functions *********************************/
void Brute::checkWildcards(){
    Enumerator_Wildcards *wildcardsEnumerator = new Enumerator_Wildcards(&scanArguments_subBrute);
    QThread *cThread = new QThread;
    wildcardsEnumerator->Enumerate(cThread);
    wildcardsEnumerator->moveToThread(cThread);
    //...
    connect(cThread, SIGNAL(finished()), this, SLOT(startEnumeration_subBrute()));
    connect(cThread, SIGNAL(finished()), wildcardsEnumerator, SLOT(deleteLater()));
    connect(cThread, SIGNAL(finished()), cThread, SLOT(deleteLater()));
    //...
    cThread->start();
}

void Brute::startEnumeration_subBrute(){
    int maxThreads = scanArguments_subBrute.maxThreads;
    int wordlistCount = ui->listWidget_wordlist_subBrute->count();
    if(maxThreads > wordlistCount){
        maxThreads = wordlistCount;
    }
    activeThreads_subBrute = maxThreads;
    scanArguments_subBrute.enumeratedWordlists = 0;
    for(int i = 0; i != maxThreads; i++){
        //...
        Enumerator_subBrute *Enumerator = new Enumerator_subBrute(&scanArguments_subBrute);
        QThread *cThread = new QThread;
        Enumerator->Enumerate(cThread);
        Enumerator->moveToThread(cThread);
        //...
        connect(Enumerator, SIGNAL(subdomain(QString)), this, SLOT(onSubdomain_subBrute(QString)));
        connect(Enumerator, SIGNAL(scanLog(QString)), this, SLOT(log_subBrute(QString)));
        connect(cThread, SIGNAL(finished()), this, SLOT(onThreadEnd_subBrute()));
        connect(cThread, SIGNAL(finished()), Enumerator, SLOT(deleteLater()));
        connect(cThread, SIGNAL(finished()), cThread, SLOT(deleteLater()));
        connect(this, SIGNAL(stop_subBrute()), Enumerator, SLOT(onStop()));
        //...
        cThread->start();
    }
}

void Brute::startEnumeration_tldBrute(){
    int maxThreads = scanArguments_tldBrute.maxThreads;
    int totalWordlist = ui->listWidget_wordlist_tldBrute->count();
    if(maxThreads > totalWordlist){
        maxThreads = totalWordlist;
    }
    activeThreads_tldBrute = maxThreads;
    scanArguments_tldBrute.enumeratedWordlists = 0;
    for(int i = 0; i != maxThreads; i++){
        //...
        Enumerator_tldBrute *Enumerator = new Enumerator_tldBrute(&scanArguments_tldBrute);
        QThread *cThread = new QThread;
        Enumerator->Enumerate(cThread);
        Enumerator->moveToThread(cThread);
        //...
        connect(Enumerator, SIGNAL(subdomain(QString)), this, SLOT(onSubdomain_tldBrute(QString)));
        connect(Enumerator, SIGNAL(scanLog(QString)), this, SLOT(log_tldBrute(QString)));
        connect(cThread, SIGNAL(finished()), this, SLOT(onThreadEnd_tldBrute()));
        connect(cThread, SIGNAL(finished()), Enumerator, SLOT(deleteLater()));
        connect(cThread, SIGNAL(finished()), cThread, SLOT(deleteLater()));
        connect(this, SIGNAL(stop_tldBrute()), Enumerator, SLOT(onStop()));
        //...
        cThread->start();
    }
}

void Brute::startEnumeration_activeSubdomains(){
    int maxThreads = scanArguments_activeSubdomains.maxThreads;
    int wordlistCount = ui->listWidget_wordlist_activeSubdomains->count();
    if(maxThreads > wordlistCount){
        maxThreads = wordlistCount;
    }
    activeThreads_activeSubdomains = maxThreads;
    scanArguments_activeSubdomains.enumeratedWordlists = 0;
    for(int i = 0; i != maxThreads; i++){
        //...
        Enumerator_activeSubdomains *Enumerator = new Enumerator_activeSubdomains(&scanArguments_activeSubdomains);
        QThread *cThread = new QThread;
        Enumerator->Enumerate(cThread);
        Enumerator->moveToThread(cThread);
        //...
        connect(Enumerator, SIGNAL(subdomain(QString)), this, SLOT(onSubdomain_activeSubdomains(QString)));
        connect(Enumerator, SIGNAL(scanLog(QString)), this, SLOT(log_activeSubdomains(QString)));
        connect(cThread, SIGNAL(finished()), this, SLOT(onThreadEnd_activeSubdomains()));
        connect(cThread, SIGNAL(finished()), Enumerator, SLOT(deleteLater()));
        connect(cThread, SIGNAL(finished()), cThread, SLOT(deleteLater()));
        connect(this, SIGNAL(stop_activeSubdomains()), Enumerator, SLOT(onStop()));
        //...
        cThread->start();
    }
}

/************************************ Receiving Results ***********************************/
void Brute::onSubdomain_subBrute(QString subdomain){
    ui->listWidget_subdomains_subBrute->addItem(subdomain);
    subdomainCount_subBrute++;
    ui->label_subdomainsCount_subBrute->setNum(subdomainCount_subBrute);
}
void Brute::onSubdomain_tldBrute(QString subdomain){
    ui->listWidget_subdomains_tldBrute->addItem(subdomain);
    subdomainCount_tldBrute++;
    ui->label_subdomainsCount_tldBrute->setNum(subdomainCount_tldBrute);
}
void Brute::onSubdomain_activeSubdomains(QString subdomain){
    ui->listWidget_subdomains_activeSubdomains->addItem(subdomain);
    subdomainCount_activeSubdomains++;
    ui->label_subdomainsCount_activeSubdomains->setNum(subdomainCount_activeSubdomains);
}

/************************************* End Of Enumeration **********************************/
void Brute::onThreadEnd_subBrute(){
    endedThreads_subBrute++;
    if(endedThreads_subBrute == activeThreads_subBrute){
        endedThreads_subBrute = 0;
        ui->pushButton_start_subBrute->setEnabled(true);
        ui->pushButton_stop_subBrute->setDisabled(true);
        //...
        sendStatus("[*] Enumeration by subBrute Complete!");
        logs_subBrute("[END] Subdomain Bruteforcing Complete!\n");
    }
}
void Brute::onThreadEnd_tldBrute(){
    endedThreads_tldBrute++;
    if(endedThreads_tldBrute == activeThreads_tldBrute){
        endedThreads_tldBrute = 0;
        ui->pushButton_start_tldBrute->setEnabled(true);
        ui->pushButton_stop_tldBrute->setDisabled(true);
        //...
        sendStatus("[*] Enumeration by tldBrute Complete!");
        logs_tldBrute("[END] TLD Bruteforcing Complete!\n");
    }
}
void Brute::onThreadEnd_activeSubdomains(){
    endedThreads_activeSubdomains++;
    if(endedThreads_activeSubdomains == activeThreads_activeSubdomains){
        endedThreads_activeSubdomains = 0;
        ui->pushButton_start_activeSubdomains->setEnabled(true);
        ui->pushButton_stop_activeSubdomains->setDisabled(true);
        //...
        sendStatus("[*] Enumeration by activeSubdomains Complete!");
        logs_activeSubdomains("[END] Active Subdomains Enumeration Complete!\n");
    }
}

/********* Restoring the default color of the wordlist items after enumeration *************/
void Brute::on_pushButton_reloadEnumeratedWordlist_subBrute_clicked(){
    for(int i = 0; i < ui->listWidget_wordlist_subBrute->count(); i++){
        ui->listWidget_wordlist_subBrute->item(i)->setForeground(Qt::black);
    }
    ui->pushButton_reloadEnumeratedWordlist_subBrute->hide();
}
void Brute::on_pushButton_reloadEnumeratedWordlist_tldBrute_clicked(){
    for(int i = 0; i < ui->listWidget_wordlist_tldBrute->count(); i++){
        ui->listWidget_wordlist_tldBrute->item(i)->setForeground(Qt::black);
    }
    ui->pushButton_reloadEnumeratedWordlist_tldBrute->hide();
}
void Brute::on_pushButton_reloadEnumeratedWordlist_activeSubdomains_clicked(){
    for(int i = 0; i < ui->listWidget_wordlist_activeSubdomains->count(); i++){
        ui->listWidget_wordlist_activeSubdomains->item(i)->setForeground(Qt::black);
    }
    ui->pushButton_reloadEnumeratedWordlist_activeSubdomains->hide();
}

/******************************** Scan Config Dialog ***************************************/
void Brute::on_toolButton_config_subBrute_clicked(){
    BruteConfigDialog *scanConfig = new BruteConfigDialog(this, &scanArguments_subBrute);
    scanConfig->setAttribute( Qt::WA_DeleteOnClose, true );
    scanConfig->show();
}
void Brute::on_toolButton_config_tldBrute_clicked(){
    BruteConfigDialog *scanConfig = new BruteConfigDialog(this, &scanArguments_tldBrute);
    scanConfig->setAttribute( Qt::WA_DeleteOnClose, true );
    scanConfig->show();
}
void Brute::on_toolButton_config_activeSubdomains_clicked(){
    BruteConfigDialog *scanConfig = new BruteConfigDialog(this, &scanArguments_activeSubdomains);
    scanConfig->setAttribute( Qt::WA_DeleteOnClose, true );
    scanConfig->show();
}

/******************************* Generate Wordlist (Dialog) ******************************/
void Brute::on_pushButton_generate_subBrute_clicked(){
    WordListGeneratorDialog *wordlistgenerator = new WordListGeneratorDialog(this);
    wordlistgenerator->setAttribute( Qt::WA_DeleteOnClose, true );
    wordlistgenerator->show();
}
void Brute::on_pushButton_generate_tldBrute_clicked(){
    WordListGeneratorDialog *wordlistgenerator = new WordListGeneratorDialog(this);
    wordlistgenerator->setAttribute( Qt::WA_DeleteOnClose, true );
    wordlistgenerator->show();
}

/**************************** Choose Wordlist To Use (Dialog) ****************************/
void Brute::on_toolButton_wordlist_subBrute_clicked(){
    WordListDialog *wordlistDialog = new WordListDialog(this, ENUMNAME_SUBBRUTE);
    wordlistDialog->setAttribute( Qt::WA_DeleteOnClose, true );
    connect(wordlistDialog, SIGNAL(wordlistFilename(QString)), this, SLOT(onWordlistFilename_subBrute(QString)));
    wordlistDialog->show();
}
void Brute::on_toolButton_wordlist_tldBrute_clicked(){
    WordListDialog *wordlistDialog = new WordListDialog(this, ENUMNAME_TLDBRUTE);
    wordlistDialog->setAttribute( Qt::WA_DeleteOnClose, true );
    connect(wordlistDialog, SIGNAL(wordlistFilename(QString)), this, SLOT(onWordlistFilename_tldBrute(QString)));
    wordlistDialog->show();
}

/********************* Receiving the Wordlist that the user choose ***************************/
void Brute::onWordlistFilename_subBrute(QString wordlistFilename){
    QFile file(wordlistFilename);
    if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
        QTextStream in(&file);
        while (!in.atEnd()){
            ui->listWidget_wordlist_subBrute->addItem(in.readLine());
            wordlistCount_subBrute++;
        }
        file.close();
    }
    ui->label_wordlistCount_subBrute->setNum(wordlistCount_subBrute);
}
void Brute::onWordlistFilename_tldBrute(QString wordlistFilename){
    QFile file(wordlistFilename);
    if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
        QTextStream in(&file);
        while (!in.atEnd()){
            ui->listWidget_wordlist_tldBrute->addItem(in.readLine());
            wordlistCount_tldBrute++;
        }
        file.close();
    }
    ui->label_wordlistCount_tldBrute->setNum(wordlistCount_tldBrute);
}

/************************************ Loading Wordlist ***********************************/
void Brute::on_pushButton_load_subBrute_clicked(){
    QString filename = QFileDialog::getOpenFileName(this, INFO_LOADFILE, CURRENT_PATH);
    if(!filename.isEmpty()){
        QFile file(filename);
        if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
            QTextStream in(&file);
            while (!in.atEnd()){
                ui->listWidget_wordlist_subBrute->addItem(in.readLine());
                wordlistCount_subBrute++;
            }
            ui->label_wordlistCount_subBrute->setNum(wordlistCount_subBrute);
            file.close();
        }else{
            QMessageBox::warning(this, TITLE_ERROR, "Failed To Open the File!");
        }
    }
}
void Brute::on_pushButton_load_tldBrute_clicked(){
    QString filename = QFileDialog::getOpenFileName(this, INFO_LOADFILE, CURRENT_PATH);
    if(!filename.isEmpty()){
        QFile file(filename);
        if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
            QTextStream in(&file);
            while (!in.atEnd()){
                ui->listWidget_wordlist_tldBrute->addItem(in.readLine());
                wordlistCount_tldBrute++;
            }
            ui->label_wordlistCount_tldBrute->setNum(wordlistCount_tldBrute);
            file.close();
        }else{
            QMessageBox::warning(this, TITLE_ERROR, "Failed To Open the File!");
        }
    }
}
void Brute::on_pushButton_load_activeSubdomains_clicked(){
    QString filename = QFileDialog::getOpenFileName(this, INFO_LOADFILE, CURRENT_PATH);
    if(!filename.isEmpty()){
        QFile file(filename);
        if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
            QTextStream in(&file);
            while (!in.atEnd()){
                ui->listWidget_wordlist_activeSubdomains->addItem(in.readLine());
                wordlistCount_activeSubdomains++;
            }
            ui->label_wordlistCount_activeSubdomains->setNum(wordlistCount_activeSubdomains);
            file.close();
        }else{
            QMessageBox::warning(this, TITLE_ERROR, "Failed To Open the File!");
        }
    }
}

/******************************** Add Item on the Wordlist *********************************/
void Brute::on_pushButton_add_subBrute_clicked(){
    if(ui->lineEdit_wordlist_subBrute->text() != EMPTY){
        ui->listWidget_wordlist_subBrute->addItem(ui->lineEdit_wordlist_subBrute->text());
        ui->lineEdit_wordlist_subBrute->clear();
        wordlistCount_subBrute++;
        ui->label_wordlistCount_subBrute->setNum(wordlistCount_subBrute);
    }
}
void Brute::on_lineEdit_wordlist_subBrute_returnPressed(){
    on_pushButton_add_subBrute_clicked();
}
void Brute::on_pushButton_add_tldBrute_clicked(){
    if(ui->lineEdit_manualWordlist_tldBrute->text() != EMPTY){
        ui->listWidget_wordlist_tldBrute->addItem(ui->lineEdit_manualWordlist_tldBrute->text());
        ui->lineEdit_manualWordlist_tldBrute->clear();
        wordlistCount_tldBrute++;
        ui->label_wordlistCount_tldBrute->setNum(wordlistCount_tldBrute);
    }
}
void Brute::on_lineEdit_manualWordlist_tldBrute_returnPressed(){
    on_pushButton_add_tldBrute_clicked();
}

void Brute::on_pushButton_add_activeSubdomains_clicked(){
    if(ui->lineEdit_manualWordlist_activeSubdomains->text() != EMPTY){
        ui->listWidget_wordlist_activeSubdomains->addItem(ui->lineEdit_manualWordlist_activeSubdomains->text());
        ui->lineEdit_manualWordlist_activeSubdomains->clear();
        wordlistCount_activeSubdomains++;
        ui->label_wordlistCount_activeSubdomains->setNum(wordlistCount_activeSubdomains);
    }
}
void Brute::on_lineEdit_manualWordlist_activeSubdomains_returnPressed(){
    on_pushButton_add_activeSubdomains_clicked();
}

/******************************** Clear The Wordlist **************************************/
void Brute::on_pushButton_clearWordlist_subBrute_clicked(){
    ui->listWidget_wordlist_subBrute->clear();
    ui->label_wordlistCount_subBrute->clear();
    wordlistCount_subBrute = 0;
}
void Brute::on_pushButton_clearWordlist_tldBrute_clicked(){
    ui->listWidget_wordlist_tldBrute->clear();
    ui->label_wordlistCount_tldBrute->clear();
    wordlistCount_tldBrute = 0;
}
void Brute::on_pushButton_clearWordlist_activeSubdomains_clicked(){
    ui->listWidget_wordlist_activeSubdomains->clear();
    ui->label_wordlistCount_activeSubdomains->clear();
    wordlistCount_activeSubdomains = 0;
}

/***************************** Removing Item From Wordlist *********************************/
void Brute::on_pushButton_remove_subBrute_clicked(){
    int selectionCount = ui->listWidget_wordlist_subBrute->selectedItems().count();
    if(selectionCount){
        qDeleteAll(ui->listWidget_wordlist_subBrute->selectedItems());
        wordlistCount_subBrute = wordlistCount_subBrute-selectionCount;
    }
    ui->label_wordlistCount_subBrute->setNum(wordlistCount_subBrute);
}
void Brute::on_pushButton_remove_tldBrute_clicked(){
    int selectionCount = ui->listWidget_wordlist_tldBrute->selectedItems().count();
    if(selectionCount){
        qDeleteAll(ui->listWidget_wordlist_tldBrute->selectedItems());
        wordlistCount_tldBrute = wordlistCount_tldBrute-selectionCount;
    }
    ui->label_wordlistCount_tldBrute->setNum(wordlistCount_tldBrute);
}
void Brute::on_pushButton_remove_activeSubdomains_clicked(){
    int selectionCount = ui->listWidget_wordlist_activeSubdomains->selectedItems().count();
    if(selectionCount){
        qDeleteAll(ui->listWidget_wordlist_activeSubdomains->selectedItems());
        wordlistCount_activeSubdomains = wordlistCount_activeSubdomains-selectionCount;
    }
    ui->label_wordlistCount_activeSubdomains->setNum(wordlistCount_activeSubdomains);
}

/**************************** Clear the Enumerated Subdomains *****************************/
void Brute::on_pushButton_clearResults_subBrute_clicked(){
    // if the current tab is subdomains clear subdomains if logs clear logs...
    if(ui->tabWidget_results_subBrute->currentIndex() == 0){
        ui->listWidget_subdomains_subBrute->clear();
        ui->label_subdomainsCount_subBrute->clear();
        subdomainCount_subBrute = 0;
    }else{
        ui->listWidget_logs_subBrute->clear();
    }
}
void Brute::on_pushButton_clearResults_tldBrute_clicked(){
    // if the current tab is subdomains clear subdomains if logs clear logs...
    if(ui->tabWidget_results_tldBrute->currentIndex() == 0){
        ui->listWidget_subdomains_tldBrute->clear();
        ui->label_subdomainsCount_tldBrute->clear();
        subdomainCount_tldBrute = 0;
    }else{
        ui->listWidget_logs_tldBrute->clear();
    }
}
void Brute::on_pushButton_clearResults_activeSubdomains_clicked(){
    // if the current tab is subdomains clear subdomains if logs clear logs...
    if(ui->tabWidget_results_activeSubdomains->currentIndex() == 0){
        ui->listWidget_subdomains_activeSubdomains->clear();
        ui->label_subdomainsCount_activeSubdomains->clear();
        subdomainCount_activeSubdomains = 0;
    }else{
        ui->listWidget_logs_activeSubdomains->clear();
    }
}

/****************************** Save And Display logs *************************************/
void Brute::logs_subBrute(QString log){
    sendLog(log);
    ui->listWidget_logs_subBrute->addItem(log);
    if (log.startsWith("[ERROR]")){
        ui->listWidget_logs_subBrute->item(ui->listWidget_logs_subBrute->count()-1)->setForeground(Qt::red);
        return;
    }
    if(log.startsWith("[START]") || log.startsWith("[END]")){
        ui->listWidget_logs_subBrute->item(ui->listWidget_logs_subBrute->count()-1)->setFont(QFont("MS Shell Dlg 2", 8, QFont::Bold));
        return;
    }
}
void Brute::logs_tldBrute(QString log){
    sendLog(log);
    ui->listWidget_logs_tldBrute->addItem(log);
    if(log.startsWith("[ERROR]")){
        ui->listWidget_logs_tldBrute->item(ui->listWidget_logs_tldBrute->count()-1)->setForeground(Qt::red);
        return;
    }
    if(log.startsWith("[START]") || log.startsWith("[END]")){
        ui->listWidget_logs_tldBrute->item(ui->listWidget_logs_tldBrute->count()-1)->setFont(QFont("MS Shell Dlg 2", 8, QFont::Bold));
        return;
    }
}
void Brute::logs_activeSubdomains(QString log){
    sendLog(log);
    ui->listWidget_logs_activeSubdomains->addItem(log);
    if(log.startsWith("[ERROR]")){
        ui->listWidget_logs_activeSubdomains->item(ui->listWidget_logs_activeSubdomains->count()-1)->setForeground(Qt::red);
        return;
    }
    if(log.startsWith("[START]") || log.startsWith("[END]")){
        ui->listWidget_logs_activeSubdomains->item(ui->listWidget_logs_activeSubdomains->count()-1)->setFont(QFont("MS Shell Dlg 2", 8, QFont::Bold));
        return;
    }
}

/***************************** Context Menu For Action Button *****************************/
void Brute::on_pushButton_action_subBrute_clicked(){
    actionContextMenu(ENUMNAME_SUBBRUTE);
}
void Brute::on_pushButton_action_tldBrute_clicked(){
    actionContextMenu(ENUMNAME_TLDBRUTE);
}
void Brute::on_pushButton_action_activeSubdomains_clicked(){
    actionContextMenu(ENUMNAME_ACTIVESUBDOMAINS);
}

/****************************** Cursor right-click Context Menu ******************************/
void Brute::on_listWidget_subdomains_subBrute_customContextMenuRequested(const QPoint &pos){
    Q_UNUSED(pos);
    rightClickContextMenu(ENUMNAME_SUBBRUTE);
}
void Brute::on_listWidget_subdomains_tldBrute_customContextMenuRequested(const QPoint &pos){
    Q_UNUSED(pos);
    rightClickContextMenu(ENUMNAME_TLDBRUTE);
}
void Brute::on_listWidget_subdomains_activeSubdomains_customContextMenuRequested(const QPoint &pos){
    Q_UNUSED(pos);
    rightClickContextMenu(ENUMNAME_ACTIVESUBDOMAINS);
}

/********************************* action Context Menu ************************************/
void Brute::actionContextMenu(int enumName){
    QMenu *menu = new QMenu(this);
    menu->setAttribute( Qt::WA_DeleteOnClose, true );
    menu->setObjectName("mainMenu");
    //...
    QAction actionSendToSave("Send To Save", this);
    QAction actionSort("Sort Subdomains", this);
    QAction actionSendToMultiLevel("Send To Multi-level Scan");
    QAction actionSendToDnsRecords("Send To DnsRecords");
    //...
    if(enumName == ENUMNAME_SUBBRUTE){
        connect(&actionSendToSave, SIGNAL(triggered()), this, SLOT(actionSendToSave_subBrute()));
        connect(&actionSort, SIGNAL(triggered()), this, SLOT(actionSort_tldBrute()));
        connect(&actionSendToDnsRecords, SIGNAL(triggered()), this, SLOT(actionSendToDnsRecords_subBrute()));
        connect(&actionSendToMultiLevel, SIGNAL(triggered()), this, SLOT(actionSendToMultiLevel_subBrute()));
    }
    if(enumName == ENUMNAME_TLDBRUTE){
        connect(&actionSendToSave, SIGNAL(triggered()), this, SLOT(actionSendToSave_tldBrute()));
        connect(&actionSort, SIGNAL(triggered()), this, SLOT(actionSort_tldBrute()));
        connect(&actionSendToDnsRecords, SIGNAL(triggered()), this, SLOT(actionSendToDnsRecords_tldBrute()));
        connect(&actionSendToMultiLevel, SIGNAL(triggered()), this, SLOT(actionSendToMultiLevel_tldBrute()));
    }
    if(enumName == ENUMNAME_ACTIVESUBDOMAINS){
        connect(&actionSendToSave, SIGNAL(triggered()), this, SLOT(actionSendToSave_activeSubdomains()));
        connect(&actionSort, SIGNAL(triggered()), this, SLOT(actionSort_activeSubdomains()));
        connect(&actionSendToDnsRecords, SIGNAL(triggered()), this, SLOT(actionSendToDnsRecords_activeSubdomains()));
        connect(&actionSendToMultiLevel, SIGNAL(triggered()), this, SLOT(actionSendToMultiLevel_activeSubdomains()));
    }
    //...
    menu->addAction(&actionSort);
    menu->addSeparator();
    menu->addAction(&actionSendToDnsRecords);
    menu->addAction(&actionSendToSave);
    menu->addAction(&actionSendToMultiLevel);
    //...
    QPoint pos;
    if(enumName == ENUMNAME_SUBBRUTE){
        pos = ui->pushButton_action_subBrute->mapToGlobal(QPoint(0,0));
    }
    if(enumName == ENUMNAME_TLDBRUTE){
        pos = ui->pushButton_action_tldBrute->mapToGlobal(QPoint(0,0));
    }
    if(enumName == ENUMNAME_ACTIVESUBDOMAINS){
        pos = ui->pushButton_action_activeSubdomains->mapToGlobal(QPoint(0,0));
    }
    int x = pos.x()+76;
    int y = pos.y();
    //...
    menu->setStyleSheet("QMenu::item::selected#mainMenu{background-color: rgb(170, 170, 127)} QMenu#mainMenu{background-color: qlineargradient(x1:0,  y1:0, x2:0, y2:1, stop: 0 white, stop: 0.8 rgb(246, 255, 199)); border-style: solid; border-color: black; border-width: 1px;}");
    menu->move(QPoint(x, y));
    menu->exec();
}

/********************************* right-click Context Menu ************************************/
void Brute::rightClickContextMenu(int enumName){
    if(enumName == ENUMNAME_SUBBRUTE){
        if(ui->listWidget_subdomains_subBrute->selectedItems().isEmpty()){
            return;
        }
    }
    if(enumName == ENUMNAME_TLDBRUTE){
        if(ui->listWidget_subdomains_tldBrute->selectedItems().isEmpty()){
            return;
        }
    }
    if(enumName == ENUMNAME_ACTIVESUBDOMAINS){
        if(ui->listWidget_subdomains_activeSubdomains->selectedItems().isEmpty()){
            return;
        }
    }
    //...
    QMenu *menu = new QMenu(this);
    menu->setAttribute( Qt::WA_DeleteOnClose, true );
    menu->setObjectName("mainMenu");
    QAction actionSendToSave("Send Selected To Save", this);
    QAction actionSendToDnsRecords("Send Selected To DnsRecords");
    QAction actionOpenInBrowser("Open Selected in Browser");
    //...
    if(enumName == ENUMNAME_SUBBRUTE){
        connect(&actionOpenInBrowser, SIGNAL(triggered()), this, SLOT(cursorOpenInBrowser_subBrute()));
        connect(&actionSendToSave, SIGNAL(triggered()), this, SLOT(cursorSendToSave_subBrute()));
        connect(&actionSendToDnsRecords, SIGNAL(triggered()), this, SLOT(cursorSendToDnsRecords_subBrute()));
    }
    if(enumName == ENUMNAME_TLDBRUTE){
        connect(&actionOpenInBrowser, SIGNAL(triggered()), this, SLOT(cursorOpenInBrowser_tldBrute()));
        connect(&actionSendToSave, SIGNAL(triggered()), this, SLOT(cursorSendToSave_tldBrute()));
        connect(&actionSendToDnsRecords, SIGNAL(triggered()), this, SLOT(cursorSendToDnsRecords_tldBrute()));
    }
    if(enumName == ENUMNAME_ACTIVESUBDOMAINS){
        connect(&actionOpenInBrowser, SIGNAL(triggered()), this, SLOT(cursorOpenInBrowser_activeSubdomains()));
        connect(&actionSendToSave, SIGNAL(triggered()), this, SLOT(cursorSendToSave_activeSubdomains()));
        connect(&actionSendToDnsRecords, SIGNAL(triggered()), this, SLOT(cursorSendToDnsRecords_activeSubdomains()));
    }
    //...
    menu->addAction(&actionOpenInBrowser);
    menu->addSeparator();
    menu->addAction(&actionSendToDnsRecords);
    menu->addAction(&actionSendToSave);
    //...
    QPoint globalCursorPos = QCursor::pos();
    int mouseScreen = qApp->desktop()->screenNumber(globalCursorPos);
    QRect mouseScreenGeometry = qApp->desktop()->screen(mouseScreen)->geometry();
    QPoint localCursorPosition = globalCursorPos - mouseScreenGeometry.topLeft();
    //...
    menu->setStyleSheet("QMenu::item::selected#mainMenu{background-color: rgb(170, 170, 127)} QMenu#mainMenu{background-color: qlineargradient(x1:0,  y1:0, x2:0, y2:1, stop: 0 white, stop: 0.8 rgb(246, 255, 199)); border-style: solid; border-color: black; border-width: 1px;}");
    menu->move(localCursorPosition);
    menu->exec();
}

/****************************** Action Context Menu Methods ***************************/
void Brute::actionSendToSave_subBrute(){
    int resultsCount = ui->listWidget_subdomains_subBrute->count();
    for(int i = 0; i != resultsCount; ++i){
        emit sendResultsToSave(ui->listWidget_subdomains_subBrute->item(i)->text());
    }
    logs_subBrute("[*] Sent "+QString::number(resultsCount)+" subBrute Enumerated Subdomains To Save Tab...");
    emit changeTabToSave();
}
void Brute::actionSendToSave_tldBrute(){
    int resultsCount = ui->listWidget_subdomains_tldBrute->count();
    for(int i = 0; i != resultsCount; ++i){
        emit sendResultsToSave(ui->listWidget_subdomains_tldBrute->item(i)->text());
    }
    logs_tldBrute("[*] Sent "+QString::number(resultsCount)+" tldBrute Enumerated Subdomains To Save Tab...");
    emit changeTabToSave();
}
void Brute::actionSendToSave_activeSubdomains(){
    int resultsCount = ui->listWidget_subdomains_activeSubdomains->count();
    for(int i = 0; i != resultsCount; ++i){
        emit sendResultsToSave(ui->listWidget_subdomains_activeSubdomains->item(i)->text());
    }
    logs_activeSubdomains("[*] Sent "+QString::number(resultsCount)+" activeSubdomains Enumerated Subdomains To Save Tab...");
    emit changeTabToSave();
}

/*********************************************************************************/
void Brute::actionSort_subBrute(){
    ui->listWidget_subdomains_subBrute->sortItems();
}
void Brute::actionSort_tldBrute(){
    ui->listWidget_subdomains_tldBrute->sortItems();
}
void Brute::actionSort_activeSubdomains(){
    ui->listWidget_subdomains_activeSubdomains->sortItems();
}

/*********************************************************************************/
void Brute::actionSendToDnsRecords_subBrute(){

}
void Brute::actionSendToDnsRecords_tldBrute(){

}
void Brute::actionSendToDnsRecords_activeSubdomains(){

}

/**********************************************************************************/
void Brute::actionSendToMultiLevel_subBrute(){

}
void Brute::actionSendToMultiLevel_tldBrute(){

}
void Brute::actionSendToMultiLevel_activeSubdomains(){

}

/*************************** Cursor Right Click Context Menu ***********************/
void Brute::cursorSendToSave_subBrute(){
    foreach(QListWidgetItem * item, ui->listWidget_subdomains_subBrute->selectedItems()){
        emit sendResultsToSave(item->text());
    }
    logs_subBrute("[*] Sent "+QString::number(ui->listWidget_subdomains_subBrute->count())+" subBrute Enumerated Subdomains To Save Tab...");
    emit changeTabToSave();
}
void Brute::cursorSendToSave_tldBrute(){
    foreach(QListWidgetItem * item, ui->listWidget_subdomains_tldBrute->selectedItems()){
        emit sendResultsToSave(item->text());
    }
    logs_subBrute("[*] Sent "+QString::number(ui->listWidget_subdomains_subBrute->count())+" tldBrute Enumerated Subdomains To Save Tab...");
    emit changeTabToSave();
}
void Brute::cursorSendToSave_activeSubdomains(){
    foreach(QListWidgetItem * item, ui->listWidget_subdomains_activeSubdomains->selectedItems()){
        emit sendResultsToSave(item->text());
    }
    logs_activeSubdomains("[*] Sent "+QString::number(ui->listWidget_subdomains_activeSubdomains->count())+" activeSubdomains Enumerated Subdomains To Save Tab...");
    emit changeTabToSave();
}

/************************************************************************************/
void Brute::cursorOpenInBrowser_subBrute(){
    // iterate and open each selected item in a browser...
    foreach(QListWidgetItem * item, ui->listWidget_subdomains_subBrute->selectedItems()){
        QDesktopServices::openUrl(QUrl("https://"+item->text(), QUrl::TolerantMode));
    }
}
void Brute::cursorOpenInBrowser_tldBrute(){
    // iterate and open each selected item in a browser...
    foreach(QListWidgetItem * item, ui->listWidget_subdomains_tldBrute->selectedItems()){
        QDesktopServices::openUrl(QUrl("https://"+item->text(), QUrl::TolerantMode));
    }
}
void Brute::cursorOpenInBrowser_activeSubdomains(){
    // iterate and open each selected item in a browser...
    foreach(QListWidgetItem * item, ui->listWidget_subdomains_activeSubdomains->selectedItems()){
        QDesktopServices::openUrl(QUrl("https://"+item->text(), QUrl::TolerantMode));
    }
}


/*************************************************************************************/
void Brute::cursorSendToDnsRecords_subBrute(){

}
void Brute::cursorSendToDnsRecords_tldBrute(){

}
void Brute::cursorSendToDnsRecords_activeSubdomains(){

}

/********** on receiving signal to send the enumeration results to the save tab ********/
void Brute::onSendResultsToSave(){
    int resultsCount = ui->listWidget_subdomains_subBrute->count();
    for(int i = 0; i != resultsCount; ++i){
        emit sendResultsToSave(ui->listWidget_subdomains_subBrute->item(i)->text());
    }
    logs_subBrute("[*] Sent "+QString::number(resultsCount)+" subBrute Enumerated Subdomains To Save Tab...");
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
}

void Brute::on_pushButton_get_activeSubdomains_clicked(){

}
