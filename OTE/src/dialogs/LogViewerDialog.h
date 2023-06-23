//  Copyright (C) 2023 OSINT Template Engine (https://github.com/3nock/OTE). All Rights Reserved.
//
//  Licensed under the GPL License, Version 3.0 (the "License");
//  you may not use this file except in compliance with the License.
//
//  Author: Enock N. Michael (enock.n.michael@gmail.com)

#ifndef DIALOGS_LOGVIEWERDIALOG_H
#define DIALOGS_LOGVIEWERDIALOG_H

#include <QDialog>
#include <QPlainTextEdit>

namespace Ui {
class LogViewerDialog;
}

class LogViewerDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LogViewerDialog(QWidget *parent = nullptr);
    ~LogViewerDialog();

    static QPlainTextEdit *logsTextEdit;

private:
    Ui::LogViewerDialog *ui;
};

#endif // DIALOGS_LOGVIEWERDIALOG_H
