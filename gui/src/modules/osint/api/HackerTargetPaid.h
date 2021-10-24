#ifndef HACKERTARGETPAID_H
#define HACKERTARGETPAID_H

#include "../AbstractOsintModule.h"

namespace ModuleInfo {
    struct HackerTargetPaid{
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
                                        {"aslookup", "ip-address"},
                                        {"ping", "ip-address"},
                                        {"traceroute", "ip-address"},
                                        {"whois", "ip-address"}};
    };
}

class HackerTargetPaid: public AbstractOsintModule{

    public:
        HackerTargetPaid(ScanArgs *args);
        ~HackerTargetPaid() override;

    public slots:
        void start() override;
        void replyFinishedSubdomainIp(QNetworkReply *reply) override;
        void replyFinishedSubdomain(QNetworkReply *reply) override;
        void replyFinishedIp(QNetworkReply *reply) override;
        void replyFinishedAsn(QNetworkReply *reply) override;

    private:
        QString m_key;
};


#endif // HACKERTARGETPAID_H
