#ifndef WAYBACKMACHINE_H
#define WAYBACKMACHINE_H

#include "../AbstractOsintModule.h"

namespace ModuleInfo {
    struct Waybackmachine{
        QString name = "Waybackmachine";
        QString url = "https://archive.org/web/";
        QString summary = "web acrhive...";
        QMap<QString, QString> flags = {{"urls", "domain name"}};
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
