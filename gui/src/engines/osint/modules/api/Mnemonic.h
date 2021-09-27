#ifndef MNEMONIC_H
#define MNEMONIC_H

#include "src/engines/osint/modules/AbstractOsintModule.h"

class Mnemonic: public AbstractOsintModule{

    public:
        Mnemonic(QString target = nullptr, QObject *parent = nullptr);
        ~Mnemonic() override;

    public slots:
        void start() override;
        void replyFinished(QNetworkReply *) override;
};

#endif // MNEMONIC_H
