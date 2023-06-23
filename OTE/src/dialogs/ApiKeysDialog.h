//  Copyright (C) 2023 OSINT Template Engine (https://github.com/3nock/OTE). All Rights Reserved.
//
//  Licensed under the GPL License, Version 3.0 (the "License");
//  you may not use this file except in compliance with the License.
//
//  Author: Enock N. Michael (enock.n.michael@gmail.com)

#ifndef DIALOGS_APIKEYS_H
#define DIALOGS_APIKEYS_H

#include <QDialog>

#include "src/template/Template.h"

namespace Ui {
    class ApiKeysDialog;
}

class ApiKeysDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ApiKeysDialog(QWidget *parent = nullptr);
    ~ApiKeysDialog();

signals:
    void save();
    void keysUpdated();

public slots:
    void onNewTemplate(OTE::Template *tmplt);

private slots:
    void on_buttonSave_clicked();
    void on_buttonCancel_clicked();

private:
    Ui::ApiKeysDialog *ui;

    void initKeys();
};

#endif // DIALOGS_APIKEYS_H
