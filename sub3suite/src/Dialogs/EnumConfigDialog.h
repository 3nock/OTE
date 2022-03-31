#ifndef ENUMCONFIGDIALOG_H
#define ENUMCONFIGDIALOG_H

#include <QDialog>
#include "src/modules/passive/AbstractOsintModule.h"


namespace Ui {
class EnumConfigDialog;
}

class EnumConfigDialog : public QDialog{
        Q_OBJECT

    public:
        EnumConfigDialog(QWidget *parent = nullptr, ScanConfig *config = nullptr);
        ~EnumConfigDialog();

        void loadConfig_asn();
        void loadConfig_cidr();
        void loadConfig_ip();
        void loadConfig_ns();
        void loadConfig_mx();
        void loadConfig_ssl();
        void loadConfig_email();

    private slots:
        void on_buttonOk_clicked();
        void on_buttonCancel_clicked();

    private:
        Ui::EnumConfigDialog *ui;
        ScanConfig *m_config;

        bool asn = false;
        bool cidr = false;
        bool ip = false;
        bool ns = false;
        bool mx = false;
        bool ssl = false;
        bool email = false;

        void saveConfig_asn();
        void saveConfig_cidr();
        void saveConfig_ip();
        void saveConfig_ns();
        void saveConfig_mx();
        void saveConfig_ssl();
        void saveConfig_email();
};

#endif // ENUMCONFIGDIALOG_H
