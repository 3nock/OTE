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

        /* change Tab */
        void changeTabToOsint();
        void changeTabToRaw();
        void changeTabToBrute();
        void changeTabToActive();
        void changeTabToDns();
        void changeTabToSSL();
        void changeTabToURL();
        void changeTabToIpEnum();
        void changeTabToAsnEnum();
        void changeTabToCidrEnum();
        void changeTabToNSEnum();
        void changeTabToMXEnum();
        void changeTabToSSLEnum();
        void changeTabToEmailEnum();
        void changeTabToProject();

        /* sending to an engine */
        void sendToOsint(QSet<QString>, RESULT_TYPE);
        void sendToRaw(QSet<QString>, RESULT_TYPE);
        void sendToBrute(QSet<QString>, RESULT_TYPE);
        void sendToActive(QSet<QString>, RESULT_TYPE);
        void sendToDns(QSet<QString>, RESULT_TYPE);
        void sendToSsl(QSet<QString>, RESULT_TYPE);
        void sendToUrl(QSet<QString>, RESULT_TYPE);
        /* sending to an Enumerator */
        void sendToIpEnum(QSet<QString>, RESULT_TYPE);
        void sendToAsnEnum(QSet<QString>, RESULT_TYPE);
        void sendToCidrEnum(QSet<QString>, RESULT_TYPE);
        void sendToNSEnum(QSet<QString>, RESULT_TYPE);
        void sendToMXEnum(QSet<QString>, RESULT_TYPE);
        void sendToSSLEnum(QSet<QString>, RESULT_TYPE);
        void sendToEmailEnum(QSet<QString>, RESULT_TYPE);
};

#endif // ABSTRACTCLASS_H
