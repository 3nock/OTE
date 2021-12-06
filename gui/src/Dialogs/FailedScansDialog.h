#ifndef FAILEDSCANSDIALOG_H
#define FAILEDSCANSDIALOG_H

#include <QDialog>

namespace Ui {
class FailedScansDialog;
}

class FailedScansDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FailedScansDialog(QWidget *parent = nullptr);
    ~FailedScansDialog();

private:
    Ui::FailedScansDialog *ui;
};

#endif // FAILEDSCANSDIALOG_H
