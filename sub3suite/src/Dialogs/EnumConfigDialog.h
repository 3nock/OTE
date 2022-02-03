#ifndef ENUMCONFIGDIALOG_H
#define ENUMCONFIGDIALOG_H

#include <QDialog>

namespace Ui {
class EnumConfigDialog;
}

class EnumConfigDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EnumConfigDialog(QWidget *parent = nullptr);
    ~EnumConfigDialog();

private slots:
    void on_buttonOk_clicked();

    void on_buttonCancel_clicked();

private:
    Ui::EnumConfigDialog *ui;
};

#endif // ENUMCONFIGDIALOG_H
