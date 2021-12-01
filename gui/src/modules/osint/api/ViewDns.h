#ifndef VIEWDNS_H
#define VIEWDNS_H

#include "../AbstractOsintModule.h"


namespace ModuleInfo {
    struct ViewDns{
        QString name = "ViewDns";
        QString url = "https://viewdns.info/";
        QString url_apiDoc = "https://viewdns.info/api/docs/";
        QString summary = "The tools provided by ViewDNS.info allow users to gather a large amount of data about a given website or IP address.";

        QMap<QString, QStringList> flags = {{"Abuse Contact Lookup",
                                             {PLACEHOLDERTEXT_DOMAIN, "Used to find the abuse contact address for a domain name. This is where you would send complaints about spam originating from that domain."}},
                                            {"DNS Propagation Checker",
                                             {PLACEHOLDERTEXT_DOMAIN, "Check whether recent changes to DNS entries have propagated to DNS servers all over the world. "
                                                                      "Useful in troubleshooting DNS issues that appear to be isolated to one geographic region. Provides a status report on DNS propagation globally."}},
                                            {"DNS Record Lookup",
                                             {PLACEHOLDERTEXT_DOMAIN, "View all configured DNS records (A, MX, CNAME etc.) for a specified domain name."}},
                                            {"Domain/IP Whois",
                                             {PLACEHOLDERTEXT_DOMAIN_OR_IP, "Displays owner/contact information for a domain name. Can also be used to determine if a domain name is registered or not."}},
                                            {"IP History",
                                             {PLACEHOLDERTEXT_DOMAIN, "Shows a historical list of IP addresses a given domain name has been hosted on as well as where that IP address is geographically located, and the owner of that IP address."}},
                                            {"IP Location Finder",
                                             {PLACEHOLDERTEXT_IP, "This tool will display geographic information about a supplied IP address including city, country, latitude, longitude and more."}},
                                            {"MAC Address Lookup",
                                             {PLACEHOLDERTEXT_MAC_ADDRESS, "This tool will display the name of the company that manufactured a specific network device based on its MAC Address."}},
                                            {"Ping",
                                             {PLACEHOLDERTEXT_DOMAIN, "Test how long a response from remote system takes to reach the ViewDNS server. Useful for detecting latency issues on network connections."}},
                                            {"Port Scanner",
                                             {PLACEHOLDERTEXT_DOMAIN, "This web based port scanner will test whether common ports are open on a server. Useful in determining if a specific service (e.g. HTTP) is up or down on a specific server. "
                                                                      "Ports scanned are: 21, 22, 23, 25, 80, 110, 139, 143, 445, 1433, 1521, 3306 and 3389"}},
                                            {"Reverse DNS Lookup",
                                             {PLACEHOLDERTEXT_IP, "Find the reverse DNS entry (PTR) for a given IP. This is generally the server or host name."}},
                                            {"Reverse IP Lookup",
                                             {PLACEHOLDERTEXT_IP, "Takes a domain or IP address and quickly shows all other domains hosted from the same server. Useful for finding phishing sites or identifying other sites on the same shared hosting server. "
                                                                  "By default, the first 10,000 results are returned."}},
                                            {"Reverse MX Lookup",
                                             {PLACEHOLDERTEXT_MX, "Takes a mail server (e.g. mail.google.com) and quickly shows all other domains that use the same mail server. Useful for identifying domains that are used as email aliases"}},
                                            {"Reverse NS Lookup",
                                             {PLACEHOLDERTEXT_NS, "Takes a nameserver (e.g. ns.example.com) and quickly shows all other domains that share the same nameserver. Useful for identifying other domains that share the same web server or hosting company."}},
                                            {"Reverse Whois Lookup",
                                             {PLACEHOLDERTEXT_DOMAIN, "This tool will allow you to find domain names owned by an individual person or company. "
                                                                      "Simply provide the email address or name of the person or company to find other domains registered using those same details. Returns 1,000 results at a time."}}};

        QMap<int, QList<int>> input_output = {{IN_DOMAIN,
                                               {OUT_SUBDOMAIN, OUT_EMAIL, OUT_IP}},
                                              {IN_IP,
                                               {OUT_SUBDOMAIN}}};
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
