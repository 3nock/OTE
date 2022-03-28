/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#ifndef SSL_H
#define SSL_H

#include <QMenu>
#include <QAction>
#include <QWidget>
#include <QElapsedTimer>

#include "../AbstractEngine.h"
#include "src/items/SSLItem.h"
#include "src/modules/active/SSLScanner.h"


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
        void onScanResultSubdomain(QStringList subdomain);
        void onScanResultRaw(QString target, QSslCertificate certificate);
        /* ... */
        void onReScan(QQueue<QString> targets);

        /* receiving targets from other engines */
        void onReceiveTargets(QSet<QString>, RESULT_TYPE);

    private slots:
        void on_buttonStart_clicked();
        void on_buttonStop_clicked();
        void on_buttonAction_clicked();
        void on_treeViewResults_customContextMenuRequested(const QPoint &pos);
        void on_comboBoxOutput_currentIndexChanged(int index);
        void on_lineEditFilter_textChanged(const QString &arg1);
        void on_lineEditTarget_returnPressed();
        void on_buttonConfig_clicked();
        void on_treeViewResults_clicked(const QModelIndex &index);

    private:
        Ui::Ssl *ui;
        QElapsedTimer m_timer;
        ssl::ScanConfig *m_scanConfig;
        ssl::ScanArgs *m_scanArgs;
        ssl::ScanStat *m_scanStats;
        QSet<QString> set_subdomain;
        QSet<QString> set_hash;
        QMap<QString, s3s_item::SSL*> set_ssl;
        QMap<QString,QString> m_failedScans;
        QStringListModel *m_targetListModel;
        QStandardItemModel *m_model_subdomain;
        QStandardItemModel *m_model_hash;
        QStandardItemModel *m_model_ssl;

        void initConfigValues();
        void initUI();

        void startScan();
        void scanSummary();

        void log(const QString &log);

        /* for context menu */
    private:
        void clearResults();
        void openInBrowser();
        void removeResults();
        void saveResults();
        void saveSelectedResults();
        void copyResults();
        void copySelectedResults();
        /* sending results */
        void sendToProject();
        void sendSelectedToProject();
        void sendToEngine(const ENGINE&, const RESULT_TYPE&);
        void sendSelectedToEngine(const ENGINE&, const RESULT_TYPE&);
        void sendToEnum(const ENUMERATOR&);
        void sendSelectedToEnum(const ENUMERATOR&);
};

#endif // SSL_H
