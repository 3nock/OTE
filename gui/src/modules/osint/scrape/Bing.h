#ifndef BING_H
#define BING_H

#include "../AbstractOsintModule.h"

namespace ModuleInfo {
    struct Bing{
        QString name = "Bing";
        QString url = "https://bing.com/";
        QString url_apiDoc = "";
        QString summary = "";
        QMap<QString, QStringList> flags = {};
        QMap<int, QList<int>> input_output = {{IN_DOMAIN,
                                               {OUT_SUBDOMAIN}}};
    };
}

class Bing: public AbstractOsintModule{

    public:
        Bing(ScanArgs *args);
        ~Bing() override;

    public slots:
        void start() override;
        void replyFinishedSubdomain(QNetworkReply *) override;

    private:
        int m_page = 0;
        void getLinks(GumboNode *node);
        GumboNode *getBody(GumboNode *node);
};
#endif // BING_H
