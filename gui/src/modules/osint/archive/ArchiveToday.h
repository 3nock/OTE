#ifndef ARCHIVETODAY_H
#define ARCHIVETODAY_H

#include "../AbstractOsintModule.h"
#include "gumbo-parser/src/gumbo.h"

class ArchiveToday: public AbstractOsintModule{

    public:
        ArchiveToday(ScanArgs *args);
        ~ArchiveToday() override;

    public slots:
        void start() override;
        void replyFinished(QNetworkReply *) override;

    private:
        void getSubdomains(GumboNode* node);
        void getScreenshots(GumboNode* node);
};

#endif // ARCHIVETODAY_H
