#ifndef RISKIQ_H
#define RISKIQ_H

#include "src/engines/osint/modules/AbstractOsintModule.h"

class RiskIq: public AbstractOsintModule{

    public:
        RiskIq(QString target = nullptr, QObject *parent = nullptr);
        ~RiskIq() override;

    public slots:
        void start() override;
        void replyFinished(QNetworkReply *) override;

    private:
        QString m_name = nullptr;
        QString m_key = nullptr;
};

#endif // RISKIQ_H
