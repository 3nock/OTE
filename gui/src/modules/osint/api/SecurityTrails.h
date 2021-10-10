#ifndef SECURITYTRAILS_H
#define SECURITYTRAILS_H

#include "../AbstractOsintModule.h"

class SecurityTrails: public AbstractOsintModule{

    public:
        SecurityTrails(ScanArgs *args);
        ~SecurityTrails() override;

    public slots:
        void start() override;
        void replyFinished(QNetworkReply *) override;

    private:
        QString m_key = nullptr;
};

#endif // SECURITYTRAILS_H
