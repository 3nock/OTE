#ifndef DNSRECORDS_H
#define DNSRECORDS_H

#include "../AbstractEngine.h"
#include "src/utils/utils.h"
#include "src/modules/scan/DnsRecordsScanner.h"
#include "src/utils/NotesSyntaxHighlighter.h"


namespace Ui {
    class DnsRecords;
}

class DnsRecords : public AbstractEngine{
    Q_OBJECT

    public:
        DnsRecords(QWidget *parent = nullptr,
                   ResultsModel *resultsModel = nullptr,
                   ProjectDataModel *project = nullptr,
                   Status *status = nullptr);
        ~DnsRecords();

    public slots:
        void onScanThreadEnded();
        void onInfoLog(QString log);
        void onErrorLog(QString log);
        void onScanResult(records::Results);
        /* receiving targets from other engines */
        void onReceiveTargets(ENGINE, RESULT_TYPE, RESULT_MODEL_TYPE);
        void onReceiveTargets(QItemSelectionModel*, RESULT_TYPE, RESULT_MODEL_TYPE);

    private slots:
        void onOpenInBrowser(QItemSelectionModel*);
        void onClearResults();
        void onExpandResults();
        void onCollapseResults();
        void onRemoveResults(QItemSelectionModel*);
        void onSaveResultsAll();
        void onSaveResults(RESULT_TYPE);
        void onSaveResults(QItemSelectionModel*);
        void onCopyResultsAll();
        void onCopyResults(RESULT_TYPE);
        void onCopyResults(QItemSelectionModel*);
        /* ... */
        void on_buttonAction_clicked();
        void on_buttonStart_clicked();
        void on_buttonStop_clicked();
        void on_buttonConfig_clicked();
        void on_comboBoxOption_currentIndexChanged(int index);
        void on_treeViewResults_customContextMenuRequested(const QPoint &pos);
        void on_tableViewSRV_customContextMenuRequested(const QPoint &pos);

    private:
        Ui::DnsRecords *ui;
        records::ScanConfig *m_scanConfig;
        records::ScanArgs *m_scanArgs;
        QStringListModel *m_targetListModel;
        QStringListModel *m_srvWordlitsModel;
        NotesSyntaxHighlighter *m_notesSyntaxHighlighter;
        //...
        void m_stopScan();
        void m_startScan();
        void m_pauseScan();
        void m_resumeScan();
        void m_loadSrvWordlist();
        void m_initActions();

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

#endif // DNSRECORDS_H
