#ifndef SUBLIST3R_H
#define SUBLIST3R_H


#include "../AbstractOsintModule.h"

namespace ModuleInfo {
    struct Sublist3r{
        QString name = "Sublist3r";
        QString url = "https://api.sublist3r.com/";
        QString summary = "Sublist3r...";
        QMap<QString, QString> flags = {{"subdomains", "domain name"}};
    };
}

class Sublist3r: public AbstractOsintModule{

    public:
        Sublist3r(ScanArgs *args);
        ~Sublist3r() override;

    public slots:
        void start() override;
        void replyFinished(QNetworkReply *) override;
};

#endif // SUBLIST3R_H
