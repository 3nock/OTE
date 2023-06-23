//  Copyright (C) 2023 OSINT Template Engine (https://github.com/3nock/OTE). All Rights Reserved.
//
//  Licensed under the GPL License, Version 3.0 (the "License");
//  you may not use this file except in compliance with the License.
//
//  Author: Enock N. Michael (enock.n.michael@gmail.com)

#ifndef DIALOGS_MULTIPLETARGETSDIALOG_H
#define DIALOGS_MULTIPLETARGETSDIALOG_H

#include <QDialog>
#include <QFile>
#include <QFileDialog>
#include <QListWidget>
#include <QMessageBox>
#include <QStringList>
#include <QStringListModel>
#include <QTextStream>


namespace Ui {
class MultipleTargetsDialog;
}

class MultipleTargetsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MultipleTargetsDialog(QStringList *targets, const QString &description, QWidget *parent = nullptr);
    ~MultipleTargetsDialog();

private slots:
    void on_buttonClear_clicked();
    void on_buttonRemove_clicked();
    void on_buttonLoad_clicked();
    void on_buttonAdd_clicked();
    void on_buttonPaste_clicked();
    void on_buttonStart_clicked();

private:
    Ui::MultipleTargetsDialog *ui;

    QStringList *mTargets;
    QStringListModel *mModel;
};

#endif // DIALOGS_MULTIPLETARGETSDIALOG_H
