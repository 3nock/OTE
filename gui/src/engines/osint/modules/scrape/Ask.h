#ifndef ASK_H
#define ASK_H

#include "src/engines/osint/modules/AbstractOsintModule.h"
#include "gumbo-parser/src/gumbo.h"

class Ask: public AbstractOsintModule{

    public:
        Ask(QString target = nullptr, QObject *parent = nullptr);
        ~Ask() override;

    public slots:
        void start() override;
        void replyFinished(QNetworkReply *) override;

    private:
        int m_page = 0;
        int activeRequests = 0;
        void getLinks(GumboNode *node);
        GumboNode *getBody(GumboNode *node);
};

#endif // ASK_H
