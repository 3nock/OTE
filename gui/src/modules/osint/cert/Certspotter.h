#ifndef CERTSPOTTER_H
#define CERTSPOTTER_H

#include "../AbstractOsintModule.h"

namespace ModuleInfo {
    struct Certspotter{
        QString name = "Certspotter";
        QString url = "https://sslmate.com/certspotter/";
        QString summary = "Cert Spotter monitors your domains for expiring, unauthorized, and invalid SSL certificates, \n"
                          "so you can act before an incident, not after";
        QMap<QString, QString> flags = {{"cert", "domain name"}};
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
