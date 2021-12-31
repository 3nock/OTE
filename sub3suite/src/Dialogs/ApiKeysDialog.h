#ifndef APIKEYS_H
#define APIKEYS_H

#include <QDialog>
#include "src/utils/utils.h"

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

        /* the button to visit site for acquiring api keys... */
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
        void on_buttonGetBinaryEdge_clicked();
        void on_buttonGetC99_clicked();
        void on_buttonGetIpInfo_clicked();
        void on_buttonGetThreatBook_clicked();
        void on_buttonGetWhoisXmlApi_clicked();
        void on_buttonGetZETAlytics_clicked();
        void on_buttonGetZoomEye_clicked();
        void on_buttonGetIpApi_clicked();
        void on_buttonGetViewDns_clicked();
        void on_buttonGetHackerTarget_clicked();
        void on_buttonGetWebResolver_clicked();
        void on_buttonGetCircl_clicked();
        void on_buttonGetMnemonic_clicked();
        void on_buttonGetRobtex_clicked();
        void on_buttonGetOtx_clicked();
        void on_buttonGetBuiltWith_clicked();
        void on_buttonGetDnslytics_clicked();
        void on_buttonGetDomainTools_clicked();
        void on_buttonGetOnyphe_clicked();
        void on_buttonGetFullHunt_clicked();
        void on_buttonGetSpamHaus_clicked();
        void on_buttonGetDebounce_clicked();
        void on_buttonGetHaveIBeenPawned_clicked();
        void on_buttonGetHost_clicked();
        void on_buttonGetJsonWhois_clicked();
        void on_buttonGetLeakIX_clicked();
        void on_buttonGetNetworksDB_clicked();
        void on_buttonGetNeutrinoApi_clicked();
        void on_buttonGetSeon_clicked();
        void on_buttonGetSpyOnWeb_clicked();
        void on_buttonGetWhatcms_clicked();
        void on_buttonGetWhoxy_clicked();
        void on_buttonGetEmailCrawlr_clicked();
        void on_buttonGetEmailFormat_clicked();
        void on_buttonGetEmailRep_clicked();
        void on_buttonGetSnov_clicked();
        void on_buttonGetIpData_clicked();
        void on_buttonGetIpGeoLocation_clicked();
        void on_buttonGetIpRegistry_clicked();
        void on_buttonGetIpStack_clicked();
        void on_buttonGetAbuseIPDB_clicked();
        void on_buttonGetFraudGuard_clicked();
        void on_buttonGetHybridAnalysis_clicked();
        void on_buttonGetIpQualityScore_clicked();
        void on_buttonGetLeakLookup_clicked();

        void on_buttonGetWappalyzer_clicked();

private:
        Ui::ApiKeysDialog *ui;
        void m_loadApiKeys();
};

#endif // OSINTAPIKEYS_H
