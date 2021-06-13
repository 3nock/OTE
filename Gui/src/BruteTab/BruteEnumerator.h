#ifndef BRUTEENUMERATORS_H
#define BRUTEENUMERATORS_H

#include "lib-brute.h"

/*************************************************************************************
 *                         SUBDOMAIN-BRUTE ENUMERATOR
 *************************************************************************************/
class Enumerator_subBrute : public QObject{
    Q_OBJECT

    private:
        int m_currentItemToEnumerate = 0;
        ScanArguments_Brute* m_scanArguments;
        QDnsLookup* m_dns;

    public:
        Enumerator_subBrute(ScanArguments_Brute *scanArguments);
        ~Enumerator_subBrute();
        //...
        void Enumerate(QThread *cThread);

    public slots:
        void onStop();

    private slots:
        void lookup();
        void onLookupFinished();

    signals:
        void performAnotherLookup();
        //...
        void resolvedSubdomain(QString subdomain, QString ipAddress);
        void scanLog(QString log);
        void quitThread();
};


/*************************************************************************************
 *                              TLD-BRUTE ENUMERATOR
 *************************************************************************************/
class Enumerator_tldBrute : public QObject{
    Q_OBJECT

    private:
        int m_currentItemToEnumerate = 0;
        ScanArguments_Brute *m_scanArguments;
        QDnsLookup *m_dns;

    public:
        Enumerator_tldBrute(ScanArguments_Brute *scanArguments);
        ~Enumerator_tldBrute();
        //...
        void Enumerate(QThread *cThread);

    public slots:
        void onStop();

    private slots:
        void lookup();
        void onLookupFinished();

    signals:
        void performAnotherLookup();
        //...
        void resolvedSubdomain(QString subdomain, QString ipAddress);
        void scanLog(QString log);
        void quitThread();
};


/*************************************************************************************
 *                              ACTIVE_SUBDOMAINS ENUMERATOR
 *************************************************************************************/
class Enumerator_activeSubdomains : public QObject{
    Q_OBJECT

    private:
        int m_currentItemToEnumerate = 0;
        ScanArguments_Brute *m_scanArguments;
        QDnsLookup *m_dns;

    public:
        Enumerator_activeSubdomains(ScanArguments_Brute *scanArguments);
        ~Enumerator_activeSubdomains();
        //...
        void Enumerate(QThread *cThread);

    public slots:
        void onStop();

    private slots:
        void lookup();
        void onLookupFinished();

    signals:
        void performAnotherLookup();
        //...
        void resolvedSubdomain(QString subdomain, QString ipAddress);
        void scanLog(QString log);
        void quitThread();
};


/*************************************************************************************
 *                              Check Wildcards
 *************************************************************************************/
class Enumerator_Wildcards: public QObject{
    Q_OBJECT

    private:
        ScanArguments_Brute *m_scanArguments;
        QDnsLookup *m_dns;

    public:
        Enumerator_Wildcards(ScanArguments_Brute *scanArguments);
        ~Enumerator_Wildcards();
        //...
        void Enumerate(QThread *cThread);

    private slots:
        void onLookupFinished();
        void lookup();

    signals:
        void quitThread();
};

#endif // BRUTEENUMERATORS_H
