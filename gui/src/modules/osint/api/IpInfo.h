#ifndef IPINFO_H
#define IPINFO_H

#include "../AbstractOsintModule.h"

class IpInfo: public AbstractOsintModule{

    public:
        IpInfo(ScanArgs *args);
        ~IpInfo() override;

    public slots:
        void start() override;
        void replyFinished(QNetworkReply *) override;

    private:
        QString m_key = nullptr;
};

#endif // IPINFO_H
