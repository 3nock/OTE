#ifndef AFRINIC_H
#define AFRINIC_H

#include "../AbstractOsintModule.h"


namespace ModuleInfo {
struct Afrinic{
    QString name = OSINT_MODULE_AFRINIC;
    QString url = "https://afrinic.net/";
    QString url_apiDoc = "https://afrinic.net/whois/rdap#how-to";
    QString summary = "AFRINIC Whois Database is a public database containing information regarding the entities that hold Internet number resources "
                      "(IP addresses and AS Numbers) in the AFRINIC service region.";

    QMap<QString, QStringList> flags = {{"IP networks",
                                         {PLACEHOLDERTEXT_CIDR, "This query will return the most-specific or smallest IP network which completely encompasses it in a hierarchy of IP networks"}},
                                        {"AS Numbers",
                                         {PLACEHOLDERTEXT_ASN, "Used to identify autonomous system registrations and associated data referenced"}},
                                        {"RDNS",
                                         {PLACEHOLDERTEXT_DOMAIN, "Used to identify reverse DNS information and associated data referenced"}},
                                        {"Entity",
                                         {"eg TEAM-AFRINIC", "Used to retrieve entity related information using an entity string identifier"}}};

    QMap<int, QList<int>> input_output = {};
};
}

class Afrinic: public AbstractOsintModule{

    public:
        explicit Afrinic(ScanArgs args);
        ~Afrinic() override;

    public slots:
        void start() override;
};

#endif // AFRINIC_H
