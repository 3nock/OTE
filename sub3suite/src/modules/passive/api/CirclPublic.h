#ifndef CIRCLPUBLIC_H
#define CIRCLPUBLIC_H

#include "../AbstractOsintModule.h"


namespace ModuleInfo {
struct CirclPublic{
    QString name = OSINT_MODULE_CIRCLPUBLIC;
    QString url = "https://www.circl.lu/";
    QString url_apiDoc = "https://github.com/D4-project/IPASN-History";
    QString summary = "The Computer Incident Response Center Luxembourg (CIRCL) is a government-driven initiative "
                      "designed to provide a systematic response facility to computer security threats and incidents.";

    QMap<QString, QStringList> flags = {{"Ip 2 ASN",
                                         {PLACEHOLDERTEXT_IP, "IP ASN History to find ASN announcing an IP and the closest prefix announcing it at a specific date"}}};

    QMap<int, QList<int>> input_output = {{IN_IP,
                                           {OUT_CIDR, OUT_ASN}}};
};
}

class CirclPublic: public AbstractOsintModule{

    public:
        explicit CirclPublic(ScanArgs args);
        ~CirclPublic() override;

    public slots:
        void start() override;
        void replyFinishedCidr(QNetworkReply *reply) override;
        void replyFinishedAsn(QNetworkReply *reply) override;

    private:
        QString m_key;
};

#endif // CIRCLPUBLIC_H
