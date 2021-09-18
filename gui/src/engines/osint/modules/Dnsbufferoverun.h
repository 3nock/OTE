#ifndef DNSBUFFEROVERUN_H
#define DNSBUFFEROVERUN_H

#include "AbstractModule.h"

class Dnsbufferoverun: public AbstractModule{

    public:
        Dnsbufferoverun(QString target = nullptr, QObject *parent = nullptr);
        ~Dnsbufferoverun() override;

    public slots:
        void start() override;
        void replyFinished(QNetworkReply *) override;
};

#endif // DNSBUFFEROVERUN_H
