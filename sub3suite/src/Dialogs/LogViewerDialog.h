/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#ifndef LOGVIEWERDIALOG_H
#define LOGVIEWERDIALOG_H

#include <QDialog>
#include "src/utils/LogsSyntaxHighlighter.h"


namespace Ui {
class LogViewerDialog;
}

class LogViewerDialog : public QDialog{
        Q_OBJECT

    public:
        explicit LogViewerDialog(QWidget *parent = nullptr);
        ~LogViewerDialog();

    private slots:
        void on_comboBoxLogFile_currentIndexChanged(int index);

    private:
        Ui::LogViewerDialog *ui;
        LogsSyntaxHighlighter *m_logsSyntaxHighlighter;
};

#endif // LOGVIEWERDIALOG_H
