#ifndef GOOGLESEARCH_H
#define GOOGLESEARCH_H

#include "../AbstractOsintModule.h"


namespace ModuleInfo {
struct GoogleSearch{
    QString name = OSINT_MODULE_GOOGLESEARCH;
    QString url = "https://google.com/";
    QString url_apiDoc = "";
    QString summary = "";
    QMap<QString, QStringList> flags = {};
    QMap<int, QList<int>> input_output = {{IN_DOMAIN,
                                           {OUT_SUBDOMAIN, OUT_URL}},
                                          {IN_EMAIL,
                                           {OUT_URL}},
                                          {IN_QUERYTERM,
                                           {OUT_SUBDOMAIN, OUT_URL}}};
};
}

class GoogleSearch: public AbstractOsintModule{

    public:
        GoogleSearch(ScanArgs args);
        ~GoogleSearch() override;

    public slots:
        void start() override;
        void replyFinishedSubdomain(QNetworkReply *reply) override;
        void replyFinishedUrl(QNetworkReply *reply) override;

    private:
        int m_offset = 0;
};

#endif // GOOGLESEARCH_H
