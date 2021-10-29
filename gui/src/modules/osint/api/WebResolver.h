#ifndef WEBRESOLVER_H
#define WEBRESOLVER_H

#include "../AbstractOsintModule.h"

namespace ModuleInfo {
    struct WebResolver{
        QString name = "WebResolver";
        QString url = "https://webresolver.nl/";
        QString summary = "WebResolver";
        QMap<QString, QString> flags = {{"GeoIP", "domain name"},
                                        {"Dns Resolver", "domain name"},
                                        {"Phone Number Check", "Phone Number"},
                                        {"Screenshot Tool", "domain name"},
                                        {"Website Whois", "domain name"},
                                        {"Website Headers", "domain name"},
                                        {"Portscan", "domain name"},
                                        {"Disposable email checker", "email"},
                                        {"Ip to Website(s)", "ip-address"},
                                        {"Domain information", "domain name"}};
    };
}

class WebResolver: public AbstractOsintModule{

    public:
        WebResolver(ScanArgs *args);
        ~WebResolver() override;

    public slots:
        void start() override;
        void replyFinishedSubdomainIp(QNetworkReply *reply) override;
        void replyFinishedSubdomain(QNetworkReply *reply) override;
        void replyFinishedIp(QNetworkReply *reply) override;

    private:
        QString m_key;
};

#endif // WEBRESOLVER_H
