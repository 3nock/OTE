#ifndef PAGESINVENTORY_H
#define PAGESINVENTORY_H

#include "src/engines/osint/modules/AbstractOsintModule.h"
#include "gumbo-parser/src/gumbo.h"

class PagesInventory: public AbstractOsintModule{

    public:
        PagesInventory(QString target = nullptr, QObject *parent = nullptr);
        ~PagesInventory() override;

    public slots:
        void start() override;
        void replyFinished(QNetworkReply *) override;

    private:
        void getSubdomains(GumboNode *node);
        GumboNode* getBody(GumboNode *node);
};

#endif // PAGESINVENTORY_H
