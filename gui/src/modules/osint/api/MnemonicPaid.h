#ifndef MNEMONICPAID_H
#define MNEMONICPAID_H

#include "../AbstractOsintModule.h"

/*
 * INPUT domain:            OUTPUT: subdomain, ip
 * INPUT ip:                OUTPUT: subdomain, ip
 */

namespace ModuleInfo {
    struct MnemonicPaid{
        QString name = "Mnemonic";
        QString url = "https://www.mnemonic.no/";
        QString url_apiDoc = "";
        QString summary = "Mnemonic";
        QMap<QString, QStringList> flags = {{"ip any record", {PLACEHOLDERTEXT_IP, ""}},
                                        {"pdns A", {PLACEHOLDERTEXT_DOMAIN, ""}},
                                        {"pdns AAAA", {PLACEHOLDERTEXT_DOMAIN, ""}},
                                        {"pdns CNAME", {PLACEHOLDERTEXT_DOMAIN, ""}},
                                        {"pdns MX", {PLACEHOLDERTEXT_DOMAIN, ""}},
                                        {"pdns NS", {PLACEHOLDERTEXT_DOMAIN, ""}}};
    };
}
class MnemonicPaid: public AbstractOsintModule{

    public:
        MnemonicPaid(ScanArgs *args);
        ~MnemonicPaid() override;

    public slots:
        void start() override;
        void replyFinishedIp(QNetworkReply *reply) override;
        void replyFinishedSubdomain(QNetworkReply *reply) override;

    private:
        QString m_key;
};

#endif // MNEMONICPAID_H
