#ifndef YAHOO_H
#define YAHOO_H

#include "../AbstractOsintModule.h"

namespace ModuleInfo {
struct Yahoo{
    QString name = "Yahoo";
    QString url = "https://yahoo.com/";
    QString url_apiDoc = "";
    QString summary = "";
    QMap<QString, QStringList> flags = {};
    QMap<int, QList<int>> input_output = {{IN_DOMAIN,
                                           {OUT_SUBDOMAIN, OUT_URL}}};
};
}


class Yahoo: public AbstractOsintModule{

    public:
        Yahoo(ScanArgs *args);
        ~Yahoo() override;

    public slots:
        void start() override;
        void replyFinishedSubdomain(QNetworkReply *reply) override;
        void replyFinishedUrl(QNetworkReply *reply) override;

    private:
        int m_page = 0;
};
#endif // YAHOO_H
