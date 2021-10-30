#ifndef HACKERTARGETFREE_H
#define HACKERTARGETFREE_H


#include "../AbstractOsintModule.h"

/*
 * INPUT domain:            OUTPUT: subdomainIp, subdomain, ip
 * INPUT ip-address:        OUTPUT: asn, subdomain
 */

namespace ModuleInfo {
    struct HackerTargetFree{
        QString name = "HackerTarget";
        QString url = "https://hackertarget.com/";
        QString url_apiDoc = "";
        QString summary = "From attack surface discovery to vulnerability identification, actionable network intelligence \n"
                          "for IT & security operations.";

        QMap<QString, QStringList> flags = {{"dnslookup", {PLACEHOLDERTEXT_DOMAIN, ""}},
                                        {"banner grabbing", {PLACEHOLDERTEXT_CIDR, ""}},
                                        {"find shared dns", {PLACEHOLDERTEXT_NS, ""}},
                                        {"hostsearch", {PLACEHOLDERTEXT_DOMAIN, ""}},
                                        {"subnet calculator", {PLACEHOLDERTEXT_CIDR, ""}},
                                        {"zonetransfer", {PLACEHOLDERTEXT_DOMAIN, ""}},
                                        {"reverse dns", {PLACEHOLDERTEXT_IP, ""}},
                                        {"geoip", {PLACEHOLDERTEXT_IP, ""}},
                                        {"reverse ip lookup", {PLACEHOLDERTEXT_IP, ""}},
                                        {"httpheaders", {PLACEHOLDERTEXT_DOMAIN, ""}},
                                        {"pagelinks", {PLACEHOLDERTEXT_DOMAIN, ""}},
                                        {"aslookup", {PLACEHOLDERTEXT_IP, ""}}};
    };
}

class HackerTargetFree: public AbstractOsintModule{

    public:
        HackerTargetFree(ScanArgs *args);
        ~HackerTargetFree() override;

    public slots:
        void start() override;
        void replyFinishedSubdomainIp(QNetworkReply *reply) override;
        void replyFinishedSubdomain(QNetworkReply *reply) override;
        void replyFinishedIp(QNetworkReply *reply) override;
        void replyFinishedAsn(QNetworkReply *reply) override;
};


#endif // HACKERTARGETFREE_H
