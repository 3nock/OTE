#ifndef THREATCROWD_H
#define THREATCROWD_H


#include "src/engines/osint/modules/AbstractOsintModule.h"

class Threatcrowd: public AbstractOsintModule{

    public:
        Threatcrowd(QString target = nullptr, QObject *parent = nullptr);
        ~Threatcrowd() override;

    public slots:
        void start() override;
        void replyFinished(QNetworkReply *) override;
};

#endif // THREATCROWD_H
