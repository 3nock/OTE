#ifndef C99_H
#define C99_H

#include "../AbstractOsintModule.h"

class C99: public AbstractOsintModule{

    public:
        C99(ScanArgs *args);
        ~C99() override;

    public slots:
        void start() override;
        void replyFinished(QNetworkReply *) override;

    private:
        QString m_key = nullptr;
};
#endif // C99_H
