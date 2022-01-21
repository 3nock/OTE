/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief : The Base class for enumerators classes
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
              project(project)
        {
        }
        ~AbstractEnum(){
            delete status;
        }

    protected:
        ScanStatus *status;
        ProjectModel *project;
        QItemSelectionModel *selectionModel;

    signals:
        /* signals to scanning threads */
        void stopScanThread();
        void pauseScanThread();
        void resumeScanThread();

        /* status To mainwindow */
        void sendStatus(QString status);

        /* chage tab to Engine */
        void changeTabToOsint();
        void changeTabToRaw();
        void changeTabToBrute();
        void changeTabToActive();
        void changeTabToDns();
        void changeTabToSSL();
        void changeTabToProject();
        /* change tab To Enum */
        void changeTabToIpEnum();
        void changeTabToAsnEnum();
        void changeTabToCidrEnum();
        void changeTabToNSEnum();
        void changeTabToMXEnum();
        void changeTabToSSLEnum();
        void changeTabToEmailEnum();
        void changeTabToUrlEnum();

        /* sending results to an engine */
        void sendResultsToOsint(QString, RESULT_TYPE);
        void sendResultsToRaw(QString, RESULT_TYPE);
        void sendResultsToBrute(QString, RESULT_TYPE);
        void sendResultsToActive(QString, RESULT_TYPE);
        void sendResultsToDns(QString, RESULT_TYPE);
        void sendResultsToCert(QString, RESULT_TYPE);
        /* sending results to an Enumerator */
        void sendResultsToIpEnum(QString, RESULT_TYPE);
        void sendResultsToAsnEnum(QString, RESULT_TYPE);
        void sendResultsToCidrEnum(QString, RESULT_TYPE);
        void sendResultsToNSEnum(QString, RESULT_TYPE);
        void sendResultsToMXEnum(QString, RESULT_TYPE);
        void sendResultsToSSLEnum(QString, RESULT_TYPE);
        void sendResultsToEmailEnum(QString, RESULT_TYPE);
        void sendResultsToUrlEnum(QString, RESULT_TYPE);
};

#endif // ABSTRACTENUM_H
