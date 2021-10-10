#ifndef URLSCAN_H
#define URLSCAN_H

#include "../AbstractOsintModule.h"

class Urlscan: public AbstractOsintModule{

    public:
        Urlscan(ScanArgs *args);
        ~Urlscan() override;

    public slots:
        void start() override;
        void replyFinished(QNetworkReply *) override;
};

#endif // URLSCAN_H
