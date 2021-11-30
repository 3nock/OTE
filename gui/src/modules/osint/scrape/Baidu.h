#ifndef BAIDU_H
#define BAIDU_H

#include "../AbstractOsintModule.h"
#include "gumbo-parser/src/gumbo.h"

class Baidu: public AbstractOsintModule{

    public:
        Baidu(ScanArgs *args);
        ~Baidu() override;

    public slots:
        void start() override;
        void replyFinishedSubdomain(QNetworkReply *) override;

    private:
        int m_page = 0;
        int activeRequests = 0;
        void getLinks(GumboNode *node);
        GumboNode *getBody(GumboNode *node);
};
#endif // BAIDU_H
