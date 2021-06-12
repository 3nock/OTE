#ifndef DNSENUMERATOR_H
#define DNSENUMERATOR_H

#include "lib-dns.h"

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
        void mxLookupFinished();
        void cnameLookupFinished();
        void nsLookupFinished();
        void txtLookupFinished();
        void srvLookupFinished();
        //void anyLookupFinished();

    signals:
        void quitThread();
        void scanlogs(QString log);
        void done();
        void doLookup();
};

#endif // DNSENUMERATOR_H
