#include "Level.h"
#include "ui_Level.h"

Level::Level(QWidget *parent) :QDialog(parent),ui(new Ui::Level),
    m_model_subdomains(new QStandardItemModel),
    m_model_results(new QStandardItemModel),
    //...
    m_scanArguments(new ScanArguments_level)
{
    ui->setupUi(this);
    //...
    ui->pushButton_stop->setDisabled(true);
    ui->pushButton_pause->setDisabled(true);
    //...
    QStringList headerLabels = {"Subdomain Name:", "IpAddress"};
    m_model_subdomains->setHorizontalHeaderLabels(headerLabels);
    m_model_results->setHorizontalHeaderLabels(headerLabels);
    //...
    ui->tableView_subdomains->setModel(m_model_subdomains);
    ui->tableView_results->setModel(m_model_results);
    //...
    ui->splitter->setSizes(QList<int>()<<220<<2);
    ui->progressBar->hide();
    //...
    ui->lineEdit_wordlist->setPlaceholderText("Enter new wordlist item...");
    ui->lineEdit_subdomains->setPlaceholderText("Enter new subdomain:ipAddress item...");
}

Level::~Level(){
    delete m_model_results;
    delete m_model_subdomains;
    delete m_scanArguments;
    //...
    delete ui;
}

/************************************** Scan ********************************************/
void Level::on_pushButton_start_clicked(){
    if(ui->listWidget_wordlist->count() < 1){
        QMessageBox::warning(this, TITLE_ERROR, "Please Enter wordlist For Enumeration!");
        return;
    }
    if(ui->tableView_subdomains->model()->rowCount() < 1){
        QMessageBox::warning(this, TITLE_ERROR, "Please Enter Subdomains For Enumeration!");
        return;
    }
    ui->label_levels->setNum(level);
    //...
    ui->pushButton_stop->setEnabled(true);
    ui->pushButton_start->setDisabled(true);
    //...
    level = 2;
    doneLevels = 0;
    sendStatus("[*] Enumerating Multi-Level Subdomains...");
    logs("[START] Enumerating Multi-Level Subdomains...");
    //...
    startEnumeration();
}
void Level::on_pushButton_stop_clicked(){
    emit stop();
}

/************************************** Start Enumeration ***********************************/
void Level::startEnumeration(){
    int maxThreads = 20;
    if(maxThreads > ui->listWidget_wordlist->count()){
        maxThreads = ui->listWidget_wordlist->count();
    }
    activeThreads = maxThreads;
    ///
    /// Getting the arguments....
    ///
    m_scanArguments->wordlist = ui->listWidget_wordlist;
    m_scanArguments->model_subdomains = m_model_subdomains;
    m_scanArguments->currentWordlistToEnumerate = 0;
    m_scanArguments->currentSubdomainToEnumerate = 0;
    ///
    /// starting the loop to enumerate subdmains according to the number of threads...
    ///
    for(int i = 0; i != maxThreads; i++){
        //...
        LevelEnumerator *Enumerator = new LevelEnumerator(m_scanArguments);
        QThread *cThread = new QThread;
        Enumerator->Enumerate(cThread);
        Enumerator->moveToThread(cThread);
        //...
        connect(Enumerator, SIGNAL(resolvedSubdomain(QString, QString)), this, SLOT(resolvedSubdomain(QString, QString)));
        connect(Enumerator, SIGNAL(scanLog(QString)), this, SLOT(logs(QString)));
        connect(cThread, SIGNAL(finished()), this, SLOT(onThreadEnd()));
        connect(cThread, SIGNAL(finished()), Enumerator, SLOT(deleteLater()));
        connect(cThread, SIGNAL(finished()), cThread, SLOT(deleteLater()));
        connect(this, SIGNAL(stop()), Enumerator, SLOT(onStop()));
        //...
        cThread->start();
    }
}

void Level::resolvedSubdomain(QString subdomain, QString ipAddress){
    m_model_results->setItem(m_model_results->rowCount(), 0, new QStandardItem(subdomain));
    m_model_results->setItem(m_model_results->rowCount()-1, 1, new QStandardItem(ipAddress));
    ui->label_resultsCount->setNum(m_model_results->rowCount());
}

void Level::onThreadEnd(){
    endedThreads++;
    if(endedThreads != activeThreads){
        return;
    }
    level--;
    ///
    /// if levels havent ended, copy the enumerated subdomains into subdomains
    /// for next enumeration and call enumeration....
    ///
    if(level && (startOfNewLevel < m_model_subdomains->rowCount())){
        ///
        /// first Clear the model of all previous subdomains...
        ///
        m_model_subdomains->clear();
        QStringList headerLabels = {"Subdomain Name:", "IpAddress"};
        m_model_subdomains->setHorizontalHeaderLabels(headerLabels);
        ///
        /// then copy the newly enumerated items from results model to the
        /// subdomains model...
        ///
        /// also keep track of the
        ///
        while(startOfNewLevel < m_model_results->rowCount()){
            m_model_subdomains->appendRow(m_model_results->item(startOfNewLevel)->clone());
            startOfNewLevel++;
        }
        ui->label_subdomainsCount->setNum(m_model_subdomains->rowCount());
        //...
        endedThreads = 0;
        doneLevels++;
        ui->label_levels->setNum(doneLevels);
        //...
        startEnumeration();
    }
    else{
        endedThreads = 0;
        ui->pushButton_start->setEnabled(true);
        ui->pushButton_stop->setDisabled(true);
        //...
        sendStatus("[*] Enumeration by tldBrute Complete!");
        logs("[END] TLD Bruteforcing Complete!\n");
    }
}

