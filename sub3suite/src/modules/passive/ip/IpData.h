#ifndef IPDATA_H
#define IPDATA_H

#include "../AbstractOsintModule.h"


namespace ModuleInfo {
struct IpData{
    QString name = OSINT_MODULE_IPDATA;
    QString url = "https://ipdata.co/";
    QString url_apiDoc = "https://docs.ipdata.co/";
    QString summary = "ipdata provides an IP Address Intelligence API that allows you to lookup the approximate location of any IP Address. "
                      "We also provide, aggregated Open Source threat intelligence data, IP to Company lookups, as well as internationalization data like currencies, timezones and languages.";

    QMap<QString, QStringList> flags = {{"Mobile Carrier Detection",
                                         {PLACEHOLDERTEXT_IP, "The ipdata API supports looking up the Mobile Carrier of an IP Address. Our database currently consists of over 2500 carriers in 234 countries and territories."}},
                                        {"Timezone Detection",
                                         {PLACEHOLDERTEXT_IP, "We provide detailed and accurate Timezone data, adjusted for DST where applicable."}},
                                        {"Currency Detection",
                                         {PLACEHOLDERTEXT_IP, "ipdata helps you personalize your ecommerce pages with the user's home currency."}},
                                        {"Proxy, Tor and Threat Detection",
                                         {PLACEHOLDERTEXT_IP, "We have over 600M malicious IP addresses in our IP Threat Intelligence database. We also track Tor nodes and open proxies. This data is updated every 15mins and is aggregated and published hourly."}},
                                        {"ASN API",
                                         {PLACEHOLDERTEXT_ASN, "We now offer an endpoint to query ASN information. The ASN number may be used at the end of the base URL to return information pertinent to the ASN."}},
                                        {"ASN Data",
                                         {PLACEHOLDERTEXT_IP, "We provide detailed ASN data for all IP Addresses with the following fields: AS Name, AS Number, AS Route, the AS Organization's domain and the usage type."}}};

    QMap<int, QList<int>> input_output = {};
};
}

class IpData: public AbstractOsintModule{

    public:
        explicit IpData(ScanArgs args);
        ~IpData() override;

    public slots:
        void start() override;
        void replyFinishedEnumIP(QNetworkReply *reply) override;
        void replyFinishedEnumASN(QNetworkReply *reply) override;
        void replyFinishedEnumASNPeers(QNetworkReply *reply) override;
        void replyFinishedEnumASNPrefixes(QNetworkReply *reply) override;

    private:
        QString m_key;
};

#endif // IPDATA_H
