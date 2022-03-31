#ifndef EXALEAD_H
#define EXALEAD_H

#include "../AbstractOsintModule.h"

namespace ModuleInfo {
struct Exalead{
    QString name = OSINT_MODULE_EXALEAD;
    QString url = "https://exalead.com/";
    QString url_apiDoc = "";
    QString summary = "";
    QMap<QString, QStringList> flags = {};
    QMap<int, QList<int>> input_output = {{IN_DOMAIN,
                                           {OUT_SUBDOMAIN, OUT_URL}},
                                          {IN_QUERYTERM,
                                           {OUT_SUBDOMAIN, OUT_URL}}};
};
}

class Exalead: public AbstractOsintModule{

    public:
        explicit Exalead(ScanArgs args);
        ~Exalead() override;

    public slots:
        void start() override;
        void replyFinishedSubdomain(QNetworkReply *reply) override;
        void replyFinishedUrl(QNetworkReply *reply) override;
};
#endif // EXALEAD_H
