#ifndef THREATCROWD_H
#define THREATCROWD_H


#include "AbstractModule.h"

class Threatcrowd: public AbstractModule{

    public:
        Threatcrowd(QString target = nullptr, QObject *parent = nullptr);
        ~Threatcrowd() override;

    public slots:
        void start() override;
        void replyFinished(QNetworkReply *) override;
};

#endif // THREATCROWD_H
