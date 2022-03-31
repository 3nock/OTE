#ifndef DNSBUFFEROVERUN_H
#define DNSBUFFEROVERUN_H

#include "../AbstractOsintModule.h"


namespace ModuleInfo {
struct Dnsbufferoverun{
    QString name = OSINT_MODULE_DNSBUFFEROVERRUN;
    QString url = "https://dns.bufferover.run/";
    QString url_apiDoc = "https://dns.bufferover.run/";
    QString summary = "Subdomain enumeration from rapid7 sonar project";
    QMap<QString, QStringList> flags = {{"subdomains",
                                         {PLACEHOLDERTEXT_DOMAIN, "Quickly Extract Ip addresses and subdomains from TLS certificates"}}};

    QMap<int, QList<int>> input_output = {{IN_DOMAIN,
                                           {OUT_SUBDOMAINIP, OUT_SUBDOMAIN, OUT_IP}}};
};
}

class Dnsbufferoverun: public AbstractOsintModule{

    public:
        explicit Dnsbufferoverun(ScanArgs args);
        ~Dnsbufferoverun() override;

    public slots:
        void start() override;
        void replyFinishedSubdomainIp(QNetworkReply *reply) override;
        void replyFinishedSubdomain(QNetworkReply *reply) override;
        void replyFinishedIp(QNetworkReply *reply) override;
};

#endif // DNSBUFFEROVERUN_H
