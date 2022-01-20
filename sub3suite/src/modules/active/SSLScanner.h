/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief : the Scanner establishes secure TLS connetion with particular
          target & a specified port that uses SSL, then obtains SSL Certificate
          & extracts valuable info from the SSL-Certificate.
*/

#ifndef SSLSCANNER_H
#define SSLSCANNER_H

#include <QMutex>
#include <QQueue>
#include <QSslSocket>
#include <QSslCertificate>
#include "AbstractScanner.h"

/* other ports that uses SSL
3306            MySQL
110             POP – Incoming
995             POP SSL – Incoming
143             IMAP – Incoming
993             IMAP SSL – Incoming
25, 80, 3535	SMTP – Outgoing
465             SMTP SSL – Outgoing
*/

struct SSL_struct{
    QString target;
};

namespace ssl {

enum PORT { // ssl port
    HTTPS,
    FTP,
    FTPs,
    SSH
};

enum OUTPUT { // scan type
    RAW,
    SHA1,
    SHA256,
    SUBDOMAIN
};

struct ScanStat {  // scan statistics
    int targets = 0;
    int threads = 0;
    int resolved = 0;
    int failed = 0;
};

struct ScanConfig { // scan configurations
    int threads = 50;
    int timeout = 3000;

    bool noDuplicates = false;
    bool autoSaveToProject = false;
};

struct ScanArgs { // scan arguments
    QMutex mutex;
    ssl::PORT port;
    ssl::OUTPUT output;
    ssl::ScanConfig *config;
    QQueue<QString> targets;
    int progress;
};


class Scanner : public AbstractScanner{
    Q_OBJECT

    public:
        Scanner(ssl::ScanArgs *args);
        ~Scanner() override;

    signals:
        void resultSHA1(QString sha1);
        void resultSHA256(QString sha256);
        void resultSubdomain(QString target, QStringList subdomain);
        void resultRaw(QString target, QSslCertificate certificate);

    private slots:
        void lookup() override;

    private:
        ssl::ScanArgs *m_args;
};

QString getTarget(ssl::ScanArgs *args);

}

#endif // CERTSCANNER_H
