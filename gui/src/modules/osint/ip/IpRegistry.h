#ifndef IPREGISTRY_H
#define IPREGISTRY_H

#include "../AbstractOsintModule.h"


namespace ModuleInfo {
    struct IpRegistry{
        QString name = "IpRegistry";
        QString url = "https://ipregistry.co/";
        QString url_apiDoc = "https://ipregistry.co/docs/";
        QString summary = "Ipregistry is a trusted and in-depth IP Geolocation and Threat detections source of information that can benefit publishers, ad networks, retailers, financial services, e-commerce stores and more.";

        QMap<QString, QStringList> flags = {{"Single IP Lookup",
                                             {PLACEHOLDERTEXT_IP, "This is the primary Ipregistry endpoint. It is used to look up any given IPv4 or IPv6 addresses."}},
                                            {"Batch IP Lookup",
                                             {PLACEHOLDERTEXT_BULK_IP, "The batch IP lookup endpoint allows you to group up to 1024 IP search requests into a single request (you can mix up IPv4 and IPv6 addresses)."}},
                                            {"Origin IP Lookup",
                                             {PLACEHOLDERTEXT_NONE, "Look up the IP address the current API request is coming from."}}/*,
                                            {"User-Agent Parsing",
                                             {PLACEHOLDERTEXT_UA, "User-Agent parsing allows detecting any browser, device & OS in real-time to bring user experience to perfection"}}*/};

        QMap<int, QList<int>> input_output = {};
    };
}

class IpRegistry: public AbstractOsintModule{

    public:
        IpRegistry(ScanArgs *args);
        ~IpRegistry() override;

    public slots:
        void start() override;

    private:
        QString m_key = nullptr;
};

#endif // IPREGISTRY_H
