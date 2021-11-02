#ifndef ARCHIVEIT_H
#define ARCHIVEIT_H

#include "../AbstractOsintModule.h"

namespace ModuleInfo {
    struct ArchiveIt{
        QString name = "ArchiveIt";
        QString url = "";
        QString url_apiDoc = "";
        QString summary = "ArchiveIt";
        QMap<QString, QStringList> flags = {{"url", {PLACEHOLDERTEXT_URL, "..."}}};
    };
}


class ArchiveIt: public AbstractOsintModule{

    public:
        ArchiveIt(ScanArgs *args);
        ~ArchiveIt() override;

    public slots:
        void start() override;
        void replyFinished(QNetworkReply *) override;
};

#endif // ARCHIVEIT_H
