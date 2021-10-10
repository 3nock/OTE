#ifndef IPAPI_H
#define IPAPI_H

#include "../AbstractOsintModule.h"

class IpApi: public AbstractOsintModule{

    public:
        IpApi(ScanArgs *args);
        ~IpApi() override;

    public slots:
        void start() override;
        void replyFinished(QNetworkReply *) override;

    private:
        QString m_key = nullptr;
};

#endif // IPAPI_H
