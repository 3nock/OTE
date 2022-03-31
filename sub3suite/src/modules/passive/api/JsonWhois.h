#ifndef JSONWHOIS_H
#define JSONWHOIS_H

#include "../AbstractOsintModule.h"

namespace ModuleInfo {
struct JsonWhois{
    QString name = OSINT_MODULE_JSONWHOIS;
    QString url = "https://jsonwhois.com/";
    QString url_apiDoc = "https://jsonwhois.com/docs";
    QString summary = "Domain API Services; WHOIS | SCREENSHOTS | SOCIAL.";

    QMap<QString, QStringList> flags = {{"Whois & Social",
                                         {PLACEHOLDERTEXT_DOMAIN, "Quickly and easily get up to date whois data for all domains."}},
                                        {"Social Data",
                                         {PLACEHOLDERTEXT_DOMAIN, ""}},
                                        {"Screenshot",
                                         {PLACEHOLDERTEXT_DOMAIN, "The fastest and most reliable screenshot API available. Grab a full page screenshot of any domain, which you can then thumbnail or display as is."}}};

    QMap<int, QList<int>> input_output = {};
};
}

class JsonWhois: public AbstractOsintModule{

    public:
        explicit JsonWhois(ScanArgs args);
        ~JsonWhois() override;

    public slots:
        void start() override;

    private:
        QString m_key;
};

#endif // JSONWHOIS_H
