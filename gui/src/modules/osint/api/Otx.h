#ifndef OTX_H
#define OTX_H

#include "../AbstractOsintModule.h"

class Otx: public AbstractOsintModule{

    public:
        Otx(ScanArgs *args);
        ~Otx() override;

    public slots:
        void start() override;
        void replyFinished(QNetworkReply *) override;
};

#endif // OTX_H
