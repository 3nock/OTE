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

    ui->labelAbout->setText("OSINT Template Engine is a research-grade tool for Information gathering & Attack Surface Mapping using customizable templates."
                            "<br>"
                            "The templates are the driving force of the OSINT Template Engine. You can create one or modify existing ones in a matter of seconds."
                            "<br>"
                            "<h3>New to OSINT Template Engine?</h3>"
                            "<li>  • Learn how to use OSINT Template Engine <a href=\"https://github.com/3nock/OTE/wiki\">here</a>."
                            "<br>"
                            "<li>  • Learn how to develop your own templates or modifying existing ones to your liking <a href=\"https://github.com/OTE/wiki\">here</a>."
                            "<br>"
                            "<h3>Want to contribute?</h3>"
                            "<li>  • You can share your templates at the general community <a href=\"https://github.com/3nock/OTE-Templates\">templates repository</a>."
                            "<br>"
                            "<li>  • Are you a developer? you can participate on the active development of the <a href=\"https://github.com/3nock/OTE\">tool</a>."
                            "<br>"
                            "<li>  • You can support the development of OSINT Template Engine <a href=\"https://spidersuite.github.io/sponsor\">financially</a>.");

    ui->labelLoadedTemplates->setTextFormat(Qt::RichText);
    ui->labelLoadedTemplates->setText("<h1>Templates</h1>");

    mVLayout = new QVBoxLayout(ui->scrollAreaWidgetContents);
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

        mVLayout->addLayout(layout, 1);
    }

    ui->scrollArea->setProperty("noBorder", true);
}

WelcomeView::~WelcomeView()
{
    delete ui;
}

void WelcomeView::onNewTemplate(OTE::Template *tmplt)
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

    mVLayout->addLayout(layout, 1);
}
