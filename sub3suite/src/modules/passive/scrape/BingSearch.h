#ifndef BINGSEARCH_H
#define BINGSEARCH_H

#include "../AbstractOsintModule.h"

namespace ModuleInfo {
struct BingSearch{
    QString name = OSINT_MODULE_BINGSEARCH;
    QString url = "https://bing.com/";
    QString url_apiDoc = "";
    QString summary = "";
    QMap<QString, QStringList> flags = {};
    QMap<int, QList<int>> input_output = {{IN_DOMAIN,
                                           {OUT_SUBDOMAIN, OUT_URL}},
                                          {IN_QUERYTERM,
                                           {OUT_SUBDOMAIN, OUT_URL}}};
};
}

class BingSearch: public AbstractOsintModule{

    public:
        BingSearch(ScanArgs args);
        ~BingSearch() override;

    public slots:
        void start() override;
        void replyFinishedSubdomain(QNetworkReply *reply) override;
        void replyFinishedUrl(QNetworkReply *reply) override;
};
#endif // BING_H
