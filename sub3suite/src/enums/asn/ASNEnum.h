/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#ifndef ASNENUM_H
#define ASNENUM_H

#include "../AbstractEnum.h"
#include "src/items/ASNItem.h"
#include "src/modules/passive/OsintModulesHeaders.h"


namespace Ui {
class ASNEnum;
}

class ASNEnum : public AbstractEnum{
        Q_OBJECT

    public:
        ASNEnum(QWidget *parent = nullptr, ProjectModel *project = nullptr);
        ~ASNEnum();

    public slots:
        void onResultsAsn(s3s_struct::ASN results);
        void onResultsAsnPeers(s3s_struct::ASN results);
        void onResultsAsnPrefixes(s3s_struct::ASN results);

        void onScanThreadEnded();
        void onInfoLog(ScanLog log);
        void onErrorLog(ScanLog log);
        void onRateLimitLog(ScanLog log);

        void onReceiveTargets(QString, RESULT_TYPE);

    private slots:
        void on_buttonStart_clicked();
        void on_buttonConfig_clicked();
        void on_buttonStop_clicked();
        void on_lineEditFilter_textChanged(const QString &arg1);
        void on_buttonAction_clicked();
        void on_treeViewResults_customContextMenuRequested(const QPoint &pos);
        void on_lineEditTarget_returnPressed();

    private:
        Ui::ASNEnum *ui;
        QStandardItemModel *m_model;
        QStringListModel *m_targetsListModel;
        QMap<QString, s3s_item::ASN*> m_resultsSet;
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
        void sendToEnum(const TOOL&);
        void sendSelectedToEnum(const TOOL&);
};

#endif // ASNENUM_H
