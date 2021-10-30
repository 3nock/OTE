#ifndef SECURITYTRAILS_H
#define SECURITYTRAILS_H

#include "../AbstractOsintModule.h"

/*
 * INPUT domain:            OUTPUT: subdomain, ip
 * INPUT ip:                OUTPUT: subdomain
 */

namespace ModuleInfo {
    struct SecurityTrails{
        QString name = "SecurityTrails";
        QString url = "https://securitytrails.com/";
        QString url_apiDoc = "";
        QString summary = "The Total Internet Inventory. Powerful tools for third-party risk, \n"
                          "attack surface management, and total intel";

        QMap<QString, QStringList> flags = {{"dns history", {PLACEHOLDERTEXT_DOMAIN, ""}},
                                        {"domain subdomain", {PLACEHOLDERTEXT_DOMAIN, ""}},
                                        {"whois", {PLACEHOLDERTEXT_DOMAIN, ""}},
                                        {"whois history", {PLACEHOLDERTEXT_DOMAIN, ""}},
                                        {"associated domains", {PLACEHOLDERTEXT_DOMAIN, ""}},
                                        {"associated ips", {PLACEHOLDERTEXT_DOMAIN, ""}},
                                        {"domain details", {PLACEHOLDERTEXT_DOMAIN, ""}},
                                        {"company details", {PLACEHOLDERTEXT_DOMAIN, ""}},
                                        {"ssl cert", {PLACEHOLDERTEXT_DOMAIN, ""}},
                                        {"ip neighbours", {PLACEHOLDERTEXT_IP, ""}},
                                        {"ip whois", {PLACEHOLDERTEXT_IP, ""}},
                                        {"ping", {PLACEHOLDERTEXT_NONE, ""}},
                                        {"account", {PLACEHOLDERTEXT_NONE, ""}},
                                        {"company associated ip", {PLACEHOLDERTEXT_DOMAIN, ""}}};
    };
}

class SecurityTrails: public AbstractOsintModule{

    public:
        SecurityTrails(ScanArgs *args);
        ~SecurityTrails() override;

    public slots:
        void start() override;
        void replyFinishedSubdomain(QNetworkReply *reply) override;
        void replyFinishedIp(QNetworkReply *reply) override;

    private:
        QString m_key = nullptr;
};

#endif // SECURITYTRAILS_H
