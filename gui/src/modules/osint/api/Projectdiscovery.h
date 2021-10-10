#ifndef PROJECTDISCOVERY_H
#define PROJECTDISCOVERY_H

#include "../AbstractOsintModule.h"

class Projectdiscovery: public AbstractOsintModule{

    public:
        Projectdiscovery(ScanArgs *args);
        ~Projectdiscovery() override;

    public slots:
        void start() override;
        void replyFinished(QNetworkReply *) override;
};

#endif // PROJECTDISCOVERY_H
