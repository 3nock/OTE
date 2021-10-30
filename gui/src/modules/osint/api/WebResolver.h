#ifndef WEBRESOLVER_H
#define WEBRESOLVER_H

#include "../AbstractOsintModule.h"

/*
 * INPUT domain:            OUTPUT: subdomainIP, subdomain, ip
 */

namespace ModuleInfo {
    struct WebResolver{
        QString name = "WebResolver";
        QString url = "https://webresolver.nl/";
        QString url_apiDoc = "";
        QString summary = "WebResolver";
        QMap<QString, QStringList> flags = {{"GeoIP", {PLACEHOLDERTEXT_DOMAIN, ""}},
                                        {"Dns Resolver", {PLACEHOLDERTEXT_DOMAIN, ""}},
                                        {"Phone Number Check", {PLACEHOLDERTEXT_PHONE, ""}},
                                        {"Screenshot Tool", {PLACEHOLDERTEXT_DOMAIN, ""}},
                                        {"Website Whois", {PLACEHOLDERTEXT_DOMAIN, ""}},
                                        {"Website Headers", {PLACEHOLDERTEXT_DOMAIN, ""}},
                                        {"Portscan", {PLACEHOLDERTEXT_DOMAIN, ""}},
                                        {"Disposable email checker", {PLACEHOLDERTEXT_EMAIL, ""}},
                                        {"Ip to Website(s)", {PLACEHOLDERTEXT_IP, ""}},
                                        {"Domain information", {PLACEHOLDERTEXT_DOMAIN, ""}}};
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
