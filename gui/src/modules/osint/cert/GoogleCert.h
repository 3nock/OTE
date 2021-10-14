#ifndef GOOGLECERT_H
#define GOOGLECERT_H

#include "../AbstractOsintModule.h"

namespace ModuleInfo {
    struct GoogleCert{
        QString name = "GoogleCert";
        QString url = "https://transparencyreport.google.com/";
        QString summary = "Sharing data that sheds light on how the policies and actions of governments and \n"
                          "corporations affect privacy, security, and access to information.";
        QMap<QString, QString> flags = {{"cert", "domain name"}};
    };
}

class GoogleCert: public AbstractOsintModule{

    public:
        GoogleCert(ScanArgs *args);
        ~GoogleCert() override;

    public slots:
        void start() override;
        void replyFinished(QNetworkReply *) override;
};

#endif // GOOGLECERT_H
