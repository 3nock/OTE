#ifndef TRELLO_H
#define TRELLO_H

#include "../AbstractOsintModule.h"


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
