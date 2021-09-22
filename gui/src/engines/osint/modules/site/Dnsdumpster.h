#ifndef DNSDUMPSTER_H
#define DNSDUMPSTER_H

#include "src/engines/osint/modules/AbstractOsintModule.h"
#include "gumbo-parser/src/gumbo.h"

class Dnsdumpster: public AbstractOsintModule{

    public:
        Dnsdumpster(QString target = nullptr, QObject *parent = nullptr);
        ~Dnsdumpster() override;

    public slots:
        void start() override;
        void replyFinished(QNetworkReply *) override;

    private:
        bool firstScanToGetToken = false;
        QString m_token;
        QMap<QString, QString> m_banner;
        void getToken(GumboNode *node);
        void getSubdomains(GumboNode *node);
};

#endif // DNSDUMPSTER_H
