#ifndef RAW_H
#define RAW_H

#include <QWidget>
#include <QTextDocument>
#include <QStandardItemModel>
#include "../AbstractEngine.h"
#include "src/utils/utils.h"
#include "src/modules/osint/OsintModulesHeaders.h"
#include "src/utils/JsonSyntaxHighlighter.h"
#include "src/utils/NotesSyntaxHighlighter.h"


namespace Ui {
    class Raw;
}

class Raw : public AbstractEngine{
        Q_OBJECT

    public:
        explicit Raw(QWidget *parent = nullptr, ProjectDataModel *project = nullptr);
        ~Raw();

    signals:
        void stopScanThread();

    public slots:
        void onEnumerationComplete();
        void onRateLimitLog(ScanLog log);
        void onErrorLog(ScanLog log);
        void onInfoLog(ScanLog log);
        //...
        void onResults(QByteArray); // for Json results
        void onResultsTxt(QByteArray); // for normal txt results

        /* receiving targets from other engines */
        void onReceiveTargets(QString, RESULT_TYPE);

    private slots:
        void on_buttonStart_clicked();
        void on_comboBoxOptions_currentIndexChanged(const QString &arg1);
        void on_buttoApiKeys_clicked();
        void on_buttonConfig_clicked();
        void on_buttonActionJson_clicked();
        void on_buttonActionTree_clicked();
        void on_treeViewResults_customContextMenuRequested(const QPoint &pos);

        /* for Find... */
        void on_buttonNext_clicked();
        void on_buttonPrev_clicked();
        void on_lineEditFind_textEdited(const QString &arg1);

        /* modules... */
        void on_moduleCertspotter_clicked();
        void on_moduleOtx_clicked();
        void on_moduleSublist3r_clicked();
        void on_moduleThreatMiner_clicked();
        void on_moduleThreatCrowd_clicked();
        void on_moduleHackerTargetFree_clicked();
        void on_moduleDnsBufferOverRun_clicked();
        void on_moduleAnubis_clicked();
        void on_moduleProjectDiscovery_clicked();
        void on_moduleSpyse_clicked();
        void on_moduleGoogleCert_clicked();
        void on_moduleOmnisint_clicked();
        void on_moduleRobtexPaid_clicked();
        void on_moduleUrlScan_clicked();
        void on_moduleVirusTotal_clicked();
        void on_moduleWaybackMachine_clicked();
        void on_moduleArchiveIt_clicked();
        void on_moduleBgpview_clicked();
        void on_moduleBinaryEdge_clicked();
        void on_moduleC99_clicked();
        void on_moduleGithub_clicked();
        void on_moduleHunter_clicked();
        void on_moduleIpInfo_clicked();
        void on_moduleMnemonicFree_clicked();
        void on_moduleRiskIq_clicked();
        void on_moduleRobtex_clicked();
        void on_moduleSecurityTrails_clicked();
        void on_moduleShodan_clicked();
        void on_moduleThreatBook_clicked();
        void on_moduleWhoisXmlApi_clicked();
        void on_moduleZETALytics_clicked();
        void on_moduleZoomEye_clicked();
        void on_moduleIpApi_clicked();
        void on_moduleViewDns_clicked();
        void on_moduleWebResolver_clicked();
        void on_moduleCircl_clicked();
        void on_moduleHackerTarget_clicked();
        void on_moduleMnemonic_clicked();
        void on_moduleArin_clicked();
        void on_moduleBuiltWith_clicked();
        void on_moduleDnslytics_clicked();
        void on_moduleDomainTools_clicked();
        void on_moduleMaltiverseFree_clicked();
        void on_moduleN45HT_clicked();
        void on_moduleOnyphe_clicked();
        void on_moduleRipe_clicked();
        void on_moduleAfrinic_clicked();
        void on_moduleApnic_clicked();
        void on_moduleFullHunt_clicked();
        void on_moduleSpamHaus_clicked();
        void on_moduleDebounce_clicked();
        void on_moduleHaveIBeenPawned_clicked();
        void on_moduleHost_clicked();
        void on_moduleJsonWhois_clicked();
        void on_moduleLeakIX_clicked();
        void on_moduleNetworksDB_clicked();
        void on_moduleNeutrinoApi_clicked();
        void on_moduleSeon_clicked();
        void on_moduleSpyOnWeb_clicked();
        void on_moduleWhatcms_clicked();
        void on_moduleWhoxy_clicked();
        void on_moduleArquivo_clicked();
        void on_moduleUKWebArchive_clicked();
        void on_moduleCertspotterFree_clicked();
        void on_moduleCensys_clicked();
        void on_moduleEmailCrawlr_clicked();
        void on_moduleEmailFormat_clicked();
        void on_moduleEmailRep_clicked();
        void on_moduleSnov_clicked();
        void on_moduleTruMail_clicked();
        void on_moduleIpData_clicked();
        void on_moduleIpGeoLocation_clicked();
        void on_moduleIpRegistry_clicked();
        void on_moduleIpStack_clicked();
        void on_moduleAbuseIPDB_clicked();
        void on_moduleBotScout_clicked();
        void on_moduleFraudGuard_clicked();
        void on_moduleHybridAnalysis_clicked();
        void on_moduleIpQualityScore_clicked();
        void on_moduleLeakLookup_clicked();

