#ifndef SPYSE_H
#define SPYSE_H

#include "AbstractModule.h"

class Spyse: public AbstractModule{

    public:
        Spyse(QString target = nullptr, QObject *parent = nullptr);
        ~Spyse() override;

    public slots:
        void start() override;
        void replyFinished(QNetworkReply *) override;
};

#endif // SPYSE_H
