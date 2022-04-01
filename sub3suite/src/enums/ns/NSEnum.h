/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#ifndef NSENUM_H
#define NSENUM_H

#include "../AbstractEnum.h"
#include "src/items/NSItem.h"
#include "src/modules/passive/OsintHeaders.h"


namespace Ui {
class NSEnum;
}

class NSEnum : public AbstractEnum {
        Q_OBJECT

    public:
        NSEnum(QWidget *parent = nullptr, ProjectModel *project = nullptr);
        ~NSEnum();

    public slots:
        void onResult(s3s_struct::NS results);

        void onScanThreadEnded();
        void onScanLog(ScanLog log);
        void onReScan(QQueue<QString> targets);

        void onReceiveTargets(QSet<QString>, RESULT_TYPE);

    private slots:
        void on_buttonStart_clicked();
        void on_buttonConfig_clicked();
        void on_lineEditTarget_returnPressed();
        void on_buttonStop_clicked();
        void on_buttonAction_clicked();
        void on_treeViewResults_customContextMenuRequested(const QPoint &pos);
        void on_lineEditFilter_textChanged(const QString &arg1);

        void on_buttoApiKeys_clicked();

private:
        Ui::NSEnum *ui;

        QStandardItemModel *m_model;
        QStringListModel *m_targetsListModel;

        QMap<QString,QString> m_failedScans;
        QMap<QString, s3s_item::NS*> m_resultsSet;

        ScanConfig *m_scanConfig;
        ScanArgs *m_scanArgs;

        void initUI();
        void initConfigValues();

        void startScan();
        void startScanThread(AbstractOsintModule*);
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
        void sendToEngine(const ENGINE&);
        void sendSelectedToEngine(const ENGINE&);
};

#endif // NSENUM_H
