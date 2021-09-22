#ifndef SUIP_H
#define SUIP_H

#include "src/engines/osint/modules/AbstractOsintModule.h"
#include "gumbo-parser/src/gumbo.h"

class Suip: public AbstractOsintModule{

    public:
        Suip(QString target = nullptr, QObject *parent = nullptr);
        ~Suip() override;

    public slots:
        void start() override;
        void replyFinished(QNetworkReply *) override;

    private:
        void getSubdomains(GumboNode *node);
};

#endif // SUIP_H
