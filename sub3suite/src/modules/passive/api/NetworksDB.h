#ifndef NETWORKSDB_H
#define NETWORKSDB_H

#include "../AbstractOsintModule.h"


namespace ModuleInfo {
struct NetworksDB{
    QString name = "NetworksDB";
    QString url = "https://networksdb.io/";
    QString url_apiDoc = "https://networksdb.io/api/docs";
    QString summary = "Our database contains information about the public IPv4 and IPv6 addresses, networks and domains owned by companies "
                      "and organisations across the world along with city-level IP geolocation data and autonomous system information.";

    QMap<QString, QStringList> flags = {{"Organisation Search",
                                         {PLACEHOLDERTEXT_QUERY, "Returns Subdomains of a particular domain"}},
                                        {"Organisation Info",
                                         {"id [organisation ID returned by the search query]", "Retreive information about an organisation."}},
                                        {"Organisation Networks",
                                         {"id [organisation ID returned by the search query]", "Search for the public networks owned by an organisation."}},
                                        {"IP Address Info",
                                         {PLACEHOLDERTEXT_IP, "Search for an IPv4 or IPv6 address."}},
                                        {"IP Geolocation",
                                         {PLACEHOLDERTEXT_IP, "Request geolocation information for a given IPv4 or IPv6 Address."}},
                                        {"IPs for Domain (Forward DNS)",
                                         {PLACEHOLDERTEXT_DOMAIN, "Perform a forward DNS lookup to find all the IPv4 and IPv6 addresses pointed to by a given domain name."}},
                                        {"Domains on IP (Reverse DNS)",
                                         {PLACEHOLDERTEXT_IP, "Perform a reverse DNS lookup to find all the domain names pointing to the given IPv4 or IPv6 address."}},
                                        {"Domains in Network (Mass Reverse DNS)",
                                         {PLACEHOLDERTEXT_CIDR, "Perform a \"mass\" reverse DNS lookup to find all the domain names pointing to any IPv4 or IPv6 address in the given network."}},
                                        {"Autonomous System Info",
                                         {PLACEHOLDERTEXT_ASN, "Search for an autonomous system."}},
                                        {"Autonomous System Networks",
                                         {PLACEHOLDERTEXT_ASN, "Search for the networks announced by an autonomous system."}}};

    QMap<int, QList<int>> input_output = {{IN_CIDR,
                                           {OUT_SUBDOMAINIP, OUT_SUBDOMAIN, OUT_IP}},
                                          {IN_DOMAIN,
                                           {OUT_IP, OUT_CIDR}},
                                          {IN_IP,
                                           {OUT_SUBDOMAIN}},
                                          {IN_ASN,
                                           {}}};
};
}

class NetworksDB: public AbstractOsintModule{

    public:
        NetworksDB(ScanArgs args);
        ~NetworksDB() override;

    public slots:
        void start() override;
        void replyFinishedSubdomainIp(QNetworkReply *reply) override;
        void replyFinishedSubdomain(QNetworkReply *reply) override;
        void replyFinishedIp(QNetworkReply *reply) override;
        void replyFinishedCidr(QNetworkReply *reply) override;

    private:
        QString m_key;
};

#endif // NETWORKSDB_H
