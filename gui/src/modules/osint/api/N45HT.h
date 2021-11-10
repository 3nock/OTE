#ifndef N45HT_H
#define N45HT_H

#include "../AbstractOsintModule.h"

/*
 * INPUT hostname:          OUTPUT: subdomains
 */
namespace ModuleInfo {
    struct N45HT{
        QString name = "N45HT";
        QString url = "https://n45ht.or.id/";
        QString url_apiDoc = "https://n45ht.or.id/";
        QString summary = "n45ht";
        QMap<QString, QStringList> flags = {{"subdomain",
                                             {PLACEHOLDERTEXT_DOMAIN, "Returns Subdomains of a particular domain"}}};
    };
}

class N45HT: public AbstractOsintModule{

    public:
        N45HT(ScanArgs *args);
        ~N45HT() override;

    public slots:
        void start() override;
        void replyFinishedSubdomain(QNetworkReply *reply) override;
};
#endif // N45HT_H
