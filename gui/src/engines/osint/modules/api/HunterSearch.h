#ifndef HUNTERSEARCH_H
#define HUNTERSEARCH_H

#include "src/engines/osint/modules/AbstractOsintModule.h"

class HunterSearch: public AbstractOsintModule{

    public:
        HunterSearch(QString target = nullptr, QObject *parent = nullptr);
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
