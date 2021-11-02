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
        QString url_apiDoc = "https://api.riskiq.net/api/concepts.html";
        QString summary = "Relevant, actionable security intelligence to protect your attack surface";

        QMap<QString, QStringList> flags = {{"pdns ip",
                                             {PLACEHOLDERTEXT_IP, "Retrieve results by searching for specified IP address within PDNS record data field"}},
                                            {"pdns name",
                                             {PLACEHOLDERTEXT_DOMAIN, "Retrieve results by searching for specified resource name within PDNS record data field."}},
                                            {"pdns raw",
                                             {"raw hex", "Retrieve results by searching for specified hexadecimal bytes within PDNS record data field."}},
                                            {"whois address",
                                             {PLACEHOLDERTEXT_IP, "Get WHOIS records associated with an address"}},
                                            {"whois domain",
                                             {PLACEHOLDERTEXT_DOMAIN, "Get the current WHOIS for a domain"}},
                                            {"whois email",
                                             {PLACEHOLDERTEXT_EMAIL, "Get WHOIS records associated with an email address"}},
                                            {"whois name",
                                             {"name", "Get WHOIS records associated with a name"}},
                                            {"whois nameserver",
                                             {PLACEHOLDERTEXT_NS, "Get WHOIS records associated with a name server"}},
                                            {"whois org",
                                             {PLACEHOLDERTEXT_ORG, "Get WHOIS records associated with an organization"}},
                                            {"whois phone",
                                             {PLACEHOLDERTEXT_PHONE, "Get WHOIS records associated with a phone number"}},
                                            {"cert host",
                                             {PLACEHOLDERTEXT_HOSTNAME, "Get SSL certificates by host name"}},
                                            {"cert name",
                                             {"name", "Get SSL certificates by name"}},
                                            {"cert serial",
                                             {"serial", "Get SSL certificates by serial number"}},
                                            {"cert sha1",
                                             {PLACEHOLDERTEXT_SSLCERT, "Get SSL certificate by SHA1 hash"}},
                                            {"hosts",
                                             {PLACEHOLDERTEXT_SSLCERT, "Get hosts by certificate"}}
                                           };
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
