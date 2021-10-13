#ifndef ANUBIS_H
#define ANUBIS_H

#include "../AbstractOsintModule.h"

namespace ModuleInfo {
    struct Anubis{
        QString name = "Anubis";
        QString url = "https://jldc.me/anubis/";
        QString summary = "Subdomain enumeration and information gathering tool";
        QMap<QString, QString> flags = {{"subdomains", "domain name"}};
    };
}

class Anubis: public AbstractOsintModule{

    public:
        Anubis(ScanArgs *args);
        ~Anubis() override;

    public slots:
        void start() override;
        void replyFinished(QNetworkReply *) override;
};

#endif // ANUBIS_H
