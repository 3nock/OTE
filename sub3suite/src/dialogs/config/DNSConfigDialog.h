/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#ifndef DNSCONFIGDIALOG_H
#define DNSCONFIGDIALOG_H

#include <QDialog>
#include <QStringListModel>

namespace Ui {
class DNSConfigDialog;
}

class DNSConfigDialog : public QDialog{
        Q_OBJECT

    public:
        DNSConfigDialog(QWidget *parent = nullptr);
        ~DNSConfigDialog();

    private slots:
        void on_buttonCancel_clicked();
        void on_buttonOk_clicked();

    private:
        Ui::DNSConfigDialog *ui;

        void mLoadConfig();
        void mSaveConfig();
};

#endif // ACTIVECONFIGDIALOG_H
