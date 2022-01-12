#ifndef SNOV_H
#define SNOV_H

#include "../AbstractOsintModule.h"


namespace ModuleInfo {
struct Snov{
    QString name = "Snov";
    QString url = "https://snov.io/";
    QString url_apiDoc = "https://snov.io/api";
    QString summary = "Snov.io API allows to get a list of all emails from a particular domain, find email addresses by name and domain, verify emails, add prospects to a list, change a recipient's status and more."
                      "Note, the API rate is limited to 60 requests per minute.";

    QMap<QString, QStringList> flags = {{"Domain Search",
                                         {PLACEHOLDERTEXT_DOMAIN, "Enter a domain name and Snov.io will return all the email addresses on the domain. If there is any additional information about the email owner available in the database, we will add it as well. "
                                                                  "Each response returns up to 100 emails. If it does not return at least one email, you will not be charged for the request."}},
                                        {"Email count",
                                         {PLACEHOLDERTEXT_DOMAIN, "With this API method, you can find out the number of email addresses from a certain domain in our database. It`s completely free, so you don`t need credits to use it!"}},
                                        {"Get profile with email",
                                         {PLACEHOLDERTEXT_EMAIL, "Provide an email address and Snov.io will return all the profile information connected to the provided email address owner from the database."
                                                                "If we find no information about the email owner in our database, you will not be charged for the request."}},
                                        {"Email Verifier",
                                         {PLACEHOLDERTEXT_EMAIL, "Check if the provided email addresses are valid and deliverable. API endpoint will return the email verification results."}},
                                        {"Check user balance",
                                         {PLACEHOLDERTEXT_NONE, "Use this method to check your credit balance."}}};

    QMap<int, QList<int>> input_output = {{IN_DOMAIN, {OUT_EMAIL}}};
};
}

class Snov: public AbstractOsintModule{

    public:
        Snov(ScanArgs args);
        ~Snov() override;

    public slots:
        void start() override;
        void replyFinishedEmail(QNetworkReply *reply) override;

    private:
        QString m_key;
};

#endif // SNOV_H
