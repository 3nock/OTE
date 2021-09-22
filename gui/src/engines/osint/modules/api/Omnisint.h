#ifndef OMNISINT_H
#define OMNISINT_H

#include "src/engines/osint/modules/AbstractOsintModule.h"

class Omnisint: public AbstractOsintModule{

    public:
        Omnisint(QString target = nullptr, QObject *parent = nullptr);
        ~Omnisint() override;

    public slots:
        void start() override;
        void replyFinished(QNetworkReply *) override;

    private:
        int m_page = 1;
};

#endif // OMNISINT_H
