/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#ifndef DNS_H
#define DNS_H

#include <QMenu>
#include <QAction>
#include <QMap>
#include <QQueue>
#include <QElapsedTimer>

#include "../AbstractEngine.h"
#include "src/utils/utils.h"
#include "src/modules/active/DNSScanner.h"
#include "src/utils/NotesSyntaxHighlighter.h"


namespace Ui {
    class Dns;
}

class Dns : public AbstractEngine{
    Q_OBJECT

    public:
        Dns(QWidget *parent = nullptr, ProjectModel *project = nullptr);
        ~Dns();

    public slots:
        void onScanThreadEnded();
        void onScanLog(scan::Log log);
        void onScanResult(s3s_struct::DNS result);
        void onReScan(QQueue<QString> targets);

        /* receiving targets from other engines */
        void onReceiveTargets(QString, RESULT_TYPE);

    private slots:
        void on_buttonAction_clicked();
        void on_buttonStart_clicked();
        void on_buttonStop_clicked();
        void on_buttonConfig_clicked();
        void on_treeViewResults_customContextMenuRequested(const QPoint &pos);
        void on_checkBoxSRV_clicked(bool checked);
        void on_lineEditTarget_returnPressed();
        void on_lineEditFilter_textChanged(const QString &arg1);

    private:
        Ui::Dns *ui;
        dns::ScanConfig *m_scanConfig;
        dns::ScanArgs *m_scanArgs;
        dns::ScanStat *m_scanStats;
        QStringListModel *m_targetListModel;
        QStringListModel *m_srvWordlitsModel;
        QStandardItemModel *m_resultModel;
        QSortFilterProxyModel *m_resultProxyModel;
        QMap<QString, s3s_item::DNS*> m_resultSet;
        QMap<QString, QString> m_failedScans; // target, error
        QElapsedTimer m_timer;
        void m_getConfigValues();
        void m_loadSrvWordlist();
        void m_startScan();
        void m_scanSummary();
        void m_log(QString log);

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
        QAction a_SendIpToOsint{"Send IpAddress To OSINT"};
        QAction a_SendIpToRaw{"Send IpAddress To RAW"};
        QAction a_SendHostToOsint{"Send Hostname To OSINT"};
        QAction a_SendHostToRaw{"Send Hostname To RAW"};
        QAction a_SendHostToBrute{"Send Hostname To BRUTE"};
        QAction a_SendHostToActive{"Send Hostname To ACTIVE"};
        QAction a_SendHostToDns{"Send Hostname To DNS"};
        QAction a_SendHostToCert{"Send Hostname To SSL"};
        QAction a_SendIpToIpTool{"Send IpAddress To IPTool"};
        QAction a_SendHostToCertTool{"Send Hostname To SSLTool"};
        QAction a_SendHostToDomainTool{"Send Hostname To DomainTool"};
        QAction a_SendMXToMXTool{"Send MX to MXTool"};
        QAction a_SendNSToNSTool{"Send NS to NSTool"};
        /* right-click */
        QAction a_SendSelectedIpToOsint{"Send IpAddress To OSINT"};
        QAction a_SendSelectedIpToRaw{"Send IpAddress To RAW"};
        QAction a_SendSelectedHostToOsint{"Send Hostname To OSINT"};
        QAction a_SendSelectedHostToRaw{"Send Hostname To RAW"};
        QAction a_SendSelectedHostToBrute{"Send Hostname To BRUTE"};
        QAction a_SendSelectedHostToActive{"Send Hostname To ACTIVE"};
        QAction a_SendSelectedHostToDns{"Send Hostname To DNS"};
        QAction a_SendSelectedHostToCert{"Send Hostname To SSL"};
        QAction a_SendSelectedIpToIpTool{"Send IpAddress To IPTool"};
        QAction a_SendSelectedHostToCertTool{"Send Hostname To SSLTool"};
        QAction a_SendSelectedHostToDomainTool{"Send Hostname To DomainTool"};
        QAction a_SendSelectedMXToMXTool{"Send MX to MXTool"};
        QAction a_SendSelectedNSToNSTool{"Send NS to NSTool"};

        /* save */
        QAction a_Save{"Save"};
        QAction a_SaveAll{"All"};
        QAction a_SaveHostnames{"Hostname"};
        QAction a_SaveIp{"IpAddress"};
        QAction a_SaveA{"A"};
        QAction a_SaveAAAA{"AAAA"};
        QAction a_SaveCNAME{"CNAME"};
        QAction a_SaveMX{"MX"};
        QAction a_SaveNS{"NS"};
        QAction a_SaveTXT{"TXT"};
        /* copy */
        QAction a_Copy{"Copy"};
        QAction a_CopyAll{"All"};
        QAction a_CopyHostnames{"Hostname"};
        QAction a_CopyIp{"IpAddress"};
        QAction a_CopyA{"A"};
        QAction a_CopyAAAA{"AAAA"};
        QAction a_CopyCNAME{"CNAME"};
        QAction a_CopyMX{"MX"};
        QAction a_CopyNS{"NS"};
        QAction a_CopyTXT{"TXT"};
};

#endif // DNS_H
