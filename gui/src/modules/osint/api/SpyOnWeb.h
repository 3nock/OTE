#ifndef SPYONWEB_H
#define SPYONWEB_H

/*
 * INPUT ip:            OUTPUT: subdomains
 * INPUT domain:        OUTPUT: ip, subdomains
 */

#include "../AbstractOsintModule.h"


namespace ModuleInfo {
    struct SpyOnWeb{
        QString name = "Spyonweb";
        QString url = "https://spyonweb.com/";
        QString url_apiDoc = "https://api.spyonweb.com/v1/docs";
        QString summary = "SpyOnWeb.com takes the information from public sources, then structures it for your quick and convenient search for the websites that probably belong to the same owner. "
                          "Our web crawler picks out the following data: ip address, google adsense id, google analytics id.";

        QMap<QString, QStringList> flags = {{"Domain",
                                             {PLACEHOLDERTEXT_DOMAIN, "The Domain API allows you to input a domain and get a list of domains that share the same identifiers (Google Adsense, Google Analytics, IP Address, Nameserver). Output is limited to 100 domains per identifier."}},
                                            {"Adsense",
                                             {PLACEHOLDERTEXT_ADSENSE, "The Google Adsense API allows you to input a google adsense identifier and get a list of domains that share the same identifier."}},
                                            {"Analytics",
                                             {"analytics code eg UA-34505845", "The Google Analytics API allows you to input a Google Analytics identifier and get a list of domains that share the same identifier."}},
                                            {"IP Address",
                                             {PLACEHOLDERTEXT_IP, "The IP Address API allows you to input an IP address and get a list of domains hosted on this IP address"}},
                                            {"Domains on Nameserver",
                                             {PLACEHOLDERTEXT_NS, "The Domains on Nameserver API allows you to input a nameserver and get a list of domains handled by this nameserver."}},
                                            {"Nameservers on IP Address",
                                             {PLACEHOLDERTEXT_IP, "The Nameservers on IP Address API allows you to input an IP address and get a list of nameservers using this IP address."}},
                                            {"Summary",
                                             {PLACEHOLDERTEXT_DOMAIN, "The Request Summary API allows you to input a domain name and get the number of domains with the same Google Adsense, Google Analytics identifiers, IP Address, and Nameserver"}}};
    };
}

class SpyOnWeb: public AbstractOsintModule{

    public:
        SpyOnWeb(ScanArgs *args);
        ~SpyOnWeb() override;

    public slots:
        void start() override;
        void replyFinishedSubdomainIp(QNetworkReply *reply) override;
        void replyFinishedSubdomain(QNetworkReply *reply) override;
        void replyFinishedIp(QNetworkReply *reply) override;

    private:
        QString m_key;
};

#endif // SPYONWEB_H
