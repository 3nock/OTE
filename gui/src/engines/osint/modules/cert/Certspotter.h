#ifndef CERTSPOTTER_H
#define CERTSPOTTER_H

#include "src/engines/osint/modules/AbstractOsintModule.h"

class Certspotter : public AbstractOsintModule {
    public:
        Certspotter(QString target = nullptr, QObject *parent = nullptr);
        ~Certspotter() override;

    public slots:
        void start() override;
        void replyFinished(QNetworkReply *reply) override;
};

#endif // CERTSPOTTER_H
