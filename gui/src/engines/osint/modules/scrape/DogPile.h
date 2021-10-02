#ifndef DOGPILE_H
#define DOGPILE_H

#include "src/engines/osint/modules/AbstractOsintModule.h"
#include "gumbo-parser/src/gumbo.h"

class DogPile: public AbstractOsintModule{

    public:
        DogPile(QString target = nullptr, QObject *parent = nullptr);
        ~DogPile() override;

    public slots:
        void start() override;
        void replyFinished(QNetworkReply *) override;

    private:
        int m_page = 0;
        int activeRequests = 0;
        void getLinks(GumboNode *node);
        GumboNode *getBody(GumboNode *node);
};
#endif // DOGPILE_H
