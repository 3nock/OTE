#ifndef THREATBOOK_H
#define THREATBOOK_H

#include "../AbstractOsintModule.h"

namespace ModuleInfo {
    struct ThreatBook{
        QString name = "ThreatBook";
        QString url = "https://threatbook.cn/";
        QString summary = "Threat detection platform";
        QMap<QString, QString> flags = {{"subdomains", "domain name"},
                                        {"domain query", "domain name"},
                                        {"domain adv query", "domain name"},
                                        {"ip adv query", "ip-address"},
                                        {"ip query", "ip-address"}};
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
