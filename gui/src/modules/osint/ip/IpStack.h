#ifndef IPSTACK_H
#define IPSTACK_H

#include "../AbstractOsintModule.h"


namespace ModuleInfo {
    struct IpStack{
        QString name = "IpStack";
        QString url = "https://ipstack.com/";
        QString url_apiDoc = "https://ipstack.com/documentation";
        QString summary = "ipstack offers a powerful, real-time IP to geolocation API capable of looking up accurate location data and assessing security threats originating from risky IP addresses.";

        QMap<QString, QStringList> flags = {{"Standard Ip Lookup",
                                             {PLACEHOLDERTEXT_IP, "The ipstack's primary endpoint is called Standard Lookup and is used to look up single IPv4 or IPv6 addresses."}},
                                            {"Bulk Ip Lookup",
                                             {PLACEHOLDERTEXT_BULK_IP, "The ipstack API also offers the ability to request data for multiple IPv4 or IPv6 addresses at the same time."}},
                                            {"Requester IP Lookup",
                                             {PLACEHOLDERTEXT_NONE, "The ipstack API also offers a separate API endpoint capable of detecting the IP address which the current API request is coming from."}}};

        QMap<int, QList<int>> input_output = {};
    };
}

class IpStack: public AbstractOsintModule{

    public:
        IpStack(ScanArgs *args);
        ~IpStack() override;

    public slots:
        void start() override;

    private:
        QString m_key = nullptr;
};

#endif // IPSTACK_H
