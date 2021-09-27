#ifndef IPINFO_H
#define IPINFO_H

#include "src/engines/osint/modules/AbstractOsintModule.h"

class IpInfo: public AbstractOsintModule{

    public:
        IpInfo(QString target = nullptr, QObject *parent = nullptr);
        ~IpInfo() override;

    public slots:
        void start() override;
        void replyFinished(QNetworkReply *) override;

    private:
        QString m_key = nullptr;
};

#endif // IPINFO_H
