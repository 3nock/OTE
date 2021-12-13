#ifndef EMAILFORMAT_H
#define EMAILFORMAT_H

#include "../AbstractOsintModule.h"


namespace ModuleInfo {
struct EmailFormat{
    QString name = "EmailFormat";
    QString url = "https://www.email-format.com/";
    QString url_apiDoc = "https://www.email-format.com/i/api_v2/";
    QString summary = "Save time and energy - find the email address formats in use at thousands of companies.";

    QMap<QString, QStringList> flags = {{"Ping",
                                         {PLACEHOLDERTEXT_NONE, "The ping call is used to validate connectivity to the Email Format API server and to confirm that your Authentication header and API private key are functioning as expected. "
                                                                 "Calls to this API are not metered and you will not be billed for their use."}},
                                        {"Get Formats",
                                         {PLACEHOLDERTEXT_DOMAIN, "Returns a JSON-encoded object with the known formats for the given domain. Calls to the API are metered and billed if there are any formats returned with the result. "
                                                                  "If there are no formats available you will not be billed."}},
                                        {"Get Best Formats",
                                         {PLACEHOLDERTEXT_DOMAIN, "Returns a JSON-encoded object with the best known formats for the given domain. Calls to the API are metered and billed if there are any formats returned with the result. "
                                                                  "If there are no formats available you will not be billed."}}};

    QMap<int, QList<int>> input_output = {};
};
}

class EmailFormat: public AbstractOsintModule{

    public:
        EmailFormat(ScanArgs args);
        ~EmailFormat() override;

    public slots:
        void start() override;

    private:
        QString m_key;
};

#endif // EMAILFORMAT_H
