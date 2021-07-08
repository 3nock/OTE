#ifndef CONFIGDIALOG_H
#define CONFIGDIALOG_H

#include "src/core.h"

namespace Ui {
    class ConfigDialog;
}

class ConfigDialog : public QDialog{
        Q_OBJECT

    private:
        Ui::ConfigDialog *ui;

    private:
        ScanConfig *m_scanConfig;

    public:
        explicit ConfigDialog(QWidget *parent = nullptr, ScanConfig *scanConfig = nullptr);
        ~ConfigDialog();

    private slots:
        void on_pushButton_clear_clicked();
        void on_pushButton_remove_clicked();
        void on_pushButton_load_clicked();
        void on_pushButton_add_clicked();
        void on_pushButton_cancel_clicked();
        void on_pushButton_ok_clicked();
        void on_checkBox_useCustomNameServers_clicked(bool checked);
};

#endif // CONFIGDIALOG_H
