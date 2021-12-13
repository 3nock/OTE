#ifndef RAPIDDNS_H
#define RAPIDDNS_H

#include "../AbstractOsintModule.h"


namespace ModuleInfo {
struct Rapiddns{
    QString name = "Rapiddns";
    QString url = "https://rapiddns.io/";
    QString url_apiDoc = "";
    QString summary = "RapidDNS is a dns query tool which make querying subdomains or sites of a same ip easy!";
    QMap<QString, QStringList> flags = {};
    QMap<int, QList<int>> input_output = {{IN_DOMAIN,
                                           {OUT_SUBDOMAINIP, OUT_SUBDOMAIN, OUT_IP}},
                                          {IN_IP,
                                           {OUT_SUBDOMAINIP, OUT_SUBDOMAIN, OUT_IP}},
                                          {IN_CIDR,
                                           {OUT_SUBDOMAINIP, OUT_SUBDOMAIN, OUT_IP}}};
};
}

class Rapiddns: public AbstractOsintModule{

    public:
        Rapiddns(ScanArgs args);
        ~Rapiddns() override;

    public slots:
        void start() override;
        void replyFinishedSubdomain(QNetworkReply *reply) override;
        void replyFinishedSubdomainIp(QNetworkReply *reply) override;
        void replyFinishedIp(QNetworkReply *reply) override;
};

#endif // RAPIDDNS_H
