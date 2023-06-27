//  Copyright (C) 2023 OSINT Template Engine (https://github.com/3nock/OTE). All Rights Reserved.
//
//  Licensed under the GPL License, Version 3.0 (the "License");
//  you may not use this file except in compliance with the License.
//
//  Author: Enock N. Michael (enock.n.michael@gmail.com)

#include "PreferencesDialog.h"
#include "ui_PreferencesDialog.h"

PreferencesDialog::PreferencesDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PreferencesDialog),
    mGeneralConfig(new GeneralConfig(this)),
    mEngineConfig(new EngineConfig(this))
{
    ui->setupUi(this);

    mGeneralConfig->openConfig();
    mEngineConfig->openConfig();

    ui->tabWidget->insertTab(0, mEngineConfig, tr("Engine"));
    ui->tabWidget->insertTab(1, mGeneralConfig, tr("General"));
}

PreferencesDialog::~PreferencesDialog()
{
    delete mEngineConfig;
    delete mGeneralConfig;
    delete ui;
}

void PreferencesDialog::on_buttonSave_clicked()
{
    mEngineConfig->saveConfig();
    mGeneralConfig->saveConfig();

    accept();
}

void PreferencesDialog::on_buttonCancel_clicked()
{
    accept();
}
