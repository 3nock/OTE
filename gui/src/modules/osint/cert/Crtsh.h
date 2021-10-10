#ifndef CRTSH_H
#define CRTSH_H

#include "../AbstractOsintModule.h"
#include "gumbo-parser/src/gumbo.h"

class Crtsh: public AbstractOsintModule{

    public:
        Crtsh(ScanArgs *args);
        ~Crtsh() override;

    public slots:
        void start() override;
        void replyFinished(QNetworkReply *) override;

    private:
        void getSubdomains(GumboNode* node);
};


#endif // CRTSH_H