        void on_moduleWappalyzer_clicked();

private:
        Ui::Raw *ui;
        QStandardItemModel *m_model;
        //...
        QStringListModel *m_targetListModel;
        QStringListModel *m_targetListModelHostname;
        QStringListModel *m_targetListModelIp;
        QStringListModel *m_targetListModelAsn;
        QStringListModel *m_targetListModelCidr;
        QStringListModel *m_targetListModelCert;
        QStringListModel *m_targetListModelEmail;
        //...
        QMap<QString, QStringList> m_optionSet;
        JsonSyntaxHighlighter *m_jsonSyntaxHighlighter;
        NotesSyntaxHighlighter *m_notesSyntaxHighlighter;
        //...
        int m_resultsCountJson = 0;
        int m_resultsCountTree = 0;
        //...
        QString m_currentModule;
        QString m_currentTarget;
        //...
        void m_startScan();
        void m_pauseScan();
        void m_resumeScan();
        void m_stopScan();
        void m_startScanThread(AbstractOsintModule *);
        //...
        void setJsonText(QJsonDocument &document);
        void setJsonTree(QJsonDocument &document);
        void treeObject(QJsonObject object, QStandardItem *item);
        void treeArray(QJsonArray array, QStandardItem *item);
        //...
        void find(QString, QTextDocument::FindFlags);
        //...
        void m_errorLog(QString log);
        void m_infoLog(QString log);

    /* for context menu */
    private:
        void m_initActions();
        /* ... */
        void m_openInBrowser(QItemSelectionModel*);
        void m_clearResults();
        void m_expandResults();
        void m_collapseResults();
        void m_removeResults(QItemSelectionModel*);
        void m_saveResultsJson();
        void m_saveResults(RESULT_TYPE);
        void m_saveResults(QItemSelectionModel*);
        void m_copyResultsJson();
        void m_copyResults(RESULT_TYPE);
        void m_copyResults(QItemSelectionModel*);
        /* sending results to other parts */
        void m_sendToEngine(ENGINE); // for asn, cidr, email, cert
        void m_sendSubdomainToEngine(ENGINE);
        void m_sendIpToEngine(ENGINE);
        void m_sendSubdomainToTool(TOOL);
        void m_sendIpToTool(TOOL);
        void m_sendToTool(TOOL);

        void m_sendToEngine(ENGINE, QItemSelectionModel*);
        void m_sendSubdomainToEngine(ENGINE, QItemSelectionModel*);
        void m_sendIpToEngine(ENGINE, QItemSelectionModel*);
        void m_sendSubdomainToTool(TOOL, QItemSelectionModel*);
        void m_sendIpToTool(TOOL, QItemSelectionModel*);
        void m_sendToTool(TOOL, QItemSelectionModel*);

    protected:
        /* general actions */
        QAction a_RemoveResults{"Remove"};
        QAction a_ClearResults{"Clear Results"};
        QAction a_OpenInBrowser{"Open in Browser"};
        QAction a_ExpandResults{"Expand Results"};
        QAction a_CollapseResults{"Collapse Results"};

