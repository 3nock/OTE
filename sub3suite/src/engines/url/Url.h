/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#ifndef URL_H
#define URL_H

#include <QElapsedTimer>
#include "../AbstractEngine.h"
#include "src/utils/utils.h"
#include "src/modules/active/UrlScanner.h"

namespace Ui {
    class Url;
}

class Url : public AbstractEngine{
        Q_OBJECT

    public:
        Url(QWidget *parent = nullptr, ProjectModel *project = nullptr);
        ~Url();

    public slots:
        void onScanThreadEnded();
        void onScanLog(scan::Log log);
        void onScanResult(s3s_struct::URL url);
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
        Ui::Url *ui;
        QElapsedTimer m_timer;
        QSet<QString> m_activeDns;
        QMap<QString,QString> m_failedScans;
        url::ScanConfig *m_scanConfig;
        url::ScanArgs *m_scanArgs;
        url::ScanStat *m_scanStats;
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
        void m_saveResults();
        void m_saveResults(QItemSelectionModel*);
        void m_copyResults();
        void m_copyResults(QItemSelectionModel*);
        /* ... */
        void m_sendToProject();
        void m_sendToProject(QItemSelectionModel*);

    protected:
        /* general actions */
        QAction a_RemoveResults{"Remove"};
        QAction a_ClearResults{"Clear Results"};
        QAction a_OpenInBrowser{"Open in Browser"};
        QAction a_SendAllToProject{"Send To Project"};
        QAction a_SendSelectedToProject{"Send To Project"};
        QAction a_Save{"Save"};
        QAction a_Copy{"Copy"};
};

#endif // URL_H
