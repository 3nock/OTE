#ifndef VIRUSTOTAL_H
#define VIRUSTOTAL_H

#include "../AbstractOsintModule.h"

/*
 * INPUT domain:            OUTPUT: subdomain, ip, url, cert
 * INPUT ip:                OUTPUT: subdomain, ip, url, cert
 */

namespace ModuleInfo {
    struct VirusTotal{
        QString name = "VirusTotal";
        QString url = "https://www.virustotal.com/";
        QString url_apiDoc = "";
        QString summary = "Analyze suspicious files and URLs to detect types of malware, \n"
                          "automaticsubdomainIpy share them with the security community";

        QMap<QString, QStringList> flags = {{"domain historical whois", {PLACEHOLDERTEXT_DOMAIN, ""}},
                                        {"domain historical ssl certs", {PLACEHOLDERTEXT_DOMAIN, ""}},
                                        {"domain cname records", {PLACEHOLDERTEXT_DOMAIN, ""}},
                                        {"domain caa records", {PLACEHOLDERTEXT_DOMAIN, ""}},
                                        {"domain ns records", {PLACEHOLDERTEXT_DOMAIN, ""}},
                                        {"domain mx records", {PLACEHOLDERTEXT_DOMAIN, ""}},
                                        {"domain parent", {PLACEHOLDERTEXT_DOMAIN, ""}},
                                        {"domain resolutions", {PLACEHOLDERTEXT_DOMAIN, ""}},
                                        {"domain siblings", {PLACEHOLDERTEXT_DOMAIN, ""}},
                                        {"domain soa records", {PLACEHOLDERTEXT_DOMAIN, ""}},
                                        {"domain subdomains", {PLACEHOLDERTEXT_DOMAIN, ""}},
                                        {"domain urls", {PLACEHOLDERTEXT_DOMAIN, ""}},
                                        {"ip historical ssl certs", {PLACEHOLDERTEXT_IP, ""}},
                                        {"ip historical whois", {PLACEHOLDERTEXT_IP, ""}},
                                        {"ip resolutions", {PLACEHOLDERTEXT_IP, ""}},
                                        {"ip urls", {PLACEHOLDERTEXT_IP, ""}},
                                        {"v2 api domain", {PLACEHOLDERTEXT_DOMAIN, ""}},
                                        {"v2 api ipaddress", {PLACEHOLDERTEXT_IP, ""}}};
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
        void replyFinishedCertFingerprint(QNetworkReply *reply) override;

    private:
        QString m_key;
};

#endif // VIRUSTOTAL_H
