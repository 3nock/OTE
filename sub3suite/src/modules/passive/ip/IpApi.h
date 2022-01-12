#ifndef IPAPI_H
#define IPAPI_H

#include "../AbstractOsintModule.h"


namespace ModuleInfo {
struct IpApi{
    QString name = "IpApi";
    QString url = "https://ipapi.com/";
    QString url_apiDoc = "https://ipapi.com/documentation";
    QString summary = "ipapi provides an easy-to-use API interface allowing customers to look various pieces "
                      "of information IPv4 and IPv6 addresses are associated with";

    QMap<QString, QStringList> flags = {{"standard lookup",
                                         {PLACEHOLDERTEXT_IP, "Look up any given IP address."}},
                                        {"bulk lookup",
                                         {PLACEHOLDERTEXT_BULK_IP, "Look up multiple IP addresses in bulk."}},
                                        {"origin lookup",
                                         {PLACEHOLDERTEXT_NONE, "Look up the IP address the current API request is coming from."}}};

    QMap<int, QList<int>> input_output = {};
};
}

class IpApi: public AbstractOsintModule{

    public:
        IpApi(ScanArgs args);
        ~IpApi() override;

    public slots:
        void start() override;
        void replyFinishedInfo(QNetworkReply *) override;

    private:
        QString m_key = nullptr;
};

#endif // IPAPI_H
