#ifndef SHODAN_H
#define SHODAN_H

#include "../AbstractOsintModule.h"


namespace ModuleInfo {
struct Shodan{
    QString name = "Shodan";
    QString url = "https://www.shodan.io/";
    QString url_apiDoc = "https://developer.shodan.io/api";
    QString summary = "Search Engine for the Internet of Everything";

    QMap<QString, QStringList> flags = {{"host ip",
                                         {PLACEHOLDERTEXT_IP, "Returns all services that have been found on the given host IP."}},
                                        {"host count",
                                         {PLACEHOLDERTEXT_QUERY, "This method behaves identical to \"/shodan/host/search\" with the only difference that "
                                                                 "this method does not return any host results, it only returns the total number of results that matched the query and any facet information that was requested"}},
                                        {"host search",
                                         {PLACEHOLDERTEXT_QUERY, "Search Shodan using the same query syntax as the website and use facets to get summary information for different properties."}},
                                        {"host search facets",
                                         {PLACEHOLDERTEXT_NONE, "This method returns a list of facets that can be used to get a breakdown of the top values for a property."}},
                                        {"host search filters",
                                         {PLACEHOLDERTEXT_NONE, "This method returns a list of search filters that can be used in the search query."}},
                                        {"host search tokens",
                                         {PLACEHOLDERTEXT_QUERY, "This method lets you determine which filters are being used by the query string and what parameters were provided to the filters."}},
                                        {"bulk data",
                                         {"dataset", "Get a list of files that are available for download from the provided dataset."}},
                                        {"account profile",
                                         {PLACEHOLDERTEXT_NONE, "Returns information about the Shodan account linked to this API key."}},
                                        {"dns domain",
                                         {PLACEHOLDERTEXT_DOMAIN, "Get all the subdomains and other DNS entries for the given domain. Uses 1 query credit per lookup."}},
                                        {"dns resolve",
                                         {"domain name, domain name", "Look up the IP address for the provided list of hostnames."}},
                                        {"dns reverse",
                                         {"ip-address, ip-address", "Look up the hostnames that have been defined for the given list of IP addresses."}}};

    QMap<int, QList<int>> input_output = {{IN_DOMAIN,
                                           {OUT_SUBDOMAINIP, OUT_SUBDOMAIN, OUT_IP}},
                                          {IN_IP,
                                           {OUT_IP, OUT_ASN}}};
};
}

class Shodan: public AbstractOsintModule{

    public:
        Shodan(ScanArgs *args);
        ~Shodan() override;

    public slots:
        void start() override;
        void replyFinishedSubdomainIp(QNetworkReply *reply) override;
        void replyFinishedSubdomain(QNetworkReply *reply) override;
        void replyFinishedIp(QNetworkReply *reply) override;
        void replyFinishedAsn(QNetworkReply *reply) override;

    private:
        QString m_key = nullptr;
};
#endif // SHODAN_H
