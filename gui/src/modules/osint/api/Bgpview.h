#ifndef BGPVIEW_H
#define BGPVIEW_H

#include "../AbstractOsintModule.h"

class Bgpview: public AbstractOsintModule{

    public:
        Bgpview(ScanArgs *args);
        ~Bgpview() override;

    public slots:
        void start() override;
        void replyFinished(QNetworkReply *) override;
};

#endif // BGPVIEW_H
