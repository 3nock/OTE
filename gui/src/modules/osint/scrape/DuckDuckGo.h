#ifndef DUCKDUCKGO_H
#define DUCKDUCKGO_H

#include "../AbstractOsintModule.h"

namespace ModuleInfo {
    struct DuckDuckGo{
        QString name = "DuckDuckGo";
        QString url = "https://duckduckgo.com/";
        QString url_apiDoc = "";
        QString summary = "";
        QMap<QString, QStringList> flags = {};
        QMap<int, QList<int>> input_output = {{IN_DOMAIN,
                                               {OUT_SUBDOMAIN}}};
    };
}

class DuckDuckGo: public AbstractOsintModule{

    public:
        DuckDuckGo(ScanArgs *args);
        ~DuckDuckGo() override;

    public slots:
        void start() override;
        void replyFinishedSubdomain(QNetworkReply *) override;

    private:
        int m_page = 0;
        void getLinks(GumboNode *node);
        GumboNode *getBody(GumboNode *node);
};
#endif // DUCKDUCKGO_H
