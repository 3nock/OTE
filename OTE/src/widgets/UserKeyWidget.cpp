//  Copyright (C) 2023 OSINT Template Engine (https://github.com/3nock/OTE). All Rights Reserved.
//
//  Licensed under the GPL License, Version 3.0 (the "License");
//  you may not use this file except in compliance with the License.
//
//  Author: Enock N. Michael (enock.n.michael@gmail.com)

#include "UserKeyWidget.h"
#include "ui_UserKeyWidget.h"

#include <QDesktopServices>
#include <QUrl>

#include "src/core/Config.h"

UserKeyWidget::UserKeyWidget(OTE::Template *tmplt, QWidget *parent):
    QWidget(parent),
    ui(new Ui::UserKeyWidget),
    mTemplate(tmplt)
{
    ui->setupUi(this);

    ui->labelTemplate->setText(mTemplate->info.name);

    ui->lineEditKey->setText(mTemplate->authentication.key);
    ui->lineEditUser->setText(mTemplate->authentication.id);
}

UserKeyWidget::~UserKeyWidget()
{
    delete ui;
}

void UserKeyWidget::onSave()
{
    mTemplate->authentication.id = ui->lineEditUser->text();
    mTemplate->authentication.key = ui->lineEditKey->text();

    OTE::gConfig.apiKeys.insert(mTemplate->info.name, {mTemplate->authentication.id,
                                                       mTemplate->authentication.key});
}

void UserKeyWidget::on_buttonGet_clicked()
{
    QDesktopServices::openUrl(QUrl(mTemplate->authentication.link, QUrl::TolerantMode));
}
