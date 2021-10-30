#ifndef THREATCROWD_H
#define THREATCROWD_H


#include "../AbstractOsintModule.h"

/*
 * INPUT domain:            OUTPUT: subdomain, ip, email
 * INPUT domain:            OUTPUT: subdomain
 * INPUT email:             OUTPUT: subdomain
 */

namespace ModuleInfo {
    struct Threatcrowd{
        QString name = "Threatcrowd";
        QString url = "https://www.threatcrowd.org/";
        QString summary = "A Search Engine for Threats";
        QMap<QString, QStringList> flags = {{"email", {PLACEHOLDERTEXT_EMAIL, ""}},
                                        {"domain", {PLACEHOLDERTEXT_DOMAIN, ""}},
                                        {"ip", {PLACEHOLDERTEXT_IP, ""}},
                                        {"antivirus", {"antivirus", ""}},
                                        {"file", {"file hash", ""}}};
    };
}

class Threatcrowd: public AbstractOsintModule{

    public:
        Threatcrowd(ScanArgs *args);
        ~Threatcrowd() override;

    public slots:
        void start() override;
        void replyFinishedSubdomain(QNetworkReply *reply) override;
        void replyFinishedIp(QNetworkReply *reply) override;
        void replyFinishedEmail(QNetworkReply *reply) override;
};

#endif // THREATCROWD_H
