/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief : resolving dns records for hostnames
*/

#ifndef DNSRECORDSSCANNER_H
#define DNSRECORDSSCANNER_H

#include <QMutex>
#include <QQueue>

#include "AbstractScanner.h"
#include "src/items/DNSItem.h"


namespace dns {

struct ScanStat {  // scan statistics
    int nameservers = 0;
    int targets = 0;
    int threads = 0;
    int resolved = 0;
    int failed = 0;
};

struct ScanConfig { // scan configurations
    QQueue<QString> nameservers;
    int threads = 50;
    int timeout = 1000;

    bool setTimeout = false;
    bool noDuplicates = false;
    bool autoSaveToProject = false;
};

struct ScanArgs {   // scan arguments
    QMutex mutex;
    QQueue<QString> targets;
    dns::ScanConfig *config;
    QStringList srvWordlist;
    QString currentTarget;
    int currentSRV;
    int progress;

    bool RecordType_a;
    bool RecordType_aaaa;
    bool RecordType_mx;
    bool RecordType_ns;
    bool RecordType_txt;
    bool RecordType_cname;
    bool RecordType_srv;
    bool RecordType_any;
};

class Scanner: public AbstractScanner {
    Q_OBJECT

    public:
        explicit Scanner(dns::ScanArgs *args);
        ~Scanner() override;

    private slots:
        void lookup() override;
        void lookupFinished_srv();
        void lookupFinished_a();
        void lookupFinished_aaaa();
        void lookupFinished_mx();
        void lookupFinished_ns();
        void lookupFinished_txt();
        void lookupFinished_cname();
        void lookupFinished_any();

    signals:
        void scanResult(s3s_struct::DNS);
        void next();

    private:
        int m_activeLookups = 0;

        bool has_record;
        QString m_currentTarget;

        dns::ScanArgs *m_args;
        s3s_struct::DNS m_result;

        QDnsLookup *m_dns_a;
        QDnsLookup *m_dns_aaaa;
        QDnsLookup *m_dns_mx;
        QDnsLookup *m_dns_ns;
        QDnsLookup *m_dns_txt;
        QDnsLookup *m_dns_cname;
        QDnsLookup *m_dns_srv;
        QDnsLookup *m_dns_any;
};

QString getTarget(dns::ScanArgs *args);
RETVAL getTarget_srv(QDnsLookup *dns, dns::ScanArgs *args);

}
#endif // DNSRECORDSSCANNER_H
