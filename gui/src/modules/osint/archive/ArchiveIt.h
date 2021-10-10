#ifndef ARCHIVEIT_H
#define ARCHIVEIT_H

#include "../AbstractOsintModule.h"

class ArchiveIt: public AbstractOsintModule{

    public:
        ArchiveIt(ScanArgs *args);
        ~ArchiveIt() override;

    public slots:
        void start() override;
        void replyFinished(QNetworkReply *) override;
};

#endif // ARCHIVEIT_H
