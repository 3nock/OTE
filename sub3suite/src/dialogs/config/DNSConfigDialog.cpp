/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#include "DNSConfigDialog.h"
#include "ui_DNSConfigDialog.h"

#include "src/utils/Config.h"


DNSConfigDialog::DNSConfigDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DNSConfigDialog)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/img/res/icons/gear.png"));

    ui->lineEditTimeout->setPlaceholderText("e.g. 3");
    ui->lineEditThreads->setPlaceholderText("e.g. 100");

    mLoadConfig();
}
DNSConfigDialog::~DNSConfigDialog(){
    delete ui;
}

void DNSConfigDialog::on_buttonCancel_clicked(){
    accept();
}

void DNSConfigDialog::on_buttonOk_clicked(){
    mSaveConfig();
    accept();
}

void DNSConfigDialog::mLoadConfig(){
    ui->lineEditThreads->setText(QString::number(gConfig.tools.dns.threads));
    ui->lineEditTimeout->setText(QString::number(gConfig.tools.dns.timeout));
    ui->checkBoxAutosave->setChecked(gConfig.tools.dns.autoSaveResults);
    ui->checkBoxNoDuplicates->setChecked(gConfig.tools.dns.noDuplicates);
    ui->groupBoxTimeout->setChecked(gConfig.tools.dns.useTimeout);

    ui->comboBoxNameserver->addItems(gConfig.tools.dns.nameservers.keys());
    ui->comboBoxNameserver->setCurrentText(gConfig.tools.dns.nameserver);
}

void DNSConfigDialog::mSaveConfig(){
    gConfig.tools.dns.threads = ui->lineEditThreads->text().toInt();
    gConfig.tools.dns.timeout = ui->lineEditTimeout->text().toInt();

    gConfig.tools.dns.noDuplicates = ui->checkBoxNoDuplicates->isChecked();
    gConfig.tools.dns.autoSaveResults = ui->checkBoxAutosave->isChecked();
    gConfig.tools.dns.useTimeout = ui->groupBoxTimeout->isChecked();

    gConfig.tools.dns.nameserver = ui->comboBoxNameserver->currentText();
}
