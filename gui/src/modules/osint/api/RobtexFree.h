#ifndef ROBTEXFREE_H
#define ROBTEXFREE_H

#include "../AbstractOsintModule.h"

namespace ModuleInfo {
    struct RobtexFree{
        QString name = "Robtex";
        QString url = "https://www.robtex.com/";
        QString summary = "Robtex is used for various kinds of research of IP numbers, Domain names, etc";

        QMap<QString, QString> flags = {{"ipquery", "ip-address"},
                                        {"asquery", "asn"},
                                        {"pdns forward", "domain name"},
                                        {"pdns reverse", "ip-address"}};
    };
}

class RobtexFree: public AbstractOsintModule{

    public:
        RobtexFree(ScanArgs *args);
        ~RobtexFree() override;

    public slots:
        void start() override;
        void replyFinishedSubdomainIp(QNetworkReply *reply) override;
        void replyFinishedSubdomain(QNetworkReply *reply) override;
        void replyFinishedIp(QNetworkReply *reply) override;
        void replyFinishedAsn(QNetworkReply *reply) override;
};

#endif // ROBTEXFREE_H
