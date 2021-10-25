#ifndef ROBTEXPAID_H
#define ROBTEXPAID_H

#include "../AbstractOsintModule.h"

namespace ModuleInfo {
    struct RobtexPaid{
        QString name = "Robtex";
        QString url = "https://www.robtex.com/";
        QString summary = "Robtex is used for various kinds of research of IP numbers, Domain names, etc";

        QMap<QString, QString> flags = {{"ipquery", "ip-address"},
                                        {"asquery", "asn"},
                                        {"pdns forward", "domain name"},
                                        {"pdns reverse", "ip-address"}};
    };
}

class RobtexPaid: public AbstractOsintModule{

    public:
        RobtexPaid(ScanArgs *args);
        ~RobtexPaid() override;

    public slots:
        void start() override;
        void replyFinishedSubdomainIp(QNetworkReply *reply) override;
        void replyFinishedSubdomain(QNetworkReply *reply) override;
        void replyFinishedIp(QNetworkReply *reply) override;
        void replyFinishedAsn(QNetworkReply *reply) override;

    private:
        QString m_key;
};
#endif // ROBTEXPAID_H
