#ifndef SECURITYTRAILS_H
#define SECURITYTRAILS_H

#include "../AbstractOsintModule.h"

namespace ModuleInfo {
    struct SecurityTrails{
        QString name = "SecurityTrails";
        QString url = "https://securitytrails.com/";
        QString summary = "The Total Internet Inventory. Powerful tools for third-party risk, \n"
                          "attack surface management, and total intel";

        QMap<QString, QString> flags = {{"dns history", "domain name"},
                                        {"domain subdomain", "domain name"},
                                        {"whois", "domain name"},
                                        {"whois history", "domain name"},
                                        {"associated domains", "domain name"},
                                        {"associated ips", "domain name"},
                                        {"domain details", "domain name"},
                                        {"company details", "domain name"},
                                        {"ssl cert", "domain name"},
                                        {"ip neighbours", "ip-address"},
                                        {"ip whois", "ip-address"},
                                        {"ping", ""},
                                        {"account", ""},
                                        {"company associated ip", "domain name"}};
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
