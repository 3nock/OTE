/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#ifndef RAWCONFIGDIALOG_H
#define RAWCONFIGDIALOG_H

#include <QDialog>


namespace Ui {
class RawConfigDialog;
}

class RawConfigDialog : public QDialog{
        Q_OBJECT

    public:
        RawConfigDialog(QWidget *parent = nullptr);
        ~RawConfigDialog();

    private slots:
        void on_buttonOk_clicked();
        void on_buttonCancel_clicked();

    private:
        Ui::RawConfigDialog *ui;

        void mLoadConfig();
        void mSaveConfig();
};

#endif // PASSIVECONFIGDIALOG_H
