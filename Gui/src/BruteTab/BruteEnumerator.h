#ifndef BRUTEENUMERATORS_H
#define BRUTEENUMERATORS_H

#include "lib-brute.h"

/*************************************************************************************
 *                         SUBDOMAIN-BRUTE ENUMERATOR
 *************************************************************************************/
class Enumerator_subBrute : public QObject{
    Q_OBJECT

    public:
        Enumerator_subBrute(ScanArguments_Brute *scanArguments);
        ~Enumerator_subBrute();
        void Enumerate(QThread *cThread);
        ScanArguments_Brute *scanArguments;
        int itemToEnumerate;
        QHostAddress nameserver;
        QDnsLookup *dns;

    public slots:
        void lookup();
        void onStop();
        void onLookupFinished();

    signals:
        void resolvedSubdomain(QString subdomain, QString ipAddress);
        void scanLog(QString log);
        void quitThread();
};


/*************************************************************************************
 *                              TLD-BRUTE ENUMERATOR
 *************************************************************************************/
class Enumerator_tldBrute : public QObject{
    Q_OBJECT

    public:
        Enumerator_tldBrute(ScanArguments_Brute *scanArguments);
        void Enumerate(QThread *cThread);
        ScanArguments_Brute *scanArguments;
        int itemToEnumerate;
        QHostAddress nameserver;
        QDnsLookup *dns;

    public slots:
        void onLookupFinished();
        void lookup();
        void onStop();

    signals:
        void resolvedSubdomain(QString subdomain, QString ipAddress);
        void scanLog(QString log);
        void quitThread();
};


/*************************************************************************************
 *                              ACTIVE_SUBDOMAINS ENUMERATOR
 *************************************************************************************/
class Enumerator_activeSubdomains : public QObject{
    Q_OBJECT

    public:
        Enumerator_activeSubdomains(ScanArguments_Brute *scanArguments);
        void Enumerate(QThread *cThread);
        ScanArguments_Brute *scanArguments;
        int itemToEnumerate;
        QHostAddress nameserver;
        QDnsLookup *dns;

    public slots:
        void onLookupFinished();
        void lookup();
        void onStop();

    signals:
        void resolvedSubdomain(QString subdomain, QString ipAddress);
        void scanLog(QString log);
        void quitThread();
};


/*************************************************************************************
 *                              Check Wildcards
 *************************************************************************************/

class Enumerator_Wildcards: public QObject{
    Q_OBJECT

    public:
        Enumerator_Wildcards(ScanArguments_Brute *scanArguments);
        void Enumerate(QThread *cThread);
        ScanArguments_Brute *scanArguments;
        QDnsLookup *dns;

    public slots:
        void onLookupFinished();
        void lookup();

    signals:
        void quitThread();
};

#endif // BRUTEENUMERATORS_H
