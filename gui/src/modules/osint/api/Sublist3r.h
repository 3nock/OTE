#ifndef SUBLIST3R_H
#define SUBLIST3R_H

#include "../AbstractOsintModule.h"

/*
 * INPUT domain:                OUTPUT: subdomain
 */

namespace ModuleInfo {
    struct Sublist3r{
        QString name = "Sublist3r";
        QString url = "https://api.sublist3r.com/";
        QString url_apiDoc = "";
        QString summary = "Sublist3r";
        QMap<QString, QStringList> flags = {{"subdomains", {PLACEHOLDERTEXT_DOMAIN, ""}}};
    };
}

class Sublist3r: public AbstractOsintModule{

    public:
        Sublist3r(ScanArgs *args);
        ~Sublist3r() override;

    public slots:
        void start() override;
        void replyFinishedSubdomain(QNetworkReply *) override;
};

#endif // SUBLIST3R_H
