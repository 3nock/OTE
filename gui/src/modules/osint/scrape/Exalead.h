#ifndef EXALEAD_H
#define EXALEAD_H

#include "../AbstractOsintModule.h"

namespace ModuleInfo {
struct Exalead{
    QString name = "Exalead";
    QString url = "https://exalead.com/";
    QString url_apiDoc = "";
    QString summary = "";
    QMap<QString, QStringList> flags = {};
    QMap<int, QList<int>> input_output = {{IN_DOMAIN,
                                           {OUT_SUBDOMAIN, OUT_URL}}};
};
}

class Exalead: public AbstractOsintModule{

    public:
        Exalead(ScanArgs *args);
        ~Exalead() override;

    public slots:
        void start() override;
        void replyFinishedSubdomain(QNetworkReply *reply) override;
        void replyFinishedUrl(QNetworkReply *reply) override;

    private:
        int m_page = 0;
};
#endif // EXALEAD_H
