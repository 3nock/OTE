#ifndef DNSBUFFEROVERUN_H
#define DNSBUFFEROVERUN_H

#include "../AbstractOsintModule.h"

class Dnsbufferoverun: public AbstractOsintModule{

    public:
        Dnsbufferoverun(ScanArgs *args);
        ~Dnsbufferoverun() override;

    public slots:
        void start() override;
        void replyFinished(QNetworkReply *) override;
};

#endif // DNSBUFFEROVERUN_H
