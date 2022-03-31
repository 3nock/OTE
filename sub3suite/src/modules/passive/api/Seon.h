#ifndef SEON_H
#define SEON_H

#include "../AbstractOsintModule.h"


namespace ModuleInfo {
struct Seon{
    QString name = OSINT_MODULE_SEON;
    QString url = "https://seon.io/";
    QString url_apiDoc = "https://docs.seon.io/";
    QString summary = "Seon";

    QMap<QString, QStringList> flags = {{"Email",
                                         {PLACEHOLDERTEXT_EMAIL, "Our Email API v2.1 aggregates hundreds of open and reachable social data sources to provide an in-depth email address investigation tool."}},
                                        {"Email Verification",
                                         {PLACEHOLDERTEXT_EMAIL, "Our Email Verification API lets you know whether an email address is valid or not, using a fast SMTP-MX check."}},
                                        {"Phone",
                                         {PLACEHOLDERTEXT_PHONE, "Our Phone API combines numerous data sources to provide an in-depth phone number investigation tool. "
                                                                 "It aggregates open and reachable social and messenger data to flag fraudsters based on a phone number only."}},
                                        {"Ip",
                                         {PLACEHOLDERTEXT_IP, "Fraudsters bypass IP address bans with proxies and VPNs. This API determines how likely an IP address is to be faked using modern computing techniques, "
                                                              "so you can block TOR, VPN and proxy users. This reduces ATO (account takeover), spyware, malware, criminal netblocks, botnets, spammers and exploit scanners."}}};

    QMap<int, QList<int>> input_output = {};
};
}

class Seon: public AbstractOsintModule{

    public:
        explicit Seon(ScanArgs args);
        ~Seon() override;

    public slots:
        void start() override;

    private:
        QString m_key;
};

#endif // SEON_H
