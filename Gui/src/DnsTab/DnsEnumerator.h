#ifndef DNSENUMERATOR_H
#define DNSENUMERATOR_H

#include "lib-dns.h"

/***********************************************************************************
                               DNS-RECORDS ENUMERATOR
************************************************************************************/

class DnsRecordsEnumerator: public QObject{
    Q_OBJECT

    private:
        ScanConfig *m_scanConfig;
        ScanArguments_Records *m_scanArguments;
        ScanResults_Records *m_scanResults;
        //...
        QStandardItem *m_dnsNameItem;
        QStandardItem *m_recordItem;
        //...
        QDnsLookup *m_dns_srv;
        QDnsLookup *m_dns_a;
        QDnsLookup *m_dns_aaaa;
        QDnsLookup *m_dns_mx;
        QDnsLookup *m_dns_ns;
        QDnsLookup *m_dns_txt;
        QDnsLookup *m_dns_cname;
        //...
        int m_currentTargetToEnumerate = 0;
        int m_currentSrvToEnumerate = 0;
        //...
        int m_activeLookups = 0;
        bool m_firstToResolve = true;
        //...
        QString m_currentTarget;

    public:
        DnsRecordsEnumerator(ScanConfig *scanConfig, ScanArguments_Records *scanArguments, ScanResults_Records *scanResults);
        ~DnsRecordsEnumerator();
        //...
        void enumerate(QThread *cThread);
        void enumerate_srv(QThread *cThread);

    public slots:
        void onStop();

    private slots:
        void lookup();
        void lookup_srv();
        //...
        void srvLookupFinished();
        void aLookupFinished();
        void aaaaLookupFinished();
        void mxLookupFinished();
        void cnameLookupFinished();
        void nsLookupFinished();
        void txtLookupFinished();

    signals:
        void progress(int value);
        void quitThread();
        void scanLog(QString log);
        void done();
        //...
        void doLookup();
        void doLookup_srv();
};

#endif // DNSENUMMERATOR_H
