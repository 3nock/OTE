#ifndef HACKERTARGET_H
#define HACKERTARGET_H


#include "AbstractModule.h"

class Hackertarget: public AbstractModule{

    public:
        Hackertarget(QString target = nullptr, QObject *parent = nullptr);
        ~Hackertarget() override;

    public slots:
        void start() override;
        void replyFinished(QNetworkReply *) override;
};


#endif // HACKERTARGET_H
