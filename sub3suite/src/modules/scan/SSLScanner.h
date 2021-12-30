#ifndef SSLSCANNER_H
#define SSLSCANNER_H

#include <QStack>
#include <QSslCertificate>
#include "AbstractScanner.h"


namespace ssl {

struct ScanConfig{
    int threadsCount = 50;
    int timeout = 3000;
};

struct ScanArgs{
    ssl::ScanConfig *config;
    QString target;
    QStack<QString> targetList;
    bool singleTarget = false;

    /* result type */
    bool raw = false;
    bool sha1 = false;
    bool sha256 = false;
    bool subdomain = false;

    /* protocal to connect to */
    bool https = false;
    bool ssh = false;
    bool ftp = false;

    /* ... */
    int progress = 0;
};

class Scanner : public AbstractScanner{

    public:
        Scanner(ssl::ScanArgs *args);
        ~Scanner() override;

    private slots:
        void lookup() override;

    private:
        ssl::ScanArgs *m_args;
};

}

#endif // CERTSCANNER_H
