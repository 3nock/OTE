#ifndef OTX_H
#define OTX_H

#include "../AbstractOsintModule.h"

namespace ModuleInfo {
    struct Otx{
        QString name = "Otx";
        QString url = "https://otx.alienvault.com/";
        QString summary = "The World’s First Truly Open Threat Intelligence Community";

        QMap<QString, QString> flags = {{"indicator ipv4", "ipv4 address"},
                                        {"indicator ipv6", "ipv6 address"},
                                        {"indicator domain", "domain name"},
                                        {"indicator hostname", "hostname"},
                                        {"indicator url", "url"}};
    };
}

class Otx: public AbstractOsintModule{

    public:
        Otx(ScanArgs *args);
        ~Otx() override;

    public slots:
        void start() override;
        void replyFinished(QNetworkReply *) override;
};

#endif // OTX_H
