#ifndef BING_H
#define BING_H

#include "src/engines/osint/modules/AbstractOsintModule.h"
#include "gumbo-parser/src/gumbo.h"

class Bing: public AbstractOsintModule{

    public:
        Bing(QString target = nullptr, QObject *parent = nullptr);
        ~Bing() override;

    public slots:
        void start() override;
        void replyFinished(QNetworkReply *) override;

    private:
        int m_page = 0;
        int activeRequests = 0;
        void getLinks(GumboNode *node);
        GumboNode *getBody(GumboNode *node);
};
#endif // BING_H
