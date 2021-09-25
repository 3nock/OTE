#ifndef ARCHIVEIT_H
#define ARCHIVEIT_H

#include "src/engines/osint/modules/AbstractOsintModule.h"

class ArchiveIt: public AbstractOsintModule{

    public:
        ArchiveIt(QString target = nullptr, QObject *parent = nullptr);
        ~ArchiveIt() override;

    public slots:
        void start() override;
        void replyFinished(QNetworkReply *) override;
};

#endif // ARCHIVEIT_H
