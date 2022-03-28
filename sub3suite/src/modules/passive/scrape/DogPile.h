#ifndef DOGPILE_H
#define DOGPILE_H

#include "../AbstractOsintModule.h"

namespace ModuleInfo {
struct DogPile{
    QString name = OSINT_MODULE_DOGPILE;
    QString url = "https://dogpile.com/";
    QString url_apiDoc = "";
    QString summary = "";
    QMap<QString, QStringList> flags = {};
    QMap<int, QList<int>> input_output = {{IN_DOMAIN,
                                           {OUT_SUBDOMAIN, OUT_URL, OUT_EMAIL}}};
};
}

class DogPile: public AbstractOsintModule{

    public:
        DogPile(ScanArgs args);
        ~DogPile() override;

    public slots:
        void start() override;
        void replyFinishedSubdomain(QNetworkReply *reply) override;
        void replyFinishedEmail(QNetworkReply *reply) override;
        void replyFinishedUrl(QNetworkReply *reply) override;

    private:
        bool m_firstRequest = false;
        int m_lastPage = 1;
        int m_sentPages = 1;
        void sendRequests(QString anotherPage);
};
#endif // DOGPILE_H
