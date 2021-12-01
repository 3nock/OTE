#ifndef ASK_H
#define ASK_H

#include "../AbstractOsintModule.h"


class Ask: public AbstractOsintModule{

    public:
        Ask(ScanArgs *args);
        ~Ask() override;

    public slots:
        void start() override;
        void replyFinishedSubdomain(QNetworkReply *) override;

    private:
        int m_page = 0;
        void getLinks(GumboNode *node);
        GumboNode *getBody(GumboNode *node);
};

#endif // ASK_H
