#ifndef COMMONCRAWL_H
#define COMMONCRAWL_H

/*
 * INPUT hostname:          OUTPUT: subdomain, url
 */

#include "../AbstractOsintModule.h"

namespace ModuleInfo {
    struct CommonCrawl{
        QString name = "CommonCrawl";
        QString url = "http://commoncrawl.org/";
        QString url_apiDoc = "https://commoncrawl.org/the-data/get-started/";
        QString summary = "We build and maintain an open repository of web crawl data that can be accessed and \n"
                          "analyzed by anyone.";

        QMap<QString, QStringList> flags = {{"index",
                                             {PLACEHOLDERTEXT_NONE, "get the index urls..."}},
                                            {"url",
                                             {PLACEHOLDERTEXT_DOMAIN, "/* not yet implemented */"}}};
    };
}

class CommonCrawl: public AbstractOsintModule{

    public:
        CommonCrawl(ScanArgs *args);
        ~CommonCrawl() override;

    public slots:
        void start() override;
        void replyFinishedIndex(QNetworkReply *reply); // to obtain index urls
        void replyFinishedUrl(QNetworkReply *reply) override;
        void replyFinishedSubdomain(QNetworkReply *reply) override;

    private:
        QStringList urlList;
};
#endif // COMMONCRAWL_H
