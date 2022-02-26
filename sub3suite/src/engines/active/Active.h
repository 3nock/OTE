/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#ifndef ACTIVE_H
#define ACTIVE_H

#include <QMenu>
#include <QAction>
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
        Active(QWidget *parent = nullptr, ProjectModel *project = nullptr);
        ~Active();

    public slots:
        void onScanThreadEnded();
        void onScanLog(scan::Log log);
        void onScanResult_dns(s3s_struct::HOST host);
        void onScanResult_port(s3s_struct::HOST host);
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

        void on_comboBoxOption_currentIndexChanged(int index);

private:
        Ui::Active *ui;
        QElapsedTimer m_timer;
        QMap<QString,QString> m_failedScans;
        QMap<QString, s3s_item::HOST*> set_subdomain;
        active::ScanConfig *m_scanConfig;
        active::ScanArgs *m_scanArgs;
        active::ScanStat *m_scanStats;
        QStringListModel *m_targetListModel;
        QStandardItemModel *m_model;
        void getConfigValues();
        void startScan();
        void log(QString log);
        void scanSummary();
        void initUI();

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
        void sendToEngine(const ENGINE&, const RESULT_TYPE&);
        void sendSelectedToEngine(const ENGINE&, const RESULT_TYPE&);
        void sendToEnum(const TOOL&);
        void sendSelectedToEnum(const TOOL&);
};

#endif // ACTIVE_H
