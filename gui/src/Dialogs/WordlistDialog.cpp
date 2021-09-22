#include "WordlistDialog.h"
#include "ui_WordlistDialog.h"
//...
#include "src/utils/Config.h"


/*
 * TODO: add wordlist generation functionality...
 *       refactor to be better interactively...
 */
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
    /// setting default texts...
    ///
    if(m_engine == ENGINE::TLDBRUTE)
        ui->labelTitle->setText("TLD Wordlist");
    if(m_engine == ENGINE::SUBBRUTE)
        ui->labelTitle->setText("Subdomain Wordlist");
    ui->lineEditName->setPlaceholderText("Enter Special wordlist Name...");
    //...
    loadWordlists();
}
WordListDialog::~WordListDialog(){
    delete ui;
}

void WordListDialog::loadWordlists(){
    if(m_engine == ENGINE::SUBBRUTE){
        m_defaultWordlist = "Subdomain-Wordlist";
        m_specialWordlist = "Subdomain-SpecialWordlist";
    }
    if(m_engine == ENGINE::TLDBRUTE){
        m_defaultWordlist = "TLD-Wordlist";
        m_specialWordlist = "TLD-SpecialWordlist";
    }
    QStringList keys;
    ///
    /// loading the worldists to the comboBox...
    ///
    Config::generalConfig().beginGroup(m_defaultWordlist);
    keys = Config::generalConfig().allKeys();
    for(int i = 0; i < keys.count(); i++)
        ui->comboBoxDefaultWordlist->addItem(keys.at(i));
    Config::generalConfig().endGroup();
    //...
    Config::generalConfig().beginGroup(m_specialWordlist);
    keys = Config::generalConfig().allKeys();
    for(int i = 0; i < keys.count(); i++)
        ui->comboBoxSpecialWordlist->addItem(keys.at(i));
    Config::generalConfig().endGroup();
}

void WordListDialog::on_buttonOk_clicked(){
    QString choosenWordlistfFile;
    if(ui->radioButtonDefaultWordlist->isChecked()){
        Config::generalConfig().beginGroup(m_defaultWordlist);
        choosenWordlistfFile = QDir::currentPath()+Config::generalConfig().value(ui->comboBoxDefaultWordlist->currentText()).toString();
        Config::generalConfig().endGroup();
        emit choosenWordlist(choosenWordlistfFile);
    }
    if(ui->radioButtonSpecialWordlist->isChecked()){
        Config::generalConfig().beginGroup(m_specialWordlist);
        choosenWordlistfFile = QDir::currentPath()+Config::generalConfig().value(ui->comboBoxSpecialWordlist->currentText()).toString();
        Config::generalConfig().endGroup();
        emit choosenWordlist(choosenWordlistfFile);
    }
    accept();
}

void WordListDialog::on_buttonCancel_clicked(){
    accept();
}

void WordListDialog::on_buttonCreate_clicked(){
    QString filePath;
    QString name = ui->lineEditName->text();
    if(name.isEmpty())
        return;
    ///
    /// saving the wordlists to the file...
    ///
    if(m_engine == ENGINE::SUBBRUTE)
        filePath = "/wordlists/subdomain/"+name+".txt";
    if(m_engine == ENGINE::TLDBRUTE)
        filePath = "/wordlists/tld/"+name+".txt";

    QFile file(QDir::currentPath()+filePath);
    if(file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        for(int i = 0; i != ui->specialWordlist->listWidget->count(); ++i)
            file.write((ui->specialWordlist->listWidget->item(i)->text()+NEWLINE).toUtf8());
        file.close();
    }
    ///
    /// saving the name of the new special wordlist profile...
    ///
    Config::generalConfig().beginGroup(m_specialWordlist);
    Config::generalConfig().setValue(name, filePath);
    Config::generalConfig().endGroup();
    ui->comboBoxSpecialWordlist->addItem(name);
    ui->lineEditName->clear();
}
