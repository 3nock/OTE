#ifndef ASRANK_H
#define ASRANK_H

#include "../AbstractOsintModule.h"


namespace ModuleInfo {
struct ASRank{
    QString name = OSINT_MODULE_ASRANK;
    QString url = "https://asrank.caida.org/";
    QString url_apiDoc = "https://api.asrank.caida.org/v2/docs";
    QString summary = "ASRank is CAIDA's ranking of Autonomous Systems (AS) (which approximately map to Internet Service Providers) and organizations (Orgs) (which are a collection of one or more ASes)."
                      "This ranking is derived from topological data collected by CAIDA's Archipelago Measurement Infrastructure and Border Gateway Protocol (BGP) routing data collected by the Route Views Project and RIPE NCC.";
    QMap<QString, QStringList> flags = {{"ASN",
                                         {PLACEHOLDERTEXT_NONE, "Returns list of ASNs"}},
                                        {"ASN by asn",
                                         {PLACEHOLDERTEXT_ASN, "Returns list of ASN by ID"}},
                                        {"ASN by name",
                                         {PLACEHOLDERTEXT_QUERY, "Returns ASN by name"}},
                                        {"ORG",
                                         {PLACEHOLDERTEXT_NONE, "Returns Organizations"}},
                                        {"ORG by organization",
                                         {PLACEHOLDERTEXT_ORG, "Returns Organization by ID"}},
                                        {"Links",
                                         {PLACEHOLDERTEXT_NONE, "Returns Links"}},
                                        {"Links by Link",
                                         {PLACEHOLDERTEXT_URL, "Returns Links by Link"}}};

    QMap<int, QList<int>> input_output = {{IN_QUERYTERM,
                                           {OUT_ASN}}};
};
}

class ASRank: public AbstractOsintModule{

    public:
        explicit ASRank(ScanArgs args);
        ~ASRank() override;

    public slots:
        void start() override;
        void replyFinishedAsn(QNetworkReply *reply) override;
};

#endif // ASRANK_H
