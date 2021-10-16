#ifndef C99_H
#define C99_H

#include "../AbstractOsintModule.h"

namespace ModuleInfo {
    struct C99{
        QString name = "C99";
        QString url = "https://api.c99.nl/";
        QString summary = "Quality APIs for Osint";
        QMap <QString, QString> flags = {{"Subdomain Finder", "domain name"}, {"Phone Lookup", "phone number"},
                                         {"Skype Resolver", "username"}, {"Ip 2 Skype", "ip-address"},
                                         {"FirewsubdomainIp Technology Detector", "domain name"},
                                         {"Multiple port scanner", "ip-address"},
                                         {"Nmap Scanner", "ip-address"}, {"Ping", "ip-address"},
                                         {"IP to Host", "ip-address"}, {"Host to IP", "domain name"},
                                         {"Domain Checker", "domain name"}, {"Domain History Check", "domain name"},
                                         {"IP 2 Domains", "ip-address"}, {"Alexa Rank Checker", "domain name"},
                                         {"Whois Checker", "domain name"}, {"Screenshot Tool", "domain name"},
                                         {"GeoIP", "ip-address"}, {"Website Up or Down Checker", "domain name"},
                                         {"Site/URL Reputation Checker", "url"}, {"Website Headers", "url"},
                                         {"Link Backup", "url"}, {"Email Validator", "email"},
                                         {"Disposable Mail Check", "email"}, {"IP Validator", "ip-address"},
                                         {"TOR Checker", "ip-address"}, {"IP Logger", ""},
                                         {"Proxy Detector", "ip-address"}};
    };
}

class C99: public AbstractOsintModule{

    public:
        C99(ScanArgs *args);
        ~C99() override;

    public slots:
        void start() override;
        void replyFinished(QNetworkReply *) override;

    private:
        QString m_key = nullptr;
};
#endif // C99_H
