#include "WordlistDialog.h"
#include "ui_WordlistDialog.h"

#include "src/utils/Config.h"
#include <QSpacerItem>


void WordListDialog::m_initChoose(){
    ui->customWordlist->setListName("Custom Wordlist");
    ui->customWordlist->setListModel(m_customWordlistModel);
    ui->groupBoxCustomWordlist->hide();

    ui->lineEditName->setPlaceholderText("Enter custom wordlist Name...");
}

void WordListDialog::initTLDWordlist(){
    m_defaultWordlist = "TLD-Wordlist";
    m_specialWordlist = "TLD-SpecialWordlist";

    /* loading the worldists names to the comboBox... */
    QStringList keys;
    Config::generalConfig().beginGroup(m_defaultWordlist);
    keys = Config::generalConfig().allKeys();
    for(int i = 0; i < keys.count(); i++)
        ui->comboBoxDefaultWordlist->addItem(keys.at(i));
    Config::generalConfig().endGroup();

    Config::generalConfig().beginGroup(m_specialWordlist);
    keys = Config::generalConfig().allKeys();
    for(int i = 0; i < keys.count(); i++)
        ui->comboBoxSpecialWordlist->addItem(keys.at(i));
    Config::generalConfig().endGroup();

    TLD = true;
}

void WordListDialog::initSubdomainWordlist(){
    m_defaultWordlist = "Subdomain-Wordlist";
    m_specialWordlist = "Subdomain-SpecialWordlist";

    /* loading the worldists names to the comboBox... */
    QStringList keys;
    Config::generalConfig().beginGroup(m_defaultWordlist);
    keys = Config::generalConfig().allKeys();
    for(int i = 0; i < keys.count(); i++)
        ui->comboBoxDefaultWordlist->addItem(keys.at(i));
    Config::generalConfig().endGroup();

    Config::generalConfig().beginGroup(m_specialWordlist);
    keys = Config::generalConfig().allKeys();
    for(int i = 0; i < keys.count(); i++)
        ui->comboBoxSpecialWordlist->addItem(keys.at(i));
    Config::generalConfig().endGroup();

    Subdomain = true;
}

void WordListDialog::on_checkBoxNewCustomWordlist_clicked(bool checked){
    if(checked)
        ui->groupBoxCustomWordlist->show();
    else
        ui->groupBoxCustomWordlist->hide();

    /* adjust the dialog size accordingly */
    ui->tabChoose->adjustSize();
    ui->tabWidget->adjustSize();
    this->adjustSize();
}

void WordListDialog::on_buttonLoadChoosen_clicked(){
    QString choosenWordlistfFile;

    if(ui->radioButtonDefaultWordlist->isChecked()){
        Config::generalConfig().beginGroup(m_defaultWordlist);
        choosenWordlistfFile = QDir::currentPath()+Config::generalConfig().value(ui->comboBoxDefaultWordlist->currentText()).toString();
        Config::generalConfig().endGroup();
    }
    if(ui->radioButtonSpecialWordlist->isChecked()){
        Config::generalConfig().beginGroup(m_specialWordlist);
        choosenWordlistfFile = QDir::currentPath()+Config::generalConfig().value(ui->comboBoxSpecialWordlist->currentText()).toString();
        Config::generalConfig().endGroup();
    }

    /* loading wordlist from file to wordlist stringlistmodel */
    QFile file(choosenWordlistfFile);
    if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
        QTextStream in(&file);
        while (!in.atEnd())
        {
            if(m_wordlistModel->insertRow(m_wordlistModel->rowCount()))
                m_wordlistModel->setData(m_wordlistModel->index(m_wordlistModel->rowCount()-1, 0), in.readLine());
        }
        file.close();
    }

    emit wordlistLoaded();
    accept();
}

void WordListDialog::on_buttonCreate_clicked(){
    QString filePath;
    QString name = ui->lineEditName->text();
    if(name.isEmpty())
        return;

    /* saving the wordlists to the file... */
    if(Subdomain)
        filePath = "/wordlists/subdomain/"+name+".txt";
    if(TLD)
        filePath = "/wordlists/tld/"+name+".txt";

    QFile file(QDir::currentPath()+filePath);
    if(file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        for(int i = 0; i != m_customWordlistModel->rowCount(); ++i)
            file.write((m_customWordlistModel->stringList().at(i)+NEWLINE).toUtf8());
        file.close();
    }

    /* saving the name of the new special wordlist profile... */
    Config::generalConfig().beginGroup(m_specialWordlist);
    Config::generalConfig().setValue(name, filePath);
    Config::generalConfig().endGroup();
    ui->comboBoxSpecialWordlist->addItem(name);
    ui->lineEditName->clear();
}
