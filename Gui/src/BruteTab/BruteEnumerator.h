#ifndef BRUTEENUMERATORS_H
#define BRUTEENUMERATORS_H

#include "lib-brute.h"

/************************************************************************************
                                 BRUTE ENUMERATOR
*************************************************************************************/
class BruteEnumerator : public QObject{
    Q_OBJECT

    private:
        int m_currentWordlistToEnumerate = 0;
        int m_currentTargetToEnumerate = 0;
        //...
        ScanArguments_Brute *m_scanArguments;
        ScanConfig *m_scanConfig;
        QDnsLookup *m_dns;

    public:
        BruteEnumerator(ScanConfig *scanConfig, ScanArguments_Brute *scanArguments);
        ~BruteEnumerator();
        //...
        void enumerate(QThread *cThread);

    public slots:
        void onStop();

    private slots:
        void lookup();
        void lookupFinished();

    signals:
        void performAnotherLookup();
        //...
        void progress(int value);
        void scanResult(QString subdomain, QString ipAddress);
        void scanLog(QString log);
        //...
        void quitThread();
};


/*************************************************************************************
 *                              Check Wildcards
 *************************************************************************************/
class BruteEnumerator_Wildcards: public QObject{
    Q_OBJECT

    private:
        ScanArguments_Brute *m_scanArguments;
        ScanConfig *m_scanConfig;
        QDnsLookup *m_dns;

    public:
        BruteEnumerator_Wildcards(ScanConfig *scanConfig, ScanArguments_Brute *scanArguments);
        ~BruteEnumerator_Wildcards();
        //...
        void enumerate(QThread *cThread);

    private slots:
        void onLookupFinished();
        void lookup();

    signals:
        void quitThread();
};

#endif // BRUTEENUMERATORS_H
