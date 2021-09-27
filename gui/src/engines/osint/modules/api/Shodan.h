#ifndef SHODAN_H
#define SHODAN_H

#include "src/engines/osint/modules/AbstractOsintModule.h"

class Shodan: public AbstractOsintModule{

    public:
        Shodan(QString target = nullptr, QObject *parent = nullptr);
        ~Shodan() override;

    public slots:
        void start() override;
        void replyFinished(QNetworkReply *) override;

    private:
        QString m_key = nullptr;
};
#endif // SHODAN_H
