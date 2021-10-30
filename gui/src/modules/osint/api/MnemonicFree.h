#ifndef MNEMONICFREE_H
#define MNEMONICFREE_H

#include "../AbstractOsintModule.h"

/*
 * INPUT domain:            OUTPUT: subdomain, ip
 * INPUT ip:                OUTPUT: subdomain, ip
 */

namespace ModuleInfo {
    struct MnemonicFree{
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

class MnemonicFree: public AbstractOsintModule{

    public:
        MnemonicFree(ScanArgs *args);
        ~MnemonicFree() override;

    public slots:
        void start() override;
        void replyFinishedIp(QNetworkReply *reply) override;
        void replyFinishedSubdomain(QNetworkReply *reply) override;
};

#endif // MNEMONICFREE_H
