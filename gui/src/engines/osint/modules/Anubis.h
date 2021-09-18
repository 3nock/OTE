#ifndef ANUBIS_H
#define ANUBIS_H

#include "AbstractModule.h"

class Anubis: public AbstractModule{

    public:
        Anubis(QString target = nullptr, QObject *parent = nullptr);
        ~Anubis() override;

    public slots:
        void start() override;
        void replyFinished(QNetworkReply *) override;
};


#endif // ANUBIS_H
