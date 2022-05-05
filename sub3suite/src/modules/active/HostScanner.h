/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief : class for resolving hostnames
*/

#ifndef HOSTSCANNER_H
#define HOSTSCANNER_H

#include <QTcpSocket>
#include <QMutex>
#include <QQueue>
#include "AbstractScanner.h"
#include "src/items/HostItem.h"

#include "PortScanner.h"
#include "PingScanner.h"

namespace host {

struct ScanStat {  // scan statistics
    int nameservers = 0;
    int targets = 0;
    int threads = 0;
    int resolved = 0;
    int failed = 0;
};

struct ScanConfig { // scan configurations
    QDnsLookup::Type recordType = QDnsLookup::A;
    QQueue<QString> nameservers;
    port::ScanArgs *portScanConfig;
    ping::ScanArgs *pingScanConfig;
    int threads = 50;
    int timeout = 3000;

    bool setTimeout = false;
    bool noDuplicates = false;
    bool autoSaveToProject = false;
};

struct ScanArgs { // scan arguments
    QMutex mutex;
    host::ScanConfig *config;
    QQueue<QString> targets;
    QHostAddress nameserver;
    int progress;
};


class Scanner : public AbstractScanner{
    Q_OBJECT

    public:
        explicit Scanner(host::ScanArgs *args);
        ~Scanner() override;

    private slots:
        void lookup() override;
        void lookupFinished();

    signals:
        void next(); // next lookup
        void scanResult(s3s_struct::HOST host); // send active enumerated results

    private:
        host::ScanArgs *m_args;
        QDnsLookup *m_dns;
};

RETVAL getTarget(QDnsLookup *dns, host::ScanArgs *args);

}

#endif // HOSTSCANNER_H
