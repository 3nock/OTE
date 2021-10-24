#ifndef BGPVIEW_H
#define BGPVIEW_H

#include "../AbstractOsintModule.h"

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
