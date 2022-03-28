#ifndef IPGEOLOCATION_H
#define IPGEOLOCATION_H

#include "../AbstractOsintModule.h"


namespace ModuleInfo {
struct IpGeoLocation{
    QString name = OSINT_MODULE_IPGEOLOCATION;
    QString url = "https://ipgeolocation.io/";
    QString url_apiDoc = "https://ipgeolocation.io/documentation/";
    QString summary = "The Trusted Source of IP Address Data";
    QMap<QString, QStringList> flags = {{"IP Geolocation",
                                         {PLACEHOLDERTEXT_IP_OR_DOMAIN, "IP Geolocation API provides real-time and accurate geolocation, and security information for any IPv4 or IPv6 address and domain name along with the user-agent detail for the provided user-agent string."}},
                                        {"IP To Time zone",
                                         {PLACEHOLDERTEXT_IP, "Time zone API provides current time, date, and time zone related information."}}};

    QMap<int, QList<int>> input_output = {};
};
}

class IpGeoLocation: public AbstractOsintModule{

    public:
        IpGeoLocation(ScanArgs args);
        ~IpGeoLocation() override;

    public slots:
        void start() override;

    private:
        QString m_key;
};

#endif // IPGEOLOCATION_H
