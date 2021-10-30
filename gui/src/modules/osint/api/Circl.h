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
        QString url_apiDoc = "";
        QString summary = "Circl";
        QMap<QString, QStringList> flags = {{"Passive dns", {PLACEHOLDERTEXT_DOMAIN_OR_IP, ""}},
                                            {"Passive SSL", {PLACEHOLDERTEXT_CIDR, ""}},
                                            {"Passive SSL Fetch", {PLACEHOLDERTEXT_SSLCERT, ""}},
                                            {"Passive SSL query", {PLACEHOLDERTEXT_SSLCERT, ""}},
                                            {"Ip 2 ASN", {PLACEHOLDERTEXT_IP, ""}}};
    };
}

class Circl: public AbstractOsintModule{

    public:
        Circl(ScanArgs *args);
        ~Circl() override;

    public slots:
        void start() override;
        void replyFinishedCertFingerprint(QNetworkReply *reply) override;
        void replyFinishedSubdomain(QNetworkReply *reply) override;
        void replyFinishedIp(QNetworkReply *reply) override;
        void replyFinishedAsn(QNetworkReply *reply) override;
};

#endif // CIRCL_H
