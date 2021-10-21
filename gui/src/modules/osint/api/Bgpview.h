#ifndef BGPVIEW_H
#define BGPVIEW_H

#include "../AbstractOsintModule.h"

#define ASN 0
#define ASN_DOWNSTREAM 1
#define ASN_IXS 2
#define ASN_PEERS 3
#define ASN_PREFIXES 4
#define ASN_UPSTREAMS 5
#define IP 6
#define IP_PREFIXES 7
#define QUERY 8

namespace ModuleInfo {
    struct Bgpview{
        QString name = "Bgpview";
        QString url = "https://bgpview.io/";
        QString summary = " API that lets you gather information about the current state and structure of the internet, "
                          "\nincluding ASNs, IP addresses, IXs, BGP Downstream & Upstream Peers, and much more";

        QMap <QString, QString> flags = {{"ip", "ip-address"},
                                         {"ip prefix", "ip-address/cdir"},
                                         {"asn", "asn"},
                                         {"asn prefixes", "asn prefixes"},
                                         {"asn peers", "asn"},
                                         {"asn peers", "asn"},
                                         {"asn upstreams", "asn"},
                                         {"asn downstreams", "asn"},
                                         {"asn ixs", "ixs"},
                                         {"query", "query term"}};
    };
}


class Bgpview: public AbstractOsintModule{

    public:
        Bgpview(ScanArgs *args);
        ~Bgpview() override;

    public slots:
        void start() override;
        void replyFinishedSubdomain(QNetworkReply *reply) override;
        void replyFinishedIp(QNetworkReply *reply) override;
        void replyFinishedAsn(QNetworkReply *reply) override;
        void replyFinishedEmail(QNetworkReply *reply) override;
};

#endif // BGPVIEW_H
