#ifndef WHOISXMLAPI_H
#define WHOISXMLAPI_H

#include "../AbstractOsintModule.h"

namespace ModuleInfo {
    struct WhoisXmlApi{
        QString name = "WhoisXmlApi";
        QString url = "https://otx.alienvault.com/";
        QString summary = "The World’s First Truly Open Threat Intelligence Community";
        QMap<QString, QString> flags = {{"whois", "domain name"},
                                        {"ip whois", "ip-address"},
                                        {"dns Lookup", "domain name"},
                                        {"email Verification", "email"},
                                        {"domain availability", "domain name"},
                                        {"ip netblocks asn", "asn"},
                                        {"ip netblocks ip", "ip-address"},
                                        {"ip netblocks org", "organization name"},
                                        {"reverse ip", "ip-address"},
                                        {"reverse mx", "mx name"},
                                        {"reverse ns", "ns name"},
                                        {"reverse whois", "domain name"},
                                        {"subdomain lookup", "domain name"},
                                        {"website contacts", "domain name"},
                                        {"website screenshot", "domain name"},
                                        {"whois history", "domain name"}};
    };
}

class WhoisXmlApi: public AbstractOsintModule{

    public:
        WhoisXmlApi(ScanArgs *args);
        ~WhoisXmlApi() override;

    public slots:
        void start() override;
        void replyFinishedSubdomain(QNetworkReply *reply) override;
        void replyFinishedAsn(QNetworkReply *reply) override;
        void replyFinishedEmail(QNetworkReply *reply) override;

    private:
        QString m_key = nullptr;
};

#endif // WHOISXMLAPI_H
