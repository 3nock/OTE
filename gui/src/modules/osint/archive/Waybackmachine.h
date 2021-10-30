#ifndef WAYBACKMACHINE_H
#define WAYBACKMACHINE_H

#include "../AbstractOsintModule.h"

namespace ModuleInfo {
    struct Waybackmachine{
        QString name = "Waybackmachine";
        QString url = "https://archive.org/web/";
        QString url_apiDoc = "";
        QString summary = "web acrhive...";
        QMap<QString, QStringList> flags = {{"urls", {PLACEHOLDERTEXT_DOMAIN, ""}}};
    };
}

class Waybackmachine: public AbstractOsintModule{

    public:
        Waybackmachine(ScanArgs *args);
        ~Waybackmachine() override;

    public slots:
        void start() override;
        void replyFinished(QNetworkReply *) override;
};

#endif // WAYBACKMACHINE_H
