#ifndef OMNISINT_H
#define OMNISINT_H

#include "../AbstractOsintModule.h"

namespace ModuleInfo {
    struct Omnisint{
        QString name = "Omnisint";
        QString url = "https://omnisint.io/";
        QString summary = "Rapid7's DNS Database easily searchable via a lightening fast API, \n"
                          "with domains available in milliseconds.";

        QMap<QString, QString> flags = {{"subdomains", "domain name"},
                                        {"tlds", "domain name"},
                                        {"subdomainIp", "domain name"},
                                        {"reverse", "ip-address or ip/cdir"}};
    };
}

class Omnisint: public AbstractOsintModule{

    public:
        Omnisint(ScanArgs *args);
        ~Omnisint() override;

    public slots:
        void start() override;
        void replyFinished(QNetworkReply *) override;

    private:
        int m_page = 1;
};

#endif // OMNISINT_H
