#ifndef DNS_H
#define DNS_H

#include "../AbstractEngine.h"
#include "src/utils/utils.h"
#include "src/modules/scan/DnsRecordsScanner.h"
#include "src/utils/NotesSyntaxHighlighter.h"


namespace Ui {
    class Dns;
}

class Dns : public AbstractEngine{
    Q_OBJECT

    public:
        Dns(QWidget *parent = nullptr,
                   ProjectDataModel *project = nullptr,
                   Status *status = nullptr);
        ~Dns();

    public slots:
        void onScanThreadEnded();
        void onInfoLog(QString log);
        void onErrorLog(QString log);
        void onScanResult(records::Results);

        /* receiving targets from other engines */
        void onReceiveTargets(QString, RESULT_TYPE);

    private slots:
        void on_buttonAction_clicked();
        void on_buttonStart_clicked();
        void on_buttonStop_clicked();
        void on_buttonConfig_clicked();
        void on_treeViewResults_customContextMenuRequested(const QPoint &pos);
        void on_checkBoxSRV_clicked(bool checked);

private:
        Ui::Dns *ui;
        records::ScanConfig *m_scanConfig;
        records::ScanArgs *m_scanArgs;
        QStringListModel *m_targetListModel;
        QStringListModel *m_srvWordlitsModel;
        QStandardItemModel *m_resultModel;
        QSortFilterProxyModel *m_resultProxyModel;
        NotesSyntaxHighlighter *m_notesSyntaxHighlighter;
        void m_loadSrvWordlist();
        /* for scan */
        void m_stopScan();
        void m_startScan();
        void m_pauseScan();
        void m_resumeScan();

    /* for context menu */
    private:
        void m_initActions();
        /* ... */
        void m_openInBrowser(QItemSelectionModel*);
        void m_clearResults();
        void m_expandResults();
        void m_collapseResults();
        void m_removeResults(QItemSelectionModel*);
        void m_saveResultsAll();
        void m_saveResults(RESULT_TYPE);
        void m_saveResults(QItemSelectionModel*);
        void m_copyResultsAll();
        void m_copyResults(RESULT_TYPE);
        void m_copyResults(QItemSelectionModel*);
        /* sending results to other parts */
        void m_sendSubdomainToEngine(ENGINE);
        void m_sendIpToEngine(ENGINE);
        void m_sendSubdomainToTool(TOOL);
        void m_sendIpToTool(TOOL);
        void m_sendNSToTool(TOOL);
        void m_sendMXToTool(TOOL);
        /* ... */
        void m_sendSubdomainToEngine(ENGINE, QItemSelectionModel*);
        void m_sendIpToEngine(ENGINE, QItemSelectionModel*);
        void m_sendSubdomainToTool(TOOL, QItemSelectionModel*);
        void m_sendIpToTool(TOOL, QItemSelectionModel*);
        void m_sendNSToTool(TOOL, QItemSelectionModel*);
        void m_sendMXToTool(TOOL, QItemSelectionModel*);

    protected:
        /* general actions */
        QAction a_RemoveResults{"Remove"};
        QAction a_ClearResults{"Clear Results"};
        QAction a_OpenInBrowser{"Open in Browser"};
        QAction a_ExpandResults{"Expand Results"};
        QAction a_CollapseResults{"Collapse Results"};

        /* action button */
        QAction a_SendIpToIp{"Send Addresses To Ip"};
        QAction a_SendIpToOsint{"Send Addresses To Osint"};
        QAction a_SendIpToRaw{"Send Address To Raw"};
        QAction a_SendHostToOsint{"Send Hostnames To Osint"};
        QAction a_SendHostToRaw{"Send Hostnames To Raw"};
        QAction a_SendHostToBrute{"Send Hostnames To Brute"};
        QAction a_SendHostToActive{"Send Hostnames To Active"};
        QAction a_SendHostToDns{"Send Hostnames To Records"};
        QAction a_SendHostToCert{"Send Hostnames To Cert"};
        QAction a_SendIpToIpTool{"Send Addresses To IpTool"};
        QAction a_SendHostToCertTool{"Send Hostnames To CertTool"};
        QAction a_SendHostToDomainTool{"Send Hostnames To DomainTool"};
        QAction a_SendMXToMXTool{"Send MX to MXTool"};
        QAction a_SendNSToNSTool{"Send NS to NSTool"};
        /* right-click */
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
        QAction a_SendSelectedMXToMXTool{"Send MX to MXTool"};
        QAction a_SendSelectedNSToNSTool{"Send NS to NSTool"};

        /* save */
        QAction a_Save{"Save"};
        QAction a_SaveAll{"All"};
        QAction a_SaveHostnames{"Hostnames"};
        QAction a_SaveIp{"ip-addresses"};
        QAction a_SaveA{"A"};
        QAction a_SaveAAAA{"AAAA"};
        QAction a_SaveCNAME{"CNAME"};
        QAction a_SaveMX{"MX"};
        QAction a_SaveNS{"NS"};
        QAction a_SaveTXT{"TXT"};
        /* copy */
        QAction a_Copy{"Copy"};
        QAction a_CopyAll{"All"};
        QAction a_CopyHostnames{"Hostnames"};
        QAction a_CopyIp{"ip-addresses"};
        QAction a_CopyA{"A"};
        QAction a_CopyAAAA{"AAAA"};
        QAction a_CopyCNAME{"CNAME"};
        QAction a_CopyMX{"MX"};
        QAction a_CopyNS{"NS"};
        QAction a_CopyTXT{"TXT"};
};

#endif // DNS_H
