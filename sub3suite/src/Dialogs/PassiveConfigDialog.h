/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#ifndef PASSIVECONFIGDIALOG_H
#define PASSIVECONFIGDIALOG_H

#include <QDialog>
#include "src/modules/passive/AbstractOsintModule.h"


namespace Ui {
class PassiveConfigDialog;
}

class PassiveConfigDialog : public QDialog{
        Q_OBJECT

    public:
        PassiveConfigDialog(QWidget *parent = nullptr, ScanConfig *config = nullptr);
        ~PassiveConfigDialog();

        void loadConfig_osint();
        void loadConfig_raw();

    private slots:
        void on_buttonOk_clicked();
        void on_checkBoxNoDuplicates_clicked(bool checked);
        void on_buttonCancel_clicked();

    private:
        Ui::PassiveConfigDialog *ui;
        ScanConfig *m_config;
        bool osint = false;
        bool raw = false;
        void saveConfig_osint();
        void saveConfig_raw();
};

#endif // PASSIVECONFIGDIALOG_H
