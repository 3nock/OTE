#ifndef ONYPHE_H
#define ONYPHE_H

#include "../AbstractOsintModule.h"


namespace ModuleInfo {
struct Onyphe{
    QString name = OSINT_MODULE_ONYPHE;
    QString url = "https://www.onyphe.io/";
    QString url_apiDoc = "https://www.onyphe.io/documentation/api";
    QString summary = "Cyber Defense Search Engine";

    QMap<QString, QStringList> flags = {{"user",
                                         {PLACEHOLDERTEXT_NONE, "Return information about your user account"}},
                                        {"summary ip",
                                         {PLACEHOLDERTEXT_IP, "It will return results about all categories of information we have for the given IPv{4,6} address."}},
                                        {"summary domain",
                                         {PLACEHOLDERTEXT_DOMAIN, "It will return results about all categories of information we have for the given IPv{4,6} address."}},
                                        {"summary hostname",
                                         {PLACEHOLDERTEXT_HOSTNAME, " It will return results about all categories of information we have for the given IPv{4,6} address."}},
                                        {"simple geoloc",
                                         {PLACEHOLDERTEXT_IP, "Return results about geoloc category of information"}},
                                        {"simple inetnum",
                                         {PLACEHOLDERTEXT_IP, "Return results about inetnum category of information"}},
                                        {"simple pastries",
                                         {PLACEHOLDERTEXT_IP, "Return results about pastries category of information"}},
                                        {"simple resolver",
                                         {PLACEHOLDERTEXT_IP, "It will return results about resolver category of information we have for the given IPv{4,6} address"}},
                                        {"simple sniffer",
                                         {PLACEHOLDERTEXT_IP, "It will return results about sniffer category of information we have for the given IPv{4,6} address with history of changes, if any"}},
                                        {"simple synscan",
                                         {PLACEHOLDERTEXT_IP, "It will return results about synscan category of information we have for the given IPv{4,6} address with history of changes, if any."}},
                                        {"simple threatlist",
                                         {PLACEHOLDERTEXT_IP, " It will return results about threatlist category of information we have for the given IPv{4,6} address with history of changes, if any."}},
                                        {"simple ctl",
                                         {PLACEHOLDERTEXT_HOSTNAME, "It will return results about ctl category of information we have for the given domain or hostname with history of changes, if any."}},
                                        {"simple datascan",
                                         {PLACEHOLDERTEXT_IP, "It will return results about datascan category of information we have for the given domain or hostname with history of changes, if any."}},
                                        {"simple datamd5",
                                         {PLACEHOLDERTEXT_MD5, "It will return results about datascan/datamd5 category of information we have for the given domain or hostname with history of changes, if any."}},
                                        {"simple forward",
                                         {PLACEHOLDERTEXT_IP, "It will return results about resolver category of information we have for the given domain or hostname with history of changes, if any."}},
                                        {"simple reverse",
                                         {PLACEHOLDERTEXT_IP, "It will return results about resolver category of information we have for the given domain or hostname with history of changes, if any."}},
                                        {"simple enterprise datashot",
                                         {PLACEHOLDERTEXT_IP, "It will return results about datashot category of information we have for the given IPv{4,6} address with history of changes, if any."}},
                                        {"simple enterprise onionshot",
                                         {PLACEHOLDERTEXT_IP, "It will return results about onionshot category of information we have for the given IPv{4,6} address with history of changes, if any."}},
                                        {"simple enterprise topsite",
                                         {PLACEHOLDERTEXT_IP, "It will return results about topsite category of information we have for the given IPv{4,6} address with history of changes, if any."}},
                                        {"simple enterprise vulnscan",
                                         {PLACEHOLDERTEXT_IP, "It will return results about vulnscan category of information we have for the given IPv{4,6} address with history of changes, if any."}},
                                        {"simple enterprise whois",
                                         {PLACEHOLDERTEXT_IP, "It will return results about whois category of information we have for the given IPv{4,6} address with history of changes, if any."}},
                                        {"simple enterprise onionscan",
                                         {PLACEHOLDERTEXT_IP, "It will return results about onionscan category of information we have for the given domain or hostname with history of changes, if any."}},
                                        {"simple best geoloc",
                                         {PLACEHOLDERTEXT_IP, "It will return one result about geoloc category of information we have for the given IPv{4,6} address. There will be no history of changes, "
                                                              "the goal of this API is to return the best matching subnet regarding the given address. Best matching subnet means the one with the smallest CIDR mask."}},
                                        {"simple best inetnum",
                                         {PLACEHOLDERTEXT_IP, " It will return one result about inetnum category of information we have for the given IPv{4,6} address. There will be no history of changes, "
                                                              "the goal of this API is to return the best matching subnet regarding the given address. Best matching subnet means the one with the smallest CIDR mask."}},
                                        {"simple best threatlist",
                                         {PLACEHOLDERTEXT_IP, " It will return 10 latest results about threatlist category of information we have for the given IPv{4,6} address. There will be no history of changes, "
                                                              "the goal of this API is to return latest malicious events for the given address that occured during the last 2-days."}},
                                        {"simple enterprise best whois",
                                         {PLACEHOLDERTEXT_IP, " It will return one result about whois category of information we have for the given IPv{4,6} address. There will be no history of changes, "
                                                              "the goal of this API is to return the best matching subnet regarding the given address. Best matching subnet means the one with the smallest CIDR mask."}}};

    QMap<int, QList<int>> input_output = {{IN_DOMAIN,
                                           {OUT_SUBDOMAIN, OUT_IP, OUT_SSLCERT}},
                                          {IN_IP,
                                           {OUT_SUBDOMAIN, OUT_SSLCERT}}};
};
}

class Onyphe: public AbstractOsintModule{

    public:
        explicit Onyphe(ScanArgs args);
        ~Onyphe() override;

    public slots:
        void start() override;
        void replyFinishedSubdomain(QNetworkReply *reply) override;
        void replyFinishedIp(QNetworkReply *reply) override;
        void replyFinishedSSL(QNetworkReply *reply) override;

    private:
        QString m_key;
};
#endif // ONYPHE_H
