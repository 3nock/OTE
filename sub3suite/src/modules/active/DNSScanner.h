/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#ifndef DNSRECORDSSCANNER_H
#define DNSRECORDSSCANNER_H

#include <QMutex>
#include <QQueue>

#include "AbstractScanner.h"
#include "src/models/DNSModel.h"


namespace dns {

enum OUTPUT { // scan type
    A,
    AAAA,
    MX,
    NS,
    CNAME,
    TXT,
    SRV
};

struct ScanStat {  // scan statistics
    int nameservers = 0;
    int targets = 0;
    int threads = 0;
    int resolved = 0;
    int failed = 0;
};

struct ScanConfig { // scan configurations
    QStringList nameservers;
    int threads = 50;

    bool noDuplicates = false;
    bool autoSaveToProject = false;
};

struct ScanArgs {   // scan arguments
    QMutex mutex;
    QQueue<QString> targets;
    dns::ScanConfig *config;
    QStringList srvWordlist;
    int progress;

    bool RecordType_a;
    bool RecordType_aaaa;
    bool RecordType_mx;
    bool RecordType_ns;
    bool RecordType_txt;
    bool RecordType_cname;
    bool RecordType_srv;
};

class Scanner: public AbstractScanner {
    Q_OBJECT

    public:
        Scanner(dns::ScanArgs *args);
        ~Scanner() override;

    private slots:
        void lookup() override;

        void srvLookupFinished();
        void aLookupFinished();
        void aaaaLookupFinished();
        void mxLookupFinished();
        void cnameLookupFinished();
        void nsLookupFinished();
        void txtLookupFinished();

    signals:
        void scanResult(s3s_struct::DNS);
        void finish();
        void next();

    private:
        int m_activeLookups = 0;
        QString m_currentTarget;
        int m_currentSrvWordlist = 0;

        dns::ScanArgs *m_args;

        QDnsLookup *m_dns_a;
        QDnsLookup *m_dns_aaaa;
        QDnsLookup *m_dns_mx;
        QDnsLookup *m_dns_ns;
        QDnsLookup *m_dns_txt;
        QDnsLookup *m_dns_cname;
        QDnsLookup *m_dns_srv;
};

QString getTarget(dns::ScanArgs *args);

}
#endif // DNSRECORDSSCANNER_H
