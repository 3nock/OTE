#ifndef ROBTEX_H
#define ROBTEX_H

#include "../AbstractOsintModule.h"

class Robtex: public AbstractOsintModule{

    public:
        Robtex(ScanArgs *args);
        ~Robtex() override;

    public slots:
        void start() override;
        void replyFinished(QNetworkReply *) override;
};

#endif // ROBTEX_H
