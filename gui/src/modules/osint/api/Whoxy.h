#ifndef WHOXY_H
#define WHOXY_H

#include "../AbstractOsintModule.h"

namespace ModuleInfo {
    struct Whoxy{
        QString name = "Whoxy";
        QString url = "https://www.whoxy.com/";
        QString url_apiDoc = "https://www.whoxy.com/#api";
        QString summary = "Our Whois API returns consistent and well-structured WHOIS data in XML & JSON format. Returned data contain parsed WHOIS fields that can be easily understood by your application. "
                          "Along with Whois API, we also provide API access to Whois History and Reverse Whois.";

        QMap<QString, QStringList> flags = {{"Account Balance Check",
                                             {PLACEHOLDERTEXT_NONE, "Check your account's API query balance:"}},
                                            {"Whois Lookup",
                                             {PLACEHOLDERTEXT_DOMAIN, "Whois API digs into WHOIS registry referral chains until the correct WHOIS servers are found, for the most complete WHOIS data."}},
                                            {"Whois History",
                                             {PLACEHOLDERTEXT_DOMAIN, "Whois History API returns all the historical WHOIS records of a domain name that is saved in our local database."}},
                                            {"Reverse Whois Name",
                                             {PLACEHOLDERTEXT_NAME, "Reverse Whois API lets you perform a comprehensive search on our database, to find domains owned by a person."}},
                                            {"Reverse Whois Email",
                                             {PLACEHOLDERTEXT_EMAIL, "Reverse Whois API lets you perform a comprehensive search on our database, to find domains by email."}},
                                            {"Reverse Whois Company",
                                             {PLACEHOLDERTEXT_ORG, "Reverse Whois API lets you perform a comprehensive search on our database, to find domains owned by a company."}},
                                            {"Reverse Whois Keyword",
                                             {"domain keyword eg google", "Reverse Whois API lets you perform a comprehensive search on our database, to find domains by a domain name keyword."}}};

        QMap<int, QList<int>> input_output = {};
    };
}

class Whoxy: public AbstractOsintModule{

    public:
        Whoxy(ScanArgs *args);
        ~Whoxy() override;

    public slots:
        void start() override;

    private:
        QString m_key;
};

#endif // WHOXY_H
