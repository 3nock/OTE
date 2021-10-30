#ifndef C99_H
#define C99_H

#include "../AbstractOsintModule.h"

/*
 * INPUT domain:            OUTPUT: subdomain
 */

namespace ModuleInfo {
    struct C99{
        QString name = "C99";
        QString url = "https://api.c99.nl/";
        QString url_apiDoc = "";
        QString summary = "Quality APIs for Osint";
        QMap <QString, QStringList> flags = {{"Subdomain Finder", {PLACEHOLDERTEXT_DOMAIN, ""}},
                                         {"Phone Lookup", {PLACEHOLDERTEXT_PHONE, ""}},
                                         {"Skype Resolver", {PLACEHOLDERTEXT_USERNAME, ""}},
                                         {"Ip 2 Skype", {PLACEHOLDERTEXT_IP, ""}},
                                         {"FirewsubdomainIp Technology Detector", {PLACEHOLDERTEXT_DOMAIN, ""}},
                                         {"Multiple port scanner", {PLACEHOLDERTEXT_IP, ""}},
                                         {"Nmap Scanner", {PLACEHOLDERTEXT_IP, ""}},
                                         {"Ping", {PLACEHOLDERTEXT_IP, ""}},
                                         {"IP to Host", {PLACEHOLDERTEXT_IP, ""}},
                                         {"Host to IP", {PLACEHOLDERTEXT_DOMAIN, ""}},
                                         {"Domain Checker", {PLACEHOLDERTEXT_DOMAIN, ""}},
                                         {"Domain History Check", {PLACEHOLDERTEXT_DOMAIN, ""}},
                                         {"IP 2 Domains", {PLACEHOLDERTEXT_IP, ""}},
                                         {"Alexa Rank Checker", {PLACEHOLDERTEXT_DOMAIN, ""}},
                                         {"Whois Checker", {PLACEHOLDERTEXT_DOMAIN, ""}},
                                         {"Screenshot Tool", {PLACEHOLDERTEXT_DOMAIN, ""}},
                                         {"GeoIP", {PLACEHOLDERTEXT_IP, ""}},
                                         {"Website Up or Down Checker", {PLACEHOLDERTEXT_DOMAIN, ""}},
                                         {"Site/URL Reputation Checker", {PLACEHOLDERTEXT_URL, ""}},
                                         {"Website Headers", {PLACEHOLDERTEXT_URL, ""}},
                                         {"Link Backup", {PLACEHOLDERTEXT_URL, ""}},
                                         {"Email Validator", {PLACEHOLDERTEXT_EMAIL, ""}},
                                         {"Disposable Mail Check", {PLACEHOLDERTEXT_EMAIL, ""}},
                                         {"IP Validator", {PLACEHOLDERTEXT_IP, ""}},
                                         {"TOR Checker", {PLACEHOLDERTEXT_IP, ""}},
                                         {"IP Logger", {PLACEHOLDERTEXT_NONE, ""}},
                                         {"Proxy Detector", {PLACEHOLDERTEXT_IP, ""}}};
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
