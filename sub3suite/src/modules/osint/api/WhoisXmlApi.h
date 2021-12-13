#ifndef WHOISXMLAPI_H
#define WHOISXMLAPI_H

#include "../AbstractOsintModule.h"


namespace ModuleInfo {
struct WhoisXmlApi{
    QString name = "WhoisXmlApi";
    QString url = "https://whoisxmlapi.com/";
    QString url_apiDoc = "https://whoisxmlapi.github.io/";
    QString summary = "The Who Behind Cyber Threat Intelligence";

    QMap<QString, QStringList> flags = {{"whois",
                                         {PLACEHOLDERTEXT_DOMAIN, "Our hosted WHOIS API service provides the registration details, also known as the WHOIS record data, of a domain name, an IP address, or an email address. "
                                                                  "All of the WHOIS information we offer is adequately parsed and normalized to a consistent format so it can easily fit in with your business operations."}},
                                        {"ip whois", {PLACEHOLDERTEXT_IP, "Our hosted WHOIS API service provides the registration details, also known as the WHOIS record data, of a domain name, an IP address, or an email address. "
                                                                          "All of the WHOIS information we offer is adequately parsed and normalized to a consistent format so it can easily fit in with your business operations."}},
                                        {"dns Lookup", {PLACEHOLDERTEXT_DOMAIN, "A DNS lookup API call gives you a domain’s corresponding IP address from "
                                                                                "its A record as well as the domain’s mail server (MX record), nameserver (NS record), SPF (TXT record), and more records."}},
                                        {"email Verification",
                                         {PLACEHOLDERTEXT_EMAIL, " Boost email marketing results and get rid of abusers with our complete email verification product line—API, web-based lookup tool, bulk processing capabilities, and disposable email domains database."}},
                                        {"domain availability", {PLACEHOLDERTEXT_DOMAIN, ""}},
                                        {"ip netblocks asn",
                                         {PLACEHOLDERTEXT_ASN, "With one API call get exhaustive information on the IP range that a given IP address belongs to, with detailed ownership information regarding each range."}},
                                        {"ip netblocks ip",
                                         {PLACEHOLDERTEXT_IP, "With one API call get exhaustive information on the IP range that a given IP address belongs to, with detailed ownership information regarding each range."}},
                                        {"ip netblocks org",
                                         {PLACEHOLDERTEXT_ORG, "With one API call get exhaustive information on the IP range that a given IP address belongs to, with detailed ownership information regarding each range."}},
                                        {"reverse ip", {PLACEHOLDERTEXT_IP, ""}},
                                        {"reverse mx",
                                         {PLACEHOLDERTEXT_MX, "Check which domains link back to a mail server via API calls to avoid hosts with a dubious reputation, improve email delivery, and other purposes."}},
                                        {"reverse ns",
                                         {PLACEHOLDERTEXT_NS, "Reverse NS API lets you see the list of domain names tied to the same nameserver via API calls with outputs in JSON and XML."}},
                                        {"reverse whois",
                                         {PLACEHOLDERTEXT_DOMAIN, "With one API call for search terms such as a domain owner’s name, email address, phone number, postal address, etc., you will get a list of all the linked domains that contain the identifier."}},
                                        {"subdomain lookup",
                                         {PLACEHOLDERTEXT_DOMAIN, "With one API call, instantly get a list of all subdomains related to a given domain name to reveal a company’s entire web infrastructure. Use it to identify and fortify potential vulnerabilities."}},
                                        {"website contacts",
                                         {PLACEHOLDERTEXT_DOMAIN, "Get well-structured domain owner contact information, including company name and key contacts with direct-dial phone numbers, email addresses, "
                                                                  "and social media links based on data parsed from website content, social networks, Secure Sockets Layer (SSL) certificates, and other sources."}},
                                        {"website screenshot",
                                         {PLACEHOLDERTEXT_DOMAIN, "Our Google Chrome engine-based Screenshot API allows you to instantly get a screenshot of any web page as a jpg, png or "
                                                                  "PDF file with embedded links which you can easily integrate into your product or use for any other business purposes."}},
                                        {"whois history",
                                         {PLACEHOLDERTEXT_DOMAIN, "Start checking historical WHOIS records via API calls in your developed systems and commercial applications."}}};

    QMap<int, QList<int>> input_output = {{IN_DOMAIN,
                                           {OUT_SUBDOMAIN, OUT_EMAIL}},
                                          {IN_IP,
                                           {OUT_SUBDOMAIN, OUT_ASN}},
                                          {IN_IP,
                                           {OUT_ASN}}};
};
}

class WhoisXmlApi: public AbstractOsintModule{

    public:
        WhoisXmlApi(ScanArgs args);
        ~WhoisXmlApi() override;

    public slots:
        void start() override;
        void replyFinishedSubdomain(QNetworkReply *reply) override;
        void replyFinishedAsn(QNetworkReply *reply) override;
        void replyFinishedEmail(QNetworkReply *reply) override;

    private:
        QString m_key = nullptr;
};

#endif // WHOISXMLAPI_H
