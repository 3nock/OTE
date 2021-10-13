#ifndef IPAPI_H
#define IPAPI_H

#include "../AbstractOsintModule.h"

namespace ModuleInfo {
    struct IpApi{
        QString name = "IpApi";
        QString url = "https://ipapi.com/";
        QString summary = "Real-time Geolocation & Reverse IP Lookup REST API";

        QMap<QString, QString> flags = {{"Standard Lookup", "ip-address"},
                                        {"Bulk Lookup", "ip-address,ip-address,ip-address"},
                                        {"Origin Lookup", ""}};
    };
}

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
