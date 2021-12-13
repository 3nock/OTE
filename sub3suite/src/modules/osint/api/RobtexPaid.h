#ifndef ROBTEXPAID_H
#define ROBTEXPAID_H

#include "../AbstractOsintModule.h"


namespace ModuleInfo {
struct RobtexPaid{
    QString name = "Robtex";
    QString url = "https://www.robtex.com/";
    QString url_apiDoc = "https://www.robtex.com/api/";
    QString summary = "Robtex is used for various kinds of research of IP numbers, Domain names, etc";

    QMap<QString, QStringList> flags = {{"ipquery",
                                         {PLACEHOLDERTEXT_IP, "This API returns the current forward and reverse of an IP number, together with GEO-location data and network data"}},
                                        {"asquery",
                                         {PLACEHOLDERTEXT_ASN, "Returns an array of networks related to a specific AS number"}},
                                        {"pdns forward",
                                         {PLACEHOLDERTEXT_DOMAIN, "Returns Passive dns Data of a domain"}},
                                        {"pdns reverse",
                                         {PLACEHOLDERTEXT_IP, "Returns Passive dns Data of an Ip address"}}};

    QMap<int, QList<int>> input_output = {{IN_DOMAIN,
                                           {OUT_SUBDOMAINIP, OUT_SUBDOMAIN, OUT_IP}},
                                          {IN_IP,
                                           {OUT_SUBDOMAINIP, OUT_SUBDOMAIN, OUT_IP, OUT_ASN}},
                                          {IN_ASN,
                                           {OUT_IP}}};
};
}

class RobtexPaid: public AbstractOsintModule{

    public:
        RobtexPaid(ScanArgs args);
        ~RobtexPaid() override;

    public slots:
        void start() override;
        void replyFinishedSubdomainIp(QNetworkReply *reply) override;
        void replyFinishedSubdomain(QNetworkReply *reply) override;
        void replyFinishedIp(QNetworkReply *reply) override;
        void replyFinishedAsn(QNetworkReply *reply) override;

    private:
        QString m_key;
};
#endif // ROBTEXPAID_H
