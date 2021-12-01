#ifndef DOGPILE_H
#define DOGPILE_H

#include "../AbstractOsintModule.h"


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
