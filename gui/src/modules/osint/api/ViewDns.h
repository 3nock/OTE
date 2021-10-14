#ifndef VIEWDNS_H
#define VIEWDNS_H

#include "../AbstractOsintModule.h"

namespace ModuleInfo {
    struct ViewDns{
        QString name = "ViewDns";
        QString url = "https://otx.alienvault.com/";
        QString summary = "The World’s First Truly Open Threat Intelligence Community";
        QMap<QString, QString> flags = {{"Abuse Contact Lookup", "domain name"},
                                        {"DNS Propagation Checker", "domain name"},
                                        {"DNS Record Lookup", "domain name"},
                                        {"Domain/IP Whois", "hostname/ip"},
                                        {"IP History", "domain name"},
                                        {"IP Location Finder", "ip-address"},
                                        {"MAC Address Lookup", "Mac address"},
                                        {"Ping", "domain name"},
                                        {"Port Scanner", "domain name"},
                                        {"Reverse DNS Lookup", "ip-address"},
                                        {"Reverse IP Lookup", "ip-address"},
                                        {"Reverse MX Lookup", "MX domain name"},
                                        {"Reverse NS Lookup", "NS domain name"},
                                        {"Reverse Whois Lookup", "domain name"}};
    };
}

class ViewDns: public AbstractOsintModule{

    public:
        ViewDns(ScanArgs *args);
        ~ViewDns() override;

    public slots:
        void start() override;
        void replyFinished(QNetworkReply *) override;

    private:
        QString m_key = nullptr;
};
#endif // VIEWDNS_H
