#ifndef UKWEBARCHIVE_H
#define UKWEBARCHIVE_H

#include "../AbstractOsintModule.h"


namespace ModuleInfo {
struct UKWebArchive{
    QString name = OSINT_MODULE_UKWEBARCHIVE;
    QString url = "https://www.webarchive.org.uk/";
    QString url_apiDoc = "https://data.webarchive.org.uk/";
    QString summary = "The UK Web Archive (UKWA) collects millions of websites each year, preserving them for future generations. "
                      "Use this site to discover old or obsolete versions of UK websites, search the text of the websites and browse websites curated on different topics and themes.";

    QMap<QString, QStringList> flags = {{"url",
                                         {PLACEHOLDERTEXT_URL, "Urls archived in UKWebArchive..."}}};

    QMap<int, QList<int>> input_output = {{IN_DOMAIN,
                                           {OUT_SUBDOMAIN, OUT_URL}},
                                          {IN_URL,
                                           {OUT_SUBDOMAIN, OUT_URL}}};
};
}


class UKWebArchive: public AbstractOsintModule{

    public:
        explicit UKWebArchive(ScanArgs args);
        ~UKWebArchive() override;

    public slots:
        void start() override;
        void replyFinishedUrl(QNetworkReply *reply) override;
        void replyFinishedSubdomain(QNetworkReply *reply) override;
};

#endif // UKWEBARCHIVE_H
