#include "WordlistDialog.h"
#include "ui_WordlistDialog.h"

/******************************** Constructor & Destructor *********************************/
WordListDialog::WordListDialog(QWidget *parent, int enumName):
    QDialog(parent), m_enumName(enumName), ui(new Ui::WordListDialog)
{
    ui->setupUi(this);
    ///
    /// hiding widgets for creating a new special wordlist untill user chooses to use custom nameservers...
    ///
    ui->frame_newSpecialWordlist->hide();
    WordListDialog::adjustSize();
    //...
    ui->label_title->setText("Choose Wordlist for "+EnumName(enumName)+" subdomain Enumeration");
    ui->lineEdit_specialName->setPlaceholderText("Enter Special wordlist Name...");
    ui->lineEdit_add->setPlaceholderText("Enter new Item...");
    //...
    m_setupSpecialWordlists();
}
WordListDialog::~WordListDialog(){
    delete ui;
}

/***************************** display all special wordlists ********************************/
void WordListDialog::m_setupSpecialWordlists(){
    QFile profile_names(QDir::currentPath()+"/wordlists/special_"+EnumName(m_enumName)+"/names.txt");
    profile_names.open(QIODevice::ReadOnly | QIODevice::Text);
    if(profile_names.isOpen()){
         QTextStream in(&profile_names);
         while (!in.atEnd()){
            ui->comboBox_special->addItem(in.readLine());
         }
         profile_names.close();
    }
}

/********************** choosen wordlist filename(full path) is Emited ***********************/
void WordListDialog::on_pushButton_ok_clicked(){
    QString choosenWordlistfFile;
    if(ui->radioButton_defaultWordlist->isChecked()){
        choosenWordlistfFile = QDir::currentPath()+"/wordlists/"+EnumName(m_enumName)+"_"+ui->comboBox_auto->currentText()+".txt";
        emit(wordlistFilename(choosenWordlistfFile));
    }
    if(ui->radioButton_specialWordlist->isChecked()){
        choosenWordlistfFile = QDir::currentPath()+"/wordlists/special_"+EnumName(m_enumName)+"/"+ui->comboBox_special->currentText()+".txt";
        emit(wordlistFilename(choosenWordlistfFile));
    }
    accept();
}
void WordListDialog::on_pushButton_cancel_clicked(){
    accept();
}

/************************** Operations On Creating New Special Wordlist **********************/
void WordListDialog::on_checkBox_newSpecialWordlist_clicked(bool createNewWordlist){
    if(createNewWordlist){
        ui->frame_newSpecialWordlist->show();
        WordListDialog::adjustSize();
    }else{
        ui->frame_newSpecialWordlist->hide();
        WordListDialog::adjustSize();
    }
}

void WordListDialog::on_pushButton_clear_clicked(){
    ui->listWidget_specialWordlist->clear();
}

void WordListDialog::on_pushButton_remove_clicked(){
    if(ui->listWidget_specialWordlist->selectedItems().count()){
        qDeleteAll(ui->listWidget_specialWordlist->selectedItems());
    }
}

void WordListDialog::on_pushButton_load_clicked(){
    QString filename = QFileDialog::getOpenFileName(this, INFO_LOADFILE, CURRENT_PATH);
    if(!filename.isEmpty()){
        QFile file(filename);
        if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
            QTextStream in(&file);
            while (!in.atEnd()){
                ui->listWidget_specialWordlist->addItem(in.readLine());
            }
            file.close();
        }
    }
}

// adding new entry in special wordlist...
void WordListDialog::on_pushButton_add_clicked(){
    ui->listWidget_specialWordlist->addItem(ui->lineEdit_add->text());
    ui->lineEdit_add->clear();
}

// Creating the new special wordlist...
void WordListDialog::on_pushButton_create_clicked(){
    QString specialWordlistName = ui->lineEdit_specialName->text();
    ///
    /// saving the wordlists to the file...
    ///
    QFile file(QDir::currentPath()+"/wordlists/special_"+EnumName(m_enumName)+"/"+specialWordlistName+".txt");
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    if(file.isOpen()){
        int wordlistCount = ui->listWidget_specialWordlist->count();
        for(int i = 0; i != wordlistCount; ++i){
            file.write((ui->listWidget_specialWordlist->item(i)->text()+NEWLINE).toUtf8());
        }
        file.close();
    }
    ///
    /// saving the name of the new special wordlist profile...
    ///
    QFile profile_names(QDir::currentPath()+"/wordlists/special_"+EnumName(m_enumName)+"/names.txt");
    profile_names.open(QIODevice::Append | QIODevice::Text);
    if(profile_names.isOpen()){
        QTextStream stream(&profile_names);
        stream << NEWLINE+specialWordlistName;
        profile_names.close();
    }
    //...
    ui->comboBox_special->addItem(specialWordlistName);
    ui->lineEdit_specialName->clear();
    ui->listWidget_specialWordlist->clear();
}
