#include "WordlistDialog.h"
#include "ui_WordlistDialog.h"


WordListDialog::WordListDialog(QWidget *parent, ENGINE engine)
    :QDialog(parent), ui(new Ui::WordListDialog),
      m_engine(engine)
{
    ui->setupUi(this);
    ///
    /// initializations...
    ///
    ui->specialWordlist->init("Wordlist");
    ///
    /// hiding widgets for creating a new special wordlist untill user chooses to use custom nameservers...
    ///
    ui->frame_newSpecialWordlist->hide();
    WordListDialog::adjustSize();
    //...
    ui->label_title->setText("Choose Wordlist for "+EnumName(m_engine)+" subdomain Enumeration");
    ui->lineEditName->setPlaceholderText("Enter Special wordlist Name...");
    //...
    setupSpecialWordlists();
}
WordListDialog::~WordListDialog(){
    delete ui;
}

/***************************** display all special wordlists ********************************/
void WordListDialog::setupSpecialWordlists(){
    QFile profile_names(QDir::currentPath()+"/wordlists/special_"+EnumName(m_engine)+"/names.txt");
    profile_names.open(QIODevice::ReadOnly | QIODevice::Text);
    if(profile_names.isOpen()){
         QTextStream in(&profile_names);
         while (!in.atEnd()){
            ui->comboBoxSpecialWordlist->addItem(in.readLine());
         }
         profile_names.close();
    }
}

/********************** choosen wordlist filename(full path) is Emited ***********************/
void WordListDialog::on_buttonOk_clicked(){
    QString choosenWordlistfFile;
    if(ui->radioButtonDefaultWordlist->isChecked()){
        choosenWordlistfFile = QDir::currentPath()+"/wordlists/"+EnumName(m_engine)+"_"+ui->comboBoxAutoWordlist->currentText()+".txt";
        emit choosenWordlist(choosenWordlistfFile);
    }
    if(ui->radioButtonSpecialWordlist->isChecked()){
        choosenWordlistfFile = QDir::currentPath()+"/wordlists/special_"+EnumName(m_engine)+"/"+ui->comboBoxSpecialWordlist->currentText()+".txt";
        emit choosenWordlist(choosenWordlistfFile);
    }
    accept();
}
void WordListDialog::on_buttonCancel_clicked(){
    accept();
}

/************************** Operations On Creating New Special Wordlist **********************/
void WordListDialog::on_checkBoxNewSpecialWordlist_clicked(bool createNewWordlist){
    if(createNewWordlist){
        ui->frame_newSpecialWordlist->show();
        WordListDialog::adjustSize();
    }else{
        ui->frame_newSpecialWordlist->hide();
        WordListDialog::adjustSize();
    }
}

// Creating the new special wordlist...
void WordListDialog::on_buttonCreate_clicked(){
    QString specialWordlistName = ui->lineEditName->text();
    ///
    /// saving the wordlists to the file...
    ///
    QFile file(QDir::currentPath()+"/wordlists/special_"+EnumName(m_engine)+"/"+specialWordlistName+".txt");
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    if(file.isOpen()){
        int wordlistCount = ui->specialWordlist->listWidget->count();
        for(int i = 0; i != wordlistCount; ++i){
            file.write((ui->specialWordlist->listWidget->item(i)->text()+NEWLINE).toUtf8());
        }
        file.close();
    }
    ///
    /// saving the name of the new special wordlist profile...
    ///
    QFile profile_names(QDir::currentPath()+"/wordlists/special_"+EnumName(m_engine)+"/names.txt");
    profile_names.open(QIODevice::Append | QIODevice::Text);
    if(profile_names.isOpen()){
        QTextStream stream(&profile_names);
        stream << NEWLINE+specialWordlistName;
        profile_names.close();
    }
    //...
    ui->comboBoxSpecialWordlist->addItem(specialWordlistName);
    ui->lineEditName->clear();
}
