#ifndef PROJECTDISCOVERY_H
#define PROJECTDISCOVERY_H

#include "../AbstractOsintModule.h"

namespace ModuleInfo {
    struct Projectdiscovery{
        QString name = "Projectdiscovery";
        QString url = "https://chaos.projectdiscovery.io/";
        QString summary = "We actively collect and maintain internet-wide assets' data, \n"
                          "this project is meant to enhance research and analyse changes around DNS for better insights";
        QMap<QString, QString> flags = {{"subdomain", "domain name"}};
    };
}

class Projectdiscovery: public AbstractOsintModule{

    public:
        Projectdiscovery(ScanArgs *args);
        ~Projectdiscovery() override;

    public slots:
        void start() override;
        void replyFinishedSubdomain(QNetworkReply *) override;

    private:
        QString m_key;
};

#endif // PROJECTDISCOVERY_H
