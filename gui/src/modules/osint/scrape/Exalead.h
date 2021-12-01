#ifndef EXALEAD_H
#define EXALEAD_H

#include "../AbstractOsintModule.h"


class Exalead: public AbstractOsintModule{

    public:
        Exalead(ScanArgs *args);
        ~Exalead() override;

    public slots:
        void start() override;
        void replyFinishedSubdomain(QNetworkReply *) override;

    private:
        int m_page = 0;
        void getLinks(GumboNode *node);
        GumboNode *getBody(GumboNode *node);
};
#endif // EXALEAD_H
