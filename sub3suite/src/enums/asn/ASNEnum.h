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

        void startScan();
        void log(QString log);

        /* for context menu */
    private:
        void initActions();
        /* ... */
        void m_openInBrowser(QItemSelectionModel*);
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
        void m_sendASNToEnum(TOOL);
        void m_sendCIDRToEnum(TOOL);
        void m_sendToProject(QItemSelectionModel*);
        void m_sendASNToEngine(ENGINE, QItemSelectionModel*);
        void m_sendCIDRToEngine(ENGINE, QItemSelectionModel*);
        void m_sendASNToEnum(TOOL, QItemSelectionModel*);
        void m_sendCIDRToEnum(TOOL, QItemSelectionModel*);

    protected:
        /* general actions */
        QAction a_RemoveResults{"Remove"};
        QAction a_ClearResults{"Clear Results"};
        QAction a_OpenInBrowser{"Open in Browser"};
        QAction a_Save{"Save"};
        QAction a_Copy{"Copy"};

        /* for all */
        QAction a_SendAllToProject{"Send To Project"};
        QAction a_SendAllASNToOsint{"Send ASN To OSINT"};
        QAction a_SendAllASNToRaw{"Send ASN To RAW"};
        QAction a_SendAllCIDRToOsint{"Send CIDR To OSINT"};
        QAction a_SendAllCIDRToRaw{"Send CIDR To RAW"};
        QAction a_SendAllASNToASNEnum{"Send ASN To ASNEnum"};
        QAction a_SendAllCIDRToCIDREnum{"Send CIDR To CIDREnum"};
        /* for selected */
        QAction a_SendSelectedToProject{"Send To Project"};
        QAction a_SendSelectedASNToOsint{"Send ASN To OSINT"};
        QAction a_SendSelectedASNToRaw{"Send ASN To RAW"};
        QAction a_SendSelectedCIDRToOsint{"Send CIDR To OSINT"};
        QAction a_SendSelectedCIDRToRaw{"Send CIDR To RAW"};
        QAction a_SendSelectedASNToASNEnum{"Send ASN To ASNEnum"};
        QAction a_SendSelectedCIDRToCIDREnum{"Send CIDR To CIDREnum"};
};

#endif // ASNENUM_H
