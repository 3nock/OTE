/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#ifndef DNSTOOL_H
#define DNSTOOL_H

#include <QMenu>
#include <QAction>
#include <QMap>
#include <QQueue>
#include <QElapsedTimer>

#include "../AbstractTool.h"
#include "src/utils/utils.h"
#include "src/modules/active/DNSScanner.h"


namespace Ui {
    class DNSTool;
}

class DNSTool : public AbstractTool{
    Q_OBJECT

    public:
        DNSTool(QWidget *parent = nullptr, ProjectModel *project = nullptr);
        ~DNSTool();

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
        void on_checkBoxA_clicked(bool checked);
        void on_checkBoxAAAA_clicked(bool checked);
        void on_checkBoxMX_clicked(bool checked);
        void on_checkBoxNS_clicked(bool checked);
        void on_checkBoxCNAME_clicked(bool checked);
        void on_checkBoxTXT_clicked(bool checked);

    private:
        Ui::DNSTool *ui;
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
        void sendToEngine(const TOOL&, const RESULT_TYPE&);
        void sendSelectedToEngine(const TOOL&, const RESULT_TYPE&);
        void sendToEnum(const ENUMERATOR&);
        void sendSelectedToEnum(const ENUMERATOR&);
};

#endif // DNSTOOL_H
