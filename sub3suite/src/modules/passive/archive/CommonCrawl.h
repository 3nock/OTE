#ifndef COMMONCRAWL_H
#define COMMONCRAWL_H

#include "../AbstractOsintModule.h"


namespace ModuleInfo {
struct CommonCrawl{
    QString name = OSINT_MODULE_COMMONCRAWL;
    QString url = "http://commoncrawl.org/";
    QString url_apiDoc = "https://commoncrawl.org/the-data/get-started/";
    QString summary = "We build and maintain an open repository of web crawl data that can be accessed and \n"
                      "analyzed by anyone.";

    QMap<QString, QStringList> flags = {{"index",
                                         {PLACEHOLDERTEXT_NONE, "get the index urls..."}},
                                        {"url",
                                         {PLACEHOLDERTEXT_DOMAIN, "/* not yet implemented */"}}};

    QMap<int, QList<int>> input_output = {{IN_DOMAIN,
                                           {OUT_SUBDOMAIN, OUT_URL}},
                                          {IN_URL,
                                           {OUT_SUBDOMAIN, OUT_URL}}};
};
}

class CommonCrawl: public AbstractOsintModule{

    public:
        explicit CommonCrawl(ScanArgs args);
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
