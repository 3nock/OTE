#ifndef THREATBOOK_H
#define THREATBOOK_H

#include "../AbstractOsintModule.h"


namespace ModuleInfo {
struct ThreatBook{
    QString name = "ThreatBook";
    QString url = "https://threatbook.cn/";
    QString url_apiDoc = "https://x.threatbook.cn/api";
    QString summary = "Threat detection platform";
    QMap<QString, QStringList> flags = {{"subdomains",
                                         {PLACEHOLDERTEXT_DOMAIN, "Advanced domain name query to obtain subdomain information of a domain name."}},
                                        {"domain query",
                                         {PLACEHOLDERTEXT_DOMAIN, "It can analyze domain names extracted from internal DNS or some NGFWs. Obtain the IP address corresponding to the domain name, the current Whois, comprehensively determine the threat types such as: remote control (C2), malware (Malware), zombie, scan (Scanner), brute force (Brute Force), etc., related attack groups Or security incident labels, original intelligence, relevant sample information, etc."}},
                                        {"domain adv query",
                                         {PLACEHOLDERTEXT_DOMAIN, "Advanced domain name query, to obtain the historical Whois information of the domain name and the historical IP information of the domain name."}},
                                        {"ip adv query",
                                         {PLACEHOLDERTEXT_IP, "IP advanced query, get current IP domain name and historical domain name information."}},
                                        {"ip query",
                                         {PLACEHOLDERTEXT_IP, "It can analyze business logs and external IP obtained from security devices such as firewalls and WAF. Obtain IP-related geographic location and ASN information, and comprehensively determine threat types such as: remote control (C2), zombie, compromised host (Compromised), scanning (Scanner), phishing, etc., related attack groups or security incidents Labels, original information, relevant sample information, etc."}}};

    QMap<int, QList<int>> input_output = {{IN_DOMAIN,
                                           {OUT_SUBDOMAIN, OUT_IP}},
                                          {IN_IP,
                                           {OUT_SUBDOMAIN, OUT_IP, OUT_ASN}}};
};
}

class ThreatBook: public AbstractOsintModule{

    public:
        ThreatBook(ScanArgs args);
        ~ThreatBook() override;

    public slots:
        void start() override;
        void replyFinishedSubdomain(QNetworkReply *reply) override;
        void replyFinishedAsn(QNetworkReply *reply) override;
        void replyFinishedIp(QNetworkReply *reply) override;

    private:
        QString m_key = nullptr;
};

#endif // THREATBOOK_H
