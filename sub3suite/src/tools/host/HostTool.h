/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#ifndef HOSTTOOL_H
#define HOSTTOOL_H

#include <QSet>
#include <QMenu>
#include <QAction>
#include <QElapsedTimer>

#include "../AbstractTool.h"
#include "src/utils/utils.h"
#include "src/modules/active/PortScanner.h"
#include "src/modules/active/PingScanner.h"
#include "src/modules/active/HostScanner.h"


namespace Ui {
    class HostTool;
}

class HostTool : public AbstractTool{
        Q_OBJECT

    public:
        HostTool(QWidget *parent = nullptr, ProjectModel *project = nullptr);
        ~HostTool();

    public slots:
        void onScanThreadEnded();
        void onScanLog(scan::Log log);
        void onScanResult_dns(s3s_struct::HOST host);
        void onScanResult_ping(QString host, QString ip, unsigned long time);
        void onScanResult_port(QString hostname, QString ip, u_short port);
        void onReScan(QQueue<QString> targets);

        /* receiving targets from other engines */
        void onReceiveTargets(QSet<QString>, RESULT_TYPE);

    private slots:
        void on_buttonStart_clicked();
        void on_buttonStop_clicked();
        void on_buttonAction_clicked();
        void on_buttonConfig_clicked();
        void on_tableViewResults_customContextMenuRequested(const QPoint &pos);
        void on_lineEditTarget_returnPressed();
        void on_lineEditFilter_textChanged(const QString &arg1);
        void on_comboBoxOption_currentIndexChanged(int index);

    private:
        Ui::HostTool *ui;
        QElapsedTimer m_timer;
        QMap<QString,QString> m_failedScans;
        QMap<QString, s3s_item::HOST*> set_subdomain;
        QMap<QString, s3s_item::IPTool*> set_ports;
        host::ScanConfig *m_scanConfig;
        host::ScanArgs *m_scanArgs;
        host::ScanStat *m_scanStats;
        port::ScanArgs *m_portscannerArgs;
        ping::ScanArgs *m_pingscannerArgs;
        QStringListModel *m_targetListModel;
        QStandardItemModel *m_model_dns;
        QStandardItemModel *m_model_port;
        QStandardItemModel *m_model_ping;

        void initUI();

        void startScan();
        void scanSummary();

        void log(QString log);

    /* for context menu */
    private:
        void openInBrowser();
        void clearResults();
        void removeResults();
        void saveResults(const RESULT_TYPE&);
        void saveSelectedResults();
        void copyResults(const RESULT_TYPE&);
        void copySelectedResults();
        /* extracting subdomain names */
        void extract(bool subdomain, bool tld);
        void extractSelected(bool subdomain, bool tld);
        /* sending results to other parts */
        void sendToProject();
        void sendSelectedToProject();
        void sendToEngine(const TOOL&, const RESULT_TYPE&);
        void sendSelectedToEngine(const TOOL&, const RESULT_TYPE&);
        void sendToEnum(const ENUMERATOR&);
        void sendSelectedToEnum(const ENUMERATOR&);
};

#endif // HOSTTOOL_H
