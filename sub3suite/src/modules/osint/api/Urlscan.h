#ifndef URLSCAN_H
#define URLSCAN_H

#include "../AbstractOsintModule.h"


namespace ModuleInfo {
struct Urlscan{
    QString name = "Urlscan";
    QString url = "https://urlscan.io/";
    QString url_apiDoc = "https://urlscan.io/docs/api/";
    QString summary = "Our APIs allow you to submit URLs for scanning and retrieve the results once the scan has finished. Furthermore, you can use an API for searching existing scans by attributes such as domains, IPs, Autonomous System (AS) numbers, hashes, etc";
    QMap<QString, QStringList> flags = {{"domain",
                                         {PLACEHOLDERTEXT_DOMAIN, "Search by domain name"}}};

    QMap<int, QList<int>> input_output = {{IN_DOMAIN, {OUT_SUBDOMAIN}}};
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
