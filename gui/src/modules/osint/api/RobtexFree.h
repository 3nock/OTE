#ifndef ROBTEXFREE_H
#define ROBTEXFREE_H

#include "../AbstractOsintModule.h"

/*
 * INPUT domain:            OUTPUT: subdomainIp, subdomain, ip
 * INPUT ip:                OUTPUT: subdomainIp, subdomain, ip, asn
 * INPUT asn:               OUTPUT: ip
 */

namespace ModuleInfo {
    struct RobtexFree{
        QString name = "Robtex";
        QString url = "https://www.robtex.com/";
        QString url_apiDoc = "";
        QString summary = "Robtex is used for various kinds of research of IP numbers, Domain names, etc";

        QMap<QString, QStringList> flags = {{"ipquery", {PLACEHOLDERTEXT_IP, ""}},
                                        {"asquery", {PLACEHOLDERTEXT_ASN, ""}},
                                        {"pdns forward", {PLACEHOLDERTEXT_DOMAIN, ""}},
                                        {"pdns reverse", {PLACEHOLDERTEXT_IP, ""}}};
    };
}

class RobtexFree: public AbstractOsintModule{

    public:
        RobtexFree(ScanArgs *args);
        ~RobtexFree() override;

    public slots:
        void start() override;
        void replyFinishedSubdomainIp(QNetworkReply *reply) override;
        void replyFinishedSubdomain(QNetworkReply *reply) override;
        void replyFinishedIp(QNetworkReply *reply) override;
        void replyFinishedAsn(QNetworkReply *reply) override;
};

#endif // ROBTEXFREE_H
