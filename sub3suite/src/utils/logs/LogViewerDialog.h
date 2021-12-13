#ifndef LOGVIEWERDIALOG_H
#define LOGVIEWERDIALOG_H

#include <QDialog>

namespace Ui {
class LogViewerDialog;
}

class LogViewerDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LogViewerDialog(QWidget *parent = nullptr);
    ~LogViewerDialog();

private:
    Ui::LogViewerDialog *ui;
};

#endif // LOGVIEWERDIALOG_H
