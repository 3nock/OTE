#ifndef ABSTRACTCLASS_H
#define ABSTRACTCLASS_H

#include <QMenu>
#include <QMap>
#include <QAction>
#include <QStringListModel>
#include "src/utils/utils.h"
#include "src/models/ResultsModels.h"
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
            delete brute;
            delete active;
            delete ip;
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
                       ResultsModel *resultsModel = nullptr,
                       ProjectDataModel *project = nullptr,
                       Status *status = nullptr)
            : QWidget(parent),
              status(status),
              result(resultsModel),
              project(project)
        {
        }
        ~AbstractEngine(){}

    protected:
        Status *status;
        ResultsModel *result;
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

        /* sending results using action-button */
        void sendResultsToOsint(ENGINE, RESULT_TYPE, RESULT_MODEL_TYPE);
        void sendResultsToRaw(ENGINE, RESULT_TYPE, RESULT_MODEL_TYPE);
        void sendResultsToBrute(ENGINE, RESULT_TYPE, RESULT_MODEL_TYPE);
        void sendResultsToActive(ENGINE, RESULT_TYPE, RESULT_MODEL_TYPE);
        void sendResultsToDns(ENGINE, RESULT_TYPE, RESULT_MODEL_TYPE);
        void sendResultsToIp(ENGINE, RESULT_TYPE, RESULT_MODEL_TYPE);
        void sendResultsToCert(ENGINE, RESULT_TYPE, RESULT_MODEL_TYPE);
        void sendResultsToASNTool(ENGINE, RESULT_TYPE, RESULT_MODEL_TYPE);
        void sendResultsToCertTool(ENGINE, RESULT_TYPE, RESULT_MODEL_TYPE);
        void sendResultsToCidrTool(ENGINE, RESULT_TYPE, RESULT_MODEL_TYPE);
        void sendResultsToDomainTool(ENGINE, RESULT_TYPE, RESULT_MODEL_TYPE);
        void sendResultsToEmailTool(ENGINE, RESULT_TYPE, RESULT_MODEL_TYPE);
        void sendResultsToIpTool(ENGINE, RESULT_TYPE, RESULT_MODEL_TYPE);
        void sendResultsToMXTool(ENGINE, RESULT_TYPE, RESULT_MODEL_TYPE);
        void sendResultsToNSTool(ENGINE, RESULT_TYPE, RESULT_MODEL_TYPE);
        void sendResultsToProject(ENGINE, RESULT_TYPE);

        /* sending results using right-click */
        void sendResultsToOsint(QItemSelectionModel*, RESULT_TYPE, RESULT_MODEL_TYPE);
        void sendResultsToRaw(QItemSelectionModel*, RESULT_TYPE, RESULT_MODEL_TYPE);
        void sendResultsToBrute(QItemSelectionModel*, RESULT_TYPE, RESULT_MODEL_TYPE);
        void sendResultsToActive(QItemSelectionModel*, RESULT_TYPE, RESULT_MODEL_TYPE);
        void sendResultsToDns(QItemSelectionModel*, RESULT_TYPE, RESULT_MODEL_TYPE);
        void sendResultsToIp(QItemSelectionModel*, RESULT_TYPE, RESULT_MODEL_TYPE);
        void sendResultsToCert(QItemSelectionModel*, RESULT_TYPE, RESULT_MODEL_TYPE);
        void sendResultsToASNTool(QItemSelectionModel*, RESULT_TYPE, RESULT_MODEL_TYPE);
        void sendResultsToCertTool(QItemSelectionModel*, RESULT_TYPE, RESULT_MODEL_TYPE);
        void sendResultsToCidrTool(QItemSelectionModel*, RESULT_TYPE, RESULT_MODEL_TYPE);
        void sendResultsToDomainTool(QItemSelectionModel*, RESULT_TYPE, RESULT_MODEL_TYPE);
        void sendResultsToEmailTool(QItemSelectionModel*, RESULT_TYPE, RESULT_MODEL_TYPE);
        void sendResultsToIpTool(QItemSelectionModel*, RESULT_TYPE, RESULT_MODEL_TYPE);
        void sendResultsToMXTool(QItemSelectionModel*, RESULT_TYPE, RESULT_MODEL_TYPE);
        void sendResultsToNSTool(QItemSelectionModel*, RESULT_TYPE, RESULT_MODEL_TYPE);
        void sendResultsToProject(QItemSelectionModel*, RESULT_TYPE, RESULT_MODEL_TYPE);
};

#endif // ABSTRACTCLASS_H
