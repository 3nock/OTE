/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#include "PassiveConfigDialog.h"
#include "ui_PassiveConfigDialog.h"

#include <QIntValidator>
#include <QMessageBox>

#include "src/utils/Config.h"


PassiveConfigDialog::PassiveConfigDialog(QWidget *parent, ScanConfig *config) : QDialog(parent),
    ui(new Ui::PassiveConfigDialog),
    m_config(config)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/img/res/icons/gear.png"));

    /* adjusting the dialog size... */
    this->adjustSize();

    /* setting placeholder texts.. */
    ui->lineEditMaxPages->setPlaceholderText("e.g. 100");

    /* validators... */
    ui->lineEditMaxPages->setValidator(new QIntValidator(1, 1000, this));
}
PassiveConfigDialog::~PassiveConfigDialog(){
    delete ui;
}

void PassiveConfigDialog::loadConfig_osint(){
    ui->lineEditMaxPages->setText(CONFIG_OSINT.value("max_pages").toString());
    ui->checkBoxAutosave->setChecked(CONFIG_OSINT.value("autosave_to_project").toBool());
    ui->checkBoxNoDuplicates->setChecked(CONFIG_OSINT.value("no_duplicates").toBool());
    osint = true;
}

void PassiveConfigDialog::loadConfig_raw(){
    ui->groupBoxScan->hide();
    ui->checkBoxAutosave->setChecked(CONFIG_RAW.value("autosave_to_project").toBool());
    ui->checkBoxNoDuplicates->setChecked(CONFIG_RAW.value("no_duplicates").toBool());
    raw = true;
}

void PassiveConfigDialog::on_buttonOk_clicked(){
    if(osint)
        this->saveConfig_osint();
    if(raw)
        this->saveConfig_raw();
    accept();
}

void PassiveConfigDialog::on_checkBoxNoDuplicates_clicked(bool checked){
    if(checked)
        QMessageBox::warning(this, tr("Warning"), tr("This option can be time-costly but results in no duplicates among results"));
}

void PassiveConfigDialog::on_buttonCancel_clicked(){
    accept();
}

void PassiveConfigDialog::saveConfig_osint(){
    m_config->maxPage = ui->lineEditMaxPages->text().toInt();
    m_config->autosaveToProject = ui->checkBoxAutosave->isChecked();
    m_config->noDuplicates = ui->checkBoxNoDuplicates->isChecked();

    CONFIG_OSINT.setValue("max_pages", ui->lineEditMaxPages->text());
    CONFIG_OSINT.setValue("autosave_to_project", ui->checkBoxAutosave->isChecked());
    CONFIG_OSINT.setValue("no_duplicates", ui->checkBoxNoDuplicates->isChecked());
}

void PassiveConfigDialog::saveConfig_raw(){
    m_config->autosaveToProject = ui->checkBoxAutosave->isChecked();
    m_config->noDuplicates = ui->checkBoxNoDuplicates->isChecked();

    CONFIG_RAW.setValue("autosave_to_project", ui->checkBoxAutosave->isChecked());
    CONFIG_RAW.setValue("no_duplicates", ui->checkBoxNoDuplicates->isChecked());
}
