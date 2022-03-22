#ifndef THREATCROWD_H
#define THREATCROWD_H

#include "../AbstractOsintModule.h"


namespace ModuleInfo {
struct Threatcrowd{
    QString name = OSINT_MODULE_THREATCROWD;
    QString url = "https://www.threatcrowd.org/";
    QString url_apiDoc = "https://github.com/AlienVault-OTX/ApiV2";
    QString summary = "A Search Engine for Threats";
    QMap<QString, QStringList> flags = {{"email",
                                         {PLACEHOLDERTEXT_EMAIL, "Search by Email"}},
                                        {"domain",
                                         {PLACEHOLDERTEXT_DOMAIN, "Search by domain name"}},
                                        {"ip",
                                         {PLACEHOLDERTEXT_IP, "Search by ip address"}},
                                        {"antivirus",
                                         {"antivirus", "Search by antivirus"}},
                                        {"file",
                                         {"file hash", "Search by file hash"}}};

    QMap<int, QList<int>> input_output = {{IN_DOMAIN,
                                           {OUT_SUBDOMAIN, OUT_IP, OUT_EMAIL}},
                                          {IN_DOMAIN,
                                           {OUT_SUBDOMAIN}},
                                          {IN_EMAIL,
                                           {OUT_SUBDOMAIN}}};
};
}

class Threatcrowd: public AbstractOsintModule{

    public:
        Threatcrowd(ScanArgs args);
        ~Threatcrowd() override;

    public slots:
        void start() override;
        void replyFinishedSubdomain(QNetworkReply *reply) override;
        void replyFinishedIp(QNetworkReply *reply) override;
        void replyFinishedEmail(QNetworkReply *reply) override;
};

#endif // THREATCROWD_H
