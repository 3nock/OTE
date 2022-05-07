/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#ifndef URLTOOL_H
#define URLTOOL_H

#include <QMenu>
#include <QAction>
#include <QElapsedTimer>

#include "../AbstractTool.h"
#include "src/utils/utils.h"
#include "src/modules/active/URLScanner.h"

namespace Ui {
    class URLTool;
}

class URLTool : public AbstractTool{
        Q_OBJECT

    public:
        URLTool(QWidget *parent = nullptr, ProjectModel *project = nullptr);
        ~URLTool();

    public slots:
        void onScanThreadEnded();
        void onScanLog(scan::Log log);
        void onScanResult(s3s_struct::URL url);
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

        void on_tableViewResults_doubleClicked(const QModelIndex &index);

    private:
        Ui::URLTool *ui;
        QElapsedTimer m_timer;
        QMap<QString,QString> m_failedScans;
        url::ScanConfig *m_scanConfig;
        url::ScanArgs *m_scanArgs;
        url::ScanStat *m_scanStats;
        QStringListModel *m_targetListModel;
        QStandardItemModel *m_model;
        QMap<QString, s3s_item::URL*> set_results;

        void initConfigValues();
        void initUI();

        void startScan();
        void log(const QString &log);
        void scanSummary();

    /* for context menu */
    private:
        void openInBrowser();
        void clearResults();
        void removeResults();
        void extract();
        void extractSelected();
        void saveResults();
        void saveSelectedResults();
        void copyResults();
        void copySelectedResults();
        /* sending results */
        void sendToProject();
        void sendSelectedToProject();
        void sendToEngine(const TOOL&);
        void sendSelectedToEngine(const TOOL&);
};

#endif // URLTOOL_H
