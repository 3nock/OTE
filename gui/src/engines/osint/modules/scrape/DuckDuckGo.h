#ifndef DUCKDUCKGO_H
#define DUCKDUCKGO_H

#include "src/engines/osint/modules/AbstractOsintModule.h"
#include "gumbo-parser/src/gumbo.h"

class DuckDuckGo: public AbstractOsintModule{

    public:
        DuckDuckGo(QString target = nullptr, QObject *parent = nullptr);
        ~DuckDuckGo() override;

    public slots:
        void start() override;
        void replyFinished(QNetworkReply *) override;

    private:
        int m_page = 0;
        int activeRequests = 0;
        void getLinks(GumboNode *node);
        GumboNode *getBody(GumboNode *node);
};
#endif // DUCKDUCKGO_H
