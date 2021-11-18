#ifndef CIRCL_H
#define CIRCL_H

#include "../AbstractOsintModule.h"

/*
 * INPUT domain:            OUTPUT: ip, subdomain
 * INPUT ip-address:        OUTPUT: ip, asn, subdomain, ssl
 * INPUT ssl:               OUTPUT: ip
 */

namespace ModuleInfo {
    struct Circl{
        QString name = "Circl";
        QString url = "https://www.circl.lu/";
        QString url_apiDoc = "https://www.circl.lu/services/";
        QString summary = "The Computer Incident Response Center Luxembourg (CIRCL) is a government-driven initiative "
                          "designed to provide a systematic response facility to computer security threats and incidents.";

        QMap<QString, QStringList> flags = {{"Passive dns",
                                             {PLACEHOLDERTEXT_DOMAIN_OR_IP, "CIRCL Passive DNS is a database storing historical DNS records from various "
                                                                            "resources including malware analysis or partners"}},
                                            {"Passive SSL",
                                             {PLACEHOLDERTEXT_CIDR, "CIRCL Passive SSL is a database storing historical X.509 certificates seen per IP address."}},
                                            {"Passive SSL Fetch",
                                             {PLACEHOLDERTEXT_SSLCERT, "CIRCL Passive SSL is a database storing historical X.509 certificates seen per IP address."}},
                                            {"Passive SSL query",
                                             {PLACEHOLDERTEXT_SSLCERT, "CIRCL Passive SSL is a database storing historical X.509 certificates seen per IP address."}},
                                            {"Ip 2 ASN",
                                             {PLACEHOLDERTEXT_IP, "IP ASN History to find ASN announcing an IP and the closest prefix announcing it at a specific date"}}};
    };
}

class Circl: public AbstractOsintModule{

    public:
        Circl(ScanArgs *args);
        ~Circl() override;

    public slots:
        void start() override;
        void replyFinishedSSLCert(QNetworkReply *reply) override;
        void replyFinishedSubdomain(QNetworkReply *reply) override;
        void replyFinishedIp(QNetworkReply *reply) override;
        void replyFinishedAsn(QNetworkReply *reply) override;
};

#endif // CIRCL_H
