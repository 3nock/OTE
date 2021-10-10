#ifndef THREATMINER_H
#define THREATMINER_H


#include "../AbstractOsintModule.h"

class Threatminer: public AbstractOsintModule{

    public:
        Threatminer(ScanArgs *args);
        ~Threatminer() override;

    public slots:
        void start() override;
        void replyFinished(QNetworkReply *) override;
};

#endif // THREATMINER_H
