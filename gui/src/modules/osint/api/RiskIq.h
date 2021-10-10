#ifndef RISKIQ_H
#define RISKIQ_H

#include "../AbstractOsintModule.h"

class RiskIq: public AbstractOsintModule{

    public:
        RiskIq(ScanArgs *args);
        ~RiskIq() override;

    public slots:
        void start() override;
        void replyFinished(QNetworkReply *) override;

    private:
        QString m_name = nullptr;
        QString m_key = nullptr;
};

#endif // RISKIQ_H
