#ifndef RISKIQ_H
#define RISKIQ_H

#include "../AbstractOsintModule.h"

namespace ModuleInfo {
    struct RiskIq{
        QString name = "RiskIq";
        QString url = "https://www.riskiq.com/";
        QString summary = "Relevant, actionable security intelligence to protect your attack surface";

        QMap<QString, QString> flags = {{"pdns ip", "ip-address"},
                                        {"pdns name", "domain name"},
                                        {"pdns raw", "raw hex"},
                                        {"whois address", "ip-address"},
                                        {"whois domain", "domain name"},
                                        {"whois email", "email"},
                                        {"whois name", "name"},
                                        {"whois nameserver", "nameserver address"},
                                        {"whois org", "organization name"},
                                        {"whois phone", "phone number"}};
    };
}

class RiskIq: public AbstractOsintModule{

    public:
        RiskIq(ScanArgs *args);
        ~RiskIq() override;

    public slots:
        void start() override;
        void replyFinished(QNetworkReply *) override;

    private:
        QString m_name = nullptr;
        QString m_key = nullptr;
};

#endif // RISKIQ_H
