#ifndef DNSBUFFEROVERUN_H
#define DNSBUFFEROVERUN_H

#include "../AbstractOsintModule.h"

/*
 * INPUT domain:            OUTPUT: ip, subdomain, subdomainIp
 */

namespace ModuleInfo {
    struct Dnsbufferoverun{
        QString name = "Dnsbufferoverun";
        QString url = "https://dns.bufferover.run/";
        QString url_apiDoc = "";
        QString summary = "Subdomain enumeration from rapid7 sonar project";
        QMap<QString, QStringList> flags = {{"subdomains", {PLACEHOLDERTEXT_DOMAIN, ""}}};
    };
}

class Dnsbufferoverun: public AbstractOsintModule{

    public:
        Dnsbufferoverun(ScanArgs *args);
        ~Dnsbufferoverun() override;

    public slots:
        void start() override;
        void replyFinishedSubdomainIp(QNetworkReply *reply) override;
        void replyFinishedSubdomain(QNetworkReply *reply) override;
        void replyFinishedIp(QNetworkReply *reply) override;
};

#endif // DNSBUFFEROVERUN_H
