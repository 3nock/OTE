#ifndef WAYBACKMACHINE_H
#define WAYBACKMACHINE_H

#include "src/engines/osint/modules/AbstractOsintModule.h"

class Waybackmachine: public AbstractOsintModule{

    public:
        Waybackmachine(QString target = nullptr, QObject *parent = nullptr);
        ~Waybackmachine() override;

    public slots:
        void start() override;
        void replyFinished(QNetworkReply *) override;
};

#endif // WAYBACKMACHINE_H
