#ifndef DOGPILE_H
#define DOGPILE_H

#include "../AbstractOsintModule.h"

namespace ModuleInfo {
    struct DogPile{
        QString name = "DogPile";
        QString url = "https://dogpile.com/";
        QString url_apiDoc = "";
        QString summary = "";
        QMap<QString, QStringList> flags = {};
        QMap<int, QList<int>> input_output = {{IN_DOMAIN,
                                               {OUT_SUBDOMAIN}}};
    };
}

class DogPile: public AbstractOsintModule{

    public:
        DogPile(ScanArgs *args);
        ~DogPile() override;

    public slots:
        void start() override;
        void replyFinishedSubdomain(QNetworkReply *) override;

    private:
        int m_page = 0;
        void getLinks(GumboNode *node);
        GumboNode *getBody(GumboNode *node);
};
#endif // DOGPILE_H
