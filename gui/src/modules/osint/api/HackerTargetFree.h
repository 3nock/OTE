#ifndef HACKERTARGETFREE_H
#define HACKERTARGETFREE_H


#include "../AbstractOsintModule.h"

namespace ModuleInfo {
    struct HackerTargetFree{
        QString name = "HackerTarget";
        QString url = "https://hackertarget.com/";
        QString summary = "From attack surface discovery to vulnerability identification, actionable network intelligence \n"
                          "for IT & security operations.";
        QMap<QString, QString> flags = {{"dnslookup", "domain name"},
                                        {"banner grabbing", "ip-address/cdir"},
                                        {"find shared dns", "dns server name eg dns.google.com"},
                                        {"hostsearch", "domain name"},
                                        {"subnet calculator", "ip-adress/cdir"},
                                        {"zonetransfer", "subdomain name"},
                                        {"reverse dns", "ip-address"},
                                        {"geoip", "ip-address"},
                                        {"reverse ip lookup", "ip-address"},
                                        {"httpheaders", "domain name"},
                                        {"pagelinks", "domain name"},
                                        {"aslookup", "ip-address"}};
    };
}

class HackerTargetFree: public AbstractOsintModule{

    public:
        HackerTargetFree(ScanArgs *args);
        ~HackerTargetFree() override;

    public slots:
        void start() override;
        void replyFinishedSubdomainIp(QNetworkReply *reply) override;
        void replyFinishedSubdomain(QNetworkReply *reply) override;
        void replyFinishedIp(QNetworkReply *reply) override;
        void replyFinishedAsn(QNetworkReply *reply) override;
};


#endif // HACKERTARGETFREE_H
