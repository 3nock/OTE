#ifndef OMNISINT_H
#define OMNISINT_H

#include "AbstractModule.h"

class Omnisint: public AbstractModule{

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
