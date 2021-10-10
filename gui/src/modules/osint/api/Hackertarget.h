#ifndef HACKERTARGET_H
#define HACKERTARGET_H


#include "../AbstractOsintModule.h"

class Hackertarget: public AbstractOsintModule{

    public:
        Hackertarget(ScanArgs *args);
        ~Hackertarget() override;

    public slots:
        void start() override;
        void replyFinished(QNetworkReply *) override;
};


#endif // HACKERTARGET_H
