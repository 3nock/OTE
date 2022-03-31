#ifndef ZOOMEYE_H
#define ZOOMEYE_H

#include "../AbstractOsintModule.h"


namespace ModuleInfo {
struct ZoomEye{
    QString name = OSINT_MODULE_ZOOMEYE;
    QString url = "https://www.zoomeye.org/";
    QString url_apiDoc = "https://www.zoomeye.org/doc";
    QString summary = "ZoomEyeis a Cyberspace Search Engine recording information of devices, websites, \n"
                      "services and components etc.";

    QMap<QString, QStringList> flags = {{"host search asn",
                                         {PLACEHOLDERTEXT_ASN, "Search by asn"}},
                                        {"host search cidr",
                                         {PLACEHOLDERTEXT_CIDR, "Search by ip-address/cidr"}},
                                        {"host search hostname",
                                         {PLACEHOLDERTEXT_DOMAIN, "Search by hostname"}},
                                        {"host search ip",
                                         {PLACEHOLDERTEXT_IP, "Search by ip-address"}},
                                        {"user information",
                                         {PLACEHOLDERTEXT_NONE, "User Information"}}};

    QMap<int, QList<int>> input_output = {{IN_DOMAIN,
                                           {OUT_SUBDOMAINIP, OUT_SUBDOMAIN, OUT_IP, OUT_ASN}},
                                          {IN_IP,
                                           {OUT_SUBDOMAINIP, OUT_SUBDOMAIN, OUT_IP, OUT_ASN}},
                                          {IN_ASN,
                                           {OUT_IP, OUT_ASN}},
                                          {IN_CIDR,
                                           {OUT_IP, OUT_ASN}}};
};
}

class ZoomEye: public AbstractOsintModule{

    public:
        explicit ZoomEye(ScanArgs args);
        ~ZoomEye() override;

    public slots:
        void start() override;
        void replyFinishedSubdomain(QNetworkReply *reply) override;
        void replyFinishedSubdomainIp(QNetworkReply *reply) override;
        void replyFinishedIp(QNetworkReply *reply) override;
        void replyFinishedAsn(QNetworkReply *reply) override;

    private:
        QString m_key;
};

#endif // ZOOMEYE_H
