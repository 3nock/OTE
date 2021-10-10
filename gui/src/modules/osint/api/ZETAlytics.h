#ifndef ZETALYTICS_H
#define ZETALYTICS_H

#include "../AbstractOsintModule.h"

class ZETAlytics: public AbstractOsintModule{

    public:
        ZETAlytics(ScanArgs *args);
        ~ZETAlytics() override;

    public slots:
        void start() override;
        void replyFinished(QNetworkReply *) override;

    private:
        QString m_key = nullptr;
};

#endif // ZETALYTICS_H
