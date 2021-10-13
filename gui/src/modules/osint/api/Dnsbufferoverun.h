#ifndef DNSBUFFEROVERUN_H
#define DNSBUFFEROVERUN_H

#include "../AbstractOsintModule.h"

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
        void replyFinished(QNetworkReply *) override;
};

#endif // DNSBUFFEROVERUN_H
