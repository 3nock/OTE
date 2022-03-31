/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief : The Base class for enumerators classes (it is not an abstract class)
*/

#ifndef ABSTRACTENUM_H
#define ABSTRACTENUM_H

#include <QStringListModel>
#include "src/utils/utils.h"
#include "src/widgets/InputWidget.h"
#include "src/models/ProjectModel.h"


class AbstractEnum : public QWidget{
        Q_OBJECT

    public:
        AbstractEnum(QWidget *parent = nullptr, ProjectModel *project = nullptr): QWidget(parent),
              status(new ScanStatus),
              project(project),
              proxyModel(new QSortFilterProxyModel)
        {
        }
        ~AbstractEnum(){
            delete proxyModel;
            delete status;
        }

    protected:
        ScanStatus *status;
        ProjectModel *project;
        QSortFilterProxyModel *proxyModel;
        QItemSelectionModel *selectionModel = nullptr;

    signals:
        /* signals to scanning threads */
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

#endif // ABSTRACTENUM_H
