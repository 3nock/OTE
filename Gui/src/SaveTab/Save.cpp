#include "Save.h"
#include "ui_Save.h"

Save::Save(QWidget *parent) : QDialog(parent), ui(new Ui::Save){
    ui->setupUi(this);

    // setting the splitter to the middle...
    ui->splitter->setSizes(QList<int>()<<500<<1);
}

// getting the Enumerated subdomains from osint and brute...
void Save::onReceiveResults(QString subdomain){
    ui->listWidget_results_Save->addItem(subdomain);
    subdomains_file.write(subdomain.toUtf8()+NEWLINE);
    total_subdomain_count++;
    ui->label_resultsCount->setNum(total_subdomain_count);
}

// sending signal for loading the Enumerated subdomains...
void Save::on_toolButton_get_Save_clicked(){
    if(ui->radioButton_getFrom_Save->isChecked()){
        if(ui->comboBox_get_Save->currentIndex() == 0){
            emit getOsintResults();
        }
        if(ui->comboBox_get_Save->currentIndex() == 1){
            emit getBruteResults();
        }
        if(ui->comboBox_get_Save->currentIndex() == 2){
            emit getActiveResults();
        }
    }
    if(ui->radioButton_getAll_Save->isChecked()){
        emit getOsintResults();
        emit getBruteResults();
        emit getActiveResults();
    }
}

// clearing the subdomains listwidget...
void Save::on_pushButton_clear_Save_clicked(){
    ui->listWidget_results_Save->clear();
    total_subdomain_count = 0;
    ui->label_resultsCount->setNum(total_subdomain_count);
    subdomains_file.remove();
    subdomains_file.open();
}

// removing subdomains from the list widget...
void Save::on_pushButton_remove_Save_clicked(){
    int count = ui->listWidget_results_Save->selectedItems().count();
    if(count){
        qDeleteAll(ui->listWidget_results_Save->selectedItems());
        total_subdomain_count = total_subdomain_count-count;
    }
}

// loading subdomains from extenal source...
void Save::on_pushButton_load_Save_clicked(){
    QString filename = QFileDialog::getOpenFileName(this, INFO_LOADFILE, CURRENT_PATH);
    QFile file(filename);
    if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
        QString line;
        QTextStream in(&file);
        while (!in.atEnd()){
            line = in.readLine();
            ui->listWidget_results_Save->addItem(line);
            subdomains_file.write(line.toUtf8()+NEWLINE);
            total_subdomain_count++;
        }
        ui->label_resultsCount->setNum(total_subdomain_count);
        file.close();
    }
}

// saving the subdomains from the list widget to the user's file...
void Save::on_pushButton_save_Save_clicked(){
    QString filename = QFileDialog::getSaveFileName(this, INFO_SAVETOFILE, CURRENT_PATH);
    QFile file(filename);
    file.open(QIODevice::Append | QIODevice::Text);
    // saving with each subdomain in new line...
    if (ui->comboBox_saveFormat_Save->currentIndex() == 0){
        if(file.isOpen()){
            QString _item;
            int the_count = ui->listWidget_results_Save->count();
            for(int i = 0; i != the_count; ++i){
                _item = ui->listWidget_results_Save->item(i)->text();
                file.write((_item+NEWLINE).toUtf8());
            }
        }
    }
    // seperating the subdomains with a comma...
    if (ui->comboBox_saveFormat_Save->currentIndex() == 1){
        if(file.isOpen()){
            int the_count = ui->listWidget_results_Save->count();
            for(int i = 0; i != the_count; ++i){
                QString _item = ui->listWidget_results_Save->item(i)->text();
                file.write((_item+",").toUtf8());
            }
        }
    }
    file.close();
}

// filtering the subdomains depending on the user's liking....
void Save::on_pushButton_filter_Save_clicked(){
    if(ui->lineEdit_filter_Save->text() != EMPTY){
        int filter_length = ui->lineEdit_filter_Save->text().length();
        QString filter = ui->lineEdit_filter_Save->text();
        if(ui->comboBox_filter_Save->currentText() == "Ends with"){
            int i = 0;
            while(i != ui->listWidget_results_Save->count()){
                if(ui->listWidget_results_Save->item(i)->text().right(filter_length) == filter){
                    i++;
                }else{
                    ui->listWidget_results_Save->takeItem(i);
                    total_subdomain_count--;
                }
            }
            ui->label_resultsCount->setNum(total_subdomain_count);
        }
        if(ui->comboBox_filter_Save->currentText() == "Starts with"){
            int i = 0;
            while(i != ui->listWidget_results_Save->count()){
                if(ui->listWidget_results_Save->item(i)->text().left(filter_length) == filter){
                    i++;
                }else{
                    ui->listWidget_results_Save->takeItem(i);
                    total_subdomain_count--;
                }
            }
            ui->label_resultsCount->setNum(total_subdomain_count);
        }
        if(ui->comboBox_filter_Save->currentText() == "Contains"){
            int i = 0;
            while(i != ui->listWidget_results_Save->count()){
                if(ui->listWidget_results_Save->item(i)->text().contains(filter, Qt::CaseInsensitive)){
                    i++;
                }else{
                    ui->listWidget_results_Save->takeItem(i);
                    total_subdomain_count--;
                }
            }
            ui->label_resultsCount->setNum(total_subdomain_count);
        }
    }
}

void Save::on_pushButton_back_Save_clicked(){
    ui->listWidget_results_Save->clear();
    total_subdomain_count = 0;
    QTextStream in(&subdomains_file);
    QString line;
    in.seek(0);
    while (!in.atEnd()){
        line = in.readLine();
        ui->listWidget_results_Save->addItem(line);
        total_subdomain_count++;
    }
    ui->label_resultsCount->setNum(total_subdomain_count);
}

// sorting the items on the listWidget...
void Save::on_pushButton_sort_Save_clicked(){
    if(sort == 0){
        ui->listWidget_results_Save->sortItems(Qt::AscendingOrder);
        sort = 1;
        return;
    }
    if(sort == 1){
        ui->listWidget_results_Save->sortItems(Qt::DescendingOrder);
        sort = 0;
    }
}

void Save::on_lineEdit_filter_Save_returnPressed(){
     on_pushButton_filter_Save_clicked();
}

// class deconstructor...
Save::~Save(){
    delete ui;
}

void Save::on_pushButton_clearResults_Save_clicked(){
    ui->listWidget_results_Save->clear();
}
