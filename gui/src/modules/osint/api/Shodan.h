#ifndef SHODAN_H
#define SHODAN_H

#include "../AbstractOsintModule.h"

class Shodan: public AbstractOsintModule{

    public:
        Shodan(ScanArgs *args);
        ~Shodan() override;

    public slots:
        void start() override;
        void replyFinished(QNetworkReply *) override;

    private:
        QString m_key = nullptr;
};
#endif // SHODAN_H
