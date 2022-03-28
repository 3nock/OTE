#ifndef RIPE_H
#define RIPE_H

#include "../AbstractOsintModule.h"


namespace ModuleInfo {
struct Ripe{
    QString name = OSINT_MODULE_RIPE;
    QString url = "https://stat.ripe.net/";
    QString url_apiDoc = "https://stat.ripe.net/docs/data_api";
    QString summary = "RIPEstat is a large-scale information service and the open data platform of the RIPE NCC. Get essential information on "
                      "IP address space and Autonomous System Numbers (ASNs) along with related statistics on specific hostnames and countries.";

    QMap<QString, QStringList> flags = {{"Abuse Contact Finder",
                                         {PLACEHOLDERTEXT_IP_OR_ASN_OR_CIDR, "The main purpose of this data call is to return abuse contact informations for a Internet number resource. "
                                                                             "Note that this information is not guaranteed to be correct to reach the approriate resource holder."}},
                                        {"Address Space Hierarchy",
                                         {PLACEHOLDERTEXT_IP_RANGE_OR_CIDR, "This data call returns address space objects (inetnum, inet6num...) from the RIPE DB (whois) related (exact, more- and less-specific) "
                                                                            "to the queried resource. Less- and more-specific results are first-level only, further levels would have to be retrieved iteratively."}},
                                        {"Address Space Usage",
                                         {PLACEHOLDERTEXT_IP_RANGE_OR_CIDR, "This data call shows the usage ratio of this prefix or IP range according to the objects currently present in the RIPE Whois database. "
                                                                            "The data is represented as assignments, allocations and statistic on the distribution of the IPs covered by the queried resource."}},
                                        {"AS Overview",
                                         {PLACEHOLDERTEXT_AS, "This data call shows general informations about an ASN like its announcement status and the name of its holder according to the WHOIS service."}},
                                        {"AS Path Length",
                                         {PLACEHOLDERTEXT_AS, "This data call returns AS-path metrics (e.g. shortest or longest AS-path to other ASNs we are peering with) for the queried ASN."}},
                                        {"AS Routing Consistency",
                                         {PLACEHOLDERTEXT_AS, "This data call shows the difference between the routing and the registration state of an ASN. A filter for BGP routes is applied removing non-globally visible prefixes that are not seen by at least 10 RIS full-table peers."}},
                                        {"ASN Neighbours",
                                         {PLACEHOLDERTEXT_AS, "This data call shows information on the network neighbours for a given ASN. This includes statistical information as well as the list of observed ASN neighbours."}},
                                        {"ASN Neighbours History",
                                         {PLACEHOLDERTEXT_AS, "This data call returns information about neigbouring ASNs of a queried ASN extended with history."}},
                                        {"Blocklist",
                                         {PLACEHOLDERTEXT_IP_RANGE_OR_CIDR, "This data call returns blocklist related data for a queried resource."}},
                                        {"Country ASNs",
                                         {PLACEHOLDERTEXT_COUNTRY_CODE, "This data call provides information on a country's registered and routed ASNs. Registered ASNs are based on registration information made public by the Regional Internet Registries. "
                                                                        "The routing information is based on the data collected with the RIPE NCC's RIS system, https://ris.ripe.net.The data call supports history, "
                                                                        "with data points being aligned to times dumps are created in RIS (00:00, 08:00 and 16:00 UTC)."}},
                                        {"Country Resource Stats",
                                         {PLACEHOLDERTEXT_COUNTRY_CODE, "This data call returns statistics on Internet resources for a country"}},
                                        {"Country Resource List",
                                         {PLACEHOLDERTEXT_COUNTRY_CODE, "This data call lists the Internet resources associated with a country, including ASNs, IPv4 ranges and IPv4/6 CIDR prefixes."}},
                                        {"Historical Whois",
                                         {PLACEHOLDERTEXT_ANY, "This data call provides information on objects that are stored in the RIPE DB. The result is aligned to a specific object, "
                                                               "which is identified by an object type and an object key, which is similar to the Whois data call. In contrast to the Whois data call, this data call puts a focus on historical changes of an object and its related objects."
                                                               "Historical changes are given in the form of versions, one version - by default the latest version - is presented with details. "
                                                               "Related objects are separated into referencing and referenced objects. Referencing objects are objects that have a reference to the object in focus and referenced objects are referenced from the object in focus."}},
                                        {"DNS Chain",
                                         {PLACEHOLDERTEXT_DOMAIN_OR_IP, "This data call returns the recursive chain of DNS forward (A/AAAA/CNAME) and reverse (PTR) records starting form either a hostname or an IP address."}},
                                        {"IANA Registry Info",
                                         {PLACEHOLDERTEXT_IP_OR_ASN_OR_CIDR, "This data call gives access to various data sources maintained by IANA"}},
                                        {"Looking Glass",
                                         {PLACEHOLDERTEXT_IP_OR_CIDR, "This data call returns information commonly coming from a Looking Glass. The data is based on a data feed from the RIPE NCC's network of BGP route collectors"
                                                                      "The data processing usually happens with a small delay and can be considered near real-time. The output is structured by collector node (RRC) accompanied by the location and the BGP peers which provide the routing information."}},
                                        {"Maxmind Geo Lite",
                                         {PLACEHOLDERTEXT_IP_OR_CIDR, "This data call returns geolocation information for the given IP space based on MaxMind's GeoLite2 data source."}},
                                        {"Maxmind Geo Lite Announced By AS",
                                         {PLACEHOLDERTEXT_AS, "This data call returns geolocation information for prefixes that are announced by an autonomous system. Annoucement information is based on RIS (http://ris.ripe.net), geolocation information is based on MaxMind's GeoLite2 data."}},
                                        {"Network Info",
                                         {PLACEHOLDERTEXT_IP, "This data call returns the containing prefix and announcing ASN of a given IP address."}},
                                        {"Prefix Overview",
                                         {PLACEHOLDERTEXT_CIDR, "This data call gives a summary of the given prefix, including whether and by whom it is announced."}},
                                        {"Prefix Routing Consistency",
                                         {PLACEHOLDERTEXT_CIDR, "This data call compares the given routes (prefix originating from an ASN) between Routing Registries and actual routing behaviour as seen by the RIPE NCC route collectors (RIS)."}},
                                        {"RIR",
                                         {PLACEHOLDERTEXT_IP_OR_ASN, "This data call shows which RIR(s) allocated/assigned a resource. Include additional information like registration status or country of registration. "
                                                                     "The data is based on RIR stats files, see ftp://ftp.ripe.net/pub/stats/."}},
                                        {"RIR Stats Country",
                                         {PLACEHOLDERTEXT_IP_OR_ASN, "This data call returns geographical information for Internet resources based on RIR Statistics data"}},
                                        {"RIR Prefix Size Distribution",
                                         {PLACEHOLDERTEXT_CIDR, "This data call returns the number of allocations and assignments (below the queried resource) according to registration data provided by Regional Internet Registries."}},
                                        {"RIS First-Last-Seen",
                                         {PLACEHOLDERTEXT_CIDR_OR_ASN, "This data call provides information on when a prefix or ASN was first and last seen in RIS data. The data generally goes back to 2000."}},
                                        {"RIS Prefixes",
                                         {PLACEHOLDERTEXT_ASN, "his data call provides information on prefixes related to an ASN. The data call distinguishes prefixes in the originated and transited ASN."}},
                                        {"RIS Peerings",
                                         {PLACEHOLDERTEXT_CIDR, "This data call returns routes for advertisements of a given IP resource, or that are originated from a given ASN, as seen by the RIPE NCC route collectors."}},
                                        {"Related Prefixes",
                                         {PLACEHOLDERTEXT_IP_RANGE_OR_CIDR, "This data call returns prefixes that overlap or are adjacent to the specified IP resource."}},
                                        {"Reverse DNS",
                                         {PLACEHOLDERTEXT_CIDR, "This data call returns details of reverse DNS delegations for IP prefixes in the RIPE region."}},
                                        {"Reverse DNS IP",
                                         {PLACEHOLDERTEXT_IP, "This is just a simple lookup for the reverse DNS info against a single IP address."}},
                                        {"Routing History",
                                         {PLACEHOLDERTEXT_CIDR_OR_ASN, "This data call shows the history of announcements for prefixes, including the origin ASN and the first hop. The data comes from the RIS route collectors."}},
                                        {"Routing Status",
                                         {PLACEHOLDERTEXT_ANY, "This data call returns a summary of the current BGP routing state of a given IP prefix or ASN, as observed by the RIS route collectors. "
                                                               "Historical lookups are supported - a query has to be aligned to the times (00:00, 08:00 and 16:00 UTC) when RIS data has been collected."}},
                                        {"RRC Info",
                                         {PLACEHOLDERTEXT_NONE, "This data call provides (meta) information on collector nodes (RRCs) of the RIS network (http://ris.ripe.net). This includes geographical and topological location and information on collectors' peers."}},
                                        {"Visibility",
                                         {PLACEHOLDERTEXT_IP_OR_ASN, "This data call provides information on the visibility of a resource as observed from RIS (http://ris.ripe.net)."}},
                                        {"Whats My Ip",
                                         {PLACEHOLDERTEXT_NONE, "his data call returns the IP address of the requestor."}},
                                        {"Whois",
                                         {PLACEHOLDERTEXT_ANY, "This data call returns whois information from the relevant Regional Internet Registry and Routing Registry."}}};

    QMap<int, QList<int>> input_output = {{IN_IP,
                                           {OUT_ASN}},
                                          {IN_ASN,
                                           {OUT_CIDR}}};
};
}

class Ripe: public AbstractOsintModule{

    public:
        Ripe(ScanArgs args);
        ~Ripe() override;

    public slots:
        void start() override;
        void replyFinishedAsn(QNetworkReply *reply) override;
        void replyFinishedCidr(QNetworkReply *reply) override;
};

#endif // RIPE_H
