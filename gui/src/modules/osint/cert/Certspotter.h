#ifndef CERTSPOTTER_H
#define CERTSPOTTER_H

#include "../AbstractOsintModule.h"

class Certspotter : public AbstractOsintModule {
    public:
        Certspotter(ScanArgs *args);
        ~Certspotter() override;

    public slots:
        void start() override;
        void replyFinished(QNetworkReply *reply) override;
};

#endif // CERTSPOTTER_H
