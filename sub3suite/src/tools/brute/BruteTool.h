/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :BRUTEFORCE Engine, obtain subdomains & top level domains(TLD) by resolving bruteforced targets.
*/

#ifndef BRUTETOOL_H
#define BRUTETOOL_H

#include <QMenu>
#include <QAction>
#include <QTime>
#include <QElapsedTimer>

#include "../AbstractTool.h"
#include "src/utils/utils.h"
#include "src/modules/active/BruteScanner.h"


namespace Ui {
    class BruteTool;
}

class BruteTool : public AbstractTool{
    Q_OBJECT

    public:
        BruteTool(QWidget *parent = nullptr, ProjectModel *project = nullptr);
        ~BruteTool();

    public slots:
        void onScanThreadEnded();
        void onScanLog(scan::Log log);
        void onWildcard(s3s_struct::Wildcard);
        void onResultSubdomain(s3s_struct::HOST host);
        void onResultTLD(s3s_struct::HOST host);
        void onReScan(QQueue<QString> targets);

        /* receiving targets from other engines */
        void onReceiveTargets(QSet<QString>, RESULT_TYPE);

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
        void on_lineEditFilter_textChanged(const QString &arg1);

    private:
        Ui::BruteTool *ui;
        brute::ScanConfig *m_scanConfig;
        brute::ScanArgs *m_scanArgs;
        brute::ScanStat *m_scanStats;
        QElapsedTimer m_timer;

        QStringListModel *m_wordlistModel;
        QStringListModel *m_targetListModel;

        QStandardItemModel *m_model_subdomain;
        QStandardItemModel *m_model_tld;

        QMap<QString, s3s_item::HOST*> set_subdomain;
        QMap<QString, s3s_item::HOST*> set_tld;
        QMap<QString,QString> m_failedScans;

        void iniUI();
        void initConfigValues();

        void startScan();
        void scanSummary();

        QString targetFilterSubdomain(QString target);
        QString targetFilterTLD(QString target);

        void log(const QString &log);

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
        /* sending results */
        void sendToProject();
        void sendSelectedToProject();
        void sendToEngine(const TOOL&, const RESULT_TYPE&);
        void sendSelectedToEngine(const TOOL&, const RESULT_TYPE&);
        void sendToEnum(const ENUMERATOR&);
        void sendSelectedToEnum(const ENUMERATOR&);
};

#endif // BRUTETOOL_H
