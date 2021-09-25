#ifndef BGPVIEW_H
#define BGPVIEW_H

#include "src/engines/osint/modules/AbstractOsintModule.h"

class Bgpview: public AbstractOsintModule{

    public:
        Bgpview(QString target = nullptr, QObject *parent = nullptr);
        ~Bgpview() override;

    public slots:
        void start() override;
        void replyFinished(QNetworkReply *) override;
};

#endif // BGPVIEW_H
