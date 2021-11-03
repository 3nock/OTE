#ifndef DETECTIFY_H
#define DETECTIFY_H

#include "../AbstractOsintModule.h"

namespace ModuleInfo {
    struct Detectify{
        QString name = "Detectify";
        QString url = "https://detectify.com/";
        QString url_apiDoc = "https://developer.detectify.com/";
        QString summary = "Detectify is an External Attack Surface Management solution powered by a world-leading ethical hacker community. "
                          "We enable security teams to map out their entire attack surface and take a proactive approach in resolving vulnerabilities within their whole IT ecosystem, including those impacting third-party vendors.";

        QMap<QString, QStringList> flags = {{"subdomain",
                                             {PLACEHOLDERTEXT_DOMAIN, "Returns Subdomains of a particular domain"}}};
    };
}

class Detectify: public AbstractOsintModule{

    public:
        Detectify(ScanArgs *args);
        ~Detectify() override;

    public slots:
        void start() override;
        void replyFinishedSubdomain(QNetworkReply *reply) override;
};
#endif // DETECTIFY_H
