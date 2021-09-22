#ifndef OTX_H
#define OTX_H

#include "src/engines/osint/modules/AbstractOsintModule.h"

class Otx: public AbstractOsintModule{

    public:
        Otx(QString target = nullptr, QObject *parent = nullptr);
        ~Otx() override;

    public slots:
        void start() override;
        void replyFinished(QNetworkReply *) override;
};

#endif // OTX_H
