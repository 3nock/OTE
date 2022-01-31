/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#include "WordlistDialog.h"
#include "ui_WordlistDialog.h"


WordListDialog::WordListDialog(QWidget *parent, QStringListModel *wordlistModel): QDialog(parent), ui(new Ui::WordListDialog),
      m_wordlistModel(wordlistModel),
      m_customWordlistModel(new QStringListModel)
{
    ui->setupUi(this);

    ui->scrollAreaGenerate->setProperty("without_border", true);
    ui->scrollAreaGenerate->setProperty("s3s_light", true);
    ui->scrollAreaWidgetContents->setProperty("s3s_light", true);

    this->m_initChoose();
    this->m_initGenerate();

    /* adjust the dialog size accordingly */


    ui->scrollAreaGenerate->hide();
    this->adjustSize();
}
WordListDialog::~WordListDialog(){
    delete m_customWordlistModel;
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
