#ifndef GITHUB_H
#define GITHUB_H

#include "../AbstractOsintModule.h"

namespace ModuleInfo {
    struct Github{
        QString name = "Github";
        QString url = "https://github.com/";
        QString summary = "Github API";
        QMap<QString, QString> flags = {{"domains", "domain name"}};
    };
}

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
