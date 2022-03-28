#ifndef DUCKDUCKGO_H
#define DUCKDUCKGO_H

#include "../AbstractOsintModule.h"

namespace ModuleInfo {
struct DuckDuckGo{
    QString name = OSINT_MODULE_DUCKDUCKGO;
    QString url = "https://duckduckgo.com/";
    QString url_apiDoc = "";
    QString summary = "";
    QMap<QString, QStringList> flags = {};
    QMap<int, QList<int>> input_output = {{IN_DOMAIN,
                                           {OUT_SUBDOMAIN, OUT_URL, OUT_EMAIL}}};
};
}

class DuckDuckGo: public AbstractOsintModule{

    public:
        DuckDuckGo(ScanArgs args);
        ~DuckDuckGo() override;

    public slots:
        void start() override;
        void replyFinishedSubdomain(QNetworkReply *reply) override;
        void replyFinishedEmail(QNetworkReply *reply) override;
        void replyFinishedUrl(QNetworkReply *reply) override;
};
#endif // DUCKDUCKGO_H
