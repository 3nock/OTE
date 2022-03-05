/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#ifndef CIDRENUM_H
#define CIDRENUM_H

#include "../AbstractEnum.h"
#include "src/items/CIDRItem.h"
#include "src/modules/passive/OsintModulesHeaders.h"


namespace Ui {
class CidrEnum;
}

class CidrEnum : public AbstractEnum {
        Q_OBJECT

    public:
        CidrEnum(QWidget *parent = nullptr, ProjectModel *project = nullptr);
        ~CidrEnum();

    public slots:
        void onResult(s3s_struct::CIDR);

        void onScanThreadEnded();
        void onErrorLog(ScanLog log);
        void onInfoLog(ScanLog log);
        void onRateLimitLog(ScanLog log);
        void onReScan(QQueue<QString> targets);

        void onReceiveTargets(QSet<QString>, RESULT_TYPE);

    private slots:
        void on_buttonStart_clicked();
        void on_buttonConfig_clicked();
        void on_lineEditTarget_returnPressed();
        void on_buttonStop_clicked();
        void on_lineEditFilter_textChanged(const QString &arg1);
        void on_treeViewResults_customContextMenuRequested(const QPoint &pos);
        void on_buttonAction_clicked();

    private:
        Ui::CidrEnum *ui;

        QStandardItemModel *m_model;
        QStringListModel *m_targetsListModel;

        QMap<QString,QString> m_failedScans;
        QMap<QString, s3s_item::CIDR*> m_resultsSet;

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
        void sendToEngine(const ENGINE&, const RESULT_TYPE&);
        void sendSelectedToEngine(const ENGINE&, const RESULT_TYPE&);
        void sendToEnum(const TOOL&, const RESULT_TYPE&);
        void sendSelectedToEnum(const TOOL&, const RESULT_TYPE&);
};

#endif // CIDRENUM_H
