#ifndef OTXPAID_H
#define OTXPAID_H

#include "../AbstractOsintModule.h"

namespace ModuleInfo {
    struct OtxPaid{
        QString name = "Otx";
        QString url = "https://otx.alienvault.com/";
        QString summary = "The World’s First Truly Open Threat Intelligence Community";

        QMap<QString, QString> flags = {{"indicator ipv4 general", "ipv4 address"},
                                        {"indicator ipv4 geo", "ipv4 address"},
                                        {"indicator ipv4 malware", "ipv4 address"},
                                        {"indicator ipv4 passive_dns", "ipv4 address"},
                                        {"indicator ipv4 reputation", "ipv4 address"},
                                        {"indicator ipv4 url_list", "ipv4 address"},
                                        {"indicator ipv6 general", "ipv6 address"},
                                        {"indicator ipv6 geo", "ipv6 address"},
                                        {"indicator ipv6 malware", "ipv6 address"},
                                        {"indicator ipv6 passive_dns", "ipv6 address"},
                                        {"indicator ipv6 reputation", "ipv6 address"},
                                        {"indicator ipv6 url_list", "ipv6 address"},
                                        {"indicator domain general", "domain name"},
                                        {"indicator domain geo", "domain name"},
                                        {"indicator domain http_scans", "domain name"},
                                        {"indicator domain malware", "domain name"},
                                        {"indicator domain passive_dns", "domain name"},
                                        {"indicator domain url_list", "domain name"},
                                        {"indicator domain whois", "domain name"},
                                        {"indicator hostname general", "hostname"},
                                        {"indicator hostname geo", "hostname"},
                                        {"indicator hostname http_scans", "hostname"},
                                        {"indicator hostname malware", "hostname"},
                                        {"indicator hostname passive_dns", "hostname"},
                                        {"indicator hostname url_list", "hostname"},
                                        {"indicator url general", "url"},
                                        {"indicator url url_list", "url"}};
    };
}

class OtxPaid: public AbstractOsintModule{

    public:
        OtxPaid(ScanArgs *args);
        ~OtxPaid() override;

    public slots:
        void start() override;
        void replyFinishedSubdomainIp(QNetworkReply *reply) override;
        void replyFinishedSubdomain(QNetworkReply *reply) override;
        void replyFinishedIp(QNetworkReply *reply) override;
        void replyFinishedAsn(QNetworkReply *reply) override;

    private:
        QString m_key;
};
#endif // OTXPAID_H
