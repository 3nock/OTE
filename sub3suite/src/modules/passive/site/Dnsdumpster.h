#ifndef DNSDUMPSTER_H
#define DNSDUMPSTER_H

#include "../AbstractOsintModule.h"


namespace ModuleInfo {
struct Dnsdumpster{
    QString name = OSINT_MODULE_DNSDUMPSTER;
    QString url = "https://dnsdumpster.com/";
    QString url_apiDoc = "";
    QString summary = "";
    QMap<QString, QStringList> flags = {};
    QMap<int, QList<int>> input_output = {{IN_DOMAIN,
                                           {OUT_SUBDOMAINIP, OUT_SUBDOMAIN, OUT_IP}}};
};
}

class Dnsdumpster: public AbstractOsintModule{

    public:
        explicit Dnsdumpster(ScanArgs args);
        ~Dnsdumpster() override;

    public slots:
        void start() override;
        void replyFinishedSubdomainIp(QNetworkReply *reply) override;
        void replyFinishedSubdomain(QNetworkReply *reply) override;
        void replyFinishedIp(QNetworkReply *reply) override;

    private:
        bool m_queryToGetToken;
        void m_getToken(QNetworkReply *reply);
};

#endif // DNSDUMPSTER_H
