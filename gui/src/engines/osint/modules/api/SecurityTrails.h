#ifndef SECURITYTRAILS_H
#define SECURITYTRAILS_H

#include "src/engines/osint/modules/AbstractOsintModule.h"

class SecurityTrails: public AbstractOsintModule{

    public:
        SecurityTrails(QString target = nullptr, QObject *parent = nullptr);
        ~SecurityTrails() override;

    public slots:
        void start() override;
        void replyFinished(QNetworkReply *) override;

    private:
        QString m_key = nullptr;
};

#endif // SECURITYTRAILS_H
