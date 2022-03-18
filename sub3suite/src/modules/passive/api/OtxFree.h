#ifndef OTXFREE_H
#define OTXFREE_H

#include "../AbstractOsintModule.h"


namespace ModuleInfo {
struct OtxFree{
    QString name = OSINT_MODULE_OTX;
    QString url = "https://otx.alienvault.com/";
    QString url_apiDoc = "https://otx.alienvault.com/api";
    QString summary = "The World’s First Truly Open Threat Intelligence Community";

    QMap<QString, QStringList> flags = {{"indicator ipv4 general",
                                         {PLACEHOLDERTEXT_IP4, " General information about the IP, such as geo data, and a list of the other sections currently available for this IP address."}},
                                        {"indicator ipv4 geo",
                                         {PLACEHOLDERTEXT_IP4, "A more verbose listing of geographic data (Country code, coordinates, etc.)"}},
                                        {"indicator ipv4 malware",
                                         {PLACEHOLDERTEXT_IP4, "Malware samples analyzed by AlienVault Labs which have been observed connecting to this IP address."}},
                                        {"indicator ipv4 passive_dns",
                                         {PLACEHOLDERTEXT_IP4, "passive dns information about hostnames/domains observed by AlienVault Labs pointing to this IP address."}},
                                        {"indicator ipv4 reputation",
                                         {PLACEHOLDERTEXT_IP4, "OTX data on malicious activity observed by AlienVault Labs (IP Reputation)."}},
                                        {"indicator ipv4 url_list",
                                         {PLACEHOLDERTEXT_IP4, "URLs analyzed by AlienVault Labs which point to or are somehow associated with this IP address."}},
                                        {"indicator ipv6 general",
                                         {PLACEHOLDERTEXT_IP6, " General information about the IP, such as geo data, and a list of the other sections currently available for this IP address."}},
                                        {"indicator ipv6 geo",
                                         {PLACEHOLDERTEXT_IP6, "A more verbose listing of geographic data (Country code, coordinates, etc.)"}},
                                        {"indicator ipv6 malware",
                                         {PLACEHOLDERTEXT_IP6, "Malware samples analyzed by AlienVault Labs which have been observed connecting to this IP address."}},
                                        {"indicator ipv6 passive_dns",
                                         {PLACEHOLDERTEXT_IP6, "passive dns information about hostnames/domains observed by AlienVault Labs pointing to this IP address."}},
                                        {"indicator ipv6 reputation",
                                         {PLACEHOLDERTEXT_IP6, "OTX data on malicious activity observed by AlienVault Labs (IP Reputation)."}},
                                        {"indicator ipv6 url_list",
                                         {PLACEHOLDERTEXT_IP6, "URLs analyzed by AlienVault Labs which point to or are somehow associated with this IP address."}},
                                        {"indicator domain general",
                                         {PLACEHOLDERTEXT_DOMAIN, "General information about the domain, including any pulses, and a list of the other sections currently available for this domain."}},
                                        {"indicator domain geo",
                                         {PLACEHOLDERTEXT_DOMAIN, "A more verbose listing of geographic data (Country code, coordinates, etc.)"}},
                                        {"indicator domain http_scans",
                                         {PLACEHOLDERTEXT_DOMAIN, "Meta data for http(s) connections to the domain."}},
                                        {"indicator domain malware",
                                         {PLACEHOLDERTEXT_DOMAIN, "Malware samples analyzed by AlienVault Labs which have been observed connecting to this domain."}},
                                        {"indicator domain passive_dns",
                                         {PLACEHOLDERTEXT_DOMAIN, "Passive dns records observed by AlienVault Labs pointing to this domain."}},
                                        {"indicator domain url_list",
                                         {PLACEHOLDERTEXT_DOMAIN, "URLs analyzed by AlienVault Labs on this domain."}},
                                        {"indicator domain whois",
                                         {PLACEHOLDERTEXT_DOMAIN, "Whois records for the domain."}},
                                        {"indicator hostname general",
                                         {PLACEHOLDERTEXT_HOSTNAME, "General information about the hostname, including any pulses, and a list of the other sections currently available for this hostname."}},
                                        {"indicator hostname geo",
                                         {PLACEHOLDERTEXT_HOSTNAME, "A more verbose listing of geographic data (Country code, coordinates, etc.)"}},
                                        {"indicator hostname http_scans",
                                         {PLACEHOLDERTEXT_HOSTNAME, "Meta data for http(s) connections to the hostname."}},
                                        {"indicator hostname malware",
                                         {PLACEHOLDERTEXT_HOSTNAME, "Malware samples analyzed by AlienVault Labs which have been observed connecting to this hostname."}},
                                        {"indicator hostname passive_dns",
                                         {PLACEHOLDERTEXT_HOSTNAME, "passive dns records observed by AlienVault Labs pointing to this hostname."}},
                                        {"indicator hostname url_list",
                                         {PLACEHOLDERTEXT_HOSTNAME, "URLs analyzed by AlienVault Labs on this hostname."}},
                                        {"indicator url general",
                                         {PLACEHOLDERTEXT_URL, "Historical geographic info, any pulses this indicator is on, list of the other sections currently available for this URL."}},
                                        {"indicator url url_list",
                                         {PLACEHOLDERTEXT_URL, "Full results (potentially multiple) from AlienVault Labs url analysis."}}};

    QMap<int, QList<int>> input_output = {{IN_DOMAIN,
                                           {OUT_SUBDOMAINIP, OUT_SUBDOMAIN, OUT_IP, OUT_ASN}},
                                          {IN_IP,
                                           {OUT_SUBDOMAINIP, OUT_SUBDOMAIN, OUT_IP, OUT_ASN}}};
};
}

class OtxFree: public AbstractOsintModule{

    public:
        OtxFree(ScanArgs args);
        ~OtxFree() override;

    public slots:
        void start() override;
        void replyFinishedSubdomainIp(QNetworkReply *reply) override;
        void replyFinishedSubdomain(QNetworkReply *reply) override;
        void replyFinishedIp(QNetworkReply *reply) override;
        void replyFinishedAsn(QNetworkReply *reply) override;
};

#endif // OTXFREE_H
