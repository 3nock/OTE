#ifndef DNSENUMERATOR_H
#define DNSENUMERATOR_H

#include "lib-dns.h"
#include <QHostInfo>

/***********************************************************************************
                               DNS-RECORDS ENUMERATOR
************************************************************************************/

class DnsRecordsEnumerator: public QObject{
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
        QDnsLookup *m_dns_cname;
        //...
        int m_currentTargetToEnumerate = 0;
        int m_finishedLookups = 0;
        bool m_firstToResolve = true;
        //...
        QString m_currentTarget;

    public:
        DnsRecordsEnumerator(scanArguments_dnsRecords *scanArguments, scanResults_dnsRecords *scanResults);
        ~DnsRecordsEnumerator();
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
        //void anyLookupFinished();

    signals:
        void progressBarValue(int value);
        void quitThread();
        void scanLog(QString log);
        void done();
        void doLookup();
};

#endif // DNSENUMMERATOR_H
