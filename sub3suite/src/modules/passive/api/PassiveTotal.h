#ifndef PASSIVETOTAL_H
#define PASSIVETOTAL_H

#include "../AbstractOsintModule.h"


namespace ModuleInfo {
struct PassiveTotal{
    QString name = OSINT_MODULE_PASSIVETOTAL;
    QString url = "https://community.riskiq.com/";
    QString url_apiDoc = "https://api.riskiq.net/api/pt_started.html";
    QString summary = "PassiveTotal provides RiskIQ customers the ability to investigate threat to their environment through a rich user interface; "
                      "broad access to aggregated, curated, and interconnected data from both RiskIQ and partner sources; "
                      "the ability to seamlessly pivot through these data sets; and robust integrations with third party security providers, "
                      "bringing the PassiveTotal experience to security tools already deployed within your environment.";

    QMap<QString, QStringList> flags = {{"Account",
                                         {PLACEHOLDERTEXT_NONE, "allow you to see all the information related to your account."}},
                                        {"Account History",
                                         {PLACEHOLDERTEXT_NONE, "Read API usage history of the account."}},
                                        {"Account Monitors",
                                         {PLACEHOLDERTEXT_NONE, "Get active monitors"}},
                                        {"Account Organization",
                                         {PLACEHOLDERTEXT_NONE, "Read current organization metadata"}},
                                        {"Account Quotas",
                                         {PLACEHOLDERTEXT_NONE, "Read current account and organization quotas."}},
                                        {"Account Sources",
                                         {PLACEHOLDERTEXT_NONE, "Check sources being used for queries."}},
                                        {"Account TeamStream",
                                         {PLACEHOLDERTEXT_NONE, "Read team activity."}},
                                        {"Account Classifications",
                                         {PLACEHOLDERTEXT_NONE, "Retrieve items with the specified classification."}},
                                        {"Certificates",
                                         {PLACEHOLDERTEXT_SSLCERT, "Retrieves an SSL certificate by its SHA-1 hash."}},
                                        {"Certificates History",
                                         {"certificate SHA-1 hash or IP address", "Retrieves the SSL certificate history for a given certificate SHA-1 hash or IP address."}},
                                        {"Certificate Keyword",
                                         {"Keyword e.g. google", "Retrieves SSL certificates for a given keyword."}},
                                        {"Passive DNS",
                                         {PLACEHOLDERTEXT_DOMAIN, "Retrieves the passive DNS results from active account sources."}},
                                        {"Passive DNS unique",
                                         {PLACEHOLDERTEXT_DOMAIN, "Retrieves the unique passive DNS results from active account sources."}},
                                        {"Passive DNS search",
                                         {PLACEHOLDERTEXT_QUERY, "Searches the Passive DNS data for a keyword query."}},
                                        {"Whois",
                                         {PLACEHOLDERTEXT_DOMAIN, "Retrieves the WHOIS data for the specified query"}},
                                        {"Whois Keyword",
                                         {PLACEHOLDERTEXT_QUERY, "Search WHOIS data for a keyword."}}};

    QMap<int, QList<int>> input_output = {{IN_DOMAIN,
                                           {OUT_SUBDOMAINIP, OUT_SUBDOMAIN, OUT_IP}},
                                          {IN_IP,
                                           {OUT_SSLCERT}},
                                          {IN_SSLCERT,
                                           {OUT_IP, OUT_SSLCERT}},
                                          {IN_QUERYTERM,
                                           {OUT_SUBDOMAIN, OUT_SSLCERT}}};
};
}

class PassiveTotal: public AbstractOsintModule{

    public:
        PassiveTotal(ScanArgs args);
        ~PassiveTotal() override;

    public slots:
        void start() override;
        void replyFinishedSubdomainIp(QNetworkReply *reply) override;
        void replyFinishedSubdomain(QNetworkReply *reply) override;
        void replyFinishedIp(QNetworkReply *reply) override;
        void replyFinishedSSL(QNetworkReply *reply) override;

    private:
        QString m_key;
        QString m_username;
};

#endif // PASSIVETOTAL_H
