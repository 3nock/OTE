#include "Dns.h"
#include "ui_Dns.h"

/*************************************************************************************
 *                          Constructor & Destructor
 *************************************************************************************/
Dns::Dns(QWidget *parent) :QDialog(parent), ui(new Ui::Dns){
    ui->setupUi(this);
    // setting the splitter to the middle...
    ui->splitter->setSizes(QList<int>()<<150<<1);

    ui->lineEdit_target_dnsRecords->setPlaceholderText("Enter Target domain/subdomain...");
    ui->lineEdit_targetInput_dnsRecords->setPlaceholderText("Enter Target domains/subdomains..");
}
Dns::~Dns(){
    delete ui;
}

/**************************************************************************************
 *                                  Scan
 **************************************************************************************/
void Dns::on_pushButton_start_dnsRecords_clicked(){

}
void Dns::on_pushButton_stop_dnsRecords_clicked(){

}

void Dns::on_toolButton_config_dnsRecords_clicked(){
    //BruteConfig *bruteconfig = new BruteConfig(this, ENUMNAME_ACTIVESUBDOMAINS);
    //bruteconfig->setAttribute( Qt::WA_DeleteOnClose, true );
    //bruteconfig->show();
}

/**************************************************************************************
 *                          Wordlist Processing
 *************************************************************************************/
void Dns::on_pushButton_remove_dnsRecords_clicked(){
    int count = ui->listWidget_wordlist_dnsRecords->selectedItems().count();
    if(count){
        qDeleteAll(ui->listWidget_wordlist_dnsRecords->selectedItems());
        wordlistCount_dnsRecords = wordlistCount_dnsRecords-count;
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
    if(ui->lineEdit_targetInput_dnsRecords->text() != EMPTY){
        ui->listWidget_wordlist_dnsRecords->addItem(ui->lineEdit_targetInput_dnsRecords->text());
        ui->lineEdit_targetInput_dnsRecords->clear();
        wordlistCount_dnsRecords++;
        ui->label_wordlistCount_dnsRecords->setNum(wordlistCount_dnsRecords);
    }
}

void Dns::on_pushButton_clearOutput_dnsRecords_clicked(){
    if(ui->tabWidget_output_dnsRecords->currentIndex() == 0){
        ui->textEdit_results_dnsRecords->clear();
    }else{
        ui->listWidget_logs_dnsRecords->clear();
    }
}

void Dns::on_pushButton_reload_dnsRecords_clicked(){
    int count = ui->listWidget_wordlist_dnsRecords->count();
    for(int i = 0; i < count; i++){
        ui->listWidget_wordlist_dnsRecords->item(i)->setForeground(Qt::black);
    }
    ui->pushButton_reload_dnsRecords->hide();
}

/**********************************************************************************
 *                          Context Menu
 *********************************************************************************/
void Dns::on_pushButton_action_dnsRecords_clicked(){

}
