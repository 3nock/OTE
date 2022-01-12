/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#ifndef ACTIVESCANNER_H
#define ACTIVESCANNER_H

#include <QTcpSocket>
#include <QMutex>
#include <QQueue>
#include "AbstractScanner.h"


namespace active {

enum OUTPUT { // scan type
    SUBOMAIN,
    DNS,
    HTTP,
    HTTPS,
    FTP,
    SMTP,
    CUSTOM
};

struct ScanStat {  // scan statistics
    int nameservers = 0;
    int targets = 0;
    int threads = 0;
    int resolved = 0;
    int failed = 0;
};

struct ScanConfig { // scan configurations
    QDnsLookup::Type recordType = QDnsLookup::A;
    QStringList nameservers;
    int threads = 50;
    int timeout = 3000;

    bool noDuplicates = false;
    bool autoSaveToProject = false;
    bool checkWildcard = false;
};

struct ScanArgs { // scan arguments
    QMutex mutex;
    active::OUTPUT output;
    active::ScanConfig *config;
    QQueue<QString> targets;
    QHostAddress nameserver;
    int progress;

    quint16 service;
    bool checkActiveService;
};


class Scanner : public AbstractScanner{
    Q_OBJECT

    public:
        Scanner(active::ScanArgs *args);
        ~Scanner() override;


    private slots:
        void lookup() override;
        void lookupFinished();

    signals:
        void next(); // next lookup
        void scanResult(QString domain, QString ip); // send active enumerated results

    private:
        active::ScanArgs *m_args;
        QDnsLookup *m_dns;
        QTcpSocket *m_socket;
};

RETVAL lookupActiveDNS(QDnsLookup *dns, active::ScanArgs *args);
RETVAL lookupActiveService(active::ScanArgs *args);

}

#endif // ACTIVESCANNER_H
