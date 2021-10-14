#ifndef ZOOMEYE_H
#define ZOOMEYE_H

#include "../AbstractOsintModule.h"

namespace ModuleInfo {
    struct ZoomEye{
        QString name = "ZoomEye";
        QString url = "https://www.zoomeye.org/";
        QString summary = "ZoomEye";
        QMap<QString, QString> flags = {{"domain", "domain name"}};
    };
}

class ZoomEye: public AbstractOsintModule{

    public:
        ZoomEye(ScanArgs *args);
        ~ZoomEye() override;

    public slots:
        void start() override;
        void replyFinished(QNetworkReply *) override;

    private:
        QString m_key = nullptr;
};

#endif // ZOOMEYE_H
