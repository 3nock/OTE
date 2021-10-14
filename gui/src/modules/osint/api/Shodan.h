#ifndef SHODAN_H
#define SHODAN_H

#include "../AbstractOsintModule.h"

namespace ModuleInfo {
    struct Shodan{
        QString name = "Shodan";
        QString url = "https://www.shodan.io/";
        QString summary = "Search Engine for the Internet of Everything";

        QMap<QString, QString> flags = {{"host ip", "ip-address"},
                                        {"host count", "query"},
                                        {"host search", "query"},
                                        {"host search facets", ""},
                                        {"host search filters", ""},
                                        {"host search tokens", "query"},
                                        {"bulk data", "dataset"},
                                        {"account profile", ""},
                                        {"dns domain", "domain name"},
                                        {"dns resolve", "domain name, domain name"},
                                        {"dns reverse", "ip-address, ip-address"}};
    };
}

class Shodan: public AbstractOsintModule{

    public:
        Shodan(ScanArgs *args);
        ~Shodan() override;

    public slots:
        void start() override;
        void replyFinished(QNetworkReply *) override;

    private:
        QString m_key = nullptr;
};
#endif // SHODAN_H
