#ifndef GITHUB_H
#define GITHUB_H

#include "src/engines/osint/modules/AbstractOsintModule.h"

class Github: public AbstractOsintModule{

    public:
        Github(QString target = nullptr, QObject *parent = nullptr);
        ~Github() override;

    public slots:
        void start() override;
        void replyFinished(QNetworkReply *) override;

    private:
        QString m_key = nullptr;
};
#endif // GITHUB_H
