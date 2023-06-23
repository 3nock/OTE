//  Copyright (C) 2023 OSINT Template Engine (https://github.com/3nock/OTE). All Rights Reserved.
//
//  Licensed under the GPL License, Version 3.0 (the "License");
//  you may not use this file except in compliance with the License.
//
//  Author: Enock N. Michael (enock.n.michael@gmail.com)

#include "LogViewerDialog.h"
#include "ui_LogViewerDialog.h"


QPlainTextEdit *LogViewerDialog::logsTextEdit = nullptr;

LogViewerDialog::LogViewerDialog(QWidget *parent):
    QDialog(parent),
    ui(new Ui::LogViewerDialog)
{
    ui->setupUi(this);

    logsTextEdit = new QPlainTextEdit;
    ui->verticalLayout->insertWidget(0, logsTextEdit, 1);
}

LogViewerDialog::~LogViewerDialog()
{
    logsTextEdit->setParent(nullptr);
    delete logsTextEdit;
    logsTextEdit = nullptr;

    delete ui;
}
