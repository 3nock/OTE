#ifndef RISKIQ_H
#define RISKIQ_H

#include "../AbstractOsintModule.h"

/*
 * INPUT domain:            OUTPUT: subdomainIp, subdomain, ip, ssl
 * INPUT ip:                OUTPUT: subdomainIp, subdomain, ip, ssl
 * INPUT ssl:               OUTPUT: ip, ssl
 */

namespace ModuleInfo {
    struct RiskIq{
        QString name = "RiskIq";
        QString url = "https://www.riskiq.com/";
        QString url_apiDoc = "";
        QString summary = "Relevant, actionable security intelligence to protect your attack surface";

        QMap<QString, QStringList> flags = {{"pdns ip", {PLACEHOLDERTEXT_IP, ""}},
                                        {"pdns name", {PLACEHOLDERTEXT_DOMAIN, ""}},
                                        {"pdns raw", {"raw hex", ""}},
                                        {"whois address", {PLACEHOLDERTEXT_IP, ""}},
                                        {"whois domain", {PLACEHOLDERTEXT_DOMAIN, ""}},
                                        {"whois email", {PLACEHOLDERTEXT_EMAIL, ""}},
                                        {"whois name", {"name", ""}},
                                        {"whois nameserver", {PLACEHOLDERTEXT_NS, ""}},
                                        {"whois org", {PLACEHOLDERTEXT_ORG, ""}},
                                        {"whois phone", {PLACEHOLDERTEXT_PHONE, ""}},
                                        {"cert host", {PLACEHOLDERTEXT_HOSTNAME, ""}},
                                        {"cert name", {"name", ""}},
                                        {"cert serial", {"serial", ""}},
                                        {"cert sha1", {PLACEHOLDERTEXT_SSLCERT, ""}}};
    };
}

class RiskIq: public AbstractOsintModule{

    public:
        RiskIq(ScanArgs *args);
        ~RiskIq() override;

    public slots:
        void start() override;
        void replyFinishedSubdomainIp(QNetworkReply *reply) override;
        void replyFinishedSubdomain(QNetworkReply *reply) override;
        void replyFinishedIp(QNetworkReply *reply) override;
        void replyFinishedCertFingerprint(QNetworkReply *reply) override;

    private:
        QString m_name = nullptr;
        QString m_key = nullptr;
};

#endif // RISKIQ_H
