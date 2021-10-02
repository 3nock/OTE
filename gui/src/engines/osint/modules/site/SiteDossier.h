#ifndef SITEDOSSIER_H
#define SITEDOSSIER_H

#include "src/engines/osint/modules/AbstractOsintModule.h"
#include "gumbo-parser/src/gumbo.h"

class SiteDossier: public AbstractOsintModule{

    public:
        SiteDossier(QString target = nullptr, QObject *parent = nullptr);
        ~SiteDossier() override;

    public slots:
        void start() override;
        void replyFinished(QNetworkReply *) override;

    private:
        int m_page = 1;
        void getSubdomains(GumboNode *node);
        GumboNode *getBody(GumboNode *node);
};

#endif // SITEDOSSIER_H
