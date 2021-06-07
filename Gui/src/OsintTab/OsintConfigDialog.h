#ifndef OSINTCONFIGDIALOG_H
#define OSINTCONFIGDIALOG_H

#include "lib-osint.h"

namespace Ui {
    class OsintConfigDialog;
}

class OsintConfigDialog : public QDialog{
        Q_OBJECT

    public:
        explicit OsintConfigDialog(QWidget *parent = nullptr);
        ~OsintConfigDialog();

    private slots:
        void on_pushButton_cancel_clicked();

    private:
            Ui::OsintConfigDialog *ui;
};

#endif // OSINTCONFIGDIALOG_H
