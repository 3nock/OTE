//  Copyright (C) 2023 OSINT Template Engine (https://github.com/3nock/OTE). All Rights Reserved.
//
//  Licensed under the GPL License, Version 3.0 (the "License");
//  you may not use this file except in compliance with the License.
//
//  Author: Enock N. Michael (enock.n.michael@gmail.com)

#include "WelcomeView.h"
#include "ui_WelcomeView.h"

#include <QFile>
#include <QHBoxLayout>

WelcomeView::WelcomeView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WelcomeView)
{
    ui->setupUi(this);

    ui->labelWelcome->setTextFormat(Qt::RichText);
    ui->labelAbout->setTextFormat(Qt::RichText);
    ui->labelSocial->setTextFormat(Qt::MarkdownText);
    ui->labelWelcome->setTextInteractionFlags(Qt::TextBrowserInteraction);
    ui->labelAbout->setTextInteractionFlags(Qt::TextBrowserInteraction);
    ui->labelSocial->setTextInteractionFlags(Qt::TextBrowserInteraction);
    ui->labelWelcome->setOpenExternalLinks(true);
    ui->labelAbout->setOpenExternalLinks(true);
    ui->labelSocial->setOpenExternalLinks(true);

    ui->labelWelcome->setText(tr("<h1>Welcome to OSINT Template Engine</h1>"
                                 "<h2>Version 1.0.0</h2>"
                                 "<h3>© Copyright 2023</h3>"));

    ui->labelSocial->setText(tr("[Website](https://SpiderSuite.github.io/OTE) | [Twitter](https://twitter.com/spider_suite) | [Telegram](https://t.me/SpiderSuite) | [Github](https://github.com/3nock/OTE)"));

    ui->labelLoadedTemplates->setTextFormat(Qt::RichText);
    ui->labelLoadedTemplates->setText("<h1>Templates</h1>");

    QVBoxLayout *vLayout = new QVBoxLayout(ui->scrollAreaWidgetContents);
    foreach(OTE::Template *tmplt, OTE::gTemplates)
    {
        QHBoxLayout *layout = new QHBoxLayout;
        QLabel *nameLabel = new QLabel(this);
        nameLabel->setText(tmplt->info.name);

        QLabel *linkLabel = new QLabel(this);
        linkLabel->setTextFormat(Qt::MarkdownText);
        linkLabel->setTextInteractionFlags(Qt::TextBrowserInteraction);
        linkLabel->setOpenExternalLinks(true);
        linkLabel->setText(tmplt->info.link);
        auto font = linkLabel->font();
        font.setUnderline(true);
        linkLabel->setFont(font);

        layout->addWidget(nameLabel);
        layout->addWidget(linkLabel, 1);

        vLayout->addLayout(layout, 1);
    }

    ui->scrollArea->setProperty("noBorder", true);
}

WelcomeView::~WelcomeView()
{
    delete ui;
}

void WelcomeView::onNewTemplate(OTE::Template *tmplt)
{

}
