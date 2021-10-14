#ifndef SPYSE_H
#define SPYSE_H

#include "../AbstractOsintModule.h"

namespace ModuleInfo {
    struct Spyse{
        QString name = "Spyse";
        QString url = "https://spyse.com/";
        QString summary = "Find any Internet asset by digital fingerprints";

        QMap<QString, QString> flags = {{"Domain", "domain name"},
                                        {"IPv4 Host", "ip-address"},
                                        {"SSL/TLS Certificate", "cert id"},
                                        {"AS", "asn"},
                                        {"CVE", "CVE id"},
                                        {"Emails", "email"},
                                        {"DNS History", "domain name"}};
    };
}

class Spyse: public AbstractOsintModule{

    public:
        Spyse(ScanArgs *args);
        ~Spyse() override;

    public slots:
        void start() override;
        void replyFinished(QNetworkReply *) override;
};

#endif // SPYSE_H
