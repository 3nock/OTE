#ifndef FULLHUNT_H
#define FULLHUNT_H

#include "../AbstractOsintModule.h"


namespace ModuleInfo {
    struct FullHunt{
        QString name = "FullHunt";
        QString url = "https://fullhunt.io/";
        QString url_apiDoc = "https://api-docs.fullhunt.io/";
        QString summary = "FullHunt is the attack surface database of the entire Internet. FullHunt enables companies to discover all of their attack surfaces, "
                          "monitor them for exposure, and continuously scan them for the latest security vulnerabilities. All, in a single platform, and more.";

        QMap<QString, QStringList> flags = {{"Domain details",
                                             {PLACEHOLDERTEXT_DOMAIN, "Get domain details, subdomains, associated hosts, DNS records, exposed ports, and more."}},
                                            {"Domain subdomains",
                                             {PLACEHOLDERTEXT_DOMAIN, "List all discovered subdomains for a given domain."}},
                                            {"Host details",
                                             {PLACEHOLDERTEXT_HOSTNAME, "This endpoint retrieves details for a given host."}}};

        QMap<int, QList<int>> input_output = {{IN_DOMAIN, {OUT_SUBDOMAIN}}};
    };
}

class FullHunt: public AbstractOsintModule{

    public:
        FullHunt(ScanArgs *args);
        ~FullHunt() override;

    public slots:
        void start() override;
        void replyFinishedSubdomain(QNetworkReply *reply) override;

    private:
        QString m_key;
};

#endif // FULLHUNT_H
