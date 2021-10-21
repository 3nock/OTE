#ifndef DNSBUFFEROVERUN_H
#define DNSBUFFEROVERUN_H

#include "../AbstractOsintModule.h"

#define SUBOMAINS 0

namespace ModuleInfo {
    struct Dnsbufferoverun{
        QString name = "Dnsbufferoverun";
        QString url = "https://dns.bufferover.run/";
        QString summary = "Subdomain enumeration from rapid7 sonar project";
        QMap<QString, QString> flags = {{"subdomains", "domain name"}};
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
