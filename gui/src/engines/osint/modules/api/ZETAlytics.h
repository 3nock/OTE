#ifndef ZETALYTICS_H
#define ZETALYTICS_H

#include "src/engines/osint/modules/AbstractOsintModule.h"

class ZETAlytics: public AbstractOsintModule{

    public:
        ZETAlytics(QString target = nullptr, QObject *parent = nullptr);
        ~ZETAlytics() override;

    public slots:
        void start() override;
        void replyFinished(QNetworkReply *) override;

    private:
        QString m_key = nullptr;
};

#endif // ZETALYTICS_H
