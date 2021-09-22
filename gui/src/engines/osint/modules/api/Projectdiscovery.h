#ifndef PROJECTDISCOVERY_H
#define PROJECTDISCOVERY_H

#include "src/engines/osint/modules/AbstractOsintModule.h"

class Projectdiscovery: public AbstractOsintModule{

    public:
        Projectdiscovery(QString target = nullptr, QObject *parent = nullptr);
        ~Projectdiscovery() override;

    public slots:
        void start() override;
        void replyFinished(QNetworkReply *) override;
};

#endif // PROJECTDISCOVERY_H
