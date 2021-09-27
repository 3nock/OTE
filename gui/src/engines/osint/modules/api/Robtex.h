#ifndef ROBTEX_H
#define ROBTEX_H

#include "src/engines/osint/modules/AbstractOsintModule.h"

class Robtex: public AbstractOsintModule{

    public:
        Robtex(QString target = nullptr, QObject *parent = nullptr);
        ~Robtex() override;

    public slots:
        void start() override;
        void replyFinished(QNetworkReply *) override;
};

#endif // ROBTEX_H
