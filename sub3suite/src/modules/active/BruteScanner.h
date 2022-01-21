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
#include "AbstractScanner.h"


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

    private slots:
        void lookup() override;
        void lookupFinished();

    signals:
        void next(); // next lookup
        void nextLevel(); // send signal, going to the next level
        void newProgress(int); // send new progressBar max value
        void scanResult(QString subdomain, QString ip); // lookup results

    private:
        brute::ScanArgs *m_args;
        QDnsLookup *m_dns;
};

RETVAL lookupSubdomain(QDnsLookup*, ScanArgs*);
RETVAL lookupTLD(QDnsLookup*, ScanArgs*);
RETVAL lookupReScan(QDnsLookup*, ScanArgs*);

}
#endif //BRUTE_H
