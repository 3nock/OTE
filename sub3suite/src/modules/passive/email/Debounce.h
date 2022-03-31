#ifndef DEBOUNCE_H
#define DEBOUNCE_H

#include "../AbstractOsintModule.h"


namespace ModuleInfo {
struct Debounce{
    QString name = OSINT_MODULE_DEBOUNCE;
    QString url = "https://debounce.io/";
    QString url_apiDoc = "https://docs.debounce.io/";
    QString summary = "Email Validation Tool; Simple, Fast & Accurate.";

    QMap<QString, QStringList> flags = {{"Single Validation",
                                         {PLACEHOLDERTEXT_EMAIL, "Perform a single email validation request."}},
                                        {"Disposable Email Detector",
                                         {PLACEHOLDERTEXT_EMAIL, "DeBounce provides a free & powerful API endpoint for checking a domain or email address against a realtime up-to-date list of disposable domains."}},
                                        {"Account Balance",
                                         {PLACEHOLDERTEXT_NONE, "Get the remaining balance of your DeBounce account."}},
                                        {"Api Usage History",
                                         {PLACEHOLDERTEXT_NONE, "You can get the number of calls between two dates for the desired API key"}}};

    QMap<int, QList<int>> input_output = {};
};
}

class Debounce: public AbstractOsintModule{

    public:
        explicit Debounce(ScanArgs args);
        ~Debounce() override;

    public slots:
        void start() override;

    private:
        QString m_key;
};

#endif // DEBOUNCE_H
