#ifndef GOOGLECERT_H
#define GOOGLECERT_H

#include "AbstractModule.h"

class GoogleCert: public AbstractModule{

    public:
        GoogleCert(QString target = nullptr, QObject *parent = nullptr);
        ~GoogleCert() override;

    public slots:
        void start() override;
        void replyFinished(QNetworkReply *) override;
};

#endif // GOOGLECERT_H
