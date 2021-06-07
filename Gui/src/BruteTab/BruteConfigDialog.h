#ifndef BRUTECONFIGDIALOG_H
#define BRUTECONFIGDIALOG_H

#include "lib-brute.h"

namespace Ui {
    class BruteConfigDialog;
}

class BruteConfigDialog : public QDialog{
        Q_OBJECT

    public:
        explicit BruteConfigDialog(QWidget *parent = nullptr, ScanArguments *scanArguments = nullptr);
        ~BruteConfigDialog();
        ScanArguments *scanArguments;

    private slots:
        void on_pushButton_clear_clicked();
        void on_pushButton_remove_clicked();
        void on_pushButton_load_clicked();
        void on_pushButton_add_clicked();
        void on_pushButton_cancel_clicked();
        void on_pushButton_ok_clicked();
        void on_checkBox_useCustomNameServers_clicked(bool checked);

    private:
        Ui::BruteConfigDialog *ui;
};

#endif // BRUTECONFIGDIALOG_H
