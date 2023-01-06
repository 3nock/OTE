/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#include "SSLConfigDialog.h"
#include "ui_SSLConfigDialog.h"

#include "src/utils/Config.h"


SSLConfigDialog::SSLConfigDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SSLConfigDialog)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/img/res/icons/gear.png"));

    ui->groupBoxTimeout->setCheckable(false);

    ui->lineEditTimeout->setPlaceholderText("e.g. 3");
    ui->lineEditThreads->setPlaceholderText("e.g. 100");

    mLoadConfig();
}
SSLConfigDialog::~SSLConfigDialog(){
    delete ui;
}

void SSLConfigDialog::on_buttonCancel_clicked(){
    accept();
}

void SSLConfigDialog::on_buttonOk_clicked(){
    mSaveConfig();
    accept();
}

void SSLConfigDialog::mLoadConfig(){
    ui->lineEditThreads->setText(QString::number(gConfig.tools.ssl.threads));
    ui->lineEditTimeout->setText(QString::number(gConfig.tools.ssl.timeout));
    ui->checkBoxAutosave->setChecked(gConfig.tools.ssl.autoSaveResults);
    ui->checkBoxNoDuplicates->setChecked(gConfig.tools.ssl.noDuplicates);
    ui->groupBoxTimeout->setChecked(gConfig.tools.ssl.useTimeout);
}

void SSLConfigDialog::mSaveConfig(){
    gConfig.tools.ssl.threads = ui->lineEditThreads->text().toInt();
    gConfig.tools.ssl.timeout = ui->lineEditTimeout->text().toInt();
    gConfig.tools.ssl.noDuplicates = ui->checkBoxNoDuplicates->isChecked();
    gConfig.tools.ssl.autoSaveResults = ui->checkBoxAutosave->isChecked();
    gConfig.tools.ssl.useTimeout = ui->groupBoxTimeout->isChecked();
}
