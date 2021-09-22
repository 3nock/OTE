#ifndef ANUBIS_H
#define ANUBIS_H

#include "src/engines/osint/modules/AbstractOsintModule.h"

class Anubis: public AbstractOsintModule{

    public:
        Anubis(QString target = nullptr, QObject *parent = nullptr);
        ~Anubis() override;

    public slots:
        void start() override;
        void replyFinished(QNetworkReply *) override;
};


#endif // ANUBIS_H
