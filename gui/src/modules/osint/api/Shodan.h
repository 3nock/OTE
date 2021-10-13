#ifndef SHODAN_H
#define SHODAN_H

#include "../AbstractOsintModule.h"

namespace ModuleInfo {
    struct Shodan{
        QString name = "Shodan";
        QString url = "https://www.shodan.io/";
        QString summary = "Search Engine for the Internet of Everything";

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

class Shodan: public AbstractOsintModule{

    public:
        Shodan(ScanArgs *args);
        ~Shodan() override;

    public slots:
        void start() override;
        void replyFinished(QNetworkReply *) override;

    private:
        QString m_key = nullptr;
};
#endif // SHODAN_H
