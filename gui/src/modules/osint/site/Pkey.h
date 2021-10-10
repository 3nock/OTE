#ifndef PKEY_H
#define PKEY_H

#include "../AbstractOsintModule.h"
#include "gumbo-parser/src/gumbo.h"

class Pkey: public AbstractOsintModule{

    public:
        Pkey(ScanArgs *args);
        ~Pkey() override;

    public slots:
        void start() override;
        void replyFinished(QNetworkReply *) override;

    private:
        void getSubdomains(GumboNode *node);
};

#endif // PKEY_H
