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

    private slots:
        void on_buttonOk_clicked();
        void on_buttonCancel_clicked();

    private:
        Ui::EnumConfigDialog *ui;
        ScanConfig *m_config;
        bool asn = false;
        bool cidr = false;

        void saveConfig_asn();
        void saveConfig_cidr();
};

#endif // ENUMCONFIGDIALOG_H
