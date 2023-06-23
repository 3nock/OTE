//  Copyright (C) 2023 OSINT Template Engine (https://github.com/3nock/OTE). All Rights Reserved.
//
//  Licensed under the GPL License, Version 3.0 (the "License");
//  you may not use this file except in compliance with the License.
//
//  Author: Enock N. Michael (enock.n.michael@gmail.com)

#ifndef DIALOGS_TEMPLATEDIALOG_H
#define DIALOGS_TEMPLATEDIALOG_H

#include <QDialog>
#include <QStandardItem>

#include "src/template/Template.h"

namespace Ui {
class TemplateDialog;
}

class TemplateDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TemplateDialog(QWidget *parent, OTE::Template *tmplt);
    ~TemplateDialog();

private slots:
    void on_buttonSave_clicked();
    void on_buttonAddEndPoint_clicked();
    void on_buttonEditEndPoint_clicked();
    void on_buttonRemoveEndPoint_clicked();
    void on_radioButtonQueryAuth_clicked(bool checked);
    void on_radioButtonHeaderAuth_clicked(bool checked);
    void on_radioButtonBasicAuth_clicked(bool checked);
    void on_radioButtonBearerAuth_clicked(bool checked);

private:
    Ui::TemplateDialog *ui;

    OTE::Template *mTemplate;

    QStandardItemModel *mModelEndpoints;
};

#endif // DIALOGS_TEMPLATEDIALOG_H