        /* for all */
        QAction a_SendAllIpToIp{"Send Addresses To Ip"};
        QAction a_SendAllIpToOsint{"Send Addresses To Osint"};
        QAction a_SendAllIpToRaw{"Send Address To Raw"};
        QAction a_SendAllHostToOsint{"Send Hostnames To Osint"};
        QAction a_SendAllHostToRaw{"Send Hostnames To Raw"};
        QAction a_SendAllHostToBrute{"Send Hostnames To Brute"};
        QAction a_SendAllHostToActive{"Send Hostnames To Active"};
        QAction a_SendAllHostToDns{"Send Hostnames To Records"};
        QAction a_SendAllHostToCert{"Send Hostnames To Cert"};
        QAction a_SendAllEmailToRaw{"Send Emails To Raw"};
        QAction a_SendAllUrlToRaw{"Send Urls To Raw"};
        QAction a_SendAllAsnToRaw{"Send ASNs To Raw"};
        QAction a_SendAllCertToRaw{"Send Cert To Raw"};
        QAction a_SendAllCidrToRaw{"Send Cidr To Raw"};
        QAction a_SendAllEmailToOsint{"Send Emails To Osint"};
        QAction a_SendAllAsnToOsint{"Send ASNs To Osint"};
        QAction a_SendAllCertToOsint{"Send Cert To Osint"};
        QAction a_SendAllCidrToOsint{"Send Cidr To Osint"};
        QAction a_SendAllIpToIpTool{"Send Addresses To IpTool"};
        QAction a_SendAllHostToCertTool{"Send Hostnames To CertTool"};
        QAction a_SendAllHostToDomainTool{"Send Hostnames To DomainTool"};
        QAction a_SendAllAsnToAsnTool{"Send ASN to ASNTool"};
        QAction a_SendAllCertToCertTool{"Send Cert To CertTool"};
        QAction a_SendAllCidrToCidrTool{"Send Cidr To CidrTool"};
        QAction a_SendAllEmailToEmailTool{"Send Emails To EmailTool"};
        /* for selected */
        QAction a_SendSelectedIpToIp{"Send Addresses To Ip"};
        QAction a_SendSelectedIpToOsint{"Send Addresses To Osint"};
        QAction a_SendSelectedIpToRaw{"Send Address To Raw"};
        QAction a_SendSelectedHostToOsint{"Send Hostnames To Osint"};
        QAction a_SendSelectedHostToRaw{"Send Hostnames To Raw"};
        QAction a_SendSelectedHostToBrute{"Send Hostnames To Brute"};
        QAction a_SendSelectedHostToActive{"Send Hostnames To Active"};
        QAction a_SendSelectedHostToDns{"Send Hostnames To Records"};
        QAction a_SendSelectedHostToCert{"Send Hostnames To Cert"};
        QAction a_SendSelectedEmailToRaw{"Send Emails To Raw"};
        QAction a_SendSelectedUrlToRaw{"Send Urls To Raw"};
        QAction a_SendSelectedAsnToRaw{"Send ASNs To Raw"};
        QAction a_SendSelectedCertToRaw{"Send Cert To Raw"};
        QAction a_SendSelectedCidrToRaw{"Send Cidr To Raw"};
        QAction a_SendSelectedEmailToOsint{"Send Emails To Osint"};
        QAction a_SendSelectedAsnToOsint{"Send ASNs To Osint"};
        QAction a_SendSelectedCertToOsint{"Send Cert To Osint"};
        QAction a_SendSelectedCidrToOsint{"Send Cidr To Osint"};
        QAction a_SendSelectedIpToIpTool{"Send Addresses To IpTool"};
        QAction a_SendSelectedHostToCertTool{"Send Hostnames To CertTool"};
        QAction a_SendSelectedHostToDomainTool{"Send Hostnames To DomainTool"};
        QAction a_SendSelectedAsnToAsnTool{"Send ASN to ASNTool"};
        QAction a_SendSelectedCertToCertTool{"Send Cert To CertTool"};
        QAction a_SendSelectedCidrToCidrTool{"Send Cidr To CidrTool"};
        QAction a_SendSelectedEmailToEmailTool{"Send Emails To EmailTool"};

        /* save */
        QAction a_Save{"Save"};
        QAction a_SaveJson{"Save as Json"};
        /* copy */
        QAction a_Copy{"Copy"};
        QAction a_CopyJson{"Copy as Json"};
};

#endif // Raw_H
