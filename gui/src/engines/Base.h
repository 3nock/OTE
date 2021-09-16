#ifndef BASECLASS_H
#define BASECLASS_H

#include "src/utils.h"
#include "ContextMenu.h"
#include "src/widgets/InputWidget.h"

struct ScanStatus{
    bool isRunning = false;
    bool isStopped = false;
    bool isPaused = false;
    int activeThreads = 0;
};

struct Status{
    ScanStatus *osint = nullptr;
    ScanStatus *brute = nullptr;
    ScanStatus *active = nullptr;
    ScanStatus *ip = nullptr;
    ScanStatus *records = nullptr;
    int totalThreadsInUse(){
        return osint->activeThreads+
                brute->activeThreads+
                active->activeThreads+
                ip->activeThreads+
                records->activeThreads;
    }
};

class BaseClass :public  ContextMenu{
        Q_OBJECT

    public:
        BaseClass(ENGINE engineName, ResultsModel *resultsModel, Status *status, QWidget *parent = nullptr);
        ~BaseClass();
        void initBaseClass(InputWidget* targets){m_targets = targets;}

    public:
        Status *status;
        ScanConfig *scanConfig;
        int activeThreads = 0;

    signals:
        void stopScan();
        void pauseScan();
        //...
        void sendLog(QString log);
        void sendNotes(QString notes);
        void sendStatus(QString status);

    public slots:
        void logs(QString log);
        void onReceiveTargets(ENGINE, CHOICE);
        void onReceiveTargets(QItemSelectionModel*);

    private:
        InputWidget* m_targets = nullptr;
};

#endif // BASECLASS_H
