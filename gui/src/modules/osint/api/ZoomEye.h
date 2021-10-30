#ifndef ZOOMEYE_H
#define ZOOMEYE_H

#include "../AbstractOsintModule.h"

/*
 * INPUT domain:            OUTPUT: subdomainIp, subdomain, ip, asn
 * INPUT ip:                OUTPUT: subdomainIp, subdomain, ip, asn
 * INPUT asn:               OUTPUT: ip, asn
 */

namespace ModuleInfo {
    struct ZoomEye{
        QString name = "ZoomEye";
        QString url = "https://www.zoomeye.org/";
        QString url_apiDoc = "";
        QString summary = "ZoomEyeis a Cyberspace Search Engine recording information of devices, websites, \n"
                          "services and components etc.";

        QMap<QString, QStringList> flags = {{"host search asn", {PLACEHOLDERTEXT_ASN, ""}},
                                        {"host search cidr", {PLACEHOLDERTEXT_CIDR, ""}},
                                        {"host search hostname", {PLACEHOLDERTEXT_DOMAIN, ""}},
                                        {"host search ip", {PLACEHOLDERTEXT_IP, ""}},
                                        {"user information", {PLACEHOLDERTEXT_NONE, ""}}};
    };
}

class ZoomEye: public AbstractOsintModule{

    public:
        ZoomEye(ScanArgs *args);
        ~ZoomEye() override;

    public slots:
        void start() override;
        void replyFinishedSubdomain(QNetworkReply *reply) override;
        void replyFinishedSubdomainIp(QNetworkReply *reply) override;
        void replyFinishedIp(QNetworkReply *reply) override;
        void replyFinishedAsn(QNetworkReply *reply) override;

    private:
        QString m_key = nullptr;
};

#endif // ZOOMEYE_H
