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
        void replyFinished(QNetworkReply *) override;
};

#endif // URLSCAN_H
