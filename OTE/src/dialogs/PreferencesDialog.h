//  Copyright (C) 2023 OSINT Template Engine (https://github.com/3nock/OTE). All Rights Reserved.
//
//  Licensed under the GPL License, Version 3.0 (the "License");
//  you may not use this file except in compliance with the License.
//
//  Author: Enock N. Michael (enock.n.michael@gmail.com)

#ifndef DIALOGS_PREFERENCESDIALOG_H
#define DIALOGS_PREFERENCESDIALOG_H

#include <QDialog>

#include "src/dialogs/preferences/EngineConfig.h"
#include "src/dialogs/preferences/GeneralConfig.h"

namespace Ui {
class PreferencesDialog;
}

class PreferencesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PreferencesDialog(QWidget *parent = nullptr);
    ~PreferencesDialog();

private slots:
    void on_buttonSave_clicked();
    void on_buttonCancel_clicked();

private:
    Ui::PreferencesDialog *ui;

    GeneralConfig *mGeneralConfig;
    EngineConfig *mEngineConfig;
};

#endif // DIALOGS_PREFERENCESDIALOG_H
