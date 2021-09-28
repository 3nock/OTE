#ifndef IPAPI_H
#define IPAPI_H

#include "src/engines/osint/modules/AbstractOsintModule.h"

class IpApi: public AbstractOsintModule{

    public:
        IpApi(QString target = nullptr, QObject *parent = nullptr);
        ~IpApi() override;

    public slots:
        void start() override;
        void replyFinished(QNetworkReply *) override;

    private:
        QString m_key = nullptr;
};

#endif // IPAPI_H
