#ifndef WAYBACKMACHINE_H
#define WAYBACKMACHINE_H

#include "../AbstractOsintModule.h"

class Waybackmachine: public AbstractOsintModule{

    public:
        Waybackmachine(ScanArgs *args);
        ~Waybackmachine() override;

    public slots:
        void start() override;
        void replyFinished(QNetworkReply *) override;
};

#endif // WAYBACKMACHINE_H
