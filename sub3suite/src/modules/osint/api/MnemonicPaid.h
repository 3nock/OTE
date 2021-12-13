#ifndef MNEMONICPAID_H
#define MNEMONICPAID_H

#include "../AbstractOsintModule.h"


namespace ModuleInfo {
struct MnemonicPaid{
    QString name = "Mnemonic";
    QString url = "https://www.mnemonic.no/";
    QString url_apiDoc = "https://docs.mnemonic.no/";
    QString summary = "The mnemonic PassiveDNS service passively collects DNS queries from our sensor network around the globe."
                      "Parts of the collected data is private, while other data is collected from public collection points.";

    QMap<QString, QStringList> flags = {{"ip any record",
                                         {PLACEHOLDERTEXT_IP, "Returns ANY dns Records"}},
                                        {"pdns A",
                                         {PLACEHOLDERTEXT_DOMAIN, "Returns A dns Records"}},
                                        {"pdns AAAA",
                                         {PLACEHOLDERTEXT_DOMAIN, "Returns AAAA dns Records"}},
                                        {"pdns CNAME",
                                         {PLACEHOLDERTEXT_DOMAIN, "Returns CNAME dns Records"}},
                                        {"pdns MX",
                                         {PLACEHOLDERTEXT_DOMAIN, "Returns MX dns Records"}},
                                        {"pdns NS",
                                         {PLACEHOLDERTEXT_DOMAIN, "Returns NS dns Records"}}};

    QMap<int, QList<int>> input_output = {{IN_DOMAIN,
                                           {OUT_SUBDOMAIN, OUT_IP}},
                                          {IN_IP,
                                           {OUT_SUBDOMAIN, OUT_IP}}};
};
}

class MnemonicPaid: public AbstractOsintModule{

    public:
        MnemonicPaid(ScanArgs args);
        ~MnemonicPaid() override;

    public slots:
        void start() override;
        void replyFinishedIp(QNetworkReply *reply) override;
        void replyFinishedSubdomain(QNetworkReply *reply) override;

    private:
        QString m_key;
};

#endif // MNEMONICPAID_H
