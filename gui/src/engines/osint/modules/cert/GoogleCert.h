#ifndef GOOGLECERT_H
#define GOOGLECERT_H

#include "src/engines/osint/modules/AbstractOsintModule.h"

class GoogleCert: public AbstractOsintModule{

    public:
        GoogleCert(QString target = nullptr, QObject *parent = nullptr);
        ~GoogleCert() override;

    public slots:
        void start() override;
        void replyFinished(QNetworkReply *) override;
};

#endif // GOOGLECERT_H
