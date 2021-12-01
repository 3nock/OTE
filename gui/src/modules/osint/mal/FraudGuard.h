#ifndef FRAUDGUARD_H
#define FRAUDGUARD_H

#include "../AbstractOsintModule.h"


namespace ModuleInfo {
    struct FraudGuard{
        QString name = "FraudGuard";
        QString url = "https://fraudguard.io/";
        QString url_apiDoc = "https://docs.fraudguard.io/";
        QString summary = "FraudGuard...";

        QMap<QString, QStringList> flags = {{"Specific IP Reputation v2",
                                             {PLACEHOLDERTEXT_IP, "This API endpoint retrieves IP reputation data for a specific IP. It is vastly more accurate and detailed but requires a paid plan."}},
                                            {"Specific Hostname Reputation v2",
                                             {PLACEHOLDERTEXT_HOSTNAME, "This API endpoint retrieves IP reputation data for a specific Hostname. It is vastly more accurate and detailed but requires a paid plan."}},
                                            {"Specific IP v2 Geo,ISP & Org",
                                             {PLACEHOLDERTEXT_IP, "This API endpoint retrieves Geographic, ISP and Organizational data for a specific IP."}},
                                            {"Specific IP Reputation v1",
                                             {PLACEHOLDERTEXT_IP, "This API endpoint retrieves IP reputation data for a specific IP. It is less accurate and far less detailed but is free forever."}},
                                            {"Get Custom Blacklist",
                                             {PLACEHOLDERTEXT_NONE, "This API endpoint retrieves your custom blacklist built in FraudGuard.io"}},
                                            {"Get Custom Whitelist",
                                             {PLACEHOLDERTEXT_NONE, "This API endpoint retrieves your custom whitelist built in FraudGuard.io"}},
                                            {"Get Custom GeoBlock",
                                             {PLACEHOLDERTEXT_NONE, "This API endpoint retrieves your custom Geographic blacklist built in FraudGuard.io"}}};

        QMap<int, QList<int>> input_output = {};
    };
}

class FraudGuard: public AbstractOsintModule{

    public:
        FraudGuard(ScanArgs *args);
        ~FraudGuard() override;

    public slots:
        void start() override;

    private:
        QString m_user;
        QString m_pass;
};

#endif // FRAUDGUARD_H
