#ifndef NETCRAFT_H
#define NETCRAFT_H

#include "../AbstractOsintModule.h"


namespace ModuleInfo {
struct Netcraft{
    QString name = OSINT_MODULE_NETCRAFT;
    QString url = "https://netcraft.com/";
    QString url_apiDoc = "";
    QString summary = "Netcraft is an internet services company based in the United Kingdom which provides internet security services, including cybercrime disruption, application security testing and automated vulnerability scanning."
                      "We have explored the internet for a quarter of a century and are an authority on many aspects of the internet.";
    QMap<QString, QStringList> flags = {};
    QMap<int, QList<int>> input_output = {{IN_DOMAIN, {OUT_SUBDOMAIN}}};
};
}

class Netcraft: public AbstractOsintModule{

    public:
        explicit Netcraft(ScanArgs args);
        ~Netcraft() override;

    public slots:
        void start() override;
        void replyFinishedSubdomain(QNetworkReply *reply) override;
};

#endif // NETCRAFT_H
