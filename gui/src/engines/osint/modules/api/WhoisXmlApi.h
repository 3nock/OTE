#ifndef WHOISXMLAPI_H
#define WHOISXMLAPI_H

#include "src/engines/osint/modules/AbstractOsintModule.h"

class WhoisXmlApi: public AbstractOsintModule{

    public:
        WhoisXmlApi(QString target = nullptr, QObject *parent = nullptr);
        ~WhoisXmlApi() override;

    public slots:
        void start() override;
        void replyFinished(QNetworkReply *) override;

    private:
        QString m_key = nullptr;
};

#endif // WHOISXMLAPI_H
