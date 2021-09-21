#ifndef OSINTCONFIGDIALOG_H
#define OSINTCONFIGDIALOG_H

#include <QDialog>

namespace Ui {
    class OsintConfigDialog;
}

class OsintConfigDialog : public QDialog
{
        Q_OBJECT

    public:
        explicit OsintConfigDialog(QWidget *parent = nullptr);
        ~OsintConfigDialog();

    private:
        Ui::OsintConfigDialog *ui;
};

#endif // OSINTCONFIGDIALOG_H
