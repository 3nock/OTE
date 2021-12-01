#ifndef APNIC_H
#define APNIC_H

#include "../AbstractOsintModule.h"

namespace ModuleInfo {
    struct Apnic{
        QString name = "Apnic";
        QString url = "https://Apnic.net/";
        QString url_apiDoc = "https://www.apnic.net/about-apnic/whois_search/about/rdap/";
        QString summary = "The APNIC Whois Database is an official record that contains information regarding organizations that hold IP addresses and AS Numbers in the Asia Pacific region "
                          "Users querying the database can determine who is responsible for an IP address range or an AS Number.";

        QMap<QString, QStringList> flags = {{"IP networks",
                                             {PLACEHOLDERTEXT_IP, "This query will return the most-specific or smallest IP network which completely encompasses it in a hierarchy of IP networks"}}};

        QMap<int, QList<int>> input_output = {};
    };
}

class Apnic: public AbstractOsintModule{

    public:
        Apnic(ScanArgs *args);
        ~Apnic() override;

    public slots:
        void start() override;
};
#endif // APNIC_H
