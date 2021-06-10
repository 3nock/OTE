#ifndef OSINTENUMERATOR_H
#define OSINTENUMERATOR_H

#include "lib-osint.h"

/***************************** ENUMERATOR CLASS *******************************/
class Enumerator: public QObject{
    Q_OBJECT

    public:
        Enumerator(ScanArguments_Osint *scanArguments, ScanResults_Osint *scanResults);
        void Enumerate(QThread *cThread);
        //...
        ScanArguments_Osint *scanArguments;
        ScanResults_Osint *scanResults;
        //...
        bool stopEnumeration = false;

    public slots:
        void onStopEnumeration();

    private slots:
        void worker();

    signals:
        void scanLogs(QString log);
        void enumerationComplete();
        //...
        void quitThread();
};

#endif // OSINTENUMERATOR_H
