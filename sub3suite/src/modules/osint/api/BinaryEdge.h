#ifndef BINARYEDGE_H
#define BINARYEDGE_H

#include "../AbstractOsintModule.h"


namespace ModuleInfo {
struct BinaryEdge{
    QString name = OSINT_MODULE_BGPVIEW;
    QString url = "https://binaryedge.io/";
    QString url_apiDoc = "https://docs.binaryedge.io/";
    QString summary = "BinaryEdge is Cybersecurity/Data Science company that focuses its effort on acquiring, "
                      "analyzing and classifying internet wide data.";

    QMap <QString, QStringList> flags = {{"host ip",
                                          {PLACEHOLDERTEXT_IP_OR_CIDR, "Details about an Host. List of recent events for the specified host, including details of exposed ports and services."}},
                                         {"host historical",
                                          {PLACEHOLDERTEXT_IP, "Details about an Host, with data up to 6 months. "
                                                               "List of events for the specified host, with events for each time that: "\
                                                               "A port was detected open, A service was found running, Other modules were successfully executed"}},
                                         {"host search",
                                          {PLACEHOLDERTEXT_QUERY, "Events based on a Query. List of recent events for the given query, including details of exposed ports and services."\
                                                                  "Can be used with specific parameters and/or full-text search."}},
                                         {"host search stats",
                                          {PLACEHOLDERTEXT_QUERY, "Statistics of recent events for the given query. Can be used with specific parameters and/or full-text search."}},
                                         {"domains subdomain",
                                          {PLACEHOLDERTEXT_DOMAIN, "Return list of subdomains known from the target domains"}},
                                         {"domains dns",
                                          {PLACEHOLDERTEXT_DOMAIN, "Return list of known DNS results for the target domain. Possible types of records currently available:A,AAAA,NS,MX"}},
                                         {"domains ip",
                                          {PLACEHOLDERTEXT_DOMAIN, "Return records that have the specified IP address in their A or AAAA records."}},
                                         {"domains search",
                                          {PLACEHOLDERTEXT_QUERY, "List of Domains/DNS data based on a Query. Can be used with specific parameters and/or full-text search. "
                                                                  "Possible types of records currently available: A, AAAA, NS, MX, CNAME, TXT"}},
                                         {"domains enumeration",
                                          {PLACEHOLDERTEXT_DOMAIN, "This endpoint attempts to enumerate subdomains from a larger dataset. "
                                                                   "The validate flag can be used to have all subdomains resolved on the fly and only those with DNS entries behind them returned."}},
                                         {"domains homoglyphs",
                                          {PLACEHOLDERTEXT_DOMAIN, "This endpoint generates a list of homoglyphs for a base domain. The validate flag can be used to have all homoglyphs "
                                                                   "resolved on the fly and only those with DNS entries behind them returned."}},
                                         {"subscription",
                                          {PLACEHOLDERTEXT_NONE, "Return details about your current subscription package."}}};

    QMap<int, QList<int>> input_output = {{IN_DOMAIN,
                                           {OUT_SUBDOMAINIP, OUT_SUBDOMAIN, OUT_IP}},
                                          {IN_IP,
                                           {OUT_SUBDOMAINIP, OUT_SUBDOMAIN, OUT_IP}}};
};
}

class BinaryEdge: public AbstractOsintModule{

    public:
        BinaryEdge(ScanArgs args);
        ~BinaryEdge() override;

    public slots:
        void start() override;
        void replyFinishedSubdomainIp(QNetworkReply *reply) override;
        void replyFinishedSubdomain(QNetworkReply *reply) override;
        void replyFinishedIp(QNetworkReply *reply) override;

    private:
        int m_page = 1;
        QString m_key = nullptr;
};

#endif // BINARYEDGE_H
