#ifndef HACKERTARGETFREE_H
#define HACKERTARGETFREE_H


#include "../AbstractOsintModule.h"

namespace ModuleInfo {
    struct HackerTargetFree{
        QString name = "HackerTarget";
        QString url = "https://hackertarget.com/";
        QString summary = "From attack surface discovery to vulnerability identification, actionable network intelligence \n"
                          "for IT & security operations.";
        QMap<QString, QString> flags = {{"dnslookup", "domain name"},
                                        {"reversedns", "ip-address"},
                                        {"geoip", "ip-address"},
                                        {"reverseiplookup", "ip-address"},
                                        {"httpheaders", "domain name"},
                                        {"pagelinks", "domain name"},
                                        {"aslookup", "ip-address"}};
    };
}

class HackerTargetFree: public AbstractOsintModule{

    public:
        HackerTargetFree(ScanArgs *args);
        ~HackerTargetFree() override;

    public slots:
        void start() override;
        void replyFinished(QNetworkReply *) override;
};


#endif // HACKERTARGETFREE_H
