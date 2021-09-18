#ifndef THREATMINER_H
#define THREATMINER_H


#include "AbstractModule.h"

class Threatminer: public AbstractModule{

    public:
        Threatminer(QString target = nullptr, QObject *parent = nullptr);
        ~Threatminer() override;

    public slots:
        void start() override;
        void replyFinished(QNetworkReply *) override;
};

#endif // THREATMINER_H
