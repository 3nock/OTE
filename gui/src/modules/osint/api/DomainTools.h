#ifndef DOMAINTOOLS_H
#define DOMAINTOOLS_H

#include "../AbstractOsintModule.h"


namespace ModuleInfo {
    struct DomainTools{
        QString name = "DomainTools";
        QString url = "https://www.domaintools.com/";
        QString url_apiDoc = "https://www.domaintools.com/resources/api-documentation";
        QString summary = "Connect indicators from your network with nearly every active domain and IP address on the Internet. "
                          "Learn how this data can inform risk assessments, help profile attackers, guide online fraud investigations, and map cyber activity to attacker infrastructure.";

        QMap<QString, QStringList> flags = {{"Account Information",
                                             {PLACEHOLDERTEXT_NONE, "Provides a snapshot of product usage for an account"}},
                                            {"Brand Monitor",
                                             {PLACEHOLDERTEXT_QUERY, "Monitor new domain registrations for specific keywords"}},
                                            {"Domain Profile",
                                             {PLACEHOLDERTEXT_DOMAIN, "Basic registrant, server, and registration data for a domain name, plus preview data for other products"}},
                                            {"Domain Reputation",
                                             {PLACEHOLDERTEXT_DOMAIN, "Provides risk scores based on a domain's proximity to known-bad domains"}},
                                            {"Domain Risk Score",
                                             {PLACEHOLDERTEXT_DOMAIN, "Provides risk scores and threat predictions based on DomainTools Proximity and Threat Profile algorithms"}},
                                            {"Domain Search",
                                             {PLACEHOLDERTEXT_QUERY, "Searches active and deleted domain names that match a query string"}},
                                            {"Hosting History",
                                             {PLACEHOLDERTEXT_DOMAIN, "Provides the registrar, IP and name server history for a domain name"}},
                                            {"IP Monitor",
                                             {PLACEHOLDERTEXT_IP, "Receive notification when there are new and/or deleted domains on a given IP Address"}},
                                            {"IP Registrant Monitor",
                                             {PLACEHOLDERTEXT_QUERY, "Receive notification when specific people or organizations are allocated new IP ranges or have existing ranges de-allocated"}},
                                            {"Iris Enrich",
                                             {"domain, domain", "Designed to support high query volumes with batch processing and fast response times, "
                                                                    "the Iris Enrich API provides actionable insights-at-scale with enterprise-scale ingestion of DomainTools data"}},
                                            {"Iris Investigate Domain",
                                             {"domain, domain", "Suited for investigation and orchestration at human scale, the Iris Investigate API delivers dozens of domain name attributes on every result"}},
                                            {"Iris Investigate Ip",
                                             {PLACEHOLDERTEXT_IP, "Suited for investigation and orchestration at human scale, the Iris Investigate API delivers dozens of domain name attributes on every result"}},
                                            {"Iris Pivot Domain",
                                             {"domain, domain", "Leverage Iris' pivot engine in API form"}},
                                            {"Iris Pivot Ip",
                                             {PLACEHOLDERTEXT_IP, "Leverage Iris' pivot engine in API form"}},
                                            {"Name Server Monitor",
                                             {PLACEHOLDERTEXT_NS, "Receive notification when there are new and/or deleted domains on a given Domain Name Server"}},
                                            {"Parsed Whois",
                                             {PLACEHOLDERTEXT_DOMAIN, "Parsed results for Whois records for domain names and IP addresses"}},
                                            {"PhishEye",
                                             {PLACEHOLDERTEXT_QUERY, "Provides programmatic access to daily monitor results from the DomainTools PhishEye product"}},
                                            {"Registrant Monitor",
                                             {PLACEHOLDERTEXT_QUERY, "Receive notification when specific people or organizations register, renew or delete domain names"}},
                                            {"Reverse IP",
                                             {PLACEHOLDERTEXT_IP, "List of domains that share the same network host"}},
                                            {"Reverse IP Whois",
                                             {PLACEHOLDERTEXT_NONE, "Provides a list of IP network ranges with Whois records that match a specific query"}},
                                            {"Reverse Name Server",
                                             {PLACEHOLDERTEXT_NS, "List of domains that share the same primary name server"}},
                                            {"Reverse Whois",
                                             {PLACEHOLDERTEXT_QUERY, "Provides a list of domain names with Whois records that match a specific query"}},
                                            {"Whois History",
                                             {PLACEHOLDERTEXT_NONE, "Historical Whois records"}},
                                            {"Whois Lookup",
                                             {PLACEHOLDERTEXT_NONE, "Whois records for domain names and IP addresses"}},
                                           };
        QMap<int, QList<int>> input_output = {{IN_DOMAIN,
                                               {OUT_SUBDOMAIN}}};
    };
}


class DomainTools: public AbstractOsintModule{

    public:
        DomainTools(ScanArgs *args);
        ~DomainTools() override;

    public slots:
        void start() override;
        void replyFinishedSubdomainIp(QNetworkReply *reply) override;
        void replyFinishedSubdomain(QNetworkReply *reply) override;

    private:
        QString m_key;
        QString m_username;
};
#endif // DOMAINTOOLS_H
