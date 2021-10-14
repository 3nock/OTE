#ifndef THREATCROWD_H
#define THREATCROWD_H


#include "../AbstractOsintModule.h"

namespace ModuleInfo {
    struct Threatcrowd{
        QString name = "Threatcrowd";
        QString url = "https://www.threatcrowd.org/";
        QString summary = "A Search Engine for Threats";
        QMap<QString, QString> flags = {{"email", "email"},
                                        {"domain", "domain name"},
                                        {"ip", "ip-address"},
                                        {"antivirus", "antivirus"},
                                        {"file", "file hash"}};
    };
}

class Threatcrowd: public AbstractOsintModule{

    public:
        Threatcrowd(ScanArgs *args);
        ~Threatcrowd() override;

    public slots:
        void start() override;
        void replyFinished(QNetworkReply *) override;
};

#endif // THREATCROWD_H
