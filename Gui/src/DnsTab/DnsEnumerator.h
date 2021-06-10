#ifndef DNSENUMERATOR_H
#define DNSENUMERATOR_H

#include "lib-dns.h"

/***********************************************************************************
                               DNS-RECORDS ENUMERATOR
************************************************************************************/

class Enumerator_dnsRecords: public QObject{
    Q_OBJECT

    public:
        Enumerator_dnsRecords(scanArguments_dnsRecords *scanArguments, scanResults_dnsRecords *scanResults);
        ~Enumerator_dnsRecords();
        //...
        void Enumerate(QThread *cThread);
        int finishedLookups = 0;
        bool firstToResolve = true;
        //...
        scanArguments_dnsRecords *scanArguments;
        scanResults_dnsRecords *scanResults;
        //...
        QStandardItem *dnsNameItem;
        QStandardItem *recordItem;
        //...
        QHostAddress nameserver;
        QDnsLookup *dns_mx;
        QDnsLookup *dns_ns;
        QDnsLookup *dns_cname;
        QDnsLookup *dns_txt;
        QDnsLookup *dns_srv;
        //QDnsLookup *dns_any;
        //...
        int currentTargetToEnumerate = 0;
        QString currentTarget;

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
