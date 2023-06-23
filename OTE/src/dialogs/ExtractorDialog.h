//  Copyright (C) 2023 OSINT Template Engine (https://github.com/3nock/OTE). All Rights Reserved.
//
//  Licensed under the GPL License, Version 3.0 (the "License");
//  you may not use this file except in compliance with the License.
//
//  Author: Enock N. Michael (enock.n.michael@gmail.com)

#ifndef DIALOGS_EXTRACTORDIALOG_H
#define DIALOGS_EXTRACTORDIALOG_H

#include <QDialog>

#include "src/core/Highlighter.h"
#include "src/template/Extractor.h"

namespace Ui {
class ExtractorDialog;
}

class ExtractorDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ExtractorDialog(QWidget *parent, OTE::Extractor *extractor);
    ~ExtractorDialog();

private slots:
    void on_buttonOK_clicked();

private:
    Ui::ExtractorDialog *ui;

    OTE::Extractor *mExtractor;
    OTE::SyntaxHighlighter *mHighlighter = nullptr;
};

#endif // DIALOGS_EXTRACTORDIALOG_H
