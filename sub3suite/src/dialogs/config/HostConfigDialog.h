/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#ifndef HOSTCONFIGDIALOG_H
#define HOSTCONFIGDIALOG_H

#include <QDialog>
#include <QStringListModel>

namespace Ui {
class HostConfigDialog;
}

class HostConfigDialog : public QDialog{
        Q_OBJECT

    public:
        HostConfigDialog(QWidget *parent = nullptr);
        ~HostConfigDialog();

    private slots:
        void on_buttonCancel_clicked();
        void on_buttonOk_clicked();

    private:
        Ui::HostConfigDialog *ui;

        void mLoadConfig();
        void mSaveConfig();
};

#endif // ACTIVECONFIGDIALOG_H
