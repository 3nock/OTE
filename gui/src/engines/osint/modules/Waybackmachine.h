#ifndef WAYBACKMACHINE_H
#define WAYBACKMACHINE_H

#include "AbstractModule.h"

class Waybackmachine: public AbstractModule{

    public:
        Waybackmachine(QString target = nullptr, QObject *parent = nullptr);
        ~Waybackmachine() override;

    public slots:
        void start() override;
        void replyFinished(QNetworkReply *) override;
};

#endif // WAYBACKMACHINE_H
