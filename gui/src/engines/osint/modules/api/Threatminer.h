#ifndef THREATMINER_H
#define THREATMINER_H


#include "src/engines/osint/modules/AbstractOsintModule.h"

class Threatminer: public AbstractOsintModule{

    public:
        Threatminer(QString target = nullptr, QObject *parent = nullptr);
        ~Threatminer() override;

    public slots:
        void start() override;
        void replyFinished(QNetworkReply *) override;
};

#endif // THREATMINER_H
