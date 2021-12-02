#ifndef HACKERTARGETPAID_H
#define HACKERTARGETPAID_H

#include "../AbstractOsintModule.h"


namespace ModuleInfo {
struct HackerTargetPaid{
    QString name = "HackerTarget";
    QString url = "https://hackertarget.com/";
    QString url_apiDoc = "";
    QString summary = "From attack surface discovery to vulnerability identification, actionable network intelligence \n"
                      "for IT & security operations.";

    QMap<QString, QStringList> flags = {{"dnslookup",
                                         {PLACEHOLDERTEXT_DOMAIN, "Find DNS records for a domain, results are determined using the dig DNS tool."}},
                                        {"banner grabbing",
                                         {PLACEHOLDERTEXT_CIDR, "Discover network services by querying the service port."}},
                                        {"find shared dns",
                                         {PLACEHOLDERTEXT_NS, "Find hosts sharing DNS servers."}},
                                        {"hostsearch",
                                         {PLACEHOLDERTEXT_DOMAIN, ""}},
                                        {"subnet calculator",
                                         {PLACEHOLDERTEXT_CIDR, "Determine the properties of a network subnet"}},
                                        {"zonetransfer",
                                         {PLACEHOLDERTEXT_DOMAIN, "Online Test of a zone transfer that will attempt to get all DNS records for a target domain"}},
                                        {"reverse dns",
                                         {PLACEHOLDERTEXT_IP, "Find Reverse DNS records for an IP address or a range of IP addresses."}},
                                        {"geoip",
                                         {PLACEHOLDERTEXT_IP, "Find the location of an IP address using the GeoIP lookup location tool."}},
                                        {"reverse ip lookup",
                                         {PLACEHOLDERTEXT_IP, "Discover web hosts sharing an IP address with a reverse IP lookup."}},
                                        {"httpheaders",
                                         {PLACEHOLDERTEXT_DOMAIN, "View HTTP Headers of a web site. The HTTP Headers reveal system and web application details."}},
                                        {"pagelinks",
                                         {PLACEHOLDERTEXT_DOMAIN, "Dump all the links from a web page."}},
                                        {"aslookup",
                                         {PLACEHOLDERTEXT_IP, "Get Autonomous System Number or ASN details from an AS or an IP address."}},
                                        {"ping",
                                         {PLACEHOLDERTEXT_IP, "A common tool for testing connectivity to a host, perform a ping from our server."}},
                                        {"traceroute",
                                         {PLACEHOLDERTEXT_IP, "Using mtr an advanced traceroute tool trace the path of an Internet connection."}},
                                        {"whois",
                                         {PLACEHOLDERTEXT_IP, "Determine the registered owner of a domain or IP address block with the whois tool."}}};

    QMap<int, QList<int>> input_output = {{IN_DOMAIN,
                                           {OUT_SUBDOMAINIP, OUT_SUBDOMAIN, OUT_IP}},
                                          {IN_IP,
                                           {OUT_SUBDOMAIN, OUT_ASN}}};
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
