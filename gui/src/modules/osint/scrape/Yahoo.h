#ifndef YAHOO_H
#define YAHOO_H

#include "../AbstractOsintModule.h"

namespace ModuleInfo {
    struct Yahoo{
        QString name = "Yahoo";
        QString url = "https://yahoo.com/";
        QString url_apiDoc = "";
        QString summary = "";
        QMap<QString, QStringList> flags = {};
        QMap<int, QList<int>> input_output = {{IN_DOMAIN,
                                               {OUT_SUBDOMAIN}}};
    };
}


class Yahoo: public AbstractOsintModule{

    public:
        Yahoo(ScanArgs *args);
        ~Yahoo() override;

    public slots:
        void start() override;
        void replyFinishedSubdomain(QNetworkReply *) override;

    private:
        int m_page = 0;
        void getLinks(GumboNode *node);
        GumboNode *getBody(GumboNode *node);
};
#endif // YAHOO_H
