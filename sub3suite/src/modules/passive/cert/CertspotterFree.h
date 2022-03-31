#ifndef CERTSPOTTERFREE_H
#define CERTSPOTTERFREE_H

#include "../AbstractOsintModule.h"


namespace ModuleInfo {
struct CertspotterFree{
    QString name = "Certspotter";
    QString url = "https://sslmate.com/certspotter/";
    QString url_apiDoc = "https://sslmate.com/help/reference/ct_search_api_v1";
    QString summary = "SSLMate monitors public Certificate Transparency logs, coalescing all the certificates and precertificates for a particular issuance event into a single issuance object. "
                      "SSLMate indexes the issuances by DNS name, and you can use the API described below to efficiently query all the issuances for a particular domain, and then poll for new issuances";

    QMap<QString, QStringList> flags = {{"issuances",
                                         {PLACEHOLDERTEXT_DOMAIN, "List issuances"}}};

    QMap<int, QList<int>> input_output = {{IN_DOMAIN,
                                           {OUT_SUBDOMAIN, OUT_SSLCERT}}};
};
}

class CertspotterFree : public AbstractOsintModule {
    public:
        explicit CertspotterFree(ScanArgs args);
        ~CertspotterFree() override;

    public slots:
        void start() override;
        void replyFinishedSubdomain(QNetworkReply *reply) override;
        void replyFinishedSSL(QNetworkReply *reply) override;
};

#endif // CERTSPOTTERFREE_H
