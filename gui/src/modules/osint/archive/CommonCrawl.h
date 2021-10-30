#ifndef COMMONCRAWL_H
#define COMMONCRAWL_H

#include "../AbstractOsintModule.h"

namespace ModuleInfo {
    struct CommonCrawl{
        QString name = "CommonCrawl";
        QString url = "http://commoncrawl.org/";
        QString url_apiDoc = "";
        QString summary = "We build and maintain an open repository of web crawl data that can be accessed and \n"
                          "analyzed by anyone.";
        QMap<QString, QStringList> flags = {{"index", {PLACEHOLDERTEXT_NONE, ""}},
                                        {"url", {PLACEHOLDERTEXT_DOMAIN, ""}}};
    };
}

class CommonCrawl: public AbstractOsintModule{

    public:
        CommonCrawl(ScanArgs *args);
        ~CommonCrawl() override;

    public slots:
        void start() override;
        void replyFinished(QNetworkReply *) override;

    private:
        bool getArchive = true;
        QStringList urlList;
};
#endif // COMMONCRAWL_H
