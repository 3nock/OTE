/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#ifndef BRUTESCANNER_H
#define BRUTESCANNER_H

#include <QMutex>
#include <QQueue>
#include <QDnsLookup>
#include <QHostAddress>
#include <QWaitCondition>

#include "AbstractScanner.h"
#include "src/models/HostModel.h"
#include "src/models/WildcardModel.h"


namespace brute {

enum OUTPUT{ // scan type
    SUBDOMAIN,
    TLD
};

struct ScanStat{  // scan statistics
    int nameservers = 0;
    int wordlist = 0;
    int targets = 0;
    int threads = 0;
    int resolved = 0;
    int failed = 0;
};

struct ScanConfig{ // scan configurations
    QDnsLookup::Type recordType = QDnsLookup::A;
    QStringList nameservers;
    int levels = 0;
    int threads = 50;

    bool noDuplicates = false;
    bool autoSaveToProject = false;
    bool multiLevelScan = false;
    bool checkWildcard = false;
};

struct ScanArgs { // scan arguments
    QMutex mutex;
    QHostAddress nameserver;
    brute::OUTPUT output;
    brute::ScanConfig *config;
    QQueue<QString> nextLevelTargets;
    QQueue<QString> targets;
    QStringList wordlist;
    QString currentTarget;
    int currentWordlist;
    int currentLevel;
    int progress;
    bool reScan;
};

class Scanner : public AbstractScanner{
    Q_OBJECT

    public:
        Scanner(brute::ScanArgs *args);
        ~Scanner() override;

        void lookup_wildcard();

    private slots:
        void lookup() override;
        void lookupFinished();
        void lookupFinished_wildcard();

    signals:
        void next(); // next lookup
        void nextLevel(); // send signal, going to the next level
        void newProgress(int); // send new progressBar max value
        void scanResult(s3s_struct::HOST host); // lookup results
        void wildcard(s3s_struct::Wildcard wildcard); // found wildcard

    private:
        brute::ScanArgs *m_args;
        QDnsLookup *m_dns;
        QDnsLookup *m_dns_wildcard;
        QMutex m_mutex;
        QWaitCondition m_wait;

        /* for wildcards */
        bool has_wildcards = true;
        QString wildcard_ip;
};

RETVAL getTarget_subdomain(brute::Scanner*, QDnsLookup*, ScanArgs*);
RETVAL getTarget_tld(QDnsLookup*, ScanArgs*);
RETVAL getTarget_reScan(QDnsLookup*, ScanArgs*);

}
#endif //BRUTE_H
