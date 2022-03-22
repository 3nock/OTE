#ifndef WAPPALYZER_H
#define WAPPALYZER_H

#include "../AbstractOsintModule.h"


namespace ModuleInfo {
struct Wappalyzer{
    QString name = OSINT_MODULE_WAPPALYZER;
    QString url = "https://www.wappalyzer.com/";
    QString url_apiDoc = "https://www.wappalyzer.com/docs/api/v2/basics/";
    QString summary = "We track over a thousand web technologies across millions of websites. We know who uses your software and that of your competitors.";

    QMap<QString, QStringList> flags = {{"Balance",
                                         {PLACEHOLDERTEXT_NONE, "Endpoint to get your remaining credit balance"}},
                                        {"Technology Lookup",
                                         {PLACEHOLDERTEXT_URL, "Find out the technology stack of any website, such as the CMS or ecommerce platform. "
                                                               "Perform near-instant queries against our comprehensive database of millions of websites or analyse websites live for real-time results. "
                                                               "In addition to technologies, responses optionally include "
                                                               "company details, verified email addresses, phone numbers, social media profiles, locale information, keywords, metadata and more."}},
                                        {"Email Verification",
                                         {PLACEHOLDERTEXT_EMAIL, "Verify email addresses before sending to improve delivery. A high bounce rate lowers your sender reputation and you risk getting blocked by email providers."}}};

    QMap<int, QList<int>> input_output = {};
};
}

class Wappalyzer: public AbstractOsintModule{

    public:
        Wappalyzer(ScanArgs args);
        ~Wappalyzer() override;

    public slots:
        void start() override;

    private:
        QString m_key;
};


#endif // WAPPALYZER_H
