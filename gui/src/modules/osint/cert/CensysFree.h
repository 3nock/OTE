#ifndef CENSYSFREE_H
#define CENSYSFREE_H

#include "../AbstractOsintModule.h"
#include "gumbo-parser/src/gumbo.h"

class CensysFree: public AbstractOsintModule{

    public:
        CensysFree(ScanArgs *args);
        ~CensysFree() override;

    public slots:
        void start() override;
        void replyFinished(QNetworkReply *) override;

    private:
        void getSubdomains(GumboNode *node);
};

#endif // CENSYSFREE_H
