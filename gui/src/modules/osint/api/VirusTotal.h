#ifndef VIRUSTOTAL_H
#define VIRUSTOTAL_H

#include "../AbstractOsintModule.h"

namespace ModuleInfo {
    struct VirusTotal{
        QString name = "VirusTotal";
        QString url = "https://www.virustotal.com/";
        QString summary = "Analyze suspicious files and URLs to detect types of malware, \n"
                          "automaticsubdomainIpy share them with the security community";

        QMap<QString, QString> flags = {{"domain historical whois", "domain name"},
                                        {"domain historical ssl certs", "domain name"},
                                        {"domain cname records", "domain name"},
                                        {"domain caa records", "domain name"},
                                        {"domain ns records", "domain name"},
                                        {"domain mx records", "domain name"},
                                        {"domain parent", "domain name"},
                                        {"domain resolutions", "domain name"},
                                        {"domain siblings", "domain name"},
                                        {"domain soa records", "domain name"},
                                        {"domain subdomains", "domain name"},
                                        {"domain urls", "domain name"},
                                        {"ip historical ssl certs", "ip-address"},
                                        {"ip historical whois", "ip-address"},
                                        {"ip resolutions", "ip-address"},
                                        {"ip urls", "ip-address"},
                                        {"v2 api domain", "domain name"},
                                        {"v2 api ipaddress", "ip-address"}};
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
