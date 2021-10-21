#ifndef C99_H
#define C99_H

#include "../AbstractOsintModule.h"

#define ALEXA_RANK 0
#define DISPOSABLE_MAIL_CHECK 1
#define DOMAIN_CHECKER 2
#define DOMAIN_HISTORY_CHECKER 3
#define EMAIL_VALIDATOR 4
#define WAF_CHECKER 5
#define GEOIP 6
#define HOST_TO_IP 7
#define IP_2_DOMAINS 8
#define IP_LOGGER 9
#define IP_VALIDATOR 10
#define IP_2_HOST 11
#define IP_2_SKYPE 12
#define LINK_BACKUP 13
#define MULTIPLE_PORT_SCANNER 14
#define NMAP_SCANNER 15
#define PHONE_LOOKUP 16
#define PING 17
#define PROXY_DETECTOR 18
#define SCREENSHOT_TOOL 19
#define SITE_REPUTATION_CHECKER 20
#define SKYPE_RESOLVER 21
#define SUBDOMAIN_FINDER 22
#define TOR_CHECKER 23
#define WEBSITE_HEADERS 24
#define WEBSITE_UP_OR_DOWN_CHECKER 25
#define WHOIS_CHECKER 26


namespace ModuleInfo {
    struct C99{
        QString name = "C99";
        QString url = "https://api.c99.nl/";
        QString summary = "Quality APIs for Osint";
        QMap <QString, QString> flags = {{"Subdomain Finder", "domain name"},
                                         {"Phone Lookup", "phone number"},
                                         {"Skype Resolver", "username"},
                                         {"Ip 2 Skype", "ip-address"},
                                         {"FirewsubdomainIp Technology Detector", "domain name"},
                                         {"Multiple port scanner", "ip-address"},
                                         {"Nmap Scanner", "ip-address"},
                                         {"Ping", "ip-address"},
                                         {"IP to Host", "ip-address"},
                                         {"Host to IP", "domain name"},
                                         {"Domain Checker", "domain name"},
                                         {"Domain History Check", "domain name"},
                                         {"IP 2 Domains", "ip-address"},
                                         {"Alexa Rank Checker", "domain name"},
                                         {"Whois Checker", "domain name"},
                                         {"Screenshot Tool", "domain name"},
                                         {"GeoIP", "ip-address"},
                                         {"Website Up or Down Checker", "domain name"},
                                         {"Site/URL Reputation Checker", "url"},
                                         {"Website Headers", "url"},
                                         {"Link Backup", "url"},
                                         {"Email Validator", "email"},
                                         {"Disposable Mail Check", "email"},
                                         {"IP Validator", "ip-address"},
                                         {"TOR Checker", "ip-address"},
                                         {"IP Logger", ""},
                                         {"Proxy Detector", "ip-address"}};
    };
}

class C99: public AbstractOsintModule{

    public:
        C99(ScanArgs *args);
        ~C99() override;

    public slots:
        void start() override;
        void replyFinishedSubdomain(QNetworkReply *reply) override;

    private:
        QString m_key = nullptr;
};

#endif // C99_H
