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
                                        {"ip query", "ip-address"}};
    };
}

class ThreatBook: public AbstractOsintModule{

    public:
        ThreatBook(ScanArgs *args);
        ~ThreatBook() override;

    public slots:
        void start() override;
        void replyFinished(QNetworkReply *) override;

    private:
        QString m_key = nullptr;
};

#endif // THREATBOOK_H
