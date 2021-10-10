#ifndef SUIP_H
#define SUIP_H

#include "../AbstractOsintModule.h"
#include "gumbo-parser/src/gumbo.h"

class Suip: public AbstractOsintModule{

    public:
        Suip(ScanArgs *args);
        ~Suip() override;

    public slots:
        void start() override;
        void replyFinished(QNetworkReply *) override;

    private:
        void getSubdomains(GumboNode *node);
};

#endif // SUIP_H
