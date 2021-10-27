#ifndef THREATMINER_H
#define THREATMINER_H


#include "../AbstractOsintModule.h"

namespace ModuleInfo {
    struct Threatminer{
        QString name = "Threatminer";
        QString url = "https://threatminer.org/";
        QString summary = "Data Mining for threat Intelligence";
        QMap<QString, QString> flags = {{"domain whois", "domain name"},
                                        {"domain passive dns", "domain name"},
                                        {"domain subdomains", "domain name"},
                                        {"domain Query URI", "domain name"},
                                        {"ip whois", "ip-address"},
                                        {"ip passive dns", "ip-address"},
                                        {"ip Query URI", "ip-address"},
                                        {"ip ssl certs", "ip-address"},
                                        {"ssl Hosts", "cert id"}};
    };
}

class Threatminer: public AbstractOsintModule{

    public:
        Threatminer(ScanArgs *args);
        ~Threatminer() override;

    public slots:
        void start() override;
        void replyFinishedSubdomain(QNetworkReply *reply) override;
        void replyFinishedIp(QNetworkReply *reply) override;
        void replyFinishedEmail(QNetworkReply *reply) override;
        void replyFinishedAsn(QNetworkReply *reply) override;
        void replyFinishedUrl(QNetworkReply *reply) override;
        void replyFinishedCertFingerprint(QNetworkReply *reply) override;
};

#endif // THREATMINER_H
