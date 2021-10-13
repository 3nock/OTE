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
                                        {"subdomains", "domain name"},
                                        {"whois", "domain name"},
                                        {"whois history", "domain name"},
                                        {"associated domains", "domain name"},
                                        {"associated ips", "domain name"},
                                        {"domain", "domain name"},
                                        {"company", "domain name"},
                                        {"ssl cert", "domain name"},
                                        {"ip whois", "nameserver address"},
                                        {"ping", ""},
                                        {"account", ""}};
    };
}

class SecurityTrails: public AbstractOsintModule{

    public:
        SecurityTrails(ScanArgs *args);
        ~SecurityTrails() override;

    public slots:
        void start() override;
        void replyFinished(QNetworkReply *) override;

    private:
        QString m_key = nullptr;
};

#endif // SECURITYTRAILS_H
