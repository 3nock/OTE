/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#include "BruteConfigDialog.h"
#include "ui_BruteConfigDialog.h"

#include "src/utils/Config.h"


BruteConfigDialog::BruteConfigDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BruteConfigDialog)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/img/res/icons/gear.png"));

    ui->lineEditTimeout->setPlaceholderText("e.g. 3");
    ui->lineEditThreads->setPlaceholderText("e.g. 100");

    mLoadConfig();
}

BruteConfigDialog::~BruteConfigDialog(){
    delete ui;
}

void BruteConfigDialog::on_buttonCancel_clicked(){
    accept();
}

void BruteConfigDialog::on_buttonOk_clicked(){
    mSaveConfig();
    accept();
}

void BruteConfigDialog::mLoadConfig(){
    ui->lineEditThreads->setText(QString::number(gConfig.tools.brute.threads));
    ui->lineEditTimeout->setText(QString::number(gConfig.tools.brute.timeout));
    ui->checkBoxWildcards->setChecked(gConfig.tools.brute.wildcard);
    ui->checkBoxAutosave->setChecked(gConfig.tools.brute.autoSaveResults);
    ui->checkBoxNoDuplicates->setChecked(gConfig.tools.brute.noDuplicates);
    ui->groupBoxTimeout->setChecked(gConfig.tools.brute.useTimeout);

    switch (gConfig.tools.brute.record) {
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

    ui->comboBoxNameserver->addItems(gConfig.tools.brute.nameservers.keys());
    ui->comboBoxNameserver->setCurrentText(gConfig.tools.brute.nameserver);
}

void BruteConfigDialog::mSaveConfig(){
    gConfig.tools.brute.threads = ui->lineEditThreads->text().toInt();
    gConfig.tools.brute.timeout = ui->lineEditTimeout->text().toInt();

    gConfig.tools.brute.wildcard = ui->checkBoxWildcards->isChecked();
    gConfig.tools.brute.noDuplicates = ui->checkBoxNoDuplicates->isChecked();
    gConfig.tools.brute.autoSaveResults = ui->checkBoxAutosave->isChecked();
    gConfig.tools.brute.useTimeout = ui->groupBoxTimeout->isChecked();

    if(ui->radioButtonA->isChecked())
        gConfig.tools.brute.record = Config::DNS::A;
    if(ui->radioButtonAAAA->isChecked())
        gConfig.tools.brute.record = Config::DNS::AAAA;
    if(ui->radioButtonANY->isChecked())
        gConfig.tools.brute.record = Config::DNS::ANY;

    gConfig.tools.brute.nameserver = ui->comboBoxNameserver->currentText();
}
