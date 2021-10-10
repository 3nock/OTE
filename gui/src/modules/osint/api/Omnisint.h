#ifndef OMNISINT_H
#define OMNISINT_H

#include "../AbstractOsintModule.h"

class Omnisint: public AbstractOsintModule{

    public:
        Omnisint(ScanArgs *args);
        ~Omnisint() override;

    public slots:
        void start() override;
        void replyFinished(QNetworkReply *) override;

    private:
        int m_page = 1;
};

#endif // OMNISINT_H
