#ifndef SECURITYTRAILS_H
#define SECURITYTRAILS_H

#include "../AbstractOsintModule.h"


namespace ModuleInfo {
struct SecurityTrails{
    QString name = OSINT_MODULE_SECURITYTRAILS;
    QString url = "https://securitytrails.com/";
    QString url_apiDoc = "https://securitytrails.com/corp/api";
    QString summary = "The Total Internet Inventory. Powerful tools for third-party risk, \n"
                      "attack surface management, and total intel";

    QMap<QString, QStringList> flags = {{"history dns",
                                         {PLACEHOLDERTEXT_DOMAIN, "Lists out specific historical information about the given hostname parameter"}},
                                        {"history whois",
                                         {PLACEHOLDERTEXT_DOMAIN, "Returns historical WHOIS information about the given domain"}},
                                        {"domain subdomain",
                                         {PLACEHOLDERTEXT_DOMAIN, "Returns child and sibling subdomains for a given hostname."}},
                                        {"domain whois",
                                         {PLACEHOLDERTEXT_DOMAIN, "Returns the current WHOIS data about a given hostname with the stats merged together"}},
                                        {"domain associated domains",
                                         {PLACEHOLDERTEXT_DOMAIN, "Find all domains that are related to a hostname you input."}},
                                        {"domain details",
                                         {PLACEHOLDERTEXT_DOMAIN, "Returns the current data about the given hostname. "
                                                                  "In addition to the current data, you also get the current statistics associated with a particular record."}},
                                        {"domain ssl cert",
                                         {PLACEHOLDERTEXT_DOMAIN, "Fetch current and historical certificate information for any hostname"}},
                                        {"ip neighbours",
                                         {PLACEHOLDERTEXT_IP, "Returns the neighbors in any given IP level range and essentially allows you to explore closeby IP addresses. "
                                                              "It will divide the range into 16 groups. Example: a /28 would be divided into 16 /32 blocks or a /24 would be divided into 16 /28 blocks"}},
                                        {"ip whois",
                                         {PLACEHOLDERTEXT_IP, "Fetch current IP information for a single IPv4 address."}},
                                        {"general ping",
                                         {PLACEHOLDERTEXT_NONE, "You can use this simple endpoint to test your authentication and access to the SecurityTrails API."}},
                                        {"general usage",
                                         {PLACEHOLDERTEXT_NONE, "Usage statistics of the API for the current month"}},
                                        {"company details",
                                         {PLACEHOLDERTEXT_DOMAIN, "Returns details for a company domain."}},
                                        {"company associated ip",
                                         {PLACEHOLDERTEXT_DOMAIN, "Returns associated IPs for a company domain. The result is not paginated nor limited. "
                                                                  "The data is based on whois data with the names matched to the domains."}}};

    QMap<int, QList<int>> input_output = {{IN_DOMAIN,
                                           {OUT_SUBDOMAIN, OUT_IP, OUT_CIDR}},
                                          {IN_IP,
                                           {OUT_SUBDOMAIN}}};
};
}

class SecurityTrails: public AbstractOsintModule{

    public:
        SecurityTrails(ScanArgs args);
        ~SecurityTrails() override;

    public slots:
        void start() override;
        void replyFinishedSubdomain(QNetworkReply *reply) override;
        void replyFinishedIp(QNetworkReply *reply) override;
        void replyFinishedCidr(QNetworkReply *reply) override;

    private:
        QString m_key;
};

#endif // SECURITYTRAILS_H
