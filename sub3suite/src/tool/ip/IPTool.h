/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#ifndef IPTOOL_H
#define IPTOOL_H

#include <QSet>
#include <QMenu>
#include <QAction>
#include <QElapsedTimer>

#include "../AbstractTool.h"
#include "src/utils/utils.h"
#include "src/modules/active/ReverseIPScanner.h"
#include "src/modules/active/PortScanner.h"
#include "src/modules/active/PingScanner.h"


namespace Ui {
    class IPTool;
}

class IPTool : public AbstractTool{
        Q_OBJECT

    public:
        IPTool(QWidget *parent = nullptr, ProjectModel *project = nullptr);
        ~IPTool();

    public slots:
        void onScanThreadEnded();
        void onScanLog(scan::Log log);
        void onScanResult_dns(QString ip, QString hostname);
        void onScanResult_port(QString ip, u_short port);
        void onScanResult_ping(QString ip, unsigned long time);
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
        Ui::IPTool *ui;
        QElapsedTimer m_timer;
        QMap<QString,QString> m_failedScans;
        QMap<QString, s3s_item::IPTool*> set_ports;
        reverseip::ScanConfig *m_scanConfig;
        reverseip::ScanArgs *m_scanArgs;
        reverseip::ScanStat *m_scanStats;
        port::ScanArgs *m_portscannerArgs;
        ping::ScanArgs *m_pingscannerArgs;
        QStringListModel *m_targetListModel;
        QStandardItemModel *m_model_dns;
        QStandardItemModel *m_model_port;
        QStandardItemModel *m_model_ping;

        void initUI();
        void initConfigValues();

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
        /* sending results to other parts */
        void sendToProject();
        void sendSelectedToProject();
        void sendToEngine(const TOOL&, const RESULT_TYPE&);
        void sendSelectedToEngine(const TOOL&, const RESULT_TYPE&);
        void sendToEnum(const ENUMERATOR&);
        void sendSelectedToEnum(const ENUMERATOR&);
};

#endif // IPTOOL_H
