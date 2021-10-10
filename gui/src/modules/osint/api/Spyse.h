#ifndef SPYSE_H
#define SPYSE_H

#include "../AbstractOsintModule.h"

class Spyse: public AbstractOsintModule{

    public:
        Spyse(ScanArgs *args);
        ~Spyse() override;

    public slots:
        void start() override;
        void replyFinished(QNetworkReply *) override;
};

#endif // SPYSE_H
