#include "BruteConfigDialog.h"
#include "ui_BruteConfigDialog.h"


/********************************* Constructor & Destructor ***************************************/
BruteConfigDialog::BruteConfigDialog(QWidget *parent, ScanArguments_Brute *scanArguments)
    :QDialog(parent), m_scanArguments(scanArguments), ui(new Ui::BruteConfigDialog)
{
    ui->setupUi(this);
    ///
    /// hiding widgets for custom nameservers input untill user chooses to use custom nameservers...
    ///
    ui->frame_nameservers->hide();
    BruteConfigDialog::adjustSize();
    //...
    ui->lineEdit_nameservers->setPlaceholderText("Enter Nameserver...");
    ui->lineEdit_threads->setPlaceholderText("Thread count...");
    ui->lineEdit_threads->setText(QString::number(scanArguments->maxThreads));
}
BruteConfigDialog::~BruteConfigDialog(){
    delete ui;
}

/*********************************** Saving Configurations ***************************************/
void BruteConfigDialog::on_pushButton_ok_clicked(){
    if(ui->lineEdit_threads->text().toInt() <= MAX_THREADS && ui->lineEdit_threads->text().toInt() > 0){
        m_scanArguments->maxThreads = ui->lineEdit_threads->text().toInt();
    }
    if(ui->radioButton_dnsRecordType_A->isChecked()){
        m_scanArguments->dnsRecordType = QDnsLookup::A;
    }
    if(ui->radioButton_dnsRecordType_AAAA->isChecked()){
        m_scanArguments->dnsRecordType = QDnsLookup::AAAA;
    }
    if(ui->checkBox_checkWildcards->isChecked()){
        m_scanArguments->checkWildcardSubdomains = true;
    }
    if(ui->checkBox_useCustomNameServers->isChecked() && ui->listWidget_nameservers->count() > 0){
        QFile temp_file(QDir::currentPath()+WORDLIST_CUSTOM_NAMESERVERS);
        temp_file.open(QIODevice::WriteOnly | QIODevice::Text);
        if(temp_file.isOpen()){
           int the_count = ui->listWidget_nameservers->count();
           for(int i = 0; i != the_count; ++i){
               temp_file.write((ui->listWidget_nameservers->item(i)->text()+NEWLINE).toUtf8());
            }
            temp_file.close();
        }
        m_scanArguments->useCustomNameServers = true;
    }else{
        m_scanArguments->useCustomNameServers = false;
    }
    accept();
}
void BruteConfigDialog::on_pushButton_cancel_clicked(){
    accept();
}

/******************************* Operations On Custom-Nameservers *********************************/
void BruteConfigDialog::on_checkBox_useCustomNameServers_clicked(bool checked){
    if(checked){
        ui->frame_nameservers->show();
    }else{
        ui->frame_nameservers->hide();
    }
    BruteConfigDialog::adjustSize();
}

void BruteConfigDialog::on_pushButton_clear_clicked(){
    ui->listWidget_nameservers->clear();
}

void BruteConfigDialog::on_pushButton_remove_clicked(){
    if(ui->listWidget_nameservers->selectedItems().count()){
        qDeleteAll(ui->listWidget_nameservers->selectedItems());
    }
}

void BruteConfigDialog::on_pushButton_load_clicked(){
    QString filename = QFileDialog::getOpenFileName(this, INFO_LOADFILE, CURRENT_PATH);
    if(!filename.isEmpty()){
        QFile file(filename);
        if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
            QTextStream in(&file);
            while (!in.atEnd()){
                ui->listWidget_nameservers->addItem(in.readLine());
            }
            file.close();
        }
    }
}

void BruteConfigDialog::on_pushButton_add_clicked(){
    ui->listWidget_nameservers->addItem(ui->lineEdit_nameservers->text());
    ui->lineEdit_nameservers->clear();
}
