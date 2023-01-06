/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#include "URLConfigDialog.h"
#include "ui_URLConfigDialog.h"

#include "src/utils/Config.h"


URLConfigDialog::URLConfigDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::URLConfigDialog)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/img/res/icons/gear.png"));

    ui->lineEditTimeout->setPlaceholderText("e.g. 3");
    ui->lineEditThreads->setPlaceholderText("e.g. 100");

    this->adjustSize();
    mLoadConfig();
}

URLConfigDialog::~URLConfigDialog(){
    delete ui;
}

void URLConfigDialog::on_buttonCancel_clicked(){
    accept();
}

void URLConfigDialog::on_buttonOk_clicked(){
    mSaveConfig();
    accept();
}

void URLConfigDialog::mLoadConfig(){
    ui->lineEditThreads->setText(QString::number(gConfig.tools.url.threads));
    ui->lineEditTimeout->setText(QString::number(gConfig.tools.url.timeout));
    ui->checkBoxAutosave->setChecked(gConfig.tools.url.autoSaveResults);
    ui->checkBoxNoDuplicates->setChecked(gConfig.tools.url.noDuplicates);
    ui->groupBoxTimeout->setChecked(gConfig.tools.url.useTimeout);
    ui->checkBoxTitle->setChecked(gConfig.tools.url.getTitle);
    ui->checkBoxRedirects->setChecked(gConfig.tools.url.followRedirects);
    ui->groupBoxScheme->setChecked(gConfig.tools.url.forceScheme);

    if(gConfig.tools.url.scheme == "https")
        ui->radioButtonHTTPS->setChecked(true);
    else if(gConfig.tools.url.scheme == "http")
        ui->radioButtonHTTP->setChecked(true);
    else if(gConfig.tools.url.scheme == "ftp")
        ui->radioButtonFTP->setChecked(true);
}

void URLConfigDialog::mSaveConfig(){
    gConfig.tools.url.threads = ui->lineEditThreads->text().toInt();
    gConfig.tools.url.timeout = ui->lineEditTimeout->text().toInt();
    gConfig.tools.url.noDuplicates = ui->checkBoxNoDuplicates->isChecked();
    gConfig.tools.url.autoSaveResults = ui->checkBoxAutosave->isChecked();
    gConfig.tools.url.useTimeout = ui->groupBoxTimeout->isChecked();
    gConfig.tools.url.getTitle = ui->checkBoxTitle->isChecked();
    gConfig.tools.url.followRedirects = ui->checkBoxRedirects->isChecked();
    gConfig.tools.url.forceScheme = ui->groupBoxScheme->isChecked();

    if(ui->radioButtonHTTP->isChecked())
        gConfig.tools.url.scheme = "http";
    else if(ui->radioButtonHTTPS->isChecked())
        gConfig.tools.url.scheme = "https";
    else if(ui->radioButtonFTP->isChecked())
        gConfig.tools.url.scheme = "ftp";
}
