#ifndef NETCRAFT_H
#define NETCRAFT_H

#include "../AbstractOsintModule.h"

/*
 * INPUT domain:            OUTPUT: subdomain
 */
namespace ModuleInfo {
    struct Netcraft{
        QString name = "Netcraft";
        QString url = "https://netcraft.com/";
        QString url_apiDoc = "";
        QString summary = "";
        QMap<QString, QStringList> flags = {};
    };
}

class Netcraft: public AbstractOsintModule{

    public:
        Netcraft(ScanArgs *args);
        ~Netcraft() override;

    public slots:
        void start() override;
        void replyFinishedSubdomain(QNetworkReply *reply) override;
};

#endif // NETCRAFT_H
