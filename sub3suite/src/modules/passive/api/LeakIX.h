#ifndef LEAKIX_H
#define LEAKIX_H

#include "../AbstractOsintModule.h"

namespace ModuleInfo {
struct LeakIX{
    QString name = OSINT_MODULE_LEAKIX;
    QString url = "https://leakix.net/";
    QString url_apiDoc = "https://leakix.net/api-documentation";
    QString summary = "LeakIX is the first platform combining a search engine indexing public information AND an open reporting platform linked to the results.";

    QMap<QString, QStringList> flags = {{"Host",
                                         {PLACEHOLDERTEXT_IP, "All available data on the Host."}},
                                        {"Leak",
                                         {PLACEHOLDERTEXT_IP, "Leaks."}},
                                        {"Services",
                                         {PLACEHOLDERTEXT_IP, "Services."}}
                                       };

    QMap<int, QList<int>> input_output = {{IN_IP,
                                           {OUT_ASN, OUT_SUBDOMAIN, OUT_SSLCERT}}};
};
}

class LeakIX: public AbstractOsintModule{

    public:
        LeakIX(ScanArgs args);
        ~LeakIX() override;

    public slots:
        void start() override;
        void replyFinishedSubdomain(QNetworkReply *reply) override;
        void replyFinishedSSL(QNetworkReply *reply) override;
        void replyFinishedAsn(QNetworkReply *reply) override;

    private:
        QString m_key;
};

#endif // LEAKIX_H
