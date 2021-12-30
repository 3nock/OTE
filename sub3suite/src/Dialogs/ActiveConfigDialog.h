#ifndef ACTIVECONFIGDIALOG_H
#define ACTIVECONFIGDIALOG_H

#include <QDialog>
#include <QSettings>
#include <QStringListModel>
#include "src/modules/scan/BruteScanner.h"
#include "src/modules/scan/ActiveScanner.h"
#include "src/modules/scan/DNSScanner.h"
#include "src/modules/scan/SSLScanner.h"
#include "src/modules/scan/IpScanner.h"


namespace Ui {
class ActiveConfigDialog;
}

class ActiveConfigDialog : public QDialog{
        Q_OBJECT

    public:
        ActiveConfigDialog(QWidget *parent = nullptr, brute::ScanConfig *config = nullptr);
        ActiveConfigDialog(QWidget *parent = nullptr, active::ScanConfig *config = nullptr);
        ActiveConfigDialog(QWidget *parent = nullptr, dns::ScanConfig *config = nullptr);
        ActiveConfigDialog(QWidget *parent = nullptr, ssl::ScanConfig *config = nullptr);
        ActiveConfigDialog(QWidget *parent = nullptr, ip::ScanConfig *config = nullptr);
        ~ActiveConfigDialog();

    private slots:
        void on_buttonCancel_clicked();
        void on_buttonOk_clicked();

    private:
        Ui::ActiveConfigDialog *ui;
        bool brute = false;
        bool active = false;
        bool dns = false;
        bool ssl = false;
        bool ip = false;
        brute::ScanConfig *m_configBrute = nullptr;
        active::ScanConfig *m_configActive = nullptr;
        dns::ScanConfig *m_configDns = nullptr;
        ssl::ScanConfig *m_configSSL = nullptr;
        ip::ScanConfig *m_configIp = nullptr;
        /* ... */
        QStringListModel *m_customNameserverListModel;
        QSettings *m_settings = nullptr;
        /* ... */
        void m_initWidgets();
        void m_loadConfigValues();
        void m_loadDefaultNameservers();
        void m_loadCustomNameservers();
        /* ... */
        void m_saveBrute();
        void m_saveActive();
        void m_saveDns();
        void m_saveSSL();
        void m_saveIp();

};

#endif // ACTIVECONFIGDIALOG_H
