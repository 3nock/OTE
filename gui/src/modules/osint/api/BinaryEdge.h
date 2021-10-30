#ifndef BINARYEDGE_H
#define BINARYEDGE_H

#include "../AbstractOsintModule.h"

/*
 * INPUT ip:            OUTPUT: subdomainIp, subdomain, ip
 * INPUT cidr:          OUTPUT:
 * INPUT search term:   OUTPUT:
 * INPUT domain:        OUTPUT: subdomainIP, subdomain, ip
 */

namespace ModuleInfo {
    struct BinaryEdge{
        QString name = "BinaryEdge";
        QString url = "https://binaryedge.io/";
        QString url_apiDoc = "";
        QString summary = "We scan the entire public internet, create real-time threat intelligence streams,\n"
                          "and reports that show the exposure of what is connected to the Internet";

        QMap <QString, QStringList> flags = {{"host ip", {PLACEHOLDERTEXT_IP_OR_CIDR, ""}},
                                         {"host historical", {PLACEHOLDERTEXT_IP, ""}},
                                         {"host search", {PLACEHOLDERTEXT_QUERY, ""}},
                                         {"host search stats", {PLACEHOLDERTEXT_QUERY, ""}},
                                         {"domains subdomain", {PLACEHOLDERTEXT_DOMAIN, ""}},
                                         {"domains dns", {PLACEHOLDERTEXT_DOMAIN, ""}},
                                         {"domains ip", {PLACEHOLDERTEXT_DOMAIN, ""}},
                                         {"domains search", {PLACEHOLDERTEXT_QUERY, ""}},
                                         {"domains enumeration", {PLACEHOLDERTEXT_DOMAIN, ""}},
                                         {"domains homoglyphs", {PLACEHOLDERTEXT_DOMAIN, ""}}};
    };
}

class BinaryEdge: public AbstractOsintModule{

    public:
        BinaryEdge(ScanArgs *args);
        ~BinaryEdge() override;

    public slots:
        void start() override;
        void replyFinishedSubdomainIp(QNetworkReply *reply) override;
        void replyFinishedSubdomain(QNetworkReply *reply) override;
        void replyFinishedIp(QNetworkReply *reply) override;

    private:
        int m_page = 1;
        QString m_key = nullptr;
};

#endif // BINARYEDGE_H
