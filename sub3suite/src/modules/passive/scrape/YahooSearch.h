#ifndef YAHOO_H
#define YAHOO_H

#include "../AbstractOsintModule.h"

namespace ModuleInfo {
struct YahooSearch{
    QString name = OSINT_MODULE_YAHOOSEARCH;
    QString url = "https://yahoo.com/";
    QString url_apiDoc = "";
    QString summary = "";
    QMap<QString, QStringList> flags = {};
    QMap<int, QList<int>> input_output = {{IN_DOMAIN,
                                           {OUT_SUBDOMAIN, OUT_URL}},
                                          {IN_QUERYTERM,
                                           {OUT_SUBDOMAIN, OUT_URL}}};
};
}


class YahooSearch: public AbstractOsintModule{

    public:
        explicit YahooSearch(ScanArgs args);
        ~YahooSearch() override;

    public slots:
        void start() override;
        void replyFinishedSubdomain(QNetworkReply *reply) override;
        void replyFinishedUrl(QNetworkReply *reply) override;
};
#endif // YAHOO_H
