//  Copyright (C) 2023 OSINT Template Engine (https://github.com/3nock/OTE). All Rights Reserved.
//
//  Licensed under the GPL License, Version 3.0 (the "License");
//  you may not use this file except in compliance with the License.
//
//  Author: Enock N. Michael (enock.n.michael@gmail.com)

#include "AboutDialog.h"
#include "ui_AboutDialog.h"

#include <QFile>

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);

    // setting up logo
    QPixmap logo(":/icons/res/icons/logo256.png");
    ui->labelLogo->setPixmap(logo);

    ui->labelAbout->setTextFormat(Qt::RichText);
    ui->labelAbout->setTextInteractionFlags(Qt::TextBrowserInteraction);
    ui->labelAbout->setOpenExternalLinks(true);

    // setting up description
    QFile file(":/files/res/files/about.html");
    if(file.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        ui->labelAbout->setText(file.readAll());
        file.close();
    }
}

AboutDialog::~AboutDialog()
{
    delete ui;
}

void AboutDialog::on_buttonOK_clicked()
{
    accept();
}
