#ifndef QWANT_H
#define QWANT_H

#include "src/engines/osint/modules/AbstractOsintModule.h"

class Qwant: public AbstractOsintModule{

    public:
        Qwant(QString target = nullptr, QObject *parent = nullptr);
        ~Qwant() override;

    public slots:
        void start() override;
        void replyFinished(QNetworkReply *) override;

    private:
        int m_offset = 0;
};

#endif // QWANT_H
