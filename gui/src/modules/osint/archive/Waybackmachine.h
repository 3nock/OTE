#ifndef WAYBACKMACHINE_H
#define WAYBACKMACHINE_H

#include "../AbstractOsintModule.h"


namespace ModuleInfo {
    struct Waybackmachine{
        QString name = "Waybackmachine";
        QString url = "https://archive.org/web/";
        QString url_apiDoc = "https://github.com/internetarchive/wayback/tree/master/wayback-cdx-server";
        QString summary = "The Internet Archive, a 501(c)(3) non-profit, is building a digital library of Internet sites and other cultural artifacts in digital form. "
                          "Like a paper library, we provide free access to researchers, historians, scholars, the print disabled, and the general public";

        QMap<QString, QStringList> flags = {{"MatchType Prefix",
                                             {PLACEHOLDERTEXT_DOMAIN, "will return results for all results under the path archive.org/about/"}},
                                            {"MatchType Exact",
                                             {PLACEHOLDERTEXT_DOMAIN, "will return results matching exactly archive.org/about/"}},
                                            {"MatchType Host",
                                             {PLACEHOLDERTEXT_DOMAIN, "will return results from host archive.org"}},
                                            {"MatchType Domain",
                                             {PLACEHOLDERTEXT_DOMAIN, "will return results from host archive.org and all subhosts *.archive.org"}}};

        QMap<int, QList<int>> input_output = {{IN_DOMAIN, {OUT_SUBDOMAIN, OUT_URL}}};
    };
}

class Waybackmachine: public AbstractOsintModule{

    public:
        Waybackmachine(ScanArgs *args);
        ~Waybackmachine() override;

    public slots:
        void start() override;
        void replyFinishedUrl(QNetworkReply *reply) override;
        void replyFinishedSubdomain(QNetworkReply *reply) override;
};

#endif // WAYBACKMACHINE_H
