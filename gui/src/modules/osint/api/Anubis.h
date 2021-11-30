#ifndef ANUBIS_H
#define ANUBIS_H

#include "../AbstractOsintModule.h"

namespace ModuleInfo {
    struct Anubis{
        QString name = "Anubis";
        QString url = "https://jldc.me/anubis/";
        QString url_apiDoc = "";
        QString summary = "Subdomain enumeration and information gathering tool";
        QMap<QString, QStringList> flags = {{"subdomain",
                                             {PLACEHOLDERTEXT_DOMAIN, "Returns Subdomains of a particular domain"}}};
        QMap<int, QList<int>> input_output = {{IN_DOMAIN, {OUT_SUBDOMAIN}}};
    };
}

class Anubis: public AbstractOsintModule{

    public:
        Anubis(ScanArgs *args);
        ~Anubis() override;

    public slots:
        void start() override;
        void replyFinishedSubdomain(QNetworkReply *reply) override;
};

#endif // ANUBIS_H
