#ifndef BRUTE_H
#define BRUTE_H

#include "../AbstractEngine.h"
#include "src/utils/utils.h"
#include "src/modules/scan/BruteScanner.h"
#include "src/utils/NotesSyntaxHighlighter.h"


namespace Ui {
    class Brute;
}

class Brute : public AbstractEngine{
    Q_OBJECT

    public:
        Brute(QWidget *parent = nullptr, ProjectDataModel *project = nullptr);
        ~Brute();

    public slots:
        void onScanThreadEnded();
        void onScanLog(brute::ScanLog log);
        void onResultSubdomain(QString subdomain, QString ip);
        void onResultTLD(QString tld, QString ip);

        /* receiving targets from other engines */
        void onReceiveTargets(QString, RESULT_TYPE);

    private slots:
        void on_buttonStart_clicked();
        void on_buttonStop_clicked();
        void on_buttonAction_clicked();
        void on_buttonConfig_clicked();
        void on_buttonWordlist_clicked();
        void on_lineEditTarget_returnPressed();
        void on_tableViewResults_customContextMenuRequested(const QPoint &pos);
        void on_checkBoxMultipleTargets_stateChanged(int arg1);
        void on_comboBoxOutput_currentIndexChanged(int index);

    private:
        Ui::Brute *ui;

        QSet<QString> m_subdomainSet;
        QSet<QString> m_tldSet;

        brute::ScanConfig *m_scanConfig;
        brute::ScanArgs *m_scanArgs;

        QStringListModel *m_wordlistModel;
        QStringListModel *m_targetListModel;

        QStandardItemModel *m_resultModelSubdomain;
        QStandardItemModel *m_resultModelTld;
        QSortFilterProxyModel *m_resultProxyModel;

        NotesSyntaxHighlighter *m_notesSyntaxHighlighter;
        /* ... */
        void m_log(QString log);
        void m_getConfigValues();
        /* for scan */
        void m_stopScan();
        void m_startScan();
        void m_pauseScan();
        void m_resumeScan();


        /* ... */
        QString targetFilterSubdomain(QString target);
        QString targetFilterTLD(QString target);

        /* for context menu */
    private:
        void m_initActions();
        /* ... */
        void m_openInBrowser(QItemSelectionModel*);
        void m_clearResults();
        void m_removeResults(QItemSelectionModel*);
        void m_saveResults(RESULT_TYPE);
        void m_saveResults(QItemSelectionModel*);
        void m_copyResults(RESULT_TYPE);
        void m_copyResults(QItemSelectionModel*);
        /* sending results to other parts */
        void m_sendSubdomainToEngine(ENGINE);
        void m_sendIpToEngine(ENGINE);
        void m_sendSubdomainToTool(TOOL);
        void m_sendIpToTool(TOOL);
        void m_sendSubdomainToEngine(ENGINE, QItemSelectionModel*);
        void m_sendIpToEngine(ENGINE, QItemSelectionModel*);
        void m_sendSubdomainToTool(TOOL, QItemSelectionModel*);
        void m_sendIpToTool(TOOL, QItemSelectionModel*);

    protected:
        /* general actions */
        QAction a_RemoveResults{"Remove"};
        QAction a_ClearResults{"Clear Results"};
        QAction a_OpenInBrowser{"Open in Browser"};

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
        QAction a_SendAllIpToIpTool{"Send Addresses To IpTool"};
        QAction a_SendAllHostToCertTool{"Send Hostnames To CertTool"};
        QAction a_SendAllHostToDomainTool{"Send Hostnames To DomainTool"};
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
        QAction a_SendSelectedIpToIpTool{"Send Addresses To IpTool"};
        QAction a_SendSelectedHostToCertTool{"Send Hostnames To CertTool"};
        QAction a_SendSelectedHostToDomainTool{"Send Hostnames To DomainTool"};

        /* save */
        QAction a_Save{"Save"};
        QAction a_SaveSubdomainIp{"subdomain | ip"};
        QAction a_SaveSubdomain{"subdomains"};
        QAction a_SaveIp{"ip-addresses"};
        /* copy */
        QAction a_Copy{"Copy"};
        QAction a_CopySubdomainIp{"subdomain | ip"};
        QAction a_CopySubdomain{"subdomains"};
        QAction a_CopyIp{"ip-addresses"};
};

#endif // BRUTE_H
