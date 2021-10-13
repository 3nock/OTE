#ifndef HACKERTARGET_H
#define HACKERTARGET_H


#include "../AbstractOsintModule.h"

namespace ModuleInfo {
    struct Hackertarget{
        QString name = "HackerTarget";
        QString url = "https://hackertarget.com/";
        QString summary = "From attack surface discovery to vulnerability identification, actionable network intelligence \n"
                          "for IT & security operations.";
        QMap<QString, QString> flags = {{"subdomains&Ip", "domain name"}, {"asnLookup", "ip-address"}};
    };
}

class Hackertarget: public AbstractOsintModule{

    public:
        Hackertarget(ScanArgs *args);
        ~Hackertarget() override;

    public slots:
        void start() override;
        void replyFinished(QNetworkReply *) override;
};


#endif // HACKERTARGET_H
