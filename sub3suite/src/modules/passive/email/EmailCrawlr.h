#ifndef EMAILCRAWLR_H
#define EMAILCRAWLR_H

#include "../AbstractOsintModule.h"


namespace ModuleInfo {
struct EmailCrawlr{
    QString name = OSINT_MODULE_EMAILCRAWLR;
    QString url = "https://emailcrawlr.com/";
    QString url_apiDoc = "https://emailcrawlr.com/docs";
    QString summary = "By using the EmailCrawlr JSON API you can: Get key information about company websites. Find all email addresses associated with a domain. "
                      "Get social accounts associated with an email. Verify email address deliverability.";

    QMap<QString, QStringList> flags = {{"Domain Lookup",
                                         {PLACEHOLDERTEXT_DOMAIN, "Look up a domain name and get back information such as email addresses, people, job titles, phone numbers, addresses and more…"}},
                                        {"Email Lookup",
                                         {PLACEHOLDERTEXT_EMAIL, "Look up a domain name and get back information such as email addresses, people, job titles, phone numbers, addresses and more…"}},
                                        {"Email Verification",
                                         {PLACEHOLDERTEXT_EMAIL, "Check/Validate an email address's deliverability. Checks MX records as well as email receipt."}},
                                        {"Account",
                                         {PLACEHOLDERTEXT_NONE, "This call does not count towards your usage."}}};

    QMap<int, QList<int>> input_output = {{IN_DOMAIN,
                                           {OUT_EMAIL}}};
};
}

class EmailCrawlr: public AbstractOsintModule{

    public:
        EmailCrawlr(ScanArgs args);
        ~EmailCrawlr() override;

    public slots:
        void start() override;
        void replyFinishedEmail(QNetworkReply *reply) override;

    private:
        QString m_key;
};

#endif // EMAILCRAWLR_H
