#ifndef MNEMONIC_H
#define MNEMONIC_H

#include "../AbstractOsintModule.h"

class Mnemonic: public AbstractOsintModule{

    public:
        Mnemonic(ScanArgs *args);
        ~Mnemonic() override;

    public slots:
        void start() override;
        void replyFinished(QNetworkReply *) override;
};

#endif // MNEMONIC_H
