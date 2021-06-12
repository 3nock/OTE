#ifndef APIKEYS_H
#define APIKEYS_H

// headers...
#include "lib-osint.h"

namespace Ui {
    class ApiKeysDialog;
}

class ApiKeysDialog : public QDialog{
    Q_OBJECT

    private:
        void m_setAvailableApiKeys();
        QString currentPath;

    public:
        explicit ApiKeysDialog(QWidget *parent = nullptr);
        ~ApiKeysDialog();

    private slots:
        void on_pushButton_save_clicked();
        void on_pushButton_cancel_clicked();
        ///
        /// the button to visit site for acquiring api keys...
        ///
        void on_toolButton_getApiKey_censys_clicked();
        void on_toolButton_getApiKey_virustotal_clicked();
        void on_toolButton_getApiKey_github_clicked();
        void on_toolButton_getApiKey_shodan_clicked();
        void on_toolButton_getApiKey_cloudflare_clicked();
        void on_toolButton_getApiKey_bing_clicked();
        void on_toolButton_getApiKey_google_clicked();
        void on_toolButton_getApiKey_hunter_clicked();
        void on_toolButton_getApiKey_certspotter_clicked();
        void on_toolButton_getApiKey_intelx_clicked();
        void on_toolButton_getApiKey_securityTrails_clicked();
        void on_toolButton_getApiKey_spyse_clicked();
        void on_toolButton_getApiKey_riskiq_clicked();
        void on_toolButton_getApiKey_pentesttools_clicked();
        void on_toolButton_getApiKey_projectdiscovery_clicked();

    private:
        Ui::ApiKeysDialog *ui;
};

#endif // OSINTAPIKEYS_H
