/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#include "PassiveConfigDialog.h"
#include "ui_PassiveConfigDialog.h"

#include <QIntValidator>
#include <QMessageBox>


PassiveConfigDialog::PassiveConfigDialog(QWidget *parent, ScanConfig *config) : QDialog(parent),
    ui(new Ui::PassiveConfigDialog),
    m_config(config)
{
    ui->setupUi(this);

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

void PassiveConfigDialog::on_buttonOk_clicked(){
    m_config->autosaveToProject = ui->checkBoxAutosave->isChecked();
    m_config->noDuplicates = ui->checkBoxNoDuplicates->isChecked();
    accept();
}

void PassiveConfigDialog::on_checkBoxNoDuplicates_clicked(bool checked){
    if(checked)
        QMessageBox::warning(this, "Warning", "This option can be time-costly but results in no duplicates among results");
}

void PassiveConfigDialog::on_buttonCancel_clicked(){
    accept();
}
