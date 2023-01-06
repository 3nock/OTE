/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#ifndef BRUTECONFIGDIALOG_H
#define BRUTECONFIGDIALOG_H

#include <QDialog>
#include <QStringListModel>

namespace Ui {
class BruteConfigDialog;
}

class BruteConfigDialog : public QDialog{
        Q_OBJECT

    public:
        BruteConfigDialog(QWidget *parent = nullptr);
        ~BruteConfigDialog();

    private slots:
        void on_buttonCancel_clicked();
        void on_buttonOk_clicked();

    private:
        Ui::BruteConfigDialog *ui;

        void mLoadConfig();
        void mSaveConfig();
};

#endif // ACTIVECONFIGDIALOG_H
