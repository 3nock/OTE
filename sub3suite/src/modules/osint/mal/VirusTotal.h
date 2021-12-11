#ifndef VIRUSTOTAL_H
#define VIRUSTOTAL_H

#include "../AbstractOsintModule.h"


namespace ModuleInfo {
struct VirusTotal{
    QString name = "VirusTotal";
    QString url = "https://www.virustotal.com/";
    QString url_apiDoc = "https://developers.virustotal.com/reference";
    QString summary = "Analyze suspicious files and URLs to detect types of malware, \n"
                      "automaticsubdomainIpy share them with the security community";

    QMap<QString, QStringList> flags = {{"domain historical whois",
                                         {PLACEHOLDERTEXT_DOMAIN, "All whois records that have been associated with the domain at some moment in time."}},
                                        {"domain historical ssl certs",
                                         {PLACEHOLDERTEXT_DOMAIN, "All SSL certificates that have been associated with the domain at some moment in time."}},
                                        {"domain cname records",
                                         {PLACEHOLDERTEXT_DOMAIN, "The cname_records relationship returns a list of all domain's CNAME records"}},
                                        {"domain caa records",
                                         {PLACEHOLDERTEXT_DOMAIN, "The caa_records relationship returns a list of all domain's CAA records"}},
                                        {"domain ns records",
                                         {PLACEHOLDERTEXT_DOMAIN, "The ns_records relationship returns a list of all domain's NS records."}},
                                        {"domain mx records",
                                         {PLACEHOLDERTEXT_DOMAIN, "The mx_records relationship returns a list of all domain's MX records. "}},
                                        {"domain parent",
                                         {PLACEHOLDERTEXT_DOMAIN, "The parent relationship returns the domain's object parent."}},
                                        {"domain resolutions",
                                         {PLACEHOLDERTEXT_DOMAIN, "The resolutions relationship returns a list of past and current IP resolutions for a given domain or subdomain."}},
                                        {"domain siblings",
                                         {PLACEHOLDERTEXT_DOMAIN, "The siblings relationship returns a list of subdomains at the same level as the given subdomain for a domain, along with their information."}},
                                        {"domain soa records",
                                         {PLACEHOLDERTEXT_DOMAIN, "The soa_records relationship returns a list of all domain's CNAME records."}},
                                        {"domain subdomains",
                                         {PLACEHOLDERTEXT_DOMAIN, "The subdomains relationship returns a list of all domain's subdomains. This relationships only returns direct subdomains, it's not recursive (it won't return a subdomain's subdomains)."}},
                                        {"domain urls",
                                         {PLACEHOLDERTEXT_DOMAIN, "The urls relationship returns a list of the domain's URLs"}},
                                        {"ip historical ssl certs",
                                         {PLACEHOLDERTEXT_IP, "The historical_ssl_certificates relationship returns a list of SSL certificates that have been associated with the IP address at some moment in time."}},
                                        {"ip historical whois",
                                         {PLACEHOLDERTEXT_IP, "The historical_whois relationship returns a list of whois records that have been associated with the IP address at some moment in time."}},
                                        {"ip resolutions",
                                         {PLACEHOLDERTEXT_IP, "The resolutions relationship returns a list of past and current domain resolutions for a IP address."}},
                                        {"ip urls",
                                         {PLACEHOLDERTEXT_IP, "The urls relationship returns a list of the IP's URLs"}},
                                        {"v2 api domain",
                                         {PLACEHOLDERTEXT_DOMAIN, "Returns all the available data assosiated with domain, uses v2 api"}},
                                        {"v2 api ipaddress",
                                         {PLACEHOLDERTEXT_IP, "Returns all the available data assosiated with ip-address, uses v2 api"}}};

    QMap<int, QList<int>> input_output = {{IN_DOMAIN,
                                           {OUT_SUBDOMAIN, OUT_IP, OUT_URL, OUT_SSLCERT}},
                                          {IN_IP,
                                           {OUT_SUBDOMAIN, OUT_IP, OUT_URL, OUT_SSLCERT}}};
};
}

class VirusTotal: public AbstractOsintModule{

    public:
        VirusTotal(ScanArgs *args);
        ~VirusTotal() override;

    public slots:
        void start() override;
        void replyFinishedSubdomain(QNetworkReply *reply) override;
        void replyFinishedIp(QNetworkReply *reply) override;
        void replyFinishedUrl(QNetworkReply *reply) override;
        void replyFinishedSSLCert(QNetworkReply *reply) override;

    private:
        QString m_key;
};

#endif // VIRUSTOTAL_H
