#ifndef TRELLO_H
#define TRELLO_H

#include "../AbstractOsintModule.h"

namespace ModuleInfo {
    struct Trello{
        QString name = "Trello";
        QString url = "https://trello.com/";
        QString url_apiDoc = "";
        QString summary = "";
        QMap<QString, QStringList> flags = {};
        QMap<int, QList<int>> input_output = {{IN_DOMAIN,
                                               {OUT_SUBDOMAIN}}};
    };
}


class Trello: public AbstractOsintModule{

    public:
        Trello(ScanArgs *args);
        ~Trello() override;

    public slots:
        void start() override;
        void replyFinishedSubdomain(QNetworkReply *) override;

    private:
        int m_page = 0;
        void getLinks(GumboNode *node);
        GumboNode *getBody(GumboNode *node);
};
#endif // TRELLO_H
