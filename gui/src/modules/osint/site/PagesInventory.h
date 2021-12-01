#ifndef PAGESINVENTORY_H
#define PAGESINVENTORY_H

#include "../AbstractOsintModule.h"


namespace ModuleInfo {
    struct PagesInventory{
        QString name = "PagesInventory";
        QString url = "https://pagesinventory.com/";
        QString url_apiDoc = "";
        QString summary = "";
        QMap<QString, QStringList> flags = {};
        QMap<int, QList<int>> input_output = {{IN_DOMAIN,
                                               {OUT_SUBDOMAINIP, OUT_SUBDOMAIN, OUT_IP}}};
    };
}

class PagesInventory: public AbstractOsintModule{

    public:
        PagesInventory(ScanArgs *args);
        ~PagesInventory() override;

    public slots:
        void start() override;
        void replyFinishedSubdomainIp(QNetworkReply *reply) override;
        void replyFinishedSubdomain(QNetworkReply *reply) override;
        void replyFinishedIp(QNetworkReply *reply) override;
};

#endif // PAGESINVENTORY_H
