//  Copyright (C) 2023 OSINT Template Engine (https://github.com/3nock/OTE). All Rights Reserved.
//
//  Licensed under the GPL License, Version 3.0 (the "License");
//  you may not use this file except in compliance with the License.
//
//  Author: Enock N. Michael (enock.n.michael@gmail.com)

#ifndef DIALOGS_ENDPOINTDIALOG_H
#define DIALOGS_ENDPOINTDIALOG_H

#include <QDialog>
#include <QStandardItemModel>
#include <QStringListModel>

#include "src/template/Endpoint.h"

namespace Ui {
class EndpointDialog;
}

class EndpointDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EndpointDialog(QWidget *parent, OTE::Endpoint *endpoint);
    ~EndpointDialog();

private slots:
    void on_buttonOK_clicked();
    void on_buttonAdd_clicked();
    void on_buttonEdit_clicked();
    void on_buttonRemove_clicked();
    void on_buttonAddInputType_clicked();
    void on_radioButtonGetReq_toggled(bool checked);
    void on_radioButtonPostReq_toggled(bool checked);

private:
    Ui::EndpointDialog *ui;

    OTE::Endpoint *mEndpoint;
    QStringListModel *mModelInputTypes;
    QStandardItemModel *mModelExtrators;
};

#endif // DIALOGS_ENDPOINTDIALOG_H
