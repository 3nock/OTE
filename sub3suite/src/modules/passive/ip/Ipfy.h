#ifndef IPFY_H
#define IPFY_H

#include "../AbstractOsintModule.h"


namespace ModuleInfo {
struct Ipfy{
    QString name = OSINT_MODULE_IPFY;
    QString url = "https://www.ipify.org/";
    QString url_apiDoc = "https://geo.ipify.org/docs";
    QString summary = "Ipfy provides an easy-to-use API interface allowing customers to look various pieces "
                      "of information IPv4 and IPv6 addresses are associated with";

    QMap<QString, QStringList> flags = {{"Public Ipv4",
                                         {PLACEHOLDERTEXT_NONE, "A public IP API service. for IPv4"}},
                                        {"Public Ipv6",
                                         {PLACEHOLDERTEXT_NONE, "A public IP API service. for IPv6"}},
                                        {"IP Geolocation by Domain",
                                         {PLACEHOLDERTEXT_DOMAIN, "IP Geolocation API allows you to locate and identify website visitors by IP address. "
                                                              "IP location helps prevent fraud, customize web experiences, and maintain regulatory compliance."}},
                                        {"IP Geolocation by Email",
                                         {PLACEHOLDERTEXT_EMAIL, "IP Geolocation API allows you to locate and identify website visitors by IP address. "
                                                              "IP location helps prevent fraud, customize web experiences, and maintain regulatory compliance."}},
                                        {"IP Geolocation by IP",
                                         {PLACEHOLDERTEXT_IP, "IP Geolocation API allows you to locate and identify website visitors by IP address. "
                                                              "IP location helps prevent fraud, customize web experiences, and maintain regulatory compliance."}}};

    QMap<int, QList<int>> input_output = {{IN_DOMAIN,
                                           {OUT_IP, OUT_ASN, OUT_CIDR, OUT_SUBDOMAIN}},
                                          {IN_IP,
                                           {OUT_ASN, OUT_CIDR, OUT_SUBDOMAIN}}};
};
}

class Ipfy: public AbstractOsintModule{

    public:
        explicit Ipfy(ScanArgs args);
        ~Ipfy() override;

    public slots:
        void start() override;
        void replyFinishedSubdomain(QNetworkReply *reply) override;
        void replyFinishedAsn(QNetworkReply *reply) override;
        void replyFinishedIp(QNetworkReply *reply) override;
        void replyFinishedCidr(QNetworkReply *reply) override;

    private:
        QString m_key;
};

#endif // IPFY_H
