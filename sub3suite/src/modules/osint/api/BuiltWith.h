#ifndef BUILTWITH_H
#define BUILTWITH_H

#include "../AbstractOsintModule.h"


namespace ModuleInfo {
struct BuiltWith{
    QString name = OSINT_MODULE_BUILTWITH;
    QString url = "https://builtwith.com/";
    QString url_apiDoc = "https://api.builtwith.com/";
    QString summary = "Find out what websites are Built With";
    QMap<QString, QStringList> flags = {{"Free API",
                                         {PLACEHOLDERTEXT_DOMAIN, "Returns the technology that the domain is builtwith"}},
                                       {"Domain API",
                                        {PLACEHOLDERTEXT_DOMAIN, "The BuiltWith Domain API provides XML and JSON access to the technology information of a website "
                                                                 "which includes all technical information as found on detailed lookups at builtwith.com and additional meta data where available."}},
                                       {"List API",
                                         {"Technology Name eg Shopify", "The BuiltWith Lists API provides XML and JSON access to lists of websites using particular web technologies across the entire internet."}},
                                        {"Relationships API",
                                         {PLACEHOLDERTEXT_DOMAIN, "Get XML and JSON results of relationships between websites that show what sites are linked together, by what and for how long."}},
                                       {"Redirects API",
                                         {PLACEHOLDERTEXT_DOMAIN, "API endpoint that allows you to get access to live and historical redirects for a website."}},
                                       {"Keyword API",
                                         {PLACEHOLDERTEXT_DOMAIN, "API endpoint that allows you to get access to keywords connected to websites."}},
                                       {"Trends API",
                                         {"Technology Name eg Shopify", "The BuiltWith Trends API provides XML and JSON access to the technology trends data found on BuiltWith Trends."}},
                                       {"Company API",
                                        {"Company name eg BuiltWith", "Get domain names from lists of company names you supply to us."}},
                                       {"Live API",
                                        {PLACEHOLDERTEXT_DOMAIN, "The BuiltWith Domain Live API does a comprehensive technology search across the domain you provide us, "
                                                                 "this includes indexing internal pages, subdomains, tag managers, ads.txt and technology versions on the site live."}},
                                       {"Trust API",
                                        {PLACEHOLDERTEXT_DOMAIN, "The BuiltWith Trust API provides you with an overview of a website domain - allowing you to make the call on how much it can be trusted. "
                                                                 "We use attributes such as technology spend, time, relationships with other sites, current response, "
                                                                 "keywords and undisclosed methods to determine the trustworthiness of a domain."}}};

    QMap<int, QList<int>> input_output = {};
};
}

class BuiltWith: public AbstractOsintModule{

    public:
        BuiltWith(ScanArgs args);
        ~BuiltWith() override;

    public slots:
        void start() override;

    private:
        QString m_key;
};
#endif // BUILTWITH_H
