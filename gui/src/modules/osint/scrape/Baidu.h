#ifndef BAIDU_H
#define BAIDU_H

#include "../AbstractOsintModule.h"

namespace ModuleInfo {
    struct Baidu{
        QString name = "Baidu";
        QString url = "https://baidu.com/";
        QString url_apiDoc = "";
        QString summary = "";
        QMap<QString, QStringList> flags = {};
        QMap<int, QList<int>> input_output = {{IN_DOMAIN,
                                               {OUT_SUBDOMAIN}}};
    };
}

class Baidu: public AbstractOsintModule{

    public:
        Baidu(ScanArgs *args);
        ~Baidu() override;

    public slots:
        void start() override;
        void replyFinishedSubdomain(QNetworkReply *) override;

    private:
        int m_page = 0;
        void getLinks(GumboNode *node);
        GumboNode *getBody(GumboNode *node);
};
#endif // BAIDU_H
