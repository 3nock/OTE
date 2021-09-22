#ifndef CONFIGDIALOG_H
#define CONFIGDIALOG_H

#include <QDialog>
#include <QDnsLookup>
#include "src/utils/utils.h"

namespace Ui {
    class ConfigDialog;
}

class ConfigDialog : public QDialog{
        Q_OBJECT

    public:
        ConfigDialog(QWidget *parent = nullptr, ScanConfig *scanConfig = nullptr);
        ~ConfigDialog();

    private slots:
        void on_buttonCancel_clicked();
        void on_buttonOk_clicked();
        void on_checkBoxCustomNameServers_clicked(bool checked);

    private:
        Ui::ConfigDialog *ui;
        ScanConfig *m_scanConfig;
        void loadConfigValues();
        void saveConfigValues();
};

#endif // CONFIGDIALOG_H
