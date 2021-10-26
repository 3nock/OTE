#ifndef URLSCAN_H
#define URLSCAN_H

#include "../AbstractOsintModule.h"

namespace ModuleInfo {
    struct Urlscan{
        QString name = "Urlscan";
        QString url = "https://urlscan.io/";
        QString summary = "A sandbox for the web";
        QMap<QString, QString> flags = {{"domain", "domain name"}};
    };
}

class Urlscan: public AbstractOsintModule{

    public:
        Urlscan(ScanArgs *args);
        ~Urlscan() override;

    public slots:
        void start() override;
        void replyFinishedSubdomain(QNetworkReply *reply) override;
        void replyFinishedIp(QNetworkReply *reply) override;
        void replyFinishedUrl(QNetworkReply *reply) override;
        void replyFinishedAsn(QNetworkReply *reply) override;
};

#endif // URLSCAN_H
