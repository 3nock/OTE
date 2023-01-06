/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#ifndef OSINTCONFIGDIALOG_H
#define OSINTCONFIGDIALOG_H

#include <QDialog>

namespace Ui {
class OsintConfigDialog;
}

class OsintConfigDialog : public QDialog{
        Q_OBJECT

    public:
        OsintConfigDialog(QWidget *parent = nullptr);
        ~OsintConfigDialog();

    private slots:
        void on_buttonOk_clicked();
        void on_buttonCancel_clicked();

    private:
        Ui::OsintConfigDialog *ui;

        void mLoadConfig();
        void mSaveConfig();
};

#endif // PASSIVECONFIGDIALOG_H
