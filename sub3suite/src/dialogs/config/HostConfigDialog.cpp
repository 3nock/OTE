/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#include "HostConfigDialog.h"
#include "ui_HostConfigDialog.h"

#include "src/utils/Config.h"

HostConfigDialog::HostConfigDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HostConfigDialog)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/img/res/icons/gear.png"));

    ui->lineEditTimeout->setPlaceholderText("e.g. 3");
    ui->lineEditThreads->setPlaceholderText("e.g. 100");

    mLoadConfig();
}

HostConfigDialog::~HostConfigDialog(){
    delete ui;
}

void HostConfigDialog::on_buttonCancel_clicked(){
    accept();
}

void HostConfigDialog::on_buttonOk_clicked(){
    mSaveConfig();
    accept();
}

void HostConfigDialog::mLoadConfig(){
    ui->lineEditPingTTL->setText(QString::number(gConfig.tools.host.ping.ttl));
    ui->lineEditPingByte->setText(QString::number(gConfig.tools.host.ping.bytes));
    ui->lineEditThreads->setText(QString::number(gConfig.tools.host.threads));
    ui->lineEditTimeout->setText(QString::number(gConfig.tools.host.timeout));
    ui->checkBoxAutosave->setChecked(gConfig.tools.host.autoSaveResults);
    ui->checkBoxNoDuplicates->setChecked(gConfig.tools.host.noDuplicates);
    ui->groupBoxTimeout->setChecked(gConfig.tools.host.useTimeout);

    switch (gConfig.tools.host.record) {
    case Config::DNS::A:
        ui->radioButtonA->setChecked(true);
        break;
    case Config::DNS::AAAA:
        ui->radioButtonAAAA->setChecked(true);
        break;
    case Config::DNS::ANY:
        ui->radioButtonANY->setChecked(true);
        break;
    default:
        break;
    }

    ui->comboBoxNameserver->addItems(gConfig.tools.host.nameservers.keys());
    ui->comboBoxNameserver->setCurrentText(gConfig.tools.host.nameserver);
}

void HostConfigDialog::mSaveConfig(){
    gConfig.tools.host.threads = ui->lineEditThreads->text().toInt();
    gConfig.tools.host.timeout = ui->lineEditTimeout->text().toInt();
    gConfig.tools.host.ping.ttl = ui->lineEditPingTTL->text().toInt();
    gConfig.tools.host.ping.bytes = ui->lineEditPingByte->text().toInt();

    gConfig.tools.host.noDuplicates = ui->checkBoxNoDuplicates->isChecked();
    gConfig.tools.host.autoSaveResults = ui->checkBoxAutosave->isChecked();
    gConfig.tools.host.useTimeout = ui->groupBoxTimeout->isChecked();

    if(ui->radioButtonA->isChecked())
        gConfig.tools.host.record = Config::DNS::A;
    if(ui->radioButtonAAAA->isChecked())
        gConfig.tools.host.record = Config::DNS::AAAA;
    if(ui->radioButtonANY->isChecked())
        gConfig.tools.host.record = Config::DNS::ANY;

    gConfig.tools.host.nameserver = ui->comboBoxNameserver->currentText();
}
