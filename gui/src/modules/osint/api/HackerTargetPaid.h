#ifndef HACKERTARGETPAID_H
#define HACKERTARGETPAID_H

#include "../AbstractOsintModule.h"

/*
 * INPUT domain:            OUTPUT: subdomainIp, subdomain, ip
 * INPUT ip-address:        OUTPUT: asn, subdomain
 */

namespace ModuleInfo {
    struct HackerTargetPaid{
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
                                        {"aslookup", {PLACEHOLDERTEXT_IP, ""}},
                                        {"ping", {PLACEHOLDERTEXT_IP, ""}},
                                        {"traceroute", {PLACEHOLDERTEXT_IP, ""}},
                                        {"whois", {PLACEHOLDERTEXT_IP, ""}}};
    };
}

class HackerTargetPaid: public AbstractOsintModule{

    public:
        HackerTargetPaid(ScanArgs *args);
        ~HackerTargetPaid() override;

    public slots:
        void start() override;
        void replyFinishedSubdomainIp(QNetworkReply *reply) override;
        void replyFinishedSubdomain(QNetworkReply *reply) override;
        void replyFinishedIp(QNetworkReply *reply) override;
        void replyFinishedAsn(QNetworkReply *reply) override;

    private:
        QString m_key;
};


#endif // HACKERTARGETPAID_H
