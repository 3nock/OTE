#ifndef IPINFO_H
#define IPINFO_H

#include "../AbstractOsintModule.h"

namespace ModuleInfo {
    struct IpInfo{
        QString name = "IpInfo";
        QString url = "https://ipinfo.io/";
        QString url_apiDoc = "";
        QString summary = "With IPinfo, you can pinpoint your users’ locations, customize their experiences, \n"
                          "prevent fraud, ensure compliance, and so much more";

        QMap<QString, QStringList> flags = {{"IP", {PLACEHOLDERTEXT_IP, ""}},
                                        {"ASN", {PLACEHOLDERTEXT_ASN, ""}},
                                        {"Ranges", {PLACEHOLDERTEXT_DOMAIN, ""}},
                                        {"Hosted Domains", {PLACEHOLDERTEXT_IP, ""}}};
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
