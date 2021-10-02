#ifndef BAIDU_H
#define BAIDU_H

#include "src/engines/osint/modules/AbstractOsintModule.h"
#include "gumbo-parser/src/gumbo.h"

class Baidu: public AbstractOsintModule{

    public:
        Baidu(QString target = nullptr, QObject *parent = nullptr);
        ~Baidu() override;

    public slots:
        void start() override;
        void replyFinished(QNetworkReply *) override;

    private:
        int m_page = 0;
        int activeRequests = 0;
        void getLinks(GumboNode *node);
        GumboNode *getBody(GumboNode *node);
};
#endif // BAIDU_H
