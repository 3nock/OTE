#ifndef ACTIVEENUMERATOR_H
#define ACTIVEENUMERATOR_H

#include "src/BruteTab/lib-brute.h"

/*************************************************************************************
 *                              ACTIVE_SUBDOMAINS ENUMERATOR
 *************************************************************************************/
class ActiveEnumerator : public QObject{
    Q_OBJECT

    private:
        int m_currentItemToEnumerate = 0;
        ScanArguments_Brute *m_scanArguments;
        QDnsLookup *m_dns;

    public:
        ActiveEnumerator(ScanArguments_Brute *scanArguments);
        ~ActiveEnumerator();
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
#endif // ACTIVEENUMERATOR_H
