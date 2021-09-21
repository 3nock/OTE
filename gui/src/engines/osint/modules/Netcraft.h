#ifndef NETCRAFT_H
#define NETCRAFT_H

#include "AbstractModule.h"
#include "gumbo-parser/src/gumbo.h"

class Netcraft: public AbstractModule{

    public:
        Netcraft(QString target = nullptr, QObject *parent = nullptr);
        ~Netcraft() override;

    public slots:
        void start() override;
        void replyFinished(QNetworkReply *) override;

    private:
        void getSubdomains(GumboNode *node);
};

#endif // NETCRAFT_H
