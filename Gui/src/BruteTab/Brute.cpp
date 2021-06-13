#include "Brute.h"
#include "ui_Brute.h"

/*

  add an advanced option that resolves an item on wordlist with all nameservers..
  first obtains the server from the domain name, then uses those nameservers as default nameservers
  for the scan...

*/

/*************************** Class Constructor & Deconstructor *************************/
Brute::Brute(QWidget *parent) : QWidget(parent), ui(new Ui::Brute),
      //...
      model_subBrute(new QStandardItemModel),
      model_tldBrute(new QStandardItemModel),
      model_activeSubdomains(new QStandardItemModel),
      //...
      scanArguments_subBrute(new ScanArguments_Brute),
      scanArguments_tldBrute(new ScanArguments_Brute),
      scanArguments_activeSubdomains(new ScanArguments_Brute)
{
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
    QStringList headerLabels = {"Subdomain Name:", "IpAddress"};
    model_subBrute->setHorizontalHeaderLabels(headerLabels);
    model_tldBrute->setHorizontalHeaderLabels(headerLabels);
    model_activeSubdomains->setHorizontalHeaderLabels(headerLabels);
    //...
    ui->tableView_results_subBrute->setModel(model_subBrute);
    ui->tableView_results_tldBrute->setModel(model_tldBrute);
    ui->tableView_results_activeSubdomains->setModel(model_activeSubdomains);
    ///
    /// Setting highlight Color for items on the listView...
    ///
    QPalette p = palette();
    p.setColor(QPalette::Highlight, QColor(188, 188, 141));
    p.setColor(QPalette::HighlightedText, QColor(Qt::black));
    ui->tableView_results_subBrute->setPalette(p);
    ui->tableView_results_tldBrute->setPalette(p);
    ui->tableView_results_activeSubdomains->setPalette(p);
    //...
    currentPath = QDir::currentPath();
}
Brute::~Brute(){
    delete model_subBrute;
    delete model_tldBrute;
    delete model_activeSubdomains;
    //...
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
    scanArguments_subBrute->targetDomain = TargetNameFilter(ui->lineEdit_targetDomain_subBrute->text(), ENUMNAME_SUBBRUTE);
    scanArguments_subBrute->wordlist = ui->listWidget_wordlist_subBrute;
    if(scanArguments_subBrute->checkWildcardSubdomains){
        checkWildcards();
    }else{
        startEnumeration_subBrute();
    }
    //...
    sendStatus("[*] BruteForcing Subdomains For Domain"+scanArguments_subBrute->targetDomain);
    logs_subBrute("[START] BruteForcing Subdomains For Domain: "+scanArguments_subBrute->targetDomain);
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
    scanArguments_tldBrute->targetDomain = TargetNameFilter(ui->lineEdit_targetDomain_tldBrute->text(), ENUMNAME_TLDBRUTE);
    scanArguments_tldBrute->wordlist = ui->listWidget_wordlist_tldBrute;
    startEnumeration_tldBrute();
    //...
    sendStatus("[*] BruteForcing TLDs For Domain: "+scanArguments_tldBrute->targetDomain);
    logs_tldBrute("[START] BruteForcing TLDs For Domain: "+scanArguments_tldBrute->targetDomain);
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
    scanArguments_activeSubdomains->wordlist = ui->listWidget_wordlist_activeSubdomains;
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
    Enumerator_Wildcards *wildcardsEnumerator = new Enumerator_Wildcards(scanArguments_subBrute);
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
    int maxThreads = scanArguments_subBrute->maxThreads;
    int wordlistCount = ui->listWidget_wordlist_subBrute->count();
    if(maxThreads > wordlistCount){
        maxThreads = wordlistCount;
    }
    activeThreads_subBrute = maxThreads;
    scanArguments_subBrute->currentItemToEnumerate = 0;
    for(int i = 0; i != maxThreads; i++){
        //...
        Enumerator_subBrute *Enumerator = new Enumerator_subBrute(scanArguments_subBrute);
        QThread *cThread = new QThread;
        Enumerator->Enumerate(cThread);
        Enumerator->moveToThread(cThread);
        //...
        connect(Enumerator, SIGNAL(resolvedSubdomain(QString, QString)), this, SLOT(resolvedSubdomain_subBrute(QString, QString)));
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
    int maxThreads = scanArguments_tldBrute->maxThreads;
    int totalWordlist = ui->listWidget_wordlist_tldBrute->count();
    if(maxThreads > totalWordlist){
        maxThreads = totalWordlist;
    }
    activeThreads_tldBrute = maxThreads;
    scanArguments_tldBrute->currentItemToEnumerate = 0;
    for(int i = 0; i != maxThreads; i++){
        //...
        Enumerator_tldBrute *Enumerator = new Enumerator_tldBrute(scanArguments_tldBrute);
        QThread *cThread = new QThread;
        Enumerator->Enumerate(cThread);
        Enumerator->moveToThread(cThread);
        //...
        connect(Enumerator, SIGNAL(resolvedSubdomain(QString, QString)), this, SLOT(resolvedSubdomain_tldBrute(QString, QString)));
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
    int maxThreads = scanArguments_activeSubdomains->maxThreads;
    int wordlistCount = ui->listWidget_wordlist_activeSubdomains->count();
    if(maxThreads > wordlistCount){
        maxThreads = wordlistCount;
    }
    activeThreads_activeSubdomains = maxThreads;
    scanArguments_activeSubdomains->currentItemToEnumerate = 0;
    for(int i = 0; i != maxThreads; i++){
        //...
        Enumerator_activeSubdomains *Enumerator = new Enumerator_activeSubdomains(scanArguments_activeSubdomains);
        QThread *cThread = new QThread;
        Enumerator->Enumerate(cThread);
        Enumerator->moveToThread(cThread);
        //...
        connect(Enumerator, SIGNAL(resolvedSubdomain(QString, QString)), this, SLOT(resolvedSubdomain_activeSubdomains(QString, Qstring)));
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
void Brute::resolvedSubdomain_subBrute(QString subdomain, QString ipAddress){
    model_subBrute->setItem(subdomainCount_subBrute, 0, new QStandardItem(subdomain));
    model_subBrute->setItem(subdomainCount_subBrute, 1, new QStandardItem(ipAddress));
    subdomainCount_subBrute++;
    ui->label_subdomainsCount_subBrute->setNum(subdomainCount_subBrute);
}
void Brute::resolvedSubdomain_tldBrute(QString subdomain, QString ipAddress){
    model_tldBrute->setItem(subdomainCount_tldBrute, 0, new QStandardItem(subdomain));
    model_tldBrute->setItem(subdomainCount_tldBrute, 1, new QStandardItem(ipAddress));
    subdomainCount_tldBrute++;
    ui->label_subdomainsCount_tldBrute->setNum(subdomainCount_tldBrute);
}
void Brute::resolvedSubdomain_activeSubdomains(QString subdomain, QString ipAddress){
    model_activeSubdomains->setItem(subdomainCount_activeSubdomains, 0, new QStandardItem(subdomain));
    model_activeSubdomains->setItem(subdomainCount_activeSubdomains, 1, new QStandardItem(ipAddress));
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
    BruteConfigDialog *scanConfig = new BruteConfigDialog(this, scanArguments_subBrute);
    scanConfig->setAttribute( Qt::WA_DeleteOnClose, true );
    scanConfig->show();
}
void Brute::on_toolButton_config_tldBrute_clicked(){
    BruteConfigDialog *scanConfig = new BruteConfigDialog(this, scanArguments_tldBrute);
    scanConfig->setAttribute( Qt::WA_DeleteOnClose, true );
    scanConfig->show();
}
void Brute::on_toolButton_config_activeSubdomains_clicked(){
    BruteConfigDialog *scanConfig = new BruteConfigDialog(this, scanArguments_activeSubdomains);
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
    //...
    model_subBrute->setHorizontalHeaderLabels({"Subdomain Name", "IpAddress"});
}
void Brute::on_pushButton_clearWordlist_tldBrute_clicked(){
    ui->listWidget_wordlist_tldBrute->clear();
    ui->label_wordlistCount_tldBrute->clear();
    wordlistCount_tldBrute = 0;
    //...
    model_tldBrute->setHorizontalHeaderLabels({"Subdomain Name", "IpAddress"});
}
void Brute::on_pushButton_clearWordlist_activeSubdomains_clicked(){
    ui->listWidget_wordlist_activeSubdomains->clear();
    ui->label_wordlistCount_activeSubdomains->clear();
    wordlistCount_activeSubdomains = 0;
    //...
    model_activeSubdomains->setHorizontalHeaderLabels({"Subdomain Name", "IpAddress"});
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
        model_subBrute->clear();
        ui->label_subdomainsCount_subBrute->clear();
        subdomainCount_subBrute = 0;
    }else{
        ui->listWidget_logs_subBrute->clear();
    }
}
void Brute::on_pushButton_clearResults_tldBrute_clicked(){
    // if the current tab is subdomains clear subdomains if logs clear logs...
    if(ui->tabWidget_results_tldBrute->currentIndex() == 0){
        model_tldBrute->clear();
        ui->label_subdomainsCount_tldBrute->clear();
        subdomainCount_tldBrute = 0;
    }else{
        ui->listWidget_logs_tldBrute->clear();
    }
}
void Brute::on_pushButton_clearResults_activeSubdomains_clicked(){
    // if the current tab is subdomains clear subdomains if logs clear logs...
    if(ui->tabWidget_results_activeSubdomains->currentIndex() == 0){
        model_activeSubdomains->clear();
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
    // getting the position of the action button to place the context menu...
    QPoint pos = ui->pushButton_action_subBrute->mapToGlobal(QPoint(0,0));
    // showing the context menu right by the side of the action button...
    showContextMenu_actionButton(ENUMNAME_SUBBRUTE, QPoint(pos.x()+76, pos.y()));
}
void Brute::on_pushButton_action_tldBrute_clicked(){
    // getting the position of the action button to place the context menu...
    QPoint pos = ui->pushButton_action_tldBrute->mapToGlobal(QPoint(0,0));
    // showing the context menu right by the side of the action button...
    showContextMenu_actionButton(ENUMNAME_TLDBRUTE, QPoint(pos.x()+76, pos.y()));
}
void Brute::on_pushButton_action_activeSubdomains_clicked(){
    // getting the position of the action button to place the context menu...
    QPoint pos = ui->pushButton_action_activeSubdomains->mapToGlobal(QPoint(0,0));
    // showing the context menu right by the side of the action button...
    showContextMenu_actionButton(ENUMNAME_ACTIVESUBDOMAINS, QPoint(pos.x()+76, pos.y()));
}

/****************************** Cursor right-click Context Menu ******************************/
void Brute::on_tableView_results_subBrute_customContextMenuRequested(const QPoint &pos){
    Q_UNUSED(pos);
    // check if user right clicked on items else dont show the context menu...
    if(!ui->tableView_results_subBrute->selectionModel()->isSelected(ui->tableView_results_subBrute->currentIndex())){
        return;
    }
    showContextMenu_rightClick(ENUMNAME_SUBBRUTE);
}
void Brute::on_tableView_results_tldBrute_customContextMenuRequested(const QPoint &pos){
    Q_UNUSED(pos);
    // check if user right clicked on items else dont show the context menu...
    if(!ui->tableView_results_tldBrute->selectionModel()->isSelected(ui->tableView_results_tldBrute->currentIndex())){
        return;
    }
    showContextMenu_rightClick(ENUMNAME_TLDBRUTE);
}
void Brute::on_tableView_results_activeSubdomains_customContextMenuRequested(const QPoint &pos){
    Q_UNUSED(pos);
    // check if user right clicked on items else dont show the context menu...
    if(!ui->tableView_results_activeSubdomains->selectionModel()->isSelected(ui->tableView_results_activeSubdomains->currentIndex())){
        return;
    }
    showContextMenu_rightClick(ENUMNAME_ACTIVESUBDOMAINS);
}

/********************************* action Context Menu ************************************/
/*
    Re-create this functionallity on the constructor for Efficiency...
*/
void Brute::showContextMenu_actionButton(const int enumName, const QPoint &position){
    QMenu *menu = new QMenu(this);
    menu->setAttribute( Qt::WA_DeleteOnClose, true );
    menu->setObjectName("mainMenu");
    //...
    QAction actionSendToSave("Send To Save", this);
    QAction actionSendToMultiLevel("Send To Multi-level Scan");
    QAction actionSendToDnsRecords("Send To DnsRecords");
    //...
    if(enumName == ENUMNAME_SUBBRUTE){
        connect(&actionSendToSave, SIGNAL(triggered()), this, SLOT(actionSendToSave_subBrute()));
        connect(&actionSendToDnsRecords, SIGNAL(triggered()), this, SLOT(actionSendToDnsRecords_subBrute()));
        connect(&actionSendToMultiLevel, SIGNAL(triggered()), this, SLOT(actionSendToMultiLevel_subBrute()));
    }
    if(enumName == ENUMNAME_TLDBRUTE){
        connect(&actionSendToSave, SIGNAL(triggered()), this, SLOT(actionSendToSave_tldBrute()));
        connect(&actionSendToDnsRecords, SIGNAL(triggered()), this, SLOT(actionSendToDnsRecords_tldBrute()));
        connect(&actionSendToMultiLevel, SIGNAL(triggered()), this, SLOT(actionSendToMultiLevel_tldBrute()));
    }
    if(enumName == ENUMNAME_ACTIVESUBDOMAINS){
        connect(&actionSendToSave, SIGNAL(triggered()), this, SLOT(actionSendToSave_activeSubdomains()));
        connect(&actionSendToDnsRecords, SIGNAL(triggered()), this, SLOT(actionSendToDnsRecords_activeSubdomains()));
        connect(&actionSendToMultiLevel, SIGNAL(triggered()), this, SLOT(actionSendToMultiLevel_activeSubdomains()));
    }
    //...
    menu->addSeparator();
    menu->addAction(&actionSendToDnsRecords);
    menu->addAction(&actionSendToSave);
    menu->addAction(&actionSendToMultiLevel);
    //...
    menu->setStyleSheet("QMenu::item::selected#mainMenu{background-color: rgb(170, 170, 127)} QMenu#mainMenu{background-color: qlineargradient(x1:0,  y1:0, x2:0, y2:1, stop: 0 white, stop: 0.8 rgb(246, 255, 199)); border-style: solid; border-color: black; border-width: 1px;}");
    menu->move(position);
    menu->exec();
}

/********************************* right-click Context Menu ************************************/
void Brute::showContextMenu_rightClick(const int enumName){
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
    QRect mouseScreenGeometry = qApp->desktop()->screen(qApp->desktop()->screenNumber(globalCursorPos))->geometry();
    QPoint localCursorPosition = globalCursorPos - mouseScreenGeometry.topLeft();
    //...
    menu->setStyleSheet("QMenu::item::selected#mainMenu{background-color: rgb(170, 170, 127)} QMenu#mainMenu{background-color: qlineargradient(x1:0,  y1:0, x2:0, y2:1, stop: 0 white, stop: 0.8 rgb(246, 255, 199)); border-style: solid; border-color: black; border-width: 1px;}");
    menu->move(localCursorPosition);
    menu->exec();
}

/****************************** Action Context Menu Methods ***************************/
void Brute::actionSendToSave_subBrute(){
    /*
    int resultsCount = ui->listWidget_subdomains_subBrute->count();
    for(int i = 0; i != resultsCount; ++i){
        emit sendResultsToSave(ui->listWidget_subdomains_subBrute->item(i)->text());
    }
    logs_subBrute("[*] Sent "+QString::number(resultsCount)+" subBrute Enumerated Subdomains To Save Tab...");
    emit changeTabToSave();
    */
}
void Brute::actionSendToSave_tldBrute(){
    /*
    int resultsCount = ui->listWidget_subdomains_tldBrute->count();
    for(int i = 0; i != resultsCount; ++i){
        emit sendResultsToSave(ui->listWidget_subdomains_tldBrute->item(i)->text());
    }
    logs_tldBrute("[*] Sent "+QString::number(resultsCount)+" tldBrute Enumerated Subdomains To Save Tab...");
    emit changeTabToSave();
    */
}
void Brute::actionSendToSave_activeSubdomains(){
    /*
    int resultsCount = ui->listWidget_subdomains_activeSubdomains->count();
    for(int i = 0; i != resultsCount; ++i){
        emit sendResultsToSave(ui->listWidget_subdomains_activeSubdomains->item(i)->text());
    }
    logs_activeSubdomains("[*] Sent "+QString::number(resultsCount)+" activeSubdomains Enumerated Subdomains To Save Tab...");
    emit changeTabToSave();
    */
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
    /*foreach(const QModelIndex &index, ui->tableView_results_subBrute->selectionModel()->selectedIndexes()){
        // send selection
    }
    logs_subBrute("[*] Sent "+QString::number(ui->listWidget_subdomains_subBrute->count())+" subBrute Enumerated Subdomains To Save Tab...");
    emit changeTabToSave();*/
}
void Brute::cursorSendToSave_tldBrute(){
    /*foreach(QListWidgetItem * item, ui->listWidget_subdomains_tldBrute->selectedItems()){
        emit sendResultsToSave(item->text());
    }
    logs_subBrute("[*] Sent "+QString::number(ui->listWidget_subdomains_tldBrute->count())+" tldBrute Enumerated Subdomains To Save Tab...");
    emit changeTabToSave();*/
}
void Brute::cursorSendToSave_activeSubdomains(){
    /*foreach(QListWidgetItem * item, ui->listWidget_subdomains_activeSubdomains->selectedItems()){
        emit sendResultsToSave(item->text());
    }
    logs_activeSubdomains("[*] Sent "+QString::number(ui->listWidget_subdomains_activeSubdomains->count())+" activeSubdomains Enumerated Subdomains To Save Tab...");
    emit changeTabToSave();*/
}

/************************************************************************************/
void Brute::cursorOpenInBrowser_subBrute(){
    // iterate and open each selected item in a browser...
    foreach(const QModelIndex &index, ui->tableView_results_subBrute->selectionModel()->selectedIndexes()){
        QDesktopServices::openUrl(QUrl("https://"+index.data().toString(), QUrl::TolerantMode));
    }
}
void Brute::cursorOpenInBrowser_tldBrute(){
    foreach(const QModelIndex &index, ui->tableView_results_tldBrute->selectionModel()->selectedIndexes()){
        QDesktopServices::openUrl(QUrl("https://"+index.data().toString(), QUrl::TolerantMode));
    }
}
void Brute::cursorOpenInBrowser_activeSubdomains(){
    foreach(const QModelIndex &index, ui->tableView_results_activeSubdomains->selectionModel()->selectedIndexes()){
        QDesktopServices::openUrl(QUrl("https://"+index.data().toString(), QUrl::TolerantMode));
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
    /*
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
    */
}

void Brute::on_pushButton_get_activeSubdomains_clicked(){

}
