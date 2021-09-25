#ifndef COMMONCRAWL_H
#define COMMONCRAWL_H

#include "src/engines/osint/modules/AbstractOsintModule.h"

class CommonCrawl: public AbstractOsintModule{

    public:
        CommonCrawl(QString target = nullptr, QObject *parent = nullptr);
        ~CommonCrawl() override;

    public slots:
        void start() override;
        void replyFinished(QNetworkReply *) override;

    private:
        bool getArchive = true;
        QStringList urlList;
};
#endif // COMMONCRAWL_H
