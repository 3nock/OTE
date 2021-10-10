#ifndef GOOGLECERT_H
#define GOOGLECERT_H

#include "../AbstractOsintModule.h"

class GoogleCert: public AbstractOsintModule{

    public:
        GoogleCert(ScanArgs *args);
        ~GoogleCert() override;

    public slots:
        void start() override;
        void replyFinished(QNetworkReply *) override;
};

#endif // GOOGLECERT_H
