#ifndef SITEDOSSIER_H
#define SITEDOSSIER_H

#include "../AbstractOsintModule.h"

/*
 * INPUT domain:            OUTPUT: subdomain
 */
namespace ModuleInfo {
    struct SiteDossier{
        QString name = "SiteDossier";
        QString url = "https://sitedossier.com/";
        QString url_apiDoc = "";
        QString summary = "";
        QMap<QString, QStringList> flags = {};
    };
}

class SiteDossier: public AbstractOsintModule{

    public:
        SiteDossier(ScanArgs *args);
        ~SiteDossier() override;

    public slots:
        void start() override;
        void replyFinishedSubdomain(QNetworkReply *reply) override;
};

#endif // SITEDOSSIER_H
