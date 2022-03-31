#ifndef NEUTRINOAPI_H
#define NEUTRINOAPI_H

#include "../AbstractOsintModule.h"

namespace ModuleInfo {
struct NeutrinoApi{
    QString name = OSINT_MODULE_NEUTRINOAPI;
    QString url = "https://www.neutrinoapi.com/";
    QString url_apiDoc = "https://www.neutrinoapi.com/api/api-basics/";
    QString summary = "Neutrino API solves recurring problems encountered during the development of software systems. Neutrino API is a general-purpose tool which can assist in many different situations.";

    QMap<QString, QStringList> flags = {{"Email Validate",
                                         {PLACEHOLDERTEXT_EMAIL, "Parse, validate and clean an email address."}},
                                        {"Phone Validate",
                                         {PLACEHOLDERTEXT_PHONE, "Parse, validate and get location information about a phone number."}},
                                        {"Ua Lookup",
                                         {"user-agent", "Parse, validate and get detailed user-agent information from a user agent string or from client hints."}},
                                        {"Url Info",
                                         {PLACEHOLDERTEXT_URL, "Parse, analyze and retrieve content from the supplied URL."}},
                                        {"Ip Info",
                                         {PLACEHOLDERTEXT_IP, "Get location information about an IP address and do reverse DNS (PTR) lookups."}},
                                        {"Ip Probe",
                                         {PLACEHOLDERTEXT_IP, "Analyze and extract provider information for an IP address."}},
                                        {"Ip Blocklist",
                                         {PLACEHOLDERTEXT_IP, "The IP Blocklist API will detect potentially malicious or dangerous IP addresses."}},
                                        {"Host Reputation",
                                         {PLACEHOLDERTEXT_DOMAIN_OR_IP, "Check the reputation of an IP address, domain name or URL against a comprehensive list of blacklists and blocklists."}}};

    QMap<int, QList<int>> input_output = {};
};
}

class NeutrinoApi: public AbstractOsintModule{

    public:
        explicit NeutrinoApi(ScanArgs args);
        ~NeutrinoApi() override;

    public slots:
        void start() override;

    private:
        QString m_key;
        QString m_userId;
};

#endif // NEUTRINOAPI_H
