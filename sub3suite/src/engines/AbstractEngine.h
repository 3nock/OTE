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
    bool isRunning = false;
    bool isStopped = false;
    bool isPaused = false;
    int activeScanThreads = 0;
};

class Status{
    public:
        Status()
            : osint(new ScanStatus),
              raw(new ScanStatus),
              brute(new ScanStatus),
              active(new ScanStatus),
              ip(new ScanStatus),
              cert(new ScanStatus),
              dns(new ScanStatus)
        {
        }
        ~Status(){
            delete osint;
            delete raw;
            delete brute;
            delete active;
            delete ip;
            delete cert;
            delete dns;
        }
        ScanStatus *osint;
        ScanStatus *raw;
        ScanStatus *brute;
        ScanStatus *active;
        ScanStatus *ip;
        ScanStatus *cert;
        ScanStatus *dns;
        /* ... */
        int totalThreadsInUse(){
            return osint->activeScanThreads+
                    raw->activeScanThreads+
                    brute->activeScanThreads+
                    active->activeScanThreads+
                    ip->activeScanThreads+
                    cert->activeScanThreads+
                    dns->activeScanThreads;
        }
};


class AbstractEngine : public QWidget{
        Q_OBJECT

    public:
        AbstractEngine(QWidget *parent = nullptr,
                       ProjectDataModel *project = nullptr,
                       Status *status = nullptr)
            : QWidget(parent),
              status(status),
              project(project)
        {
        }
        ~AbstractEngine(){}

    protected:
        Status *status;
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
