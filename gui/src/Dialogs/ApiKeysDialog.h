#ifndef APIKEYS_H
#define APIKEYS_H

#include <QDialog>

namespace Ui {
    class ApiKeysDialog;
}

class ApiKeysDialog : public QDialog{
    Q_OBJECT

    public:
        explicit ApiKeysDialog(QWidget *parent = nullptr);
        ~ApiKeysDialog();

    private slots:
        void on_buttonSave_clicked();
        void on_buttonCancel_clicked();
        ///
        /// the button to visit site for acquiring api keys...
        ///
        void on_buttonGetCensys_clicked();
        void on_buttonGetVirusTotal_clicked();
        void on_buttonGetGithub_clicked();
        void on_buttonGetShodan_clicked();
        void on_buttonGetCloudflare_clicked();
        void on_buttonGetBing_clicked();
        void on_buttonGetGoogle_clicked();
        void on_buttonGetHunter_clicked();
        void on_buttonGetCertspotter_clicked();
        void on_buttonGetIntelX_clicked();
        void on_buttonGetSecurityTrails_clicked();
        void on_buttonGetSpyse_clicked();
        void on_buttonGetRiskIq_clicked();
        void on_buttonGetPentestTools_clicked();
        void on_buttonGetProjectDiscovery_clicked();

    private:
        Ui::ApiKeysDialog *ui;
        //...
        void loadApiKeys();
};

#endif // OSINTAPIKEYS_H
