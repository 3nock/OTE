#ifndef HOST_H
#define HOST_H

#include "../AbstractOsintModule.h"

namespace ModuleInfo {
struct Host{
    QString name = "Host";
    QString url = "https://host.io/";
    QString url_apiDoc = "https://host.io/docs";
    QString summary = "A Powerful and Fast Domain Name Data API. Get comprehensive domain name data, uncover new domains and the relationships between them. "
                      "Built for Cyber Security, Business Intelligence, Competitor Analysis, Market Research and more.";

    QMap<QString, QStringList> flags = {{"web",
                                         {PLACEHOLDERTEXT_DOMAIN, "Metadata scraped from a domain homepage."}},
                                        {"dns",
                                         {PLACEHOLDERTEXT_DOMAIN, "Get all the DNS records stored for a domain. Currently supported records are: A (IPv4 address), AAAA (IPv6 address), MX (mailserver), NS (nameserver)"}},
                                        {"related",
                                         {PLACEHOLDERTEXT_DOMAIN, "Get a count of the number of related domains for all supported lookups we offer"}},
                                        {"full",
                                         {PLACEHOLDERTEXT_DOMAIN, "A single endpoint that includes the data from /api/web, /api/dns, /api/related and IPinfo."}}};

    QMap<int, QList<int>> input_output = {};
};
}

class Host: public AbstractOsintModule{

    public:
        Host(ScanArgs args);
        ~Host() override;

    public slots:
        void start() override;

    private:
        QString m_key;
};

#endif // HOST_H
