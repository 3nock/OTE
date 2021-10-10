#ifndef GITHUB_H
#define GITHUB_H

#include "../AbstractOsintModule.h"

class Github: public AbstractOsintModule{

    public:
        Github(ScanArgs *args);
        ~Github() override;

    public slots:
        void start() override;
        void replyFinished(QNetworkReply *) override;

    private:
        QString m_key = nullptr;
};
#endif // GITHUB_H
