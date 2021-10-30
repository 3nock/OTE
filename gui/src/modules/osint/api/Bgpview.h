#ifndef BGPVIEW_H
#define BGPVIEW_H

#include "../AbstractOsintModule.h"

/*
 * INPUT ip:            OUTPUT: asn, ip, email
 * INPUT cidr:          OUTPUT:
 * INPUT asn:           OUTPUT: asn, ip, email, subdomain
 * INPUT query-term:    OUTPUT: asn, ip, email
 */

namespace ModuleInfo {
    struct Bgpview{
        QString name = "Bgpview";
        QString url = "https://bgpview.io/";
        QString url_apiDoc = "";
        QString summary = " API that lets you gather information about the current state and structure of the internet, "
                          "\nincluding ASNs, IP addresses, IXs, BGP Downstream & Upstream Peers, and much more";

        QMap <QString, QStringList> flags = {{"ip", {PLACEHOLDERTEXT_IP, ""}},
                                             {"ip prefix", {PLACEHOLDERTEXT_CIDR, ""}},
                                             {"asn", {PLACEHOLDERTEXT_ASN, ""}},
                                             {"asn prefixes", {PLACEHOLDERTEXT_ASN, ""}},
                                             {"asn peers", {PLACEHOLDERTEXT_ASN, ""}},
                                             {"asn peers", {PLACEHOLDERTEXT_ASN, ""}},
                                             {"asn upstreams", {PLACEHOLDERTEXT_ASN, ""}},
                                             {"asn downstreams", {PLACEHOLDERTEXT_ASN, ""}},
                                             {"asn ixs", {"ixs", ""}},
                                             {"query", {PLACEHOLDERTEXT_QUERY, ""}}};
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
