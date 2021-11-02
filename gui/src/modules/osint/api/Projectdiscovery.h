#ifndef PROJECTDISCOVERY_H
#define PROJECTDISCOVERY_H

#include "../AbstractOsintModule.h"

/*
 * INPUT domain:            OUTPUT: subdomain
 */

namespace ModuleInfo {
    struct Projectdiscovery{
        QString name = "Projectdiscovery";
        QString url = "https://chaos.projectdiscovery.io/";
        QString url_apiDoc = "";
        QString summary = "We actively collect and maintain internet-wide assets' data, \n"
                          "this project is meant to enhance research and analyse changes around DNS for better insights";

        QMap<QString, QStringList> flags = {{"subdomain",
                                             {PLACEHOLDERTEXT_DOMAIN, "Returns Subdomains Associated with the domain"}}};
    };
}

class Projectdiscovery: public AbstractOsintModule{

    public:
        Projectdiscovery(ScanArgs *args);
        ~Projectdiscovery() override;

    public slots:
        void start() override;
        void replyFinishedSubdomain(QNetworkReply *) override;

    private:
        QString m_key;
};

#endif // PROJECTDISCOVERY_H
