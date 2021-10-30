#ifndef URLSCAN_H
#define URLSCAN_H

#include "../AbstractOsintModule.h"

/*
 * INPUT domain:        OUTPUT: subdomain
 */

namespace ModuleInfo {
    struct Urlscan{
        QString name = "Urlscan";
        QString url = "https://urlscan.io/";
        QString summary = "A sandbox for the web";
        QMap<QString, QStringList> flags = {{"domain", {PLACEHOLDERTEXT_IP, ""}}};
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
