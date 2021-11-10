#ifndef MALTIVERSE_H
#define MALTIVERSE_H

#include "../AbstractOsintModule.h"

/*
 * INPUT domain:            OUTPUT: ip, asn
 * INPUT ip:                OUTPUT: email, asn
 */

namespace ModuleInfo {
    struct Maltiverse{
        QString name = "Maltiverse";
        QString url = "https://maltiverse.com/";
        QString url_apiDoc = "https://app.swaggerhub.com/apis-docs/maltiverse/api/1.0.0-oas3";
        QString summary = "Maltiverse";
        QMap<QString, QStringList> flags = {{"IpV4",
                                             {PLACEHOLDERTEXT_IP4, "IPv4 address IoC related methods"}},
                                            {"Hostname",
                                             {PLACEHOLDERTEXT_DOMAIN, "Hostname IoC related methods"}},
                                            {"Url",
                                             {PLACEHOLDERTEXT_CHECKSUM, "Url IOC related methods"}}
                                           };
    };
}

class Maltiverse: public AbstractOsintModule{

    public:
        Maltiverse(ScanArgs *args);
        ~Maltiverse() override;

    public slots:
        void start() override;
        void replyFinishedAsn(QNetworkReply *reply) override;
        void replyFinishedIp(QNetworkReply *reply) override;
        void replyFinishedEmail(QNetworkReply *reply) override;

    private:
        QString m_key;
};
#endif // MALTIVERSE_H
