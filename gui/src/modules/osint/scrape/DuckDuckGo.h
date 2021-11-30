#ifndef DUCKDUCKGO_H
#define DUCKDUCKGO_H

#include "../AbstractOsintModule.h"
#include "gumbo-parser/src/gumbo.h"

class DuckDuckGo: public AbstractOsintModule{

    public:
        DuckDuckGo(ScanArgs *args);
        ~DuckDuckGo() override;

    public slots:
        void start() override;
        void replyFinishedSubdomain(QNetworkReply *) override;

    private:
        int m_page = 0;
        int activeRequests = 0;
        void getLinks(GumboNode *node);
        GumboNode *getBody(GumboNode *node);
};
#endif // DUCKDUCKGO_H
