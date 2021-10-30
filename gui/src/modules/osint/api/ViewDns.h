#ifndef VIEWDNS_H
#define VIEWDNS_H

#include "../AbstractOsintModule.h"

/*
 * INPUT domain:            OUTPUT: subdomain, email, ip
 * INPUT ip:                OUTPUT: subdomain
 */

namespace ModuleInfo {
    struct ViewDns{
        QString name = "ViewDns";
        QString url = "https://otx.alienvault.com/";
        QString url_apiDoc = "";
        QString summary = "The World’s First Truly Open Threat Intelligence Community";
        QMap<QString, QStringList> flags = {{"Abuse Contact Lookup", {PLACEHOLDERTEXT_DOMAIN, ""}},
                                        {"DNS Propagation Checker", {PLACEHOLDERTEXT_DOMAIN, ""}},
                                        {"DNS Record Lookup", {PLACEHOLDERTEXT_DOMAIN, ""}},
                                        {"Domain/IP Whois", {PLACEHOLDERTEXT_DOMAIN_OR_IP, ""}},
                                        {"IP History", {PLACEHOLDERTEXT_DOMAIN, ""}},
                                        {"IP Location Finder", {PLACEHOLDERTEXT_IP, ""}},
                                        {"MAC Address Lookup", {"Mac address", ""}},
                                        {"Ping", {PLACEHOLDERTEXT_DOMAIN, ""}},
                                        {"Port Scanner", {PLACEHOLDERTEXT_DOMAIN, ""}},
                                        {"Reverse DNS Lookup", {PLACEHOLDERTEXT_IP, ""}},
                                        {"Reverse IP Lookup", {PLACEHOLDERTEXT_IP, ""}},
                                        {"Reverse MX Lookup", {PLACEHOLDERTEXT_MX, ""}},
                                        {"Reverse NS Lookup", {PLACEHOLDERTEXT_NS, ""}},
                                        {"Reverse Whois Lookup", {PLACEHOLDERTEXT_DOMAIN, ""}}};
    };
}

class ViewDns: public AbstractOsintModule{

    public:
        ViewDns(ScanArgs *args);
        ~ViewDns() override;

    public slots:
        void start() override;
        void replyFinishedSubdomain(QNetworkReply *reply) override;
        void replyFinishedEmail(QNetworkReply *reply) override;
        void replyFinishedIp(QNetworkReply *reply) override;

    private:
        QString m_key = nullptr;
};
#endif // VIEWDNS_H
