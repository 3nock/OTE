#ifndef TRELLO_H
#define TRELLO_H

#include "src/engines/osint/modules/AbstractOsintModule.h"
#include "gumbo-parser/src/gumbo.h"

class Trello: public AbstractOsintModule{

    public:
        Trello(QString target = nullptr, QObject *parent = nullptr);
        ~Trello() override;

    public slots:
        void start() override;
        void replyFinished(QNetworkReply *) override;

    private:
        int m_page = 0;
        int activeRequests = 0;
        void getLinks(GumboNode *node);
        GumboNode *getBody(GumboNode *node);
};
#endif // TRELLO_H
