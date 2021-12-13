#ifndef ASK_H
#define ASK_H

#include "../AbstractOsintModule.h"


namespace ModuleInfo {
struct Ask{
    QString name = "Ask";
    QString url = "https://ask.com/";
    QString url_apiDoc = "";
    QString summary = "";
    QMap<QString, QStringList> flags = {};
    QMap<int, QList<int>> input_output = {{IN_DOMAIN,
                                           {OUT_SUBDOMAIN, OUT_URL, OUT_EMAIL}}};
};
}

class Ask: public AbstractOsintModule{

    public:
        Ask(ScanArgs args);
        ~Ask() override;

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

#endif // ASK_H
