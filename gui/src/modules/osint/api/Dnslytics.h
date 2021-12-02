#ifndef DNSLYTICS_H
#define DNSLYTICS_H

#include "../AbstractOsintModule.h"


namespace ModuleInfo {
struct Dnslytics{
    QString name = "Dnslytics";
    QString url = "https://dnslytics.com/";
    QString url_apiDoc = "https://dnslytics.com/api";
    QString summary = "This service is provided by a Dutch ICT consultant. My goal is to provide webmasters, system administrators, forensic specialist or "
                      "anyone who is interested the information they need to view the technical details about internet, offer diagnostic tools and show information to track spammers, hackers or other suspicious activities.";

    QMap<QString, QStringList> flags = {{"AccountInfo",
                                         {PLACEHOLDERTEXT_NONE, "The AccountInfo API call returns the current status of your API account."}},
                                        {"ASInfo",
                                         {PLACEHOLDERTEXT_ASN, "The ASInfo API call retrieves information about an AS number. It is based on the AS/BGP report displayed on the website"}},
                                        {"DomainSearch",
                                         {PLACEHOLDERTEXT_DOMAIN, "This API call returns registered and previously registered domain names based on one or more keywords."}},
                                        {"DomainTypos",
                                         {PLACEHOLDERTEXT_DOMAIN, "This API call returns registered domain typos and misspellings for a domain name. A domain name with one character difference is defined as a typo. "
                                                                  "For example ddslytics.com is a typo domain for dnslytics.com. It does not return exact matched domains on other TLDs, use the DomainSearch API to find such domain names."}},
                                        {"HostingHistory",
                                         {PLACEHOLDERTEXT_DOMAIN, "This API call returns the IP/DNS history for a specific domain. The following records are supported: A, AAAA, MX, NS and SPF."}},
                                        {"IPInfo",
                                         {PLACEHOLDERTEXT_IP, "The IPInfo API call retrieves information about an IP address. It is based on the IP address report displayed on the website"}},
                                        {"IP2ASN",
                                         {PLACEHOLDERTEXT_IP, "The IP2ASN API call retrieves ASN & Subnet info about an IP address (IPv4 and IPv6)."}},
                                        {"ReverseAdsense",
                                         {"Adsense PUB ID eg pub-7232066202917795 or domain name", "This API call returns domains using the same Google Adsense ID"}},
                                        {"ReverseGAnalytics",
                                         {"Analytics ID eg ua-15589237 or domain name", "This API call returns domains using the same Google Analytics ID"}},
                                        {"ReverseIP",
                                         {PLACEHOLDERTEXT_IP, "The ReverseIP API call returns domains hosted on the same IP address."}},
                                        {"ReverseMX",
                                         {PLACEHOLDERTEXT_MX, "The ReverseMX API call returns domains hosted on the same mail server (MX record)."}},
                                        {"ReverseNS",
                                         {PLACEHOLDERTEXT_NS, "The ReverseNS API call returns domains hosted on the same name server (NS record)."}},
                                        {"SubnetInfo",
                                         {PLACEHOLDERTEXT_CIDR, "The SubnetInfo API call retrieves information about a subnet. It is based on the IP subnet report displayed on the website."
                                          "We calculate statistics of all subnets allocated by the RIRs (ARIN, RIPE etc.) and subnets part of the global routing table. In total about 700K IPv4 subnets."}}};

    QMap<int, QList<int>> input_output = {{IN_DOMAIN,
                                           {OUT_SUBDOMAINIP, OUT_SUBDOMAIN, OUT_IP}},
                                          {IN_IP,
                                           {OUT_SUBDOMAINIP, OUT_SUBDOMAIN, OUT_IP, OUT_ASN, OUT_CIDR}}};
};
}

class Dnslytics: public AbstractOsintModule{

    public:
        Dnslytics(ScanArgs *args);
        ~Dnslytics() override;

    public slots:
        void start() override;
        void replyFinishedSubdomain(QNetworkReply *reply) override;
        void replyFinishedSubdomainIp(QNetworkReply *reply) override;
        void replyFinishedAsn(QNetworkReply *reply) override;
        void replyFinishedIp(QNetworkReply *reply) override;
        void replyFinishedCidr(QNetworkReply *reply) override;

    private:
        QString m_key;
};
#endif // DNSLYTICS_H
