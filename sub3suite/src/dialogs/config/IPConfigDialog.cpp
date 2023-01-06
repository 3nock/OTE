/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#include "IPConfigDialog.h"
#include "ui_IPConfigDialog.h"

#include "src/utils/Config.h"


IPConfigDialog::IPConfigDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::IPConfigDialog)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/img/res/icons/gear.png"));

    ui->lineEditTimeout->setPlaceholderText("e.g. 3");
    ui->lineEditThreads->setPlaceholderText("e.g. 100");

    mLoadConfig();
}
IPConfigDialog::~IPConfigDialog(){
    delete ui;
}

void IPConfigDialog::on_buttonCancel_clicked(){
    accept();
}

void IPConfigDialog::on_buttonOk_clicked(){
    mSaveConfig();
    accept();
}

void IPConfigDialog::mLoadConfig(){
    ui->lineEditPingTTL->setText(QString::number(gConfig.tools.ip.ping.ttl));
    ui->lineEditPingByte->setText(QString::number(gConfig.tools.ip.ping.bytes));
    ui->lineEditThreads->setText(QString::number(gConfig.tools.ip.threads));
    ui->lineEditTimeout->setText(QString::number(gConfig.tools.ip.timeout));
    ui->checkBoxAutosave->setChecked(gConfig.tools.ip.autoSaveResults);
    ui->checkBoxNoDuplicates->setChecked(gConfig.tools.ip.noDuplicates);
    ui->groupBoxTimeout->setChecked(gConfig.tools.ip.useTimeout);
}

void IPConfigDialog::mSaveConfig(){
    gConfig.tools.ip.threads = ui->lineEditThreads->text().toInt();
    gConfig.tools.ip.timeout = ui->lineEditTimeout->text().toInt();
    gConfig.tools.ip.ping.ttl = ui->lineEditPingTTL->text().toInt();
    gConfig.tools.ip.ping.bytes = ui->lineEditPingByte->text().toInt();

    gConfig.tools.ip.noDuplicates = ui->checkBoxNoDuplicates->isChecked();
    gConfig.tools.ip.autoSaveResults = ui->checkBoxAutosave->isChecked();
    gConfig.tools.ip.useTimeout = ui->groupBoxTimeout->isChecked();
}
