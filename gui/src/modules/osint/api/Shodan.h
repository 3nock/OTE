#ifndef SHODAN_H
#define SHODAN_H

#include "../AbstractOsintModule.h"

/*
 * INPUT domain:            OUTPUT: subdomainIp, subdomain, ip
 * INPUT ip:                OUTPUT: ip, asn
 */

namespace ModuleInfo {
    struct Shodan{
        QString name = "Shodan";
        QString url = "https://www.shodan.io/";
        QString url_apiDoc = "";
        QString summary = "Search Engine for the Internet of Everything";

        QMap<QString, QStringList> flags = {{"host ip", {PLACEHOLDERTEXT_IP, ""}},
                                        {"host count", {PLACEHOLDERTEXT_QUERY, ""}},
                                        {"host search", {PLACEHOLDERTEXT_QUERY, ""}},
                                        {"host search facets", {PLACEHOLDERTEXT_NONE, ""}},
                                        {"host search filters",{PLACEHOLDERTEXT_NONE, ""}},
                                        {"host search tokens", {PLACEHOLDERTEXT_QUERY, ""}},
                                        {"bulk data", {"dataset", ""}},
                                        {"account profile", {PLACEHOLDERTEXT_NONE, ""}},
                                        {"dns domain", {PLACEHOLDERTEXT_DOMAIN, ""}},
                                        {"dns resolve", {"domain name, domain name", ""}},
                                        {"dns reverse", {"ip-address, ip-address", ""}}};
    };
}

class Shodan: public AbstractOsintModule{

    public:
        Shodan(ScanArgs *args);
        ~Shodan() override;

    public slots:
        void start() override;
        void replyFinishedSubdomainIp(QNetworkReply *reply) override;
        void replyFinishedSubdomain(QNetworkReply *reply) override;
        void replyFinishedIp(QNetworkReply *reply) override;
        void replyFinishedAsn(QNetworkReply *reply) override;

    private:
        QString m_key = nullptr;
};
#endif // SHODAN_H
