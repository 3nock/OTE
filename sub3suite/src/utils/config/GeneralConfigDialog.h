#ifndef GENERALCONFIGDIALOG_H
#define GENERALCONFIGDIALOG_H

#include <QDialog>

namespace Ui {
class GeneralConfigDialog;
}

class GeneralConfigDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GeneralConfigDialog(QWidget *parent = nullptr);
    ~GeneralConfigDialog();

private:
    Ui::GeneralConfigDialog *ui;
};

#endif // GENERALCONFIGDIALOG_H
