/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#ifndef IPENUMERATOR_H
#define IPENUMERATOR_H

#include "AbstractScanner.h"
#include <QHostInfo>

namespace ip{

struct ScanConfig{
    QDnsLookup::Type dnsRecordType = QDnsLookup::A;
    bool useCustomNameServers = false;
    QStringList customNameServers;
    int threadsCount = 50;

    bool checkWildcard = false;
    bool hasWildcard = false;
};

struct ScanArgs{
    ip::ScanConfig *config;
    QStringList targetList;

    int progress;
    int currentTargetToEnumerate;
};


class Scanner : public AbstractScanner{
    Q_OBJECT

    public:
        Scanner(ip::ScanArgs *args);
        ~Scanner() override;

    private slots:
        void lookup() override;
        void lookupFinished(QHostInfo info);

    signals:
        void anotherLookup();
        void scanResult(QString subdomain, QString ipAddress);

    private:
        int m_currentTargetToEnumerate = 0;
        ip::ScanArgs *m_args;
        QHostInfo *hostInfo;
};
}
#endif // IPENUMERATOR_H
