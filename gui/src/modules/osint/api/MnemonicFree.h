#ifndef MNEMONICFREE_H
#define MNEMONICFREE_H

#include "../AbstractOsintModule.h"

namespace ModuleInfo {
    struct MnemonicFree{
        QString name = "Mnemonic";
        QString url = "https://www.mnemonic.no/";
        QString summary = "Mnemonic";
        QMap<QString, QString> flags = {{"ip any record", "ip-address"},
                                        {"pdns A", "domain name"},
                                        {"pdns AAAA", "domain name"},
                                        {"pdns CNAME", "domain name"},
                                        {"pdns MX", "domain name"},
                                        {"pdns NS", "domain name"}};
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
