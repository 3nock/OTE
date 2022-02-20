/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#include "WordlistDialog.h"
#include "ui_WordlistDialog.h"


WordListDialog::WordListDialog(QWidget *parent, QStringListModel *wordlistModel): QDialog(parent), ui(new Ui::WordListDialog),
      m_wordlistModel(wordlistModel),
      m_listModel_choose(new QStringListModel),
      m_listModel_generate(new QStringListModel)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/img/res/icons/list.png"));

    this->m_initChoose();
    this->m_initGenerate();

    /* adjust the dialog size accordingly */


    ui->scrollAreaGenerate->hide();
    this->adjustSize();
}
WordListDialog::~WordListDialog(){
    delete m_listModel_generate;
    delete m_listModel_choose;
    delete ui;
}

void WordListDialog::on_tabWidget_currentChanged(int index){
    Q_UNUSED(index);

    /* adjust the dialog size accordingly */
    switch (index) {
    case 0: // Choose Tab
        ui->scrollAreaGenerate->hide();
        ui->groupBoxChoose->show();
        break;
    case 1: // Generate Tab
        ui->scrollAreaGenerate->show();
        ui->groupBoxChoose->hide();
    }

    ui->tabGenerate->adjustSize();
    ui->tabChoose->adjustSize();
    ui->tabWidget->adjustSize();
    this->adjustSize();
}
