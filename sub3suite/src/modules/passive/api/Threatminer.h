#ifndef THREATMINER_H
#define THREATMINER_H

#include "../AbstractOsintModule.h"


namespace ModuleInfo {
struct Threatminer{
    QString name = "Threatminer";
    QString url = "https://threatminer.org/";
    QString url_apiDoc = "https://www.threatminer.org/api.php";
    QString summary = "Data Mining for threat Intelligence";
    QMap<QString, QStringList> flags = {{"domain whois",
                                         {PLACEHOLDERTEXT_DOMAIN, "Search Whois data by domain name"}},
                                        {"domain passive dns",
                                         {PLACEHOLDERTEXT_DOMAIN, "Search passive dns data by domain name"}},
                                        {"domain subdomains",
                                         {PLACEHOLDERTEXT_DOMAIN, "Search subdomains of the domain"}},
                                        {"domain Query URI",
                                         {PLACEHOLDERTEXT_DOMAIN, "Search uri data of a domain"}},
                                        {"ip whois",
                                         {PLACEHOLDERTEXT_IP, "Search whois data of an ip address"}},
                                        {"ip passive dns",
                                         {PLACEHOLDERTEXT_IP, "Search passive dns data of an ip address"}},
                                        {"ip Query URI",
                                         {PLACEHOLDERTEXT_IP, "Search uri data of an ip address"}},
                                        {"ip ssl certs",
                                         {PLACEHOLDERTEXT_IP, "Search ssl certificate data of an ip address"}},
                                        {"ssl Hosts",
                                         {PLACEHOLDERTEXT_SSLCERT, "Search hosts data of an ssl certificate"}}};

    QMap<int, QList<int>> input_output = {{IN_DOMAIN,
                                           {OUT_SUBDOMAIN, OUT_IP, OUT_EMAIL, OUT_URL}},
                                          {IN_IP,
                                           {OUT_SUBDOMAIN, OUT_ASN, OUT_SSLCERT}},
                                          {IN_SSLCERT,
                                           {OUT_IP}}};
};
}

class Threatminer: public AbstractOsintModule{

    public:
        Threatminer(ScanArgs args);
        ~Threatminer() override;

    public slots:
        void start() override;
        void replyFinishedSubdomain(QNetworkReply *reply) override;
        void replyFinishedIp(QNetworkReply *reply) override;
        void replyFinishedEmail(QNetworkReply *reply) override;
        void replyFinishedAsn(QNetworkReply *reply) override;
        void replyFinishedUrl(QNetworkReply *reply) override;
        void replyFinishedSSL(QNetworkReply *reply) override;
};

#endif // THREATMINER_H
