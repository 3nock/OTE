#ifndef CRTSH_H
#define CRTSH_H

#include "../AbstractOsintModule.h"
#include "gumbo-parser/src/gumbo.h"
#include <QStack>

namespace ModuleInfo {
    struct Crtsh{
        QString name = "Crtsh";
        QString url = "https://crt.sh";
        QString url_apiDoc = "https://crt.sh";
        QString summary = "crt.sh";
        QMap<QString, QStringList> flags = {{"domain",
                                             {PLACEHOLDERTEXT_DOMAIN, "Search by Domain"}},
                                            {"ssl cert",
                                             {PLACEHOLDERTEXT_SSLCERT, "Search by ssl certificate: sha1/sha256/id"}}};
    };
}

class Crtsh: public AbstractOsintModule{

    public:
        Crtsh(ScanArgs *args);
        ~Crtsh() override;

    public slots:
        void start() override;
        void replyFinishedSubdomain(QNetworkReply *reply) override;
        void replyFinishedSSLCert(QNetworkReply *reply) override;
        void replyFinishedInfo(QNetworkReply *reply) override;

    private:
        void m_getCertId(QNetworkReply *reply);
        bool m_queryToGetId = true;
};


#endif // CRTSH_H
