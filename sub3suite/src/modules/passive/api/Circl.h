#ifndef CIRCL_H
#define CIRCL_H

#include "../AbstractOsintModule.h"


namespace ModuleInfo {
struct Circl{
    QString name = OSINT_MODULE_CIRCL;
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
                                         {PLACEHOLDERTEXT_SSLCERT, "CIRCL Passive SSL is a database storing historical X.509 certificates seen per IP address."}}};

    QMap<int, QList<int>> input_output = {{IN_DOMAIN,
                                           {OUT_SUBDOMAIN, OUT_IP}},
                                          {IN_SSLCERT,
                                           {OUT_IP}},
                                          {IN_IP,
                                           {OUT_SSLCERT, OUT_SUBDOMAIN, OUT_IP}}};
};
}

class Circl: public AbstractOsintModule{

    public:
        explicit Circl(ScanArgs args);
        ~Circl() override;

    public slots:
        void start() override;
        void replyFinishedSubdomain(QNetworkReply *reply) override;
        void replyFinishedIp(QNetworkReply *reply) override;
        void replyFinishedSSL(QNetworkReply *reply) override;

    private:
        QString m_id;
        QString m_password;
};

#endif // CIRCL_H
