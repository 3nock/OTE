/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#ifndef URLCONFIGDIALOG_H
#define URLCONFIGDIALOG_H

#include <QDialog>
#include <QStringListModel>

namespace Ui {
class URLConfigDialog;
}

class URLConfigDialog : public QDialog{
        Q_OBJECT

    public:
        URLConfigDialog(QWidget *parent = nullptr);
        ~URLConfigDialog();

    private slots:
        void on_buttonCancel_clicked();
        void on_buttonOk_clicked();

    private:
        Ui::URLConfigDialog *ui;

        void mLoadConfig();
        void mSaveConfig();
};

#endif // ACTIVECONFIGDIALOG_H
