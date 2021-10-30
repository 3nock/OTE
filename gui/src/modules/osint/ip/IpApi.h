#ifndef IPAPI_H
#define IPAPI_H

#include "../AbstractOsintModule.h"

namespace ModuleInfo {
    struct IpApi{
        QString name = "IpApi";
        QString url = "https://ipapi.com/";
        QString url_apiDoc = "";
        QString summary = "Real-time Geolocation & Reverse IP Lookup REST API";

        QMap<QString, QStringList> flags = {{"Standard Lookup", {PLACEHOLDERTEXT_IP, ""}},
                                        {"Bulk Lookup", {"ip-address,ip-address", ""}},
                                        {"Origin Lookup", {PLACEHOLDERTEXT_NONE, ""}}};
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
