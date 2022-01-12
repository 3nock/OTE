/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#ifndef ABSTRACTCLASS_H
#define ABSTRACTCLASS_H

#include <QMenu>
#include <QMap>
#include <QAction>
#include <QStringListModel>
#include "src/utils/utils.h"
#include "src/widgets/InputWidget.h"
#include "src/project/ProjectDataModel.h"


struct ScanStatus{
    bool isNotActive = true;
    bool isRunning = false;
    bool isStopped = false;
    bool isPaused = false;
    int activeScanThreads = 0;
};

class AbstractEngine : public QWidget{
        Q_OBJECT

    public:
        AbstractEngine(QWidget *parent = nullptr, ProjectDataModel *project = nullptr): QWidget(parent),
              status(new ScanStatus),
              project(project)
        {
        }
        ~AbstractEngine(){
            delete status;
        }

    protected:
        ScanStatus *status;
        ProjectDataModel *project;
        QItemSelectionModel *selectionModel;
        /* ... */
        bool autoSaveResultsToProject = false;
        bool noDuplicateResults = false;
        bool noDuplicateTargets = false;

    signals:
        /* signals to scanning threads */
        void stopScanThread();
        void pauseScanThread();
        void resumeScanThread();

        /* status To mainwindow */
        void sendStatus(QString status);

        /* tab change */
        void changeTabToIp();
        void changeTabToOsint();
        void changeTabToRaw();
        void changeTabToBrute();
        void changeTabToActive();
        void changeTabToDns();
        void changeTabToCert();
        void changeTabToProject();

        /* sending results to an engine */
        void sendResultsToOsint(QString, RESULT_TYPE);
        void sendResultsToRaw(QString, RESULT_TYPE);
        void sendResultsToBrute(QString, RESULT_TYPE);
        void sendResultsToActive(QString, RESULT_TYPE);
        void sendResultsToDns(QString, RESULT_TYPE);
        void sendResultsToIp(QString, RESULT_TYPE);
        void sendResultsToCert(QString, RESULT_TYPE);
};

#endif // ABSTRACTCLASS_H
