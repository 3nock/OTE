#ifndef WHOISXMLAPI_H
#define WHOISXMLAPI_H

#include "../AbstractOsintModule.h"

class WhoisXmlApi: public AbstractOsintModule{

    public:
        WhoisXmlApi(ScanArgs *args);
        ~WhoisXmlApi() override;

    public slots:
        void start() override;
        void replyFinished(QNetworkReply *) override;

    private:
        QString m_key = nullptr;
};

#endif // WHOISXMLAPI_H
