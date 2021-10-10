#ifndef HUNTERSEARCH_H
#define HUNTERSEARCH_H

#include "../AbstractOsintModule.h"

class HunterSearch: public AbstractOsintModule{

    public:
        HunterSearch(ScanArgs *args);
        ~HunterSearch() override;
        //...
        bool verifyEmail(QString email);

    public slots:
        void start() override;
        void replyFinished(QNetworkReply *) override;

    private:
        QString m_key;
};

#endif // HUNTERSEARCH_H
