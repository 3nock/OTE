#ifndef ARCHIVEIT_H
#define ARCHIVEIT_H

#include "../AbstractOsintModule.h"


namespace ModuleInfo {
struct ArchiveIt{
    QString name = "ArchiveIt";
    QString url = "https://archive-it.org/";
    QString url_apiDoc = "https://support.archive-it.org/hc/en-us/articles/360001231286-Archive-It-APIs-and-integrations";
    QString summary = "Archive-It provides tools, training, and technical support for capturing and preserving dynamic web materials, "
                      "as well as a platform for partners to share their collections, with multiple search, discovery, and access capabilities.";

    QMap<QString, QStringList> flags = {{"url",
                                         {PLACEHOLDERTEXT_URL, "Urls archived in ArchiveIt..."}}};

    QMap<int, QList<int>> input_output = {{IN_DOMAIN,
                                           {OUT_SUBDOMAIN, OUT_URL}}};
};
}


class ArchiveIt: public AbstractOsintModule{

    public:
        ArchiveIt(ScanArgs *args);
        ~ArchiveIt() override;

    public slots:
        void start() override;
        void replyFinishedUrl(QNetworkReply *reply) override;
        void replyFinishedSubdomain(QNetworkReply *reply) override;
};

#endif // ARCHIVEIT_H
