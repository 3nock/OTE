#ifndef DNSBUFFEROVERUN_H
#define DNSBUFFEROVERUN_H

#include "src/engines/osint/modules/AbstractOsintModule.h"

class Dnsbufferoverun: public AbstractOsintModule{

    public:
        Dnsbufferoverun(QString target = nullptr, QObject *parent = nullptr);
        ~Dnsbufferoverun() override;

    public slots:
        void start() override;
        void replyFinished(QNetworkReply *) override;
};

#endif // DNSBUFFEROVERUN_H
