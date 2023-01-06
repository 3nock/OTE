/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#include "RawConfigDialog.h"
#include "ui_RawConfigDialog.h"

#include <QIntValidator>
#include <QMessageBox>

#include "src/utils/Config.h"


RawConfigDialog::RawConfigDialog(QWidget *parent) : QDialog(parent),
    ui(new Ui::RawConfigDialog)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/img/res/icons/gear.png"));

    ui->lineEditTimeout->setPlaceholderText("e.g. 1000");

    mLoadConfig();
}
RawConfigDialog::~RawConfigDialog(){
    delete ui;
}

void RawConfigDialog::on_buttonOk_clicked(){
    mSaveConfig();
    accept();
}

void RawConfigDialog::mLoadConfig(){
    ui->lineEditTimeout->setText(QString::number(gConfig.tools.raw.timeout));
    ui->checkBoxAutosave->setChecked(gConfig.tools.raw.autoSaveResults);
    ui->checkBoxNoDuplicates->setChecked(gConfig.tools.raw.noDuplicates);
    ui->groupBoxTimeout->setChecked(gConfig.tools.raw.useTimeout);
}

void RawConfigDialog::mSaveConfig(){
    gConfig.tools.raw.timeout = ui->lineEditTimeout->text().toInt();
    gConfig.tools.raw.autoSaveResults = ui->checkBoxAutosave->isChecked();
    gConfig.tools.raw.noDuplicates = ui->checkBoxNoDuplicates->isChecked();
    gConfig.tools.raw.useTimeout = ui->groupBoxTimeout->isChecked();
}

void RawConfigDialog::on_buttonCancel_clicked(){
    accept();
}
