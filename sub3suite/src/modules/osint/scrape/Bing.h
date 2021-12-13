#ifndef BING_H
#define BING_H

#include "../AbstractOsintModule.h"

namespace ModuleInfo {
struct Bing{
    QString name = "Bing";
    QString url = "https://bing.com/";
    QString url_apiDoc = "";
    QString summary = "";
    QMap<QString, QStringList> flags = {};
    QMap<int, QList<int>> input_output = {{IN_DOMAIN,
                                           {OUT_SUBDOMAIN, OUT_URL, OUT_EMAIL}}};
};
}

class Bing: public AbstractOsintModule{

    public:
        Bing(ScanArgs args);
        ~Bing() override;

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
#endif // BING_H
