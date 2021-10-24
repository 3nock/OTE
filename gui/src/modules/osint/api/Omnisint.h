#ifndef OMNISINT_H
#define OMNISINT_H

#include "../AbstractOsintModule.h"

namespace ModuleInfo {
    struct Omnisint{
        QString name = "Omnisint";
        QString url = "https://omnisint.io/";
        QString summary = "Rapid7's DNS Database easily searchable via a lightening fast API, \n"
                          "with domains available in milliseconds.";

        QMap<QString, QString> flags = {{"subdomains", "domain name"},
                                        {"tlds", "domain name"},
                                        {"all", "domain name"},
                                        {"reverse ip", "ip-address or ip/cdir"}};
    };
}

class Omnisint: public AbstractOsintModule{

    public:
        Omnisint(ScanArgs *args);
        ~Omnisint() override;

    public slots:
        void start() override;
        void replyFinishedSubdomain(QNetworkReply *) override;
};

#endif // OMNISINT_H
