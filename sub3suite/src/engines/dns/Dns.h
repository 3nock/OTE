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
        void onScanResult_srv(s3s_struct::DNS result);
        void onReScan(QQueue<QString> targets);

        /* receiving targets from other engines */
        void onReceiveTargets(QSet<QString>, RESULT_TYPE);

    private slots:
        void on_buttonAction_clicked();
        void on_buttonStart_clicked();
        void on_buttonStop_clicked();
        void on_buttonConfig_clicked();
        void on_treeViewResults_customContextMenuRequested(const QPoint &pos);
        void on_checkBoxSRV_clicked(bool checked);
        void on_lineEditTarget_returnPressed();
        void on_lineEditFilter_textChanged(const QString &arg1);

        void on_checkBoxANY_toggled(bool checked);
        void on_checkBoxSRV_toggled(bool checked);

    private:
        Ui::Dns *ui;
        dns::ScanConfig *m_scanConfig;
        dns::ScanArgs *m_scanArgs;
        dns::ScanStat *m_scanStats;
        QStringListModel *m_targetListModel;
        QStringListModel *m_srvWordlitsModel;
        QStandardItemModel *m_model;
        QMap<QString, s3s_item::DNS*> m_resultSet;
        QMap<QString, QString> m_failedScans;
        QElapsedTimer m_timer;

        void initUI();
        void initConfigValues();
        void initSrvWordlist();

        void startScan();
        void scanSummary();

        void log(QString log);

    /* for context menu */
    private:
        void openInBrowser();
        void clearResults();
        void expandResults();
        void collapseResults();
        void removeResults();
        void saveResults(const RESULT_TYPE&);
        void saveSelectedResults();
        void copyResults(const RESULT_TYPE&);
        void copySelectedResults();
        /* sending target */
        void sendToProject();
        void sendSelectedToProject();
        void sendToEngine(const ENGINE&, const RESULT_TYPE&);
        void sendSelectedToEngine(const ENGINE&, const RESULT_TYPE&);
        void sendToEnum(const TOOL&);
        void sendSelectedToEnum(const TOOL&);
};

#endif // DNS_H
