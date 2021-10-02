#ifndef VIEWDNS_H
#define VIEWDNS_H

#include "src/engines/osint/modules/AbstractOsintModule.h"

class ViewDns: public AbstractOsintModule{

    public:
        ViewDns(QString target = nullptr, QObject *parent = nullptr);
        ~ViewDns() override;

    public slots:
        void start() override;
        void replyFinished(QNetworkReply *) override;

    private:
        QString m_key = nullptr;
};
#endif // VIEWDNS_H
