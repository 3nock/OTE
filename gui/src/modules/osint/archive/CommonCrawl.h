#ifndef COMMONCRAWL_H
#define COMMONCRAWL_H

#include "../AbstractOsintModule.h"

class CommonCrawl: public AbstractOsintModule{

    public:
        CommonCrawl(ScanArgs *args);
        ~CommonCrawl() override;

    public slots:
        void start() override;
        void replyFinished(QNetworkReply *) override;

    private:
        bool getArchive = true;
        QStringList urlList;
};
#endif // COMMONCRAWL_H