void Level::on_toolButton_config_clicked(){

}

/************************************** Results *****************************************/
void Level::on_pushButton_clearResults_clicked(){

}

void Level::on_pushButton_action_clicked(){

}

/************************************** Subdomains **************************************/
void Level::on_pushButton_removeSubdomains_clicked(){
    ui->tableView_subdomains->selectionModel()->clear();
    ui->label_subdomainsCount->setNum(m_model_subdomains->rowCount());
}

void Level::on_pushButton_clearSubdomains_clicked(){
    m_model_subdomains->clear();
    QStringList headerLabels = {"Subdomain Name:", "IpAddress"};
    m_model_subdomains->setHorizontalHeaderLabels(headerLabels);
    //...
    ui->label_subdomainsCount->clear();
}

void Level::on_pushButton_loadSubdomains_clicked(){
    QString filename = QFileDialog::getOpenFileName(this, INFO_LOADFILE, CURRENT_PATH);
    if(!filename.isEmpty()){
        QFile file(filename);
        QString item;
        QStringList subdomain_ip;
        if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
            QTextStream in(&file);
            while (!in.atEnd()){
                item = in.readLine();
                subdomain_ip = item.split(":", QString::SkipEmptyParts);
                if(subdomain_ip.count() > 1){
                    m_model_subdomains->setItem(m_model_subdomains->rowCount(), 0, new QStandardItem(subdomain_ip[0]));
                    m_model_subdomains->setItem(m_model_subdomains->rowCount(), 1, new QStandardItem(subdomain_ip[1]));
                }
                else{
                    m_model_subdomains->setItem(m_model_subdomains->rowCount(), 0, new QStandardItem(item));
                }
            }
            ui->label_subdomainsCount->setNum(m_model_subdomains->rowCount());
            file.close();
        }
        else{
            QMessageBox::warning(this, TITLE_ERROR, "Failed To Open the File!");
        }
    }
}

void Level::on_pushButton_addSubdomains_clicked(){
    if(ui->lineEdit_subdomains->text() != EMPTY){
        QString item = ui->lineEdit_subdomains->text();
        QStringList subdomain_ip = item.split(":", QString::SkipEmptyParts);
        if(subdomain_ip.count() > 1){
            m_model_subdomains->setItem(m_model_subdomains->rowCount(), 0, new QStandardItem(subdomain_ip[0]));
            m_model_subdomains->setItem(m_model_subdomains->rowCount(), 1, new QStandardItem(subdomain_ip[1]));
        }
        else{
            m_model_subdomains->setItem(m_model_subdomains->rowCount(), 0, new QStandardItem(item));
        }
        ui->lineEdit_subdomains->clear();
        ui->label_subdomainsCount->setNum(m_model_subdomains->rowCount());
    }
}
void Level::on_lineEdit_subdomains_returnPressed(){
    on_pushButton_addSubdomains_clicked();
}

/************************************* Wordlist *****************************************/
void Level::on_pushButton_removeWordlist_clicked(){
    if(ui->listWidget_wordlist->selectedItems().count()){
        qDeleteAll(ui->listWidget_wordlist->selectedItems());
    }
    ui->label_wordlistCount->setNum(ui->listWidget_wordlist->count());
}

void Level::on_pushButton_clearWordlist_clicked(){
    ui->listWidget_wordlist->clear();
    ui->label_wordlistCount->clear();
}

void Level::on_pushButton_loadWordlist_clicked(){
    QString filename = QFileDialog::getOpenFileName(this, INFO_LOADFILE, CURRENT_PATH);
    if(!filename.isEmpty()){
        QFile file(filename);
        if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
            QTextStream in(&file);
            while (!in.atEnd()){
                ui->listWidget_wordlist->addItem(in.readLine());
            }
            ui->label_wordlistCount->setNum(ui->listWidget_wordlist->count());
            file.close();
        }else{
            QMessageBox::warning(this, TITLE_ERROR, "Failed To Open the File!");
        }
    }
}

void Level::on_pushButton_addWordlist_clicked(){
    if(ui->lineEdit_wordlist->text() != EMPTY){
        ui->listWidget_wordlist->addItem(ui->lineEdit_wordlist->text());
        ui->lineEdit_wordlist->clear();
        ui->label_wordlistCount->setNum(ui->listWidget_wordlist->count());
    }
}
void Level::on_lineEdit_wordlist_returnPressed(){
    on_pushButton_addWordlist_clicked();
}

void Level::on_toolButton_wordlist_clicked(){
    WordListDialog *wordlistDialog = new WordListDialog(this, ENUMNAME_SUBBRUTE);
    wordlistDialog->setAttribute( Qt::WA_DeleteOnClose, true );
    connect(wordlistDialog, SIGNAL(wordlistFilename(QString)), this, SLOT(onWordlistFilename(QString)));
    wordlistDialog->show();
}

void Level::onWordlistFilename(QString wordlistFilename){
    QFile file(wordlistFilename);
    if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
        QTextStream in(&file);
        while (!in.atEnd()){
            ui->listWidget_wordlist->addItem(in.readLine());
        }
        file.close();
    }
    ui->label_wordlistCount->setNum(ui->listWidget_wordlist->count());
}

/******************************************** logs ******************************************/
void Level::logs(QString log){
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
