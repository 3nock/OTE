/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#ifndef ACTIVE_H
#define ACTIVE_H

#include <QElapsedTimer>
#include "../AbstractEngine.h"
#include "src/utils/utils.h"
#include "src/modules/active/ActiveScanner.h"
#include "src/utils/NotesSyntaxHighlighter.h"

namespace Ui {
    class Active;
}

class Active : public AbstractEngine{
        Q_OBJECT

    public:
        Active(QWidget *parent = nullptr, ProjectDataModel *project = nullptr);
        ~Active();

    public slots:
        void onScanThreadEnded();
        void onScanLog(scan::Log log);
        void onScanResult(QString host, QString ip);
        void onReScan(QQueue<QString> targets);

        /* receiving targets from other engines */
        void onReceiveTargets(QString, RESULT_TYPE);

    private slots:
        void on_buttonStart_clicked();
        void on_buttonStop_clicked();
        void on_buttonAction_clicked();
        void on_buttonConfig_clicked();
        void on_tableViewResults_customContextMenuRequested(const QPoint &pos);
        void on_lineEditTarget_returnPressed();
        void on_lineEditFilter_textChanged(const QString &arg1);

    private:
        Ui::Active *ui;
        QElapsedTimer m_timer;
        QSet<QString> m_activeDns;
        QMap<QString,QString> m_failedScans;
        active::ScanConfig *m_scanConfig;
        active::ScanArgs *m_scanArgs;
        active::ScanStat *m_scanStats;
        QStringListModel *m_targetListModel;
        QStandardItemModel *m_resultModel;
        QSortFilterProxyModel *m_resultProxyModel;
        void m_getConfigValues();
        void m_startScan();
        void m_log(QString log);
        void m_scanSummary();

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
        /* extracting subdomain names */
        void m_extract();
        void m_extract(QItemSelectionModel*);
        /* sending results to other parts */
        void m_sendToProject();
        void m_sendSubdomainToEngine(ENGINE);
        void m_sendIpToEngine(ENGINE);
        void m_sendSubdomainToTool(TOOL);
        void m_sendIpToTool(TOOL);
        void m_sendToProject(QItemSelectionModel*);
        void m_sendSubdomainToEngine(ENGINE, QItemSelectionModel*);
        void m_sendIpToEngine(ENGINE, QItemSelectionModel*);
        void m_sendSubdomainToTool(TOOL, QItemSelectionModel*);
        void m_sendIpToTool(TOOL, QItemSelectionModel*);

    protected:
        /* general actions */
        QAction a_RemoveResults{"Remove"};
        QAction a_ClearResults{"Clear Results"};
        QAction a_OpenInBrowser{"Open in Browser"};

        /* subdomain name extraction */
        QAction a_ExtractAll{"Extract Subdomain Name"};
        QAction a_ExtractSelected{"Extract Subdomain Name"};

        /* for all */
        QAction a_SendAllToProject{"Send To Project"};
        QAction a_SendAllIpToIp{"Send IpAddress To IP"};
        QAction a_SendAllIpToOsint{"Send IpAddress To OSINT"};
        QAction a_SendAllIpToRaw{"Send IpAddress To RAW"};
        QAction a_SendAllHostToOsint{"Send Hostname To OSINT"};
        QAction a_SendAllHostToRaw{"Send Hostname To RAW"};
        QAction a_SendAllHostToBrute{"Send Hostname To BRUTE"};
        QAction a_SendAllHostToActive{"Send Hostname To ACTIVE"};
        QAction a_SendAllHostToDNS{"Send Hostname To DNS"};
        QAction a_SendAllHostToSSL{"Send Hostname To SSL"};
        QAction a_SendAllIpToIPTool{"Send IpAddress To IPTool"};
        QAction a_SendAllHostToSSLTool{"Send Hostname To SSLTool"};
        QAction a_SendAllHostToDomainTool{"Send Hostname To DomainTool"};
        /* for selected */
        QAction a_SendSelectedToProject{"Send To Project"};
        QAction a_SendSelectedIpToIp{"Send IpAddress To IP"};
        QAction a_SendSelectedIpToOsint{"Send IpAddress To OSINT"};
        QAction a_SendSelectedIpToRaw{"Send Address To RAW"};
        QAction a_SendSelectedHostToOsint{"Send Hostname To OSINT"};
        QAction a_SendSelectedHostToRaw{"Send Hostname To RAW"};
        QAction a_SendSelectedHostToBrute{"Send Hostname To BRUTE"};
        QAction a_SendSelectedHostToActive{"Send Hostname To ACTIVE"};
        QAction a_SendSelectedHostToDNS{"Send Hostname To DNS"};
        QAction a_SendSelectedHostToSSL{"Send Hostname To SSL"};
        QAction a_SendSelectedIpToIPTool{"Send IpAddress To IPTool"};
        QAction a_SendSelectedHostToSSLTool{"Send Hostname To SSLTool"};
        QAction a_SendSelectedHostToDomainTool{"Send Hostname To DomainTool"};

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

#endif // ACTIVE_H
