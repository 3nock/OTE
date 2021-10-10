#ifndef VIEWDNS_H
#define VIEWDNS_H

#include "../AbstractOsintModule.h"

class ViewDns: public AbstractOsintModule{

    public:
        ViewDns(ScanArgs *args);
        ~ViewDns() override;

    public slots:
        void start() override;
        void replyFinished(QNetworkReply *) override;

    private:
        QString m_key = nullptr;
};
#endif // VIEWDNS_H
