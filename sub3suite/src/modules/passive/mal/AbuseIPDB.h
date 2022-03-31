#ifndef ABUSEIPDB_H
#define ABUSEIPDB_H

#include "../AbstractOsintModule.h"


namespace ModuleInfo {
struct AbuseIPDB{
    QString name = OSINT_MODULE_ABUSEIPDB;
    QString url = "https://abuseipdb.com/";
    QString url_apiDoc = "https://docs.abuseipdb.com/";
    QString summary = "AbuseIPDB is a project managed by Marathon Studios Inc. Our mission is to help make Web safer by providing a central repository for webmasters, system administrators, "
                      "and other interested parties to report and identify IP addresses that have been associated with malicious activity online.";

    QMap<QString, QStringList> flags = {{"CHECK Endpoint",
                                         {PLACEHOLDERTEXT_IP, "The check endpoint accepts a single IP address (v4 or v6)."}},
                                        {"BLACKLIST Endpoint",
                                         {PLACEHOLDERTEXT_NONE, "The blacklist is the culmination of all of the valiant reporting by AbuseIPDB users. It's a list of the most reported IP addresses."}},
                                        {"CHECK-BLOCK Endpoint",
                                         {PLACEHOLDERTEXT_CIDR, "The check-block endpoint accepts a subnet (v4 or v6) denoted with CIDR notation."}}};

    QMap<int, QList<int>> input_output = {};
};
}

class AbuseIPDB: public AbstractOsintModule{

    public:
        explicit AbuseIPDB(ScanArgs args);
        ~AbuseIPDB() override;

    public slots:
        void start() override;

    private:
        QString m_key;
};

#endif // ABUSEIPDB_H
