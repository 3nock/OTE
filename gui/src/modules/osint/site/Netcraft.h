#ifndef NETCRAFT_H
#define NETCRAFT_H

#include "../AbstractOsintModule.h"
#include "gumbo-parser/src/gumbo.h"

class Netcraft: public AbstractOsintModule{

    public:
        Netcraft(ScanArgs *args);
        ~Netcraft() override;

    public slots:
        void start() override;
        void replyFinished(QNetworkReply *) override;

    private:
        void getSubdomains(GumboNode *node);
};

#endif // NETCRAFT_H
