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
    ui->lineEditTimeout->setPlaceholderText("e.g. 1000");
    ui->lineEditMaxPages->setPlaceholderText("e.g. 100");
}
PassiveConfigDialog::~PassiveConfigDialog(){
    delete ui;
}

void PassiveConfigDialog::loadConfig_osint(){
    CONFIG.beginGroup(CFG_OSINT);
    ui->lineEditTimeout->setText(CONFIG.value(CFG_VAL_TIMEOUT).toString());
    ui->lineEditMaxPages->setText(CONFIG.value(CFG_VAL_MAXPAGES).toString());
    ui->checkBoxAutosave->setChecked(CONFIG.value(CFG_VAL_AUTOSAVE).toBool());
    ui->checkBoxNoDuplicates->setChecked(CONFIG.value(CFG_VAL_DUPLICATES).toBool());
    ui->groupBoxTimeout->setChecked(CONFIG.value(CFG_VAL_SETTIMEOUT).toBool());
    CONFIG.endGroup();
    osint = true;
}

void PassiveConfigDialog::loadConfig_raw(){
    ui->groupBoxScan->hide();
    CONFIG.beginGroup(CFG_RAW);
    ui->lineEditTimeout->setText(CONFIG.value(CFG_VAL_TIMEOUT).toString());
    ui->checkBoxAutosave->setChecked(CONFIG.value(CFG_VAL_AUTOSAVE).toBool());
    ui->checkBoxNoDuplicates->setChecked(CONFIG.value(CFG_VAL_DUPLICATES).toBool());
    ui->groupBoxTimeout->setChecked(CONFIG.value(CFG_VAL_SETTIMEOUT).toBool());
    CONFIG.endGroup();
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
    m_config->timeout = ui->lineEditTimeout->text().toInt();
    m_config->maxPage = ui->lineEditMaxPages->text().toInt();
    m_config->autosaveToProject = ui->checkBoxAutosave->isChecked();
    m_config->noDuplicates = ui->checkBoxNoDuplicates->isChecked();
    m_config->setTimeout = ui->groupBoxTimeout->isChecked();

    CONFIG.beginGroup(CFG_OSINT);
    CONFIG.setValue(CFG_VAL_TIMEOUT, ui->lineEditTimeout->text());
    CONFIG.setValue(CFG_VAL_MAXPAGES, ui->lineEditMaxPages->text());
    CONFIG.setValue(CFG_VAL_AUTOSAVE, ui->checkBoxAutosave->isChecked());
    CONFIG.setValue(CFG_VAL_DUPLICATES, ui->checkBoxNoDuplicates->isChecked());
    CONFIG.setValue(CFG_VAL_SETTIMEOUT, ui->groupBoxTimeout->isChecked());
    CONFIG.endGroup();
}

void PassiveConfigDialog::saveConfig_raw(){
    m_config->timeout = ui->lineEditTimeout->text().toInt();
    m_config->autosaveToProject = ui->checkBoxAutosave->isChecked();
    m_config->noDuplicates = ui->checkBoxNoDuplicates->isChecked();
    m_config->setTimeout = ui->groupBoxTimeout->isChecked();

    CONFIG.beginGroup(CFG_RAW);
    CONFIG.setValue(CFG_VAL_TIMEOUT, ui->lineEditTimeout->text());
    CONFIG.setValue(CFG_VAL_AUTOSAVE, ui->checkBoxAutosave->isChecked());
    CONFIG.setValue(CFG_VAL_DUPLICATES, ui->checkBoxNoDuplicates->isChecked());
    CONFIG.setValue(CFG_VAL_SETTIMEOUT, ui->groupBoxTimeout->isChecked());
    CONFIG.endGroup();
}
