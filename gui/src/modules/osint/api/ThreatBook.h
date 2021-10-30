#ifndef THREATBOOK_H
#define THREATBOOK_H

#include "../AbstractOsintModule.h"

/*
 * INPUT domain:            OUTPUT: subdomain, ip
 * INPUT ip:                OUTPUT: subdomain, ip, asn
 */

namespace ModuleInfo {
    struct ThreatBook{
        QString name = "ThreatBook";
        QString url = "https://threatbook.cn/";
        QString url_apiDoc = "";
        QString summary = "Threat detection platform";
        QMap<QString, QStringList> flags = {{"subdomains", {PLACEHOLDERTEXT_DOMAIN, ""}},
                                        {"domain query", {PLACEHOLDERTEXT_DOMAIN, ""}},
                                        {"domain adv query", {PLACEHOLDERTEXT_DOMAIN, ""}},
                                        {"ip adv query", {PLACEHOLDERTEXT_IP, ""}},
                                        {"ip query", {PLACEHOLDERTEXT_IP, ""}}};
    };
}

class ThreatBook: public AbstractOsintModule{

    public:
        ThreatBook(ScanArgs *args);
        ~ThreatBook() override;

    public slots:
        void start() override;
        void replyFinishedSubdomain(QNetworkReply *reply) override;
        void replyFinishedAsn(QNetworkReply *reply) override;
        void replyFinishedIp(QNetworkReply *reply) override;

    private:
        QString m_key = nullptr;
};

#endif // THREATBOOK_H
