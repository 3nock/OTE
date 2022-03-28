/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#ifndef RAW_H
#define RAW_H

#include <QMenu>
#include <QAction>
#include <QTextDocument>
#include <QStandardItemModel>

#include "../AbstractEngine.h"
#include "src/utils/utils.h"
#include "src/modules/passive/OsintModulesHeaders.h"
#include "src/utils/JsonSyntaxHighlighter.h"


namespace Ui {
    class Raw;
}

class Raw : public AbstractEngine{
        Q_OBJECT

    public:
        Raw(QWidget *parent = nullptr, ProjectModel *project = nullptr);
        ~Raw();

    signals:
        void stopScanThread();

    public slots:
        void onScanThreadEnded();
        void onScanLog(ScanLog log);

        void onResults(s3s_struct::RAW raw); // for Json results
        void onResultsTxt(s3s_struct::RAW raw); // for normal txt results
        void onReScan(QQueue<QString> targets);

        /* receiving targets from other engines */
        void onReceiveTargets(QSet<QString>, RESULT_TYPE);

    private slots:
        void on_buttonStart_clicked();
        void on_comboBoxOptions_currentIndexChanged(const QString &arg1);
        void on_buttoApiKeys_clicked();
        void on_buttonConfig_clicked();
        void on_buttonActionJson_clicked();
        void on_buttonActionTree_clicked();
        void on_treeViewResults_customContextMenuRequested(const QPoint &pos);
        void on_buttonStop_clicked();
        void on_lineEditTarget_returnPressed();
        void on_lineEditTreeFilter_textChanged(const QString &arg1);
        void on_treeViewResults_doubleClicked(const QModelIndex &index);
        void on_tabWidgetInput_currentChanged(int index);

        /* for Find */
        void on_buttonNext_clicked();
        void on_buttonPrev_clicked();
        void on_lineEditFind_textEdited(const QString &arg1);
        void on_buttonNext_json_clicked();
        void on_buttonPrev_json_clicked();
        void on_lineEditFind_json_textChanged(const QString &arg1);

        /* modules */
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
        void on_modulePassiveTotal_clicked();

        void on_moduleIpfy_clicked();

private:
        Ui::Raw *ui;
        QStandardItemModel *m_model;
        QStringListModel *m_targetListModel;
        QStringListModel *m_targetListModel_host;
        QStringListModel *m_targetListModel_ip;
        QStringListModel *m_targetListModel_asn;
        QStringListModel *m_targetListModel_cidr;
        QStringListModel *m_targetListModel_ssl;
        QStringListModel *m_targetListModel_email;
        QStringListModel *m_targetListModel_url;

        ScanConfig *m_scanConfig;
        ScanArgs *m_scanArgs;
        QMap<QString,QString> m_failedScans;
        QMap<QString, QStringList> m_optionSet;
        JsonSyntaxHighlighter *m_resultsHighlighter = nullptr;
        JsonSyntaxHighlighter *m_jsonHighlighter = nullptr;
        int m_resultsCountJson = 0;

        void initUI();
        void initConfigValues();

        void startScan();
        void startScanThread(AbstractOsintModule *);

        void find(const QString &, QTextDocument::FindFlags);
        void find_json(const QString &, QTextDocument::FindFlags);
        void log(const QString &log);

    /* for context menu */
    private:
        void openInBrowser();
        void clearResults();
        void removeResults();

        void saveResults();
        void saveSelectedResults();

        void copyResults();
        void copySelectedResults();

        void clearResults_txt();
        void saveResults_txt();
        void copyResults_txt();

        void sendToProject();
        void sendSelectedToProject();
        void sendToEngine(const ENGINE&, const RESULT_TYPE&);
        void sendToEnum(const ENUMERATOR&, const RESULT_TYPE&);
};

#endif // Raw_H
