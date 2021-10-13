#ifndef IPINFO_H
#define IPINFO_H

#include "../AbstractOsintModule.h"

namespace ModuleInfo {
    struct IpInfo{
        QString name = "IpInfo";
        QString url = "https://ipinfo.io/";
        QString summary = "With IPinfo, you can pinpoint your users’ locations, customize their experiences, \n"
                          "prevent fraud, ensure compliance, and so much more";

        QMap<QString, QString> flags = {{"IP", "ip-address"},
                                        {"ASN", "asn"},
                                        {"Ranges", "domain name"},
                                        {"Hosted Domains", "ip-address"}};
    };
}

class IpInfo: public AbstractOsintModule{

    public:
        IpInfo(ScanArgs *args);
        ~IpInfo() override;

    public slots:
        void start() override;
        void replyFinished(QNetworkReply *) override;

    private:
        QString m_key = nullptr;
};

#endif // IPINFO_H
