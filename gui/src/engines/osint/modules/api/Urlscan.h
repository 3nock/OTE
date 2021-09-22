#ifndef URLSCAN_H
#define URLSCAN_H

#include "src/engines/osint/modules/AbstractOsintModule.h"

class Urlscan: public AbstractOsintModule{

    public:
        Urlscan(QString target = nullptr, QObject *parent = nullptr);
        ~Urlscan() override;

    public slots:
        void start() override;
        void replyFinished(QNetworkReply *) override;
};

#endif // URLSCAN_H
