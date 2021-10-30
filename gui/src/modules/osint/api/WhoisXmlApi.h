#ifndef WHOISXMLAPI_H
#define WHOISXMLAPI_H

#include "../AbstractOsintModule.h"

/*
 * INPUT domain:            OUTPUT: subdomain, email
 * INPUT ip:                OUTPUT: subdomain, asn
 * INPUT asn:               OUTPUT: asn
 */

namespace ModuleInfo {
    struct WhoisXmlApi{
        QString name = "WhoisXmlApi";
        QString url = "https://whoisxmlapi.com/";
        QString url_apiDoc = "";
        QString summary = "The World’s First Truly Open Threat Intelligence Community";
        QMap<QString, QStringList> flags = {{"whois", {PLACEHOLDERTEXT_DOMAIN, ""}},
                                        {"ip whois", {PLACEHOLDERTEXT_IP, ""}},
                                        {"dns Lookup", {PLACEHOLDERTEXT_DOMAIN, ""}},
                                        {"email Verification", {PLACEHOLDERTEXT_EMAIL, ""}},
                                        {"domain availability", {PLACEHOLDERTEXT_DOMAIN, ""}},
                                        {"ip netblocks asn", {PLACEHOLDERTEXT_ASN, ""}},
                                        {"ip netblocks ip", {PLACEHOLDERTEXT_IP, ""}},
                                        {"ip netblocks org", {PLACEHOLDERTEXT_ORG, ""}},
                                        {"reverse ip", {PLACEHOLDERTEXT_IP, ""}},
                                        {"reverse mx", {PLACEHOLDERTEXT_MX, ""}},
                                        {"reverse ns", {PLACEHOLDERTEXT_NS, ""}},
                                        {"reverse whois", {PLACEHOLDERTEXT_DOMAIN, ""}},
                                        {"subdomain lookup", {PLACEHOLDERTEXT_DOMAIN, ""}},
                                        {"website contacts", {PLACEHOLDERTEXT_DOMAIN, ""}},
                                        {"website screenshot", {PLACEHOLDERTEXT_DOMAIN, ""}},
                                        {"whois history", {PLACEHOLDERTEXT_DOMAIN, ""}}};
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
