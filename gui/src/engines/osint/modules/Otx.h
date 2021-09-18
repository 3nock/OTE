#ifndef OTX_H
#define OTX_H

#include "AbstractModule.h"

class Otx: public AbstractModule{

    public:
        Otx(QString target = nullptr, QObject *parent = nullptr);
        ~Otx() override;

    public slots:
        void start() override;
        void replyFinished(QNetworkReply *) override;
};

#endif // OTX_H
