#ifndef MNEMONICPAID_H
#define MNEMONICPAID_H

#include "../AbstractOsintModule.h"

namespace ModuleInfo {
    struct MnemonicPaid{
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
