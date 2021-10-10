#ifndef THREATCROWD_H
#define THREATCROWD_H


#include "../AbstractOsintModule.h"

class Threatcrowd: public AbstractOsintModule{

    public:
        Threatcrowd(ScanArgs *args);
        ~Threatcrowd() override;

    public slots:
        void start() override;
        void replyFinished(QNetworkReply *) override;
};

#endif // THREATCROWD_H
