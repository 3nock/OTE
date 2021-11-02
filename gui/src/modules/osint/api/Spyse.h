#ifndef SPYSE_H
#define SPYSE_H

#include "../AbstractOsintModule.h"

/*
 * INPUT domain:            OUTPUT: subdomain, email, ip, ssl, asn, url
 * INPUT ip:                OUTPUT: asn
 * INPUT asn:               OUTPUT: ip, asn
 * INPUT ssl:               OUTPUT: email
 * INPUT email:             OUTPUT: email
 */

namespace ModuleInfo {
    struct Spyse{
        QString name = "Spyse";
        QString url = "https://spyse.com/";
        QString url_apiDoc = "https://spyse.com/api";
        QString summary = "Find any Internet asset by digital fingerprints";

        QMap<QString, QStringList> flags = {{"Domain",
                                             {PLACEHOLDERTEXT_DOMAIN, ""}},
                                            {"IPv4 Host",
                                             {PLACEHOLDERTEXT_IP, ""}},
                                            {"SSL/TLS Certificate",
                                             {PLACEHOLDERTEXT_SSLCERT, ""}},
                                            {"AS",
                                             {PLACEHOLDERTEXT_ASN, ""}},
                                            {"CVE",
                                             {"CVE id", ""}},
                                            {"Emails",
                                             {PLACEHOLDERTEXT_EMAIL, ""}},
                                            {"DNS History",
                                             {PLACEHOLDERTEXT_DOMAIN, ""}}};
    };
}

class Spyse: public AbstractOsintModule{

    public:
        Spyse(ScanArgs *args);
        ~Spyse() override;

    public slots:
        void start() override;
        void replyFinishedSubdomain(QNetworkReply *reply) override;
        void replyFinishedEmail(QNetworkReply *reply) override;
        void replyFinishedIp(QNetworkReply *reply) override;
        void replyFinishedCertFingerprint(QNetworkReply *reply) override;
        void replyFinishedAsn(QNetworkReply *reply) override;
        void replyFinishedUrl(QNetworkReply *reply) override;

    private:
        QString m_key;
};

#endif // SPYSE_H
