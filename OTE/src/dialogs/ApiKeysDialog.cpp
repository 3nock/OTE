//  Copyright (C) 2023 OSINT Template Engine (https://github.com/3nock/OTE). All Rights Reserved.
//
//  Licensed under the GPL License, Version 3.0 (the "License");
//  you may not use this file except in compliance with the License.
//
//  Author: Enock N. Michael (enock.n.michael@gmail.com)

#include "ApiKeysDialog.h"
#include "ui_ApiKeysDialog.h"

#include "src/widgets/KeyWidget.h"
#include "src/widgets/UserKeyWidget.h"


ApiKeysDialog::ApiKeysDialog(QWidget *parent):
    QDialog(parent),
    ui(new Ui::ApiKeysDialog)
{
    ui->setupUi(this);

    initKeys();
}

ApiKeysDialog::~ApiKeysDialog()
{
    delete ui;
}

void ApiKeysDialog::onNewTemplate(OTE::Template *tmplt)
{
    if(tmplt->authentication.uses_id && tmplt->authentication.uses_key)
    {
        UserKeyWidget *userKeyWidget = new UserKeyWidget(tmplt, this);

        connect(this, &ApiKeysDialog::save, userKeyWidget, &UserKeyWidget::onSave);

        ui->verticalLayoutScrollArea->insertWidget(0, userKeyWidget);
    }
    else if(tmplt->authentication.uses_key)
    {
        KeyWidget *keyWidget = new KeyWidget(tmplt, this);

        connect(this, &ApiKeysDialog::save, keyWidget, &KeyWidget::onSave);

        ui->verticalLayoutScrollArea->addWidget(keyWidget);
    }
}

void ApiKeysDialog::on_buttonSave_clicked()
{
    emit save();
    emit keysUpdated();
    accept();
}

void ApiKeysDialog::on_buttonCancel_clicked()
{
    accept();
}

void ApiKeysDialog::initKeys()
{
    QList<OTE::Template*> keyOnlyList;
    QList<OTE::Template*> userKeyList;

    foreach(OTE::Template *tmplt, OTE::gTemplates)
    {
        if(tmplt->authentication.uses_id && tmplt->authentication.uses_key)
            userKeyList << tmplt;
        else if(tmplt->authentication.uses_key)
            keyOnlyList << tmplt;
    }

    foreach(OTE::Template *tmplt, userKeyList)
    {
        UserKeyWidget *userKeyWidget = new UserKeyWidget(tmplt, this);

        connect(this, &ApiKeysDialog::save, userKeyWidget, &UserKeyWidget::onSave);

        ui->verticalLayoutScrollArea->addWidget(userKeyWidget);
    }

    foreach(OTE::Template *tmplt, keyOnlyList)
    {
        KeyWidget *keyWidget = new KeyWidget(tmplt, this);

        connect(this, &ApiKeysDialog::save, keyWidget, &KeyWidget::onSave);

        ui->verticalLayoutScrollArea->addWidget(keyWidget);
    }
}
