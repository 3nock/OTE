#ifndef ZOOMEYE_H
#define ZOOMEYE_H

#include "src/engines/osint/modules/AbstractOsintModule.h"

class ZoomEye: public AbstractOsintModule{

    public:
        ZoomEye(QString target = nullptr, QObject *parent = nullptr);
        ~ZoomEye() override;

    public slots:
        void start() override;
        void replyFinished(QNetworkReply *) override;

    private:
        QString m_key = nullptr;
};

#endif // ZOOMEYE_H
