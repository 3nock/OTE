#ifndef ASK_H
#define ASK_H

#include "../AbstractOsintModule.h"


class Ask: public AbstractOsintModule{

    public:
        Ask(ScanArgs *args);
        ~Ask() override;

    public slots:
        void start() override;
        void replyFinishedSubdomain(QNetworkReply *reply) override;
        void replyFinishedUrl(QNetworkReply *reply) override;

    private:
        int m_page = 0;
        bool firstReply = false;
};

#endif // ASK_H
