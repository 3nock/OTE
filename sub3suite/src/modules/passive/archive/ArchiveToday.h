#ifndef ARCHIVETODAY_H
#define ARCHIVETODAY_H

#include "../AbstractOsintModule.h"


namespace ModuleInfo {
struct ArchiveToday{
    QString name = "ArchiveToday";
    QString url = "https://archive.md/";
    QString url_apiDoc = "https://archive.md/";
    QString summary = "Archive.today is a time capsule for web pages! It takes a 'snapshot' of a webpage that will always be online even if the original page disappears."
                      "It saves a text and a graphical copy of the page for better accuracy and provides a short and reliable link to an unalterable record of any web page including those from Web 2.0 sites:";

    QMap<QString, QStringList> flags = {{"url",
                                         {PLACEHOLDERTEXT_URL, "Urls archived in Archive..."}}};

    QMap<int, QList<int>> input_output = {{IN_DOMAIN,
                                           {OUT_SUBDOMAIN, OUT_URL}}};
};
}

class ArchiveToday: public AbstractOsintModule{

    public:
        ArchiveToday(ScanArgs args);
        ~ArchiveToday() override;

    public slots:
        void start() override;
        void replyFinishedUrl(QNetworkReply *reply) override;
        void replyFinishedSubdomain(QNetworkReply *reply) override;
};

#endif // ARCHIVETODAY_H
