#ifndef OSINTENUMERATOR_H
#define OSINTENUMERATOR_H

#include "lib-osint.h"

/***************************** ENUMERATOR CLASS *******************************/
class Enumerator: public QObject{
    Q_OBJECT

    public:
        Enumerator(char *domainName, QStringList choosenEngines, QListWidget *listWidget_subdomains, QLabel *label_subdomainsCount, int *subdomainsCount);
        void Enumerate(QThread *cThread);
        // variables...
        QListWidget *listWidget_subdomains;
        QLabel *label_subdomainsCount;
        int *subdomainsCount;
        char *domainName;
        QStringList choosenEngines;
        bool stopEnumeration = false;

    public slots:
        void onStopEnumeration();

    private slots:
        void worker();

    signals:
        // send to osint..
        void scanLogs(QString log);
        void enumerationComplete();
        // send to self thread..
        void quitThread();
};

#endif // OSINTENUMERATOR_H
