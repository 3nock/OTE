#ifndef CIRCL_H
#define CIRCL_H

#include "../AbstractOsintModule.h"

namespace ModuleInfo {
    struct Circl{
        QString name = "Circl";
        QString url = "https://www.circl.lu/";
        QString summary = "Circl";
        QMap<QString, QString> flags = {{"Passive dns", "domain name / ip-address"},
                                        {"Passive SSL", "ip-address/cdir"},
                                        {"Passive SSL Fetch", "SHA1 certificate fingerprint"},
                                        {"Passive SSL query", "SHA1 certificate fingerprint"},
                                        {"Ip 2 ASN", "ip-address"}};
    };
}

class Circl: public AbstractOsintModule{

    public:
        Circl(ScanArgs *args);
        ~Circl() override;

    public slots:
        void start() override;
        void replyFinishedCertFingerprint(QNetworkReply *reply) override;
        void replyFinishedSubdomain(QNetworkReply *reply) override;
        void replyFinishedIp(QNetworkReply *reply) override;
        void replyFinishedAsn(QNetworkReply *reply) override;
};

#endif // CIRCL_H
