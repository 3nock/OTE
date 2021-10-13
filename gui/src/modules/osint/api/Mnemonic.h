#ifndef MNEMONIC_H
#define MNEMONIC_H

#include "../AbstractOsintModule.h"

namespace ModuleInfo {
    struct Mnemonic{
        QString name = "Mnemonic";
        QString url = "https://www.mnemonic.no/";
        QString summary = "Mnemonic";
        QMap<QString, QString> flags = {{"ip", "ip-address"},
                                        {"domain", "domain"}};
    };
}

class Mnemonic: public AbstractOsintModule{

    public:
        Mnemonic(ScanArgs *args);
        ~Mnemonic() override;

    public slots:
        void start() override;
        void replyFinished(QNetworkReply *) override;
};

#endif // MNEMONIC_H
