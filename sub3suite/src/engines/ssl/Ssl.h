/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#ifndef SSL_H
#define SSL_H

#include <QWidget>
#include <QElapsedTimer>
#include "../AbstractEngine.h"
#include "src/modules/active/SSLScanner.h"
#include "src/utils/NotesSyntaxHighlighter.h"


namespace Ui {
    class Ssl;
}

class Ssl : public AbstractEngine{
        Q_OBJECT

    public:
        Ssl(QWidget *parent = nullptr, ProjectModel *project = nullptr);
        ~Ssl();

    public slots:
        void onScanThreadEnded();
        void onScanLog(scan::Log log);
        /* scan results */
        void onScanResultSHA1(QString sha1);
        void onScanResultSHA256(QString sha256);
        void onScanResultSubdomain(QString target, QStringList subdomain);
        void onScanResultRaw(QString target, QSslCertificate certificate);
        /* ... */
        void onReScan(QQueue<QString> targets);

        /* receiving targets from other engines */
        void onReceiveTargets(QString, RESULT_TYPE);

    private slots:
        void on_buttonStart_clicked();
        void on_buttonStop_clicked();
        void on_buttonAction_clicked();
        void on_treeViewResults_customContextMenuRequested(const QPoint &pos);
        void on_comboBoxOutput_currentIndexChanged(int index);
        void on_lineEditFilter_textChanged(const QString &arg1);
        void on_lineEditTarget_returnPressed();
        void on_buttonConfig_clicked();

    private:
        Ui::Ssl *ui;
        QElapsedTimer m_timer;
        ssl::ScanConfig *m_scanConfig;
        ssl::ScanArgs *m_scanArgs;
        ssl::ScanStat *m_scanStats;
        QSet<QString> m_subdomainSet;
        QSet<QString> m_certIdSet;
        QSet<QString> m_certInfoSet;
        QMap<QString,QString> m_failedScans;
        QStringListModel *m_targetListModel;
        QStandardItemModel *m_resultModelSubdomain;
        QStandardItemModel *m_resultModelCertId;
        QStandardItemModel *m_resultModelCertInfo;
        QSortFilterProxyModel *m_resultProxyModel;
        NotesSyntaxHighlighter *m_notesSyntaxHighlighter;
        void m_getConfigValues();
        void m_startScan();
        void m_log(QString log);
        void m_scanSummary();

        /* for context menu */
    private:
        void m_initActions();
        /* ... */
        void m_clearResults();
        void m_expandResults();
        void m_collapseResults();
        void m_openInBrowser(QItemSelectionModel*);
        void m_removeResults(QItemSelectionModel*);
        void m_saveResults(RESULT_TYPE);
        void m_saveResults(QItemSelectionModel*);
        void m_copyResults(RESULT_TYPE);
        void m_copyResults(QItemSelectionModel*);
        /* sending results to other parts */
        void m_sendSubdomainToEngine(ENGINE);
        void m_sendCertToEngine(ENGINE);
        void m_sendSubdomainToTool(TOOL);
        void m_sendCertToTool(TOOL);
        void m_sendSubdomainToEngine(ENGINE, QItemSelectionModel*);
        void m_sendCertToEngine(ENGINE, QItemSelectionModel*);
        void m_sendSubdomainToTool(TOOL, QItemSelectionModel*);
        void m_sendCertToTool(TOOL, QItemSelectionModel*);

    protected:
        /* general actions */
        QAction a_RemoveResults{"Remove"};
        QAction a_ClearResults{"Clear Results"};
        QAction a_OpenInBrowser{"Open in Browser"};
        QAction a_ExpandResults{"Expand Results"};
        QAction a_CollapseResults{"Collapse Results"};

        /* for all */
        QAction a_SendAllCertToOsint{"Send Cert To Osint"};
        QAction a_SendAllCertToRaw{"Send Cert To Raw"};
        QAction a_SendAllHostToOsint{"Send Hostnames To Osint"};
        QAction a_SendAllHostToRaw{"Send Hostnames To Raw"};
        QAction a_SendAllHostToBrute{"Send Hostnames To Brute"};
        QAction a_SendAllHostToActive{"Send Hostnames To Active"};
        QAction a_SendAllHostToDns{"Send Hostnames To Records"};
        QAction a_SendAllHostToCert{"Send Hostnames To Cert"};
        QAction a_SendAllCertToCertTool{"Send Cert To IpTool"};
        QAction a_SendAllHostToCertTool{"Send Hostnames To CertTool"};
        QAction a_SendAllHostToDomainTool{"Send Hostnames To DomainTool"};
        /* for selected */
        QAction a_SendSelectedCertToOsint{"Send Cert To Osint"};
        QAction a_SendSelectedCertToRaw{"Send Cert To Raw"};
        QAction a_SendSelectedHostToOsint{"Send Hostnames To Osint"};
        QAction a_SendSelectedHostToRaw{"Send Hostnames To Raw"};
        QAction a_SendSelectedHostToBrute{"Send Hostnames To Brute"};
        QAction a_SendSelectedHostToActive{"Send Hostnames To Active"};
        QAction a_SendSelectedHostToDns{"Send Hostnames To Records"};
        QAction a_SendSelectedHostToCert{"Send Hostnames To Cert"};
        QAction a_SendSelectedCertToCertTool{"Send Cert To IpTool"};
        QAction a_SendSelectedHostToCertTool{"Send Hostnames To CertTool"};
        QAction a_SendSelectedHostToDomainTool{"Send Hostnames To DomainTool"};

        /* save */
        QAction a_Save{"Save"};
        /* copy */
        QAction a_Copy{"Copy"};
};

#endif // SSL_H
