#ifndef OMNISINT_H
#define OMNISINT_H

#include "../AbstractOsintModule.h"

/*
 * INPUT domain:            OUTPUT: subdomain
 * INPUT ip:                OUTPUT: subdomain
 */

namespace ModuleInfo {
    struct Omnisint{
        QString name = "Omnisint";
        QString url = "https://omnisint.io/";
        QString url_apiDoc = "https://github.com/Cgboal/SonarSearch";
        QString summary = "Rapid7's DNS Database easily searchable via a lightening fast API, \n"
                          "with domains available in milliseconds.";

        QMap<QString, QStringList> flags = {{"subdomains",
                                             {PLACEHOLDERTEXT_DOMAIN, "Perform research to enumerate instances of cloud services (eg: *.amazonaws.com, Azure and more.)"}},
                                            {"tlds",
                                             {PLACEHOLDERTEXT_DOMAIN, "Easily identify additional Top Level Domains registered for a domain without the need for brute forcing. "
                                                                      "Find additional attack surfaces by leveraging international distribution of assets."}},
                                            {"all",
                                             {PLACEHOLDERTEXT_DOMAIN, "All Results types"}},
                                            {"reverse ip",
                                             {PLACEHOLDERTEXT_IP_OR_CIDR, "Identify other domains hosted on the IP address of your target, or even their ASN as supported by CIDR range searchs."
                                                                          "Access a comprehensive reverse DNS database without limits"}}};
    };
}

class Omnisint: public AbstractOsintModule{

    public:
        Omnisint(ScanArgs *args);
        ~Omnisint() override;

    public slots:
        void start() override;
        void replyFinishedSubdomain(QNetworkReply *) override;
};

#endif // OMNISINT_H
