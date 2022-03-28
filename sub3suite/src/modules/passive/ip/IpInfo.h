#ifndef IPINFO_H
#define IPINFO_H

#include "../AbstractOsintModule.h"


namespace ModuleInfo {
struct IpInfo{
    QString name = "IpInfo";
    QString url = "https://ipinfo.io/";
    QString url_apiDoc = "https://ipinfo.io/developers";
    QString summary = "With IPinfo, you can pinpoint your users’ locations, customize their experiences, \n"
                      "prevent fraud, ensure compliance, and so much more";

    QMap<QString, QStringList> flags = {{"ip",
                                         {PLACEHOLDERTEXT_IP, "Returns All the Information about the Ip address"}},
                                        {"asn",
                                         {PLACEHOLDERTEXT_ASN, "Returns ASN information of the Ip address"}},
                                        {"ranges",
                                         {PLACEHOLDERTEXT_DOMAIN, "Returns a list of Ip ranges of the Ip address"}},
                                        {"hosted domains",
                                         {PLACEHOLDERTEXT_IP, "Returns a List of Domains hosted on the Ip address"}}};

    QMap<int, QList<int>> input_output = {{IN_IP,
                                           {OUT_SUBDOMAIN}}};
};
}

class IpInfo: public AbstractOsintModule{

    public:
        IpInfo(ScanArgs args);
        ~IpInfo() override;

    public slots:
        void start() override;
        void replyFinishedSubdomain(QNetworkReply *reply) override;
        void replyFinishedEnumIP(QNetworkReply *reply) override;

    private:
        QString m_key;
};

#endif // IPINFO_H
