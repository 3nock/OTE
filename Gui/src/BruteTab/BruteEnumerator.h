#ifndef BRUTEENUMERATORS_H
#define BRUTEENUMERATORS_H

#include "lib-brute.h"

/*************************************************************************************
 *                         SUBDOMAIN-BRUTE ENUMERATOR
 *************************************************************************************/
class BruteEnumerator_subBrute : public QObject{
    Q_OBJECT

    private:
        int m_currentItemToEnumerate = 0;
        ScanArguments_Brute* m_scanArguments;
        QDnsLookup* m_dns;

    public:
        BruteEnumerator_subBrute(ScanArguments_Brute *scanArguments);
        ~BruteEnumerator_subBrute();
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
        void progressBarValue(int value);
        void resolvedSubdomain(QString subdomain, QString ipAddress);
        void scanLog(QString log);
        void quitThread();
};


/*************************************************************************************
 *                              TLD-BRUTE ENUMERATOR
 *************************************************************************************/
class BruteEnumerator_tldBrute : public QObject{
    Q_OBJECT

    private:
        int m_currentItemToEnumerate = 0;
        ScanArguments_Brute *m_scanArguments;
        QDnsLookup *m_dns;

    public:
        BruteEnumerator_tldBrute(ScanArguments_Brute *scanArguments);
        ~BruteEnumerator_tldBrute();
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
        void progressBarValue(int value);
        void resolvedSubdomain(QString subdomain, QString ipAddress);
        void scanLog(QString log);
        void quitThread();
};


/*************************************************************************************
 *                              Check Wildcards
 *************************************************************************************/
class BruteEnumerator_Wildcards: public QObject{
    Q_OBJECT

    private:
        ScanArguments_Brute *m_scanArguments;
        QDnsLookup *m_dns;

    public:
        BruteEnumerator_Wildcards(ScanArguments_Brute *scanArguments);
        ~BruteEnumerator_Wildcards();
        //...
        void Enumerate(QThread *cThread);

    private slots:
        void onLookupFinished();
        void lookup();

    signals:
        void quitThread();
};

#endif // BRUTEENUMERATORS_H
