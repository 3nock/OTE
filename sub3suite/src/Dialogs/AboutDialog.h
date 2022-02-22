/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QDialog>
#include <QStandardItemModel>


namespace Ui {
    class AboutDialog;
}

class AboutDialog : public QDialog{
        Q_OBJECT

    public:
        explicit AboutDialog(QWidget *parent = nullptr);
        ~AboutDialog();

    private slots:
        void on_tableViewModules_customContextMenuRequested(const QPoint &pos);

        void on_tableViewContributors_customContextMenuRequested(const QPoint &pos);

        void on_tableViewDonations_customContextMenuRequested(const QPoint &pos);

        void on_buttonCopy_clicked();

private:
        Ui::AboutDialog *ui;
        QStandardItemModel *m_authorsModel;
        QStandardItemModel *m_modulesModel;
        QStandardItemModel *m_foldersModel;
};

#endif // ABOUTDIALOG_H
