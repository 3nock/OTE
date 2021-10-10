#ifndef QWANT_H
#define QWANT_H

#include "../AbstractOsintModule.h"

class Qwant: public AbstractOsintModule{

    public:
        Qwant(ScanArgs *args);
        ~Qwant() override;

    public slots:
        void start() override;
        void replyFinished(QNetworkReply *) override;

    private:
        int m_offset = 0;
};

#endif // QWANT_H
