/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#include "OsintConfigDialog.h"
#include "ui_OsintConfigDialog.h"

#include <QIntValidator>
#include <QMessageBox>

#include "src/utils/Config.h"


OsintConfigDialog::OsintConfigDialog(QWidget *parent) : QDialog(parent),
    ui(new Ui::OsintConfigDialog)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/img/res/icons/gear.png"));

    ui->lineEditTimeout->setPlaceholderText("e.g. 1000");
    ui->lineEditMaxPages->setPlaceholderText("e.g. 100");

    mLoadConfig();
}
OsintConfigDialog::~OsintConfigDialog(){
    delete ui;
}

void OsintConfigDialog::on_buttonOk_clicked(){
    mSaveConfig();
    accept();
}

void OsintConfigDialog::mLoadConfig(){
    ui->lineEditTimeout->setText(QString::number(gConfig.tools.osint.timeout));
    ui->lineEditMaxPages->setText(QString::number(gConfig.tools.osint.maxPages));
    ui->checkBoxAutosave->setChecked(gConfig.tools.osint.autoSaveResults);
    ui->checkBoxNoDuplicates->setChecked(gConfig.tools.osint.noDuplicates);
    ui->groupBoxTimeout->setChecked(gConfig.tools.osint.useTimeout);
}

void OsintConfigDialog::mSaveConfig(){
    gConfig.tools.osint.timeout = ui->lineEditTimeout->text().toInt();
    gConfig.tools.osint.maxPages = ui->lineEditMaxPages->text().toInt();
    gConfig.tools.osint.autoSaveResults = ui->checkBoxAutosave->isChecked();
    gConfig.tools.osint.noDuplicates = ui->checkBoxNoDuplicates->isChecked();
    gConfig.tools.osint.useTimeout = ui->groupBoxTimeout->isChecked();
}

void OsintConfigDialog::on_buttonCancel_clicked(){
    accept();
}
