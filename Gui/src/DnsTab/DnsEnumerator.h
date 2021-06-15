#ifndef DNSENUMERATOR_H
#define DNSENUMERATOR_H

#include "lib-dns.h"
#include <QHostInfo>

/***********************************************************************************
                               DNS-RECORDS ENUMERATOR
************************************************************************************/

class Enumerator_dnsRecords: public QObject{
    Q_OBJECT

    private:
        scanArguments_dnsRecords *m_scanArguments;
        scanResults_dnsRecords *m_scanResults;
        //...
        QStandardItem *m_dnsNameItem;
        QStandardItem *m_recordItem;
        //...
        QHostAddress m_nameserver;
        QDnsLookup *m_dns_a;
        QDnsLookup *m_dns_aaaa;
        QDnsLookup *m_dns_mx;
        QDnsLookup *m_dns_ns;
        QDnsLookup *m_dns_txt;
        QDnsLookup *m_dns_srv;
        QDnsLookup *m_dns_cname;
        //...
        int m_currentTargetToEnumerate = 0;
        int m_finishedLookups = 0;
        bool m_firstToResolve = true;
        //...
        QString m_currentTarget;

    public:
        Enumerator_dnsRecords(scanArguments_dnsRecords *scanArguments, scanResults_dnsRecords *scanResults);
        ~Enumerator_dnsRecords();
        //...
        void Enumerate(QThread *cThread);

    public slots:
        void onStop();
        void trackFinishedLookups();

    private slots:
        void lookup();
        void aLookupFinished();
        void aaaaLookupFinished();
        void mxLookupFinished();
        void cnameLookupFinished();
        void nsLookupFinished();
        void txtLookupFinished();
        void srvLookupFinished();
        //void anyLookupFinished();

    signals:
        void quitThread();
        void scanLog(QString log);
        void done();
        void doLookup();
};

/********************************************************************************
                            LOOKUPS ENUMERATOR
*********************************************************************************/

class Enumerator_lookup : public QObject{
    Q_OBJECT

    private:
        int m_currentItemToEnumerate = 0;
        scanArguments_lookup* m_scanArguments;
        scanResults_lookup* m_scanResults;

    public:
        Enumerator_lookup(scanArguments_lookup *scanArguments, scanResults_lookup *scanResults);
        ~Enumerator_lookup();
        //...
        void Enumerate_lookup(QThread *cThread);
        void Enumerate_reverseLookup(QThread *cThread);

    public slots:
        void onStop();

    private slots:
        void hostnameLookup();
        void reverseLookup();
        void onReverseLookupFinished(QHostInfo info);

    signals:
        void performAnotherReverseLookup();
        //...
        void scanLog(QString log);
        void quitThread();
};

#endif // DNSENUMERATOR_H
