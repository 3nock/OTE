#ifndef RAW_H
#define RAW_H

#include <QWidget>
#include <QTextDocument>
#include <QStandardItemModel>
#include "../AbstractEngine.h"
#include "src/utils/utils.h"
#include "src/modules/osint/OsintModulesHeaders.h"
#include "src/utils/JsonSyntaxHighlighter.h"


namespace Ui {
    class Raw;
}

class Raw : public AbstractEngine{
        Q_OBJECT

    public:
        explicit Raw(QWidget *parent = nullptr,
                     ResultsModel *resultsModel = nullptr,
                     ProjectDataModel *project = nullptr,
                     Status *status = nullptr);
        ~Raw();

    public slots:
        void onEnumerationComplete();
        void onErrorLog(QString log);
        void onInfoLog(QString log);
        void onResults(QByteArray); // for Json results
        void onResultsTxt(QByteArray); // for normal txt results

    signals:
        void stopScanThread();

    private slots:
        void on_buttonStart_clicked();
        void on_comboBoxOptions_currentIndexChanged(const QString &arg1);
        ///
        /// for Find...
        ///
        void on_buttonNext_clicked();
        void on_buttonPrev_clicked();
        void on_lineEditFind_textEdited(const QString &arg1);
        ///
        /// modules...
        ///
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

    private:
        Ui::Raw *ui;
        QStandardItemModel *m_model;
        ScanArgs *m_scanArgs;
        //...
        QMap<QString, QStringList> m_optionSet;
        JsonSyntaxHighlighter *syntaxHighlighter;
        //...
        int m_resultsCountJson = 0;
        int m_resultsCountTree = 0;
        //...
        void onCopy();
        void onSave();
        void onClear();
        //...
        void startScan();
        void startScanThread(AbstractOsintModule *);
        //...
        void setJsonText(QJsonDocument &document);
        void setJsonTree(QJsonDocument &document);
        void treeObject(QJsonObject object, QStandardItem *item);
        void treeArray(QJsonArray array, QStandardItem *item);
        //...
        void find(QString, QTextDocument::FindFlags);
};

#endif // Raw_H
