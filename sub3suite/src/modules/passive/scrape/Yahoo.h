#ifndef YAHOO_H
#define YAHOO_H

#include "../AbstractOsintModule.h"

namespace ModuleInfo {
struct Yahoo{
    QString name = OSINT_MODULE_YAHOO;
    QString url = "https://yahoo.com/";
    QString url_apiDoc = "";
    QString summary = "";
    QMap<QString, QStringList> flags = {};
    QMap<int, QList<int>> input_output = {{IN_DOMAIN,
                                           {OUT_SUBDOMAIN, OUT_URL, OUT_EMAIL}}};
};
}


class Yahoo: public AbstractOsintModule{

    public:
        Yahoo(ScanArgs args);
        ~Yahoo() override;

    public slots:
        void start() override;
        void replyFinishedSubdomain(QNetworkReply *reply) override;
        void replyFinishedEmail(QNetworkReply *reply) override;
        void replyFinishedUrl(QNetworkReply *reply) override;

    private:
        bool m_firstRequest = false;
        int m_lastPage = 1;
        void sendRequests();
};
#endif // YAHOO_H
