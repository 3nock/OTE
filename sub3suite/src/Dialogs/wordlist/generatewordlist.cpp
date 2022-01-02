#include "WordlistDialog.h"
#include "ui_WordlistDialog.h"


void WordListDialog::m_initGenerate(){
    ui->generateWordlist->setListName("Wordlist");
    //ui->generateWordlist->setListModel(m_generateWordlistModel);

    /* placeholder texts */
}

void WordListDialog::on_buttonGenerate_clicked(){
    accept();
}
