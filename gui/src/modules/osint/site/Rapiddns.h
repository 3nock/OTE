#ifndef RAPIDDNS_H
#define RAPIDDNS_H

#include "../AbstractOsintModule.h"
#include "gumbo-parser/src/gumbo.h"

class Rapiddns: public AbstractOsintModule{

    public:
        Rapiddns(ScanArgs *args);
        ~Rapiddns() override;

    public slots:
        void start() override;
        void replyFinished(QNetworkReply *) override;

    private:
        void getSubdomains(GumboNode *node);
};

#endif // RAPIDDNS_H
