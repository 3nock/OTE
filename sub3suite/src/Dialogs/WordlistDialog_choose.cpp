/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#include "WordlistDialog.h"
#include "ui_WordlistDialog.h"

#include "src/utils/config.h"
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
    CONFIG_BRUTE.beginGroup(m_defaultWordlist);
    keys = CONFIG_BRUTE.allKeys();
    for(int i = 0; i < keys.count(); i++)
        ui->comboBoxDefaultWordlist->addItem(keys.at(i));
    CONFIG_BRUTE.endGroup();

    CONFIG_BRUTE.beginGroup(m_specialWordlist);
    keys = CONFIG_BRUTE.allKeys();
    for(int i = 0; i < keys.count(); i++)
        ui->comboBoxSpecialWordlist->addItem(keys.at(i));
    CONFIG_BRUTE.endGroup();

    TLD = true;
}

void WordListDialog::initSubdomainWordlist(){
    m_defaultWordlist = "Subdomain-Wordlist";
    m_specialWordlist = "Subdomain-SpecialWordlist";

    /* loading the worldists names to the comboBox... */
    QStringList keys;
    CONFIG_BRUTE.beginGroup(m_defaultWordlist);
    keys = CONFIG_BRUTE.allKeys();
    for(int i = 0; i < keys.count(); i++)
        ui->comboBoxDefaultWordlist->addItem(keys.at(i));
    CONFIG_BRUTE.endGroup();

    CONFIG_BRUTE.beginGroup(m_specialWordlist);
    keys = CONFIG_BRUTE.allKeys();
    for(int i = 0; i < keys.count(); i++)
        ui->comboBoxSpecialWordlist->addItem(keys.at(i));
    CONFIG_BRUTE.endGroup();

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
        CONFIG_BRUTE.beginGroup(m_defaultWordlist);
        choosenWordlistfFile = QApplication::applicationDirPath()+CONFIG_BRUTE.value(ui->comboBoxDefaultWordlist->currentText()).toString();
        CONFIG_BRUTE.endGroup();
    }
    if(ui->radioButtonSpecialWordlist->isChecked()){
        CONFIG_BRUTE.beginGroup(m_specialWordlist);
        choosenWordlistfFile = QApplication::applicationDirPath()+CONFIG_BRUTE.value(ui->comboBoxSpecialWordlist->currentText()).toString();
        CONFIG_BRUTE.endGroup();
    }

    /* get the stringList from model */
    QStringList list(m_wordlistModel->stringList());

    /* loading wordlist from file to wordlist stringlistmodel */
    QFile file(choosenWordlistfFile);
    if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
        QTextStream in(&file);
        while (!in.atEnd())
            list.push_back(in.readLine());

        file.close();
    }

    /* set the stringList to the model */
    m_wordlistModel->setStringList(list);

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

    QFile file(QApplication::applicationDirPath()+filePath);
    if(file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        for(int i = 0; i != m_customWordlistModel->rowCount(); ++i)
            file.write((m_customWordlistModel->stringList().at(i)+NEWLINE).toUtf8());
        file.close();
    }

    /* saving the name of the new special wordlist profile... */
    CONFIG_BRUTE.beginGroup(m_specialWordlist);
    CONFIG_BRUTE.setValue(name, filePath);
    CONFIG_BRUTE.endGroup();
    ui->comboBoxSpecialWordlist->addItem(name);
    ui->lineEditName->clear();
}
