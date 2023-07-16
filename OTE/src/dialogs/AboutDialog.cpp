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
	ui->labelAbout-->setText(R"(<h1>OSINT Template Engine</h1>
Version 1.0.0
<br>
<br>
© Copyright OSINT Template Engine 2023
<br>
<br>
<a href="https://github.com/3nock/OTE">OSINT Template Engine</a> is a research-grade information gathering tool.
<br>
<br>
Visit <a href=https://spidersuite.github.io/OTE>OSINT Template Engine</a> for more information and Follow <a href="https://twitter.com/intent/follow?screen_name=spider_suite&tw_p=followbutton">@spider_suite</a> for updates on the project's progress.
<br>
<br>
This product includes software developed by the following open source projects:<br>
  • SQLite database library: <a href="https://sqlite.org">https://sqlite.org</a><br>
<br>
<br>
Want to help? <a href="https://spidersuite.github.io/sponsor/">Make a donation</a> or <a href="https://github.com/3nock/OTE/blob/main/CONTRIBUTING.md">get involved!</a><br>
)");
}

AboutDialog::~AboutDialog()
{
    delete ui;
}

void AboutDialog::on_buttonOK_clicked()
{
    accept();
}
