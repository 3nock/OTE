#ifndef CRTSH_H
#define CRTSH_H

#include "../AbstractOsintModule.h"
#include "src/items/SSLItem.h"


namespace ModuleInfo {
struct Crtsh{
    QString name = OSINT_MODULE_CRTSH;
    QString url = "https://crt.sh";
    QString url_apiDoc = "https://crt.sh";
    QString summary = "crt.sh";
    QMap<QString, QStringList> flags = {{"domain",
                                         {PLACEHOLDERTEXT_DOMAIN, "Search by Domain"}},
                                        {"ssl cert",
                                         {PLACEHOLDERTEXT_SSLCERT, "Search by ssl certificate: sha1/sha256/id"}}};

    QMap<int, QList<int>> input_output = {{IN_DOMAIN,
                                           {OUT_SUBDOMAIN, OUT_SSLCERT}}};
};
}

class Crtsh: public AbstractOsintModule{
    public:
        explicit Crtsh(ScanArgs args);
        ~Crtsh() override;

    public slots:
        void start() override;
        void replyFinishedSubdomain(QNetworkReply *reply) override;
        void replyFinishedSSL(QNetworkReply *reply) override;
        void replyFinishedEnumSSL(QNetworkReply *reply) override;

    private:
        void m_getCertId(QNetworkReply *reply);
        bool m_queryToGetId = true;
};


#endif // CRTSH_H
