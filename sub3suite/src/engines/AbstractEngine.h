/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief : The Base class for Engines classes.
*/

#ifndef ABSTRACTCLASS_H
#define ABSTRACTCLASS_H

#include <QWidget>
#include <QSortFilterProxyModel>

#include "src/utils/utils.h"
#include "src/utils/Config.h"
#include "src/widgets/InputWidget.h"
#include "src/project/model/ProjectModel.h"


class AbstractEngine : public QWidget{
        Q_OBJECT

    public:
        AbstractEngine(QWidget *parent = nullptr, ProjectModel *project = nullptr): QWidget(parent),
              status(new ScanStatus),
              project(project),
              proxyModel(new QSortFilterProxyModel)
        {
            proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
            proxyModel->setRecursiveFilteringEnabled(true);
            proxyModel->setFilterKeyColumn(0);
        }
        ~AbstractEngine(){
            delete proxyModel;
            delete status;
        }

    protected:
        ScanStatus *status;
        ProjectModel *project;
        QSortFilterProxyModel *proxyModel;
        QItemSelectionModel *selectionModel;

    signals:
        /* signals to scanner threads */
        void stopScanThread();
        void pauseScanThread();
        void resumeScanThread();

        /* status To mainwindow */
        void sendStatus(QString status);

        /* change to Tab */
        void changeTabToOsint();
        void changeTabToRaw();
        void changeTabToBrute();
        void changeTabToActive();
        void changeTabToDns();
        void changeTabToSSL();
        void changeTabToURL();
        /* ... */
        void changeTabToIpEnum();
        void changeTabToAsnEnum();
        void changeTabToCidrEnum();
        void changeTabToNSEnum();
        void changeTabToMXEnum();
        void changeTabToSSLEnum();
        void changeTabToEmailEnum();
        /* ... */
        void changeTabToProject();

        /* sending results to an engine */
        void sendResultsToOsint(QString, RESULT_TYPE);
        void sendResultsToRaw(QString, RESULT_TYPE);
        void sendResultsToBrute(QString, RESULT_TYPE);
        void sendResultsToActive(QString, RESULT_TYPE);
        void sendResultsToDns(QString, RESULT_TYPE);
        void sendResultsToCert(QString, RESULT_TYPE);
        void sendResultsToUrl(QString, RESULT_TYPE);
        /* sending results to an Enumerator */
        void sendResultsToIpEnum(QString, RESULT_TYPE);
        void sendResultsToAsnEnum(QString, RESULT_TYPE);
        void sendResultsToCidrEnum(QString, RESULT_TYPE);
        void sendResultsToNSEnum(QString, RESULT_TYPE);
        void sendResultsToMXEnum(QString, RESULT_TYPE);
        void sendResultsToSSLEnum(QString, RESULT_TYPE);
        void sendResultsToEmailEnum(QString, RESULT_TYPE);
};

#endif // ABSTRACTCLASS_H
