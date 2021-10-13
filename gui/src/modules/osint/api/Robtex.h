#ifndef ROBTEX_H
#define ROBTEX_H

#include "../AbstractOsintModule.h"

namespace ModuleInfo {
    struct Robtex{
        QString name = "Robtex";
        QString url = "https://www.robtex.com/";
        QString summary = "Robtex is used for various kinds of research of IP numbers, Domain names, etc";

        QMap<QString, QString> flags = {{"ipquery", "ip-address"},
                                        {"asquery", "asn"},
                                        {"pdns forward", "ip-address"},
                                        {"pdns reverse", "ip-address"}};
    };
}

class Robtex: public AbstractOsintModule{

    public:
        Robtex(ScanArgs *args);
        ~Robtex() override;

    public slots:
        void start() override;
        void replyFinished(QNetworkReply *) override;
};

#endif // ROBTEX_H
