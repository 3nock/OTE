#ifndef SPYSE_H
#define SPYSE_H

#include "../AbstractOsintModule.h"


namespace ModuleInfo {
struct Spyse{
    QString name = OSINT_MODULE_SPYSE;
    QString url = "https://spyse.com/";
    QString url_apiDoc = "https://spyse.com/api";
    QString summary = "Find any Internet asset by digital fingerprints";

    QMap<QString, QStringList> flags = {{"DOMAIN",
                                         {PLACEHOLDERTEXT_DOMAIN, ""}},
                                        {"IPv4 Host",
                                         {PLACEHOLDERTEXT_IP, ""}},
                                        {"SSL/TLS Certificate",
                                         {PLACEHOLDERTEXT_SSLCERT, ""}},
                                        {"AS",
                                         {PLACEHOLDERTEXT_ASN, ""}},
                                        {"CVE",
                                         {"CVE id", ""}},
                                        {"EMAILS",
                                         {PLACEHOLDERTEXT_EMAIL, ""}},
                                        {"DNS History",
                                         {PLACEHOLDERTEXT_DOMAIN, ""}}};

    QMap<int, QList<int>> input_output = {{IN_DOMAIN,
                                           {OUT_SUBDOMAIN, OUT_EMAIL, OUT_IP, OUT_SSLCERT, OUT_ASN, OUT_URL}},
                                          {IN_IP,
                                           {OUT_ASN}},
                                          {IN_ASN,
                                           {OUT_IP, OUT_ASN}},
                                          {IN_SSLCERT,
                                           {OUT_EMAIL}},
                                          {IN_EMAIL,
                                           {OUT_EMAIL}}};
};
}

class Spyse: public AbstractOsintModule{

    public:
        Spyse(ScanArgs args);
        ~Spyse() override;

    public slots:
        void start() override;
        void replyFinishedSubdomain(QNetworkReply *reply) override;
        void replyFinishedEmail(QNetworkReply *reply) override;
        void replyFinishedIp(QNetworkReply *reply) override;
        void replyFinishedSSL(QNetworkReply *reply) override;
        void replyFinishedAsn(QNetworkReply *reply) override;
        void replyFinishedUrl(QNetworkReply *reply) override;

    private:
        QString m_key;
};

#endif // SPYSE_H
