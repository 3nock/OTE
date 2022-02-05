/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#ifndef ASNENUM_H
#define ASNENUM_H

#include "../AbstractEnum.h"
#include "src/models/ASNModel.h"
#include "src/modules/passive/OsintModulesHeaders.h"

#include <QAction>


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
        void on_treeResults_customContextMenuRequested(const QPoint &pos);
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
        void initActions();
        /* ... */
        void m_clearResults();
        void m_removeResults(QItemSelectionModel*);
        void m_saveResults();
        void m_saveResults(QItemSelectionModel*);
        void m_copyResults();
        void m_copyResults(QItemSelectionModel*);
        /* sending results to other tools */
        void m_sendToProject();
        void m_sendASNToEngine(ENGINE);
        void m_sendCIDRToEngine(ENGINE);
        void m_sendASNToEnum();
        void m_sendCIDRToEnum();
        void m_sendToProject(QItemSelectionModel*);
        void m_sendASNToEngine(ENGINE, QItemSelectionModel*);
        void m_sendCIDRToEngine(ENGINE, QItemSelectionModel*);
        void m_sendASNToEnum(QItemSelectionModel*);
        void m_sendCIDRToEnum(QItemSelectionModel*);

    protected:
        QAction a_RemoveResults{"Remove"};
        QAction a_ClearResults{"Clear Results"};
        QAction a_Save{"Save as Json"};
        QAction a_Copy{"Copy as Json"};
        /* for all */
        QAction a_SendAllToProject{"Send To Project"};
        QAction a_SendAllASNToOsint{"Send ASNs To OSINT"};
        QAction a_SendAllASNToRaw{"Send ASNs To RAW"};
        QAction a_SendAllCIDRToOsint{"Send CIDRs To OSINT"};
        QAction a_SendAllCIDRToRaw{"Send CIDRs To RAW"};
        QAction a_SendAllASNToASNEnum{"Send ASNs To ASNEnum"};
        QAction a_SendAllCIDRToCIDREnum{"Send CIDRs To CIDREnum"};
        /* for selected */
        QAction a_SendSelectedToProject{"Send To Project"};
        QAction a_SendSelectedASNToOsint{"Send ASNs To OSINT"};
        QAction a_SendSelectedASNToRaw{"Send ASNs To RAW"};
        QAction a_SendSelectedCIDRToOsint{"Send CIDRs To OSINT"};
        QAction a_SendSelectedCIDRToRaw{"Send CIDRs To RAW"};
        QAction a_SendSelectedASNToASNEnum{"Send ASNs To ASNEnum"};
        QAction a_SendSelectedCIDRToCIDREnum{"Send CIDRs To CIDREnum"};
};

#endif // ASNENUM_H
