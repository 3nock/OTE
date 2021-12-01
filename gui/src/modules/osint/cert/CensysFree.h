#ifndef CENSYSFREE_H
#define CENSYSFREE_H

#include "../AbstractOsintModule.h"


namespace ModuleInfo {
    struct CensysFree{
        QString name = "Censys";
        QString url = "https://censys.io/";
        QString url_apiDoc = "https://censys.io/api/v1/docs/";
        QString summary = "Censys reduces your Internet attack surface by continually discovering unknown assets and helping remediate Internet facing risks";
        QMap<QString, QStringList> flags = {};

        QMap<int, QList<int>> input_output = {{IN_DOMAIN,
                                               {OUT_SUBDOMAIN, OUT_SSLCERT}},
                                              {IN_IP,
                                               {OUT_SUBDOMAIN, OUT_SSLCERT}},
                                              {IN_SSLCERT,
                                               {OUT_SUBDOMAIN, OUT_SSLCERT}}};
    };
}

class CensysFree: public AbstractOsintModule{

    public:
        CensysFree(ScanArgs *args);
        ~CensysFree() override;

    public slots:
        void start() override;
        void replyFinishedSubdomain(QNetworkReply *reply) override;
        void replyFinishedSSLCert(QNetworkReply *reply) override;
};

#endif // CENSYSFREE_H
