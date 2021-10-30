#ifndef CERTSPOTTER_H
#define CERTSPOTTER_H

#include "../AbstractOsintModule.h"

namespace ModuleInfo {
    struct Certspotter{
        QString name = "Certspotter";
        QString url = "https://sslmate.com/certspotter/";
        QString url_apiDoc = "";
        QString summary = "Cert Spotter monitors your domains for expiring, unauthorized, and invalid SSL certificates, \n"
                          "so you can act before an incident, not after";
        QMap<QString, QStringList> flags = {{"cert", {PLACEHOLDERTEXT_DOMAIN, ""}}};
    };
}

class Certspotter : public AbstractOsintModule {
    public:
        Certspotter(ScanArgs *args);
        ~Certspotter() override;

    public slots:
        void start() override;
        void replyFinished(QNetworkReply *reply) override;
};

#endif // CERTSPOTTER_H
