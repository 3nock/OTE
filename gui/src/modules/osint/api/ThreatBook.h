#ifndef THREATBOOK_H
#define THREATBOOK_H

#include "../AbstractOsintModule.h"

class ThreatBook: public AbstractOsintModule{

    public:
        ThreatBook(ScanArgs *args);
        ~ThreatBook() override;

    public slots:
        void start() override;
        void replyFinished(QNetworkReply *) override;

    private:
        QString m_key = nullptr;
};

#endif // THREATBOOK_H
