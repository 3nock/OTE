#ifndef URLSCAN_H
#define URLSCAN_H

#include "AbstractModule.h"

class Urlscan: public AbstractModule{

    public:
        Urlscan(QString target = nullptr, QObject *parent = nullptr);
        ~Urlscan() override;

    public slots:
        void start() override;
        void replyFinished(QNetworkReply *) override;
};

#endif // URLSCAN_H
