#ifndef WHOISXMLAPI_H
#define WHOISXMLAPI_H

#include "../AbstractOsintModule.h"

namespace ModuleInfo {
    struct WhoisXmlApi{
        QString name = "WhoisXmlApi";
        QString url = "https://otx.alienvault.com/";
        QString summary = "The World’s First Truly Open Threat Intelligence Community";
        QMap<QString, QString> flags = {{"whois", "domain name"},
                                        {"ipWhois", "ip-address"},
                                        {"DNS Lookup", "domain name"},
                                        {"Email Verification", "email"}};
    };
}

class WhoisXmlApi: public AbstractOsintModule{

    public:
        WhoisXmlApi(ScanArgs *args);
        ~WhoisXmlApi() override;

    public slots:
        void start() override;
        void replyFinishedSubdomain(QNetworkReply *) override;

    private:
        QString m_key = nullptr;
};

#endif // WHOISXMLAPI_H
