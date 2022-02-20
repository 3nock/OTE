/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#include "PreferenceDialog.h"
#include "ui_PreferenceDialog.h"


PreferenceDialog::PreferenceDialog(QWidget *parent): QDialog(parent), ui(new Ui::PreferenceDialog)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/img/res/icons/preferences.png"));
}
PreferenceDialog::~PreferenceDialog(){
    delete ui;
}
