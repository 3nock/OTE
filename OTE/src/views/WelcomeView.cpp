//  Copyright (C) 2023 OSINT Template Engine (https://github.com/3nock/OTE). All Rights Reserved.
//
//  Licensed under the GPL License, Version 3.0 (the "License");
//  you may not use this file except in compliance with the License.
//
//  Author: Enock N. Michael (enock.n.michael@gmail.com)

#include "WelcomeView.h"
#include "ui_WelcomeView.h"

#include <QFile>

WelcomeView::WelcomeView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WelcomeView)
{
    ui->setupUi(this);

    ui->labelWelcome->setTextFormat(Qt::RichText);
    ui->labelWelcome->setTextInteractionFlags(Qt::TextBrowserInteraction);
    ui->labelWelcome->setOpenExternalLinks(true);

    QFile file(":/files/res/files/welcome.html");
    if(file.open(QIODevice::ReadOnly))
    {
        ui->labelWelcome->setText(QString::fromUtf8(file.readAll()));
        file.close();
    }
}

WelcomeView::~WelcomeView()
{
    delete ui;
}
