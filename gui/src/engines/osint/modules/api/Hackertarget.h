#ifndef HACKERTARGET_H
#define HACKERTARGET_H


#include "src/engines/osint/modules/AbstractOsintModule.h"

class Hackertarget: public AbstractOsintModule{

    public:
        Hackertarget(QString target = nullptr, QObject *parent = nullptr);
        ~Hackertarget() override;

    public slots:
        void start() override;
        void replyFinished(QNetworkReply *) override;
};


#endif // HACKERTARGET_H
