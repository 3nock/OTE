#ifndef ANUBIS_H
#define ANUBIS_H

#include "../AbstractOsintModule.h"

class Anubis: public AbstractOsintModule{

    public:
        Anubis(ScanArgs *args);
        ~Anubis() override;

    public slots:
        void start() override;
        void replyFinished(QNetworkReply *) override;
};


#endif // ANUBIS_H
