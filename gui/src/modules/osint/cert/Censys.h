#ifndef CENSYS_H
#define CENSYS_H

#include "../AbstractOsintModule.h"


namespace ModuleInfo {
    struct Censys{
        QString name = "Censys";
        QString url = "https://censys.io/";
        QString url_apiDoc = "https://censys.io/api/v1/docs/";
        QString summary = "Censys reduces your Internet attack surface by continually discovering unknown assets and helping remediate Internet facing risks";

        QMap<QString, QStringList> flags = {{"Account",
                                            {PLACEHOLDERTEXT_NONE, "The account endpoint returns information about your Censys account."}},
                                            {"View Websites",
                                             {PLACEHOLDERTEXT_DOMAIN, "The View API exposes a single endpoint, /api/v1/view, which can be used to fetch full document from domain."}},
                                            {"View Ipv4",
                                             {PLACEHOLDERTEXT_IP4, "The View API exposes a single endpoint, /api/v1/view, which can be used to fetch full document from IP address"}},
                                            {"View Certificate",
                                             {PLACEHOLDERTEXT_SSLCERT, "The View API exposes a single endpoint, /api/v1/view, which can be used to fetch full document from certificate"}}};

        QMap<int, QList<int>> input_output = {{IN_DOMAIN,
                                               {OUT_SUBDOMAIN, OUT_SSLCERT}},
                                              {IN_IP,
                                               {OUT_SUBDOMAIN, OUT_SSLCERT}},
                                              {IN_SSLCERT,
                                               {OUT_SUBDOMAIN, OUT_SSLCERT}}};
    };
}
class Censys: public AbstractOsintModule{

    public:
        Censys(ScanArgs *args);
        ~Censys() override;

    public slots:
        void start() override;
        void replyFinishedSubdomain(QNetworkReply *reply) override;
        void replyFinishedSSLCert(QNetworkReply *reply) override;

    private:
        QString m_uid;
        QString m_key;
};

#endif // CENSYS_H
