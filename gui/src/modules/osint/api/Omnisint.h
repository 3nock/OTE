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
        QString url_apiDoc = "";
        QString summary = "Rapid7's DNS Database easily searchable via a lightening fast API, \n"
                          "with domains available in milliseconds.";

        QMap<QString, QStringList> flags = {{"subdomains", {PLACEHOLDERTEXT_DOMAIN, ""}},
                                        {"tlds", {PLACEHOLDERTEXT_DOMAIN, ""}},
                                        {"all", {PLACEHOLDERTEXT_DOMAIN, ""}},
                                        {"reverse ip", {PLACEHOLDERTEXT_IP_OR_CIDR, ""}}};
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
