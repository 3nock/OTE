#ifndef WEBRESOLVER_H
#define WEBRESOLVER_H

#include "../AbstractOsintModule.h"


namespace ModuleInfo {
struct WebResolver{
    QString name = OSINT_MODULE_WEBRESOLVER;
    QString url = "https://webresolver.nl/";
    QString url_apiDoc = "https://webresolver.nl/api";
    QString summary = "WebResolver";
    QMap<QString, QStringList> flags = {{"GeoIP",
                                         {PLACEHOLDERTEXT_DOMAIN, "Geo Location data. Supports Domain, IPv4 and IPv6."}},
                                        {"Dns Resolver",
                                         {PLACEHOLDERTEXT_DOMAIN, "Get the DNS records from a domain."}},
                                        {"Phone Number Check",
                                         {PLACEHOLDERTEXT_PHONE, "Looks up information about a specific phone number. (Use international phone format)."}},
                                        {"Screenshot Tool",
                                         {PLACEHOLDERTEXT_DOMAIN, "Creates a screenshot of any website/url."}},
                                        {"Website Whois",
                                         {PLACEHOLDERTEXT_DOMAIN, "Get the registration information from a domain."}},
                                        {"Website Headers",
                                         {PLACEHOLDERTEXT_DOMAIN, "Get the website header information from a domain."}},
                                        {"Portscan",
                                         {PLACEHOLDERTEXT_DOMAIN, "Scan a port to check if the port is open or closed on a host."}},
                                        {"Disposable email checker",
                                         {PLACEHOLDERTEXT_EMAIL, "Search trough a database with known disposable email servers to check if a domain is disposable."}},
                                        {"Ip to Website(s)",
                                         {PLACEHOLDERTEXT_IP, "Tries to find any websites linked to an IP you entered."}},
                                        {"Domain information",
                                         {PLACEHOLDERTEXT_DOMAIN, "Get all the information from a domain such as: IP history, subdomains & domain score"}}};

    QMap<int, QList<int>> input_output = {{IN_DOMAIN,
                                           {OUT_SUBDOMAINIP, OUT_SUBDOMAIN, OUT_IP}}};
};
}

class WebResolver: public AbstractOsintModule{

    public:
        explicit WebResolver(ScanArgs args);
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
