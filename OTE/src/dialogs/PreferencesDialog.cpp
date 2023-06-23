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
    mMiscConfig(new MiscConfig(this)),
    mEngineConfig(new EngineConfig(this))
{
    ui->setupUi(this);

    mMiscConfig->openConfig();
    mEngineConfig->openConfig();

    ui->tabWidget->insertTab(0, mEngineConfig, tr("Engine"));
    ui->tabWidget->insertTab(1, mMiscConfig, tr("Misc"));
}

PreferencesDialog::~PreferencesDialog()
{
    delete mEngineConfig;
    delete mMiscConfig;
    delete ui;
}

void PreferencesDialog::on_buttonSave_clicked()
{
    mEngineConfig->saveConfig();
    mMiscConfig->saveConfig();

    accept();
}

void PreferencesDialog::on_buttonCancel_clicked()
{
    accept();
}
