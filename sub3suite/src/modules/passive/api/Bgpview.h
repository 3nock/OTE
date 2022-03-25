#ifndef BGPVIEW_H
#define BGPVIEW_H

#include "../AbstractOsintModule.h"


namespace ModuleInfo {
struct Bgpview{
    QString name = OSINT_MODULE_BGPVIEW;
    QString url = "https://bgpview.io/";
    QString url_apiDoc = "https://bgpview.docs.apiary.io/";
    QString summary = "BGPView is a simple API allowing consumers to view all sort of analytics data"\
            "about the current state and structure of the internet.";

    QMap <QString, QStringList> flags = {{"ip",
                                          {PLACEHOLDERTEXT_IP, "Returns analytical data about the Ip address"}},
                                         {"ip prefix",
                                          {PLACEHOLDERTEXT_CIDR, "Returns analytical data about all the Ip address cidr"}},
                                         {"asn",
                                          {PLACEHOLDERTEXT_ASN, "Returns analytical data about the asn"}},
                                         {"asn prefixes",
                                          {PLACEHOLDERTEXT_ASN, "Returns all prefixes related to the asn"}},
                                         {"asn peers",
                                          {PLACEHOLDERTEXT_ASN, "Returns all peers associated with the asn"}},
                                         {"asn upstreams",
                                          {PLACEHOLDERTEXT_ASN, "Returns all asn upstream of the queried asn"}},
                                         {"asn downstreams",
                                          {PLACEHOLDERTEXT_ASN, "Returns all asn downstream of the queried asn"}},
                                         {"asn ixs",
                                          {"ixs", "Returns all asn associated with the ixs"}},
                                         {"query",
                                          {PLACEHOLDERTEXT_QUERY, "Returns all analytical data of the query term"}}};

    QMap<int, QList<int>> input_output = {{IN_IP,
                                            {OUT_ASN, OUT_IP, OUT_EMAIL}},
                                          {IN_ASN,
                                            {OUT_ASN, OUT_IP, OUT_EMAIL, OUT_SUBDOMAIN, OUT_CIDR}},
                                          {IN_QUERYTERM,
                                            {OUT_ASN, OUT_CIDR, OUT_EMAIL}},
                                          {IN_CIDR,
                                            {OUT_ASN, OUT_EMAIL}}};
};
}


class Bgpview: public AbstractOsintModule{

    public:
        Bgpview(ScanArgs args);
        ~Bgpview() override;

    public slots:
        void start() override;
        void replyFinishedSubdomain(QNetworkReply *reply) override;
        void replyFinishedIp(QNetworkReply *reply) override;
        void replyFinishedAsn(QNetworkReply *reply) override;
        void replyFinishedEmail(QNetworkReply *reply) override;
        void replyFinishedCidr(QNetworkReply *reply) override;
        /* ... */
        void replyFinishedEnumASN(QNetworkReply *reply) override;
        void replyFinishedEnumASNPeers(QNetworkReply *reply) override;
        void replyFinishedEnumASNPrefixes(QNetworkReply *reply) override;
        /* ... */
        void replyFinishedEnumCIDR(QNetworkReply *reply) override;
};

#endif // BGPVIEW_H
