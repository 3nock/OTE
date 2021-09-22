#ifndef CRTSH_H
#define CRTSH_H

#include "src/engines/osint/modules/AbstractOsintModule.h"
#include "gumbo-parser/src/gumbo.h"

class Crtsh: public AbstractOsintModule{

    public:
        Crtsh(QString target = nullptr, QObject *parent = nullptr);
        ~Crtsh() override;

    public slots:
        void start() override;
        void replyFinished(QNetworkReply *) override;

    private:
        void getSubdomains(GumboNode* node);
};


#endif // CRTSH_H
