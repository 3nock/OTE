#ifndef PKEY_H
#define PKEY_H

#include "../AbstractOsintModule.h"

/*
 * INPUT domain:            OUTPUT: subdomain
 */
namespace ModuleInfo {
    struct Pkey{
        QString name = "Pkey";
        QString url = "https://pkey.in/";
        QString url_apiDoc = "";
        QString summary = "";
        QMap<QString, QStringList> flags = {};
    };
}

class Pkey: public AbstractOsintModule{

    public:
        Pkey(ScanArgs *args);
        ~Pkey() override;

    public slots:
        void start() override;
        void replyFinishedSubdomain(QNetworkReply *reply) override;
        void replyFinishedSubdomainIp(QNetworkReply *reply) override;
        void replyFinishedIp(QNetworkReply *reply) override;
};

#endif // PKEY_H
