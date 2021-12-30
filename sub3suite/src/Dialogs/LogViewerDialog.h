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
        void on_comboBoxLogType_currentIndexChanged(int index);

    private:
        Ui::LogViewerDialog *ui;
        LogsSyntaxHighlighter *m_logsSyntaxHighlighter;
};

#endif // LOGVIEWERDIALOG_H
