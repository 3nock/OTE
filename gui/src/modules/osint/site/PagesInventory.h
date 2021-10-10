#ifndef PAGESINVENTORY_H
#define PAGESINVENTORY_H

#include "../AbstractOsintModule.h"
#include "gumbo-parser/src/gumbo.h"

class PagesInventory: public AbstractOsintModule{

    public:
        PagesInventory(ScanArgs *args);
        ~PagesInventory() override;

    public slots:
        void start() override;
        void replyFinished(QNetworkReply *) override;

    private:
        void getSubdomains(GumboNode *node);
        GumboNode* getBody(GumboNode *node);
};

#endif // PAGESINVENTORY_H
