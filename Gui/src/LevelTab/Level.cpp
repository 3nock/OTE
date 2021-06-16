#include "Level.h"
#include "ui_Level.h"

Level::Level(QWidget *parent) :QDialog(parent),ui(new Ui::Level),
    m_model_subdomains(new QStandardItemModel),
    m_model_results(new QStandardItemModel),
    m_model_allResults(new QStandardItemModel)
{
    ui->setupUi(this);
    //...
    QStringList headerLabels = {"Subdomain Name:", "IpAddress"};
    m_model_subdomains->setHorizontalHeaderLabels(headerLabels);
    m_model_results->setHorizontalHeaderLabels(headerLabels);
    m_model_allResults->setHorizontalHeaderLabels(headerLabels);
    //...
    ui->tableView_subdomains->setModel(m_model_subdomains);
    ui->tableView_results->setModel(m_model_results);
    ui->tableView_allSubdomains->setModel(m_model_allResults);
    //...
    ui->splitter1->setSizes(QList<int>()<<180<<2);
    //...
    ui->lineEdit_wordlist->setPlaceholderText("Enter new wordlist item...");
    ui->lineEdit_subdomains->setPlaceholderText("Enter new subdomain:ipAddress item...");
    //...
    QPalette p = palette();
    p.setColor(QPalette::Highlight, QColor(188, 188, 141));
    p.setColor(QPalette::HighlightedText, QColor(Qt::black));
    ui->tableView_subdomains->setPalette(p);
    ui->tableView_results->setPalette(p);
    ui->tableView_allSubdomains->setPalette(p);
}

Level::~Level(){
    delete m_model_results;
    delete m_model_allResults;
    delete m_model_subdomains;
    //...
    delete ui;
}

/************************************** Scan ********************************************/
void Level::on_pushButton_start_clicked(){

}

void Level::on_pushButton_stop_clicked(){

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
    subdomainsCount--;
    ui->label_subdomainsCount->setNum(subdomainsCount);
}

void Level::on_pushButton_clearSubdomains_clicked(){
    m_model_subdomains->clear();
    QStringList headerLabels = {"Subdomain Name:", "IpAddress"};
    m_model_subdomains->setHorizontalHeaderLabels(headerLabels);
    //...
    subdomainsCount = 0;
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
                    m_model_subdomains->setItem(subdomainsCount, 0, new QStandardItem(subdomain_ip[0]));
                    m_model_subdomains->setItem(subdomainsCount, 1, new QStandardItem(subdomain_ip[1]));
                }
                else{
                    m_model_subdomains->setItem(subdomainsCount, 0, new QStandardItem(item));
                }
                subdomainsCount++;
            }
            ui->label_subdomainsCount->setNum(subdomainsCount);
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
            m_model_subdomains->setItem(subdomainsCount, 0, new QStandardItem(subdomain_ip[0]));
            m_model_subdomains->setItem(subdomainsCount, 1, new QStandardItem(subdomain_ip[1]));
        }
        else{
            m_model_subdomains->setItem(subdomainsCount, 0, new QStandardItem(item));
        }
        subdomainsCount++;
        ui->lineEdit_subdomains->clear();
        ui->label_subdomainsCount->setNum(subdomainsCount);
    }
}
void Level::on_lineEdit_subdomains_returnPressed(){
    on_pushButton_addSubdomains_clicked();
}

/************************************* Wordlist *****************************************/
void Level::on_pushButton_removeWordlist_clicked(){
    int selectionCount = ui->listWidget_wordlist->selectedItems().count();
    if(selectionCount){
        qDeleteAll(ui->listWidget_wordlist->selectedItems());
        wordlistCount = wordlistCount-selectionCount;
    }
    ui->label_wordlistCount->setNum(wordlistCount);
}

void Level::on_pushButton_clearWordlist_clicked(){
    ui->listWidget_wordlist->clear();
    ui->label_wordlistCount->clear();
    wordlistCount = 0;
}

void Level::on_pushButton_loadWordlist_clicked(){
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

void Level::on_pushButton_addWordlist_clicked(){
    if(ui->lineEdit_wordlist->text() != EMPTY){
        ui->listWidget_wordlist->addItem(ui->lineEdit_wordlist->text());
        ui->lineEdit_wordlist->clear();
        wordlistCount++;
        ui->label_wordlistCount->setNum(wordlistCount);
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
            wordlistCount++;
        }
        file.close();
    }
    ui->label_wordlistCount->setNum(wordlistCount);
}
