#ifndef SPYSE_H
#define SPYSE_H

#include "src/engines/osint/modules/AbstractOsintModule.h"

class Spyse: public AbstractOsintModule{

    public:
        Spyse(QString target = nullptr, QObject *parent = nullptr);
        ~Spyse() override;

    public slots:
        void start() override;
        void replyFinished(QNetworkReply *) override;
};

#endif // SPYSE_H
