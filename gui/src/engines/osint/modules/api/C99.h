#ifndef C99_H
#define C99_H

#include "src/engines/osint/modules/AbstractOsintModule.h"

class C99: public AbstractOsintModule{

    public:
        C99(QString target = nullptr, QObject *parent = nullptr);
        ~C99() override;

    public slots:
        void start() override;
        void replyFinished(QNetworkReply *) override;

    private:
        QString m_key = nullptr;
};
#endif // C99_H
