#ifndef OTXFREE_H
#define OTXFREE_H

#include "../AbstractOsintModule.h"

/*
 * INPUT domain:            OUTPUT: subdomainIp, subdomain, asn, ip
 * INPUT ip:                OUTPUT: subdomainIp, subdomain, asn, ip
 */

namespace ModuleInfo {
    struct OtxFree{
        QString name = "Otx";
        QString url = "https://otx.alienvault.com/";
        QString url_apiDoc = "";
        QString summary = "The World’s First Truly Open Threat Intelligence Community";

        QMap<QString, QStringList> flags = {{"indicator ipv4 general", {PLACEHOLDERTEXT_IP4, ""}},
                                        {"indicator ipv4 geo", {PLACEHOLDERTEXT_IP4, ""}},
                                        {"indicator ipv4 malware", {PLACEHOLDERTEXT_IP4, ""}},
                                        {"indicator ipv4 passive_dns", {PLACEHOLDERTEXT_IP4, ""}},
                                        {"indicator ipv4 reputation", {PLACEHOLDERTEXT_IP4, ""}},
                                        {"indicator ipv4 url_list", {PLACEHOLDERTEXT_IP4, ""}},
                                        {"indicator ipv6 general", {PLACEHOLDERTEXT_IP6, ""}},
                                        {"indicator ipv6 geo", {PLACEHOLDERTEXT_IP6, ""}},
                                        {"indicator ipv6 malware", {PLACEHOLDERTEXT_IP6, ""}},
                                        {"indicator ipv6 passive_dns", {PLACEHOLDERTEXT_IP6, ""}},
                                        {"indicator ipv6 reputation", {PLACEHOLDERTEXT_IP6, ""}},
                                        {"indicator ipv6 url_list", {PLACEHOLDERTEXT_IP6, ""}},
                                        {"indicator domain general", {PLACEHOLDERTEXT_DOMAIN, ""}},
                                        {"indicator domain geo", {PLACEHOLDERTEXT_DOMAIN, ""}},
                                        {"indicator domain http_scans", {PLACEHOLDERTEXT_DOMAIN, ""}},
                                        {"indicator domain malware", {PLACEHOLDERTEXT_DOMAIN, ""}},
                                        {"indicator domain passive_dns", {PLACEHOLDERTEXT_DOMAIN, ""}},
                                        {"indicator domain url_list", {PLACEHOLDERTEXT_DOMAIN, ""}},
                                        {"indicator domain whois", {PLACEHOLDERTEXT_DOMAIN, ""}},
                                        {"indicator hostname general", {PLACEHOLDERTEXT_HOSTNAME, ""}},
                                        {"indicator hostname geo", {PLACEHOLDERTEXT_HOSTNAME, ""}},
                                        {"indicator hostname http_scans", {PLACEHOLDERTEXT_HOSTNAME, ""}},
                                        {"indicator hostname malware", {PLACEHOLDERTEXT_HOSTNAME, ""}},
                                        {"indicator hostname passive_dns", {PLACEHOLDERTEXT_HOSTNAME, ""}},
                                        {"indicator hostname url_list", {PLACEHOLDERTEXT_HOSTNAME, ""}},
                                        {"indicator url general", {PLACEHOLDERTEXT_URL, ""}},
                                        {"indicator url url_list", {PLACEHOLDERTEXT_URL, ""}}};
    };
}

class OtxFree: public AbstractOsintModule{

    public:
        OtxFree(ScanArgs *args);
        ~OtxFree() override;

    public slots:
        void start() override;
        void replyFinishedSubdomainIp(QNetworkReply *reply) override;
        void replyFinishedSubdomain(QNetworkReply *reply) override;
        void replyFinishedIp(QNetworkReply *reply) override;
        void replyFinishedAsn(QNetworkReply *reply) override;
};

#endif // OTXFREE_H
