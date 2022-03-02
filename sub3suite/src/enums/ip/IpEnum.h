/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#ifndef IPENUM_H
#define IPENUM_H

#include "../AbstractEnum.h"
#include "src/modules/passive/OsintModulesHeaders.h"

#include <QAction>


namespace Ui {
    class IpEnum;
}

class IpEnum : public AbstractEnum{
    Q_OBJECT

    public:
        IpEnum(QWidget *parent = nullptr, ProjectModel *project = nullptr);
        ~IpEnum();

    public slots:
        void onResult(s3s_struct::IP);

        void onScanThreadEnded();
        void onInfoLog(ScanLog log);
        void onErrorLog(ScanLog log);
        void onRateLimitLog(ScanLog log);
        void onReScan(QQueue<QString> targets);

        void onReceiveTargets(QString, RESULT_TYPE);

    private slots:
        void on_buttonStart_clicked();
        void on_buttonConfig_clicked();
        void on_lineEditTarget_returnPressed();
        void on_buttonStop_clicked();
        void on_treeViewResults_customContextMenuRequested(const QPoint &pos);
        void on_lineEditFilter_textChanged(const QString &arg1);
        void on_buttonAction_clicked();

    private:
        Ui::IpEnum *ui;

        QStandardItemModel *m_model;
        QStringListModel *m_targetsListModel;

        QMap<QString,QString> m_failedScans;
        QMap<QString, s3s_item::IP*> m_resultsSet;

        ScanConfig *m_scanConfig;
        ScanArgs *m_scanArgs;

        void initUI();
        void initConfigValues();

        void startScan();
        void log(QString log);

        /* for context menu */
    private:
        void clearResults();
        void removeResults();
        void saveResults();
        void saveSelectedResults();
        void copyResults();
        void copySelectedResults();
        /* sending results */
        void sendToProject();
        void sendSelectedToProject();
};

#endif // IPENUM_H
