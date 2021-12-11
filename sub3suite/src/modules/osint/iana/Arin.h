#ifndef ARIN_H
#define ARIN_H

#include "../AbstractOsintModule.h"


namespace ModuleInfo {
struct Arin{
    QString name = "Arin";
    QString url = "https://jldc.me/anubis/";
    QString url_apiDoc = "https://www.arin.net/resources/registry/whois/rws/api/";
    QString summary = "ARIN is a nonprofit, member-based organization that administers IP addresses & ASNs "
                      "in support of the operation and growth of the Internet.";

    QMap<QString, QStringList> flags = {{"registry ip",
                                         {PLACEHOLDERTEXT_IP, "Registration Info of an Ip-address"}},
                                        {"registry asn",
                                         {PLACEHOLDERTEXT_ASN, "Registration Info of an asn"}},
                                        {"whois poc",
                                         {PLACEHOLDERTEXT_POC, "Whois info of a point of contact"}},
                                        {"whois org",
                                         {PLACEHOLDERTEXT_ORG, "Whois info of an Organization"}},
                                        {"whois net",
                                         {PLACEHOLDERTEXT_NET, "Whois info of a Network"}},
                                        {"whois asn",
                                         {PLACEHOLDERTEXT_ASN, "Whois info of an asn"}},
                                        {"whois customer",
                                         {PLACEHOLDERTEXT_USERNAME, "Whois information of an organization customer"}},
                                        {"whois rdns",
                                         {PLACEHOLDERTEXT_HOSTNAME, "Whois info of a delegate"}},
                                        {"poc orgs",
                                         {PLACEHOLDERTEXT_POC, "Lists the organizations associated with a given POC"}},
                                        {"poc asns",
                                         {PLACEHOLDERTEXT_POC, "Lists the ASNs associated with a given POC"}},
                                        {"poc nets",
                                         {PLACEHOLDERTEXT_POC, "Lists the networks associated with a given POC"}},
                                        {"org pocs",
                                         {PLACEHOLDERTEXT_ORG, "Lists the POCs associate with a given organization"}},
                                        {"org asns",
                                         {PLACEHOLDERTEXT_ORG, "Lists the ASNs associated with a given organization"}},
                                        {"org nets",
                                         {PLACEHOLDERTEXT_ORG, "Lists the networks associated with a given organization"}},
                                        {"asn pocs",
                                         {PLACEHOLDERTEXT_ASN, "Lists the POCs associated with a given ASN"}},
                                        {"net pocs",
                                         {PLACEHOLDERTEXT_NET, "Lists the POCs associated with a given network"}},
                                        {"net parent",
                                         {PLACEHOLDERTEXT_NET, "Lists the parent network of a given network"}},
                                        {"net children",
                                         {PLACEHOLDERTEXT_NET, "Lists the child networks of a given network"}},
                                        {"net rdns",
                                         {PLACEHOLDERTEXT_NET, "Lists the delegations of a given network"}},
                                        {"rdns nets",
                                         {PLACEHOLDERTEXT_HOSTNAME, "Lists networks related to a given delegation"}}};

    QMap<int, QList<int>> input_output = {};
};
}


class Arin: public AbstractOsintModule{

    public:
        Arin(ScanArgs *args);
        ~Arin() override;

    public slots:
        void start() override;
};
#endif // ARIN_H
