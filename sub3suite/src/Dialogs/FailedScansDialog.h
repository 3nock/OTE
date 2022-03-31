/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#ifndef FAILEDSCANSDIALOG_H
#define FAILEDSCANSDIALOG_H

#include <QDialog>
#include <QQueue>
#include <QStandardItemModel>


namespace Ui {
class FailedScansDialog;
}

class FailedScansDialog : public QDialog{
        Q_OBJECT

    public:
        FailedScansDialog(QWidget *parent = nullptr, QMap<QString,QString> targets = {});
        ~FailedScansDialog();

    signals:
        void reScan(QQueue<QString> targets);

    private slots:
        void on_buttonReScan_clicked();
        void on_buttonCancel_clicked();
        void on_buttonCopy_clicked();
        void on_tableView_customContextMenuRequested(const QPoint &pos);

    private:
        Ui::FailedScansDialog *ui;
        QStandardItemModel *m_model;
};

#endif // FAILEDSCANSDIALOG_H
