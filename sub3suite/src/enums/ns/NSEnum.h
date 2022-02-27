/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#ifndef NSENUM_H
#define NSENUM_H

#include "../AbstractEnum.h"
#include "src/items/NSItem.h"
#include "src/modules/passive/OsintModulesHeaders.h"

#include <QAction>


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
        void onInfoLog(ScanLog log);
        void onErrorLog(ScanLog log);
        void onRateLimitLog(ScanLog log);

        void onReceiveTargets(QString, RESULT_TYPE);

    private slots:
        void on_buttonStart_clicked();
        void on_buttonConfig_clicked();
        void on_lineEditTarget_returnPressed();
        void on_buttonStop_clicked();
        void on_buttonAction_clicked();
        void on_treeResults_customContextMenuRequested(const QPoint &pos);
        void on_lineEditFilter_textChanged(const QString &arg1);

    private:
        Ui::NSEnum *ui;
        QStandardItemModel *m_model;
        QStringListModel *m_targetsListModel;
        QMap<QString, s3s_item::NS*> m_resultsSet;
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
        /* sending results */
        void m_sendToProject();
        void m_sendToProject(QItemSelectionModel*);

    protected:
        QAction a_RemoveResults{"Remove"};
        QAction a_ClearResults{"Clear Results"};
        QAction a_ExpandResults{"Expand"};
        QAction a_CollapseResults{"Collapse"};
        QAction a_Save{"Save as Json"};
        QAction a_Copy{"Copy as Json"};
        /* for all */
        QAction a_SendAllToProject{"Send To Project"};
        /* for selected */
        QAction a_SendSelectedToProject{"Send To Project"};
};

#endif // NSENUM_H
