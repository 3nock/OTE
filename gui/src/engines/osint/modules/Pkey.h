#ifndef PKEY_H
#define PKEY_H

#include "AbstractModule.h"
#include "gumbo-parser/src/gumbo.h"

class Pkey: public AbstractModule{

    public:
        Pkey(QString target = nullptr, QObject *parent = nullptr);
        ~Pkey() override;

    public slots:
        void start() override;
        void replyFinished(QNetworkReply *) override;

    private:
        void getSubdomains(GumboNode *node);
};

#endif // PKEY_H
