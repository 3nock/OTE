/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#ifndef BANNERSCANNER_H
#define BANNERSCANNER_H

#include <QTcpSocket>
#include <QStack>
#include "AbstractScanner.h"
#include "src/dialogs/ActiveConfigDialog.h"


namespace banner {

struct ScanConfig{
    QDnsLookup::Type dnsRecordType = QDnsLookup::A;
    bool useCustomNameServers = false;
    QStringList customNameServers;
    int threadsCount = 50;
    int timeout = 3000;

    bool checkWildcard = false;
    bool hasWildcard = false;
};

struct ScanArgs{
    banner::ScanConfig *config;
    QString target;
    QStack<QString> targetList;
    int progress;

    /* target type */
    bool hostname = false;
    bool ipaddress = false;

    /* service to test */
    bool http = false;
    bool https = false;
    bool ftp = false;
    bool ssh = false;
    bool smtp = false;
};

class Scanner : public AbstractScanner{
    public:
        Scanner(banner::ScanArgs *args);
        ~Scanner() override;

    private slots:
        void lookup() override;

    private:
        banner::ScanArgs *m_args;
        QTcpSocket *m_socket;
};
}
#endif // BANNERSCANNER_H
