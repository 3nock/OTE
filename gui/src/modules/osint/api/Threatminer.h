#ifndef THREATMINER_H
#define THREATMINER_H


#include "../AbstractOsintModule.h"

/*
 * INPUT domain:            OUTPUT: subdomain, ip, email, url
 * INPUT ip:                OUTPUT: subdomain, asn, ssl
 * INPUT ssl:               OUTPUT: ip
 */

namespace ModuleInfo {
    struct Threatminer{
        QString name = "Threatminer";
        QString url = "https://threatminer.org/";
        QString url_apiDoc = "";
        QString summary = "Data Mining for threat Intelligence";
        QMap<QString, QStringList> flags = {{"domain whois", {PLACEHOLDERTEXT_DOMAIN, ""}},
                                        {"domain passive dns", {PLACEHOLDERTEXT_DOMAIN, ""}},
                                        {"domain subdomains", {PLACEHOLDERTEXT_DOMAIN, ""}},
                                        {"domain Query URI", {PLACEHOLDERTEXT_DOMAIN, ""}},
                                        {"ip whois", {PLACEHOLDERTEXT_IP, ""}},
                                        {"ip passive dns", {PLACEHOLDERTEXT_IP, ""}},
                                        {"ip Query URI", {PLACEHOLDERTEXT_IP, ""}},
                                        {"ip ssl certs", {PLACEHOLDERTEXT_IP, ""}},
                                        {"ssl Hosts", {PLACEHOLDERTEXT_SSLCERT, ""}}};
    };
}

class Threatminer: public AbstractOsintModule{

    public:
        Threatminer(ScanArgs *args);
        ~Threatminer() override;

    public slots:
        void start() override;
        void replyFinishedSubdomain(QNetworkReply *reply) override;
        void replyFinishedIp(QNetworkReply *reply) override;
        void replyFinishedEmail(QNetworkReply *reply) override;
        void replyFinishedAsn(QNetworkReply *reply) override;
        void replyFinishedUrl(QNetworkReply *reply) override;
        void replyFinishedCertFingerprint(QNetworkReply *reply) override;
};

#endif // THREATMINER_H
