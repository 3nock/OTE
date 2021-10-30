#ifndef GITHUB_H
#define GITHUB_H

#include "../AbstractOsintModule.h"

/*
 * INPUT domain:            OUPUT: subdomain
 */

namespace ModuleInfo {
    struct Github{
        QString name = "Github";
        QString url = "https://github.com/";
        QString url_apiDoc = "";
        QString summary = "Github API";
        QMap<QString, QStringList> flags = {{"code", {PLACEHOLDERTEXT_DOMAIN, ""}}};
    };
}

class Github: public AbstractOsintModule{

    public:
        Github(ScanArgs *args);
        ~Github() override;

    public slots:
        void start() override;
        void replyFinishedSubdomain(QNetworkReply *) override;

    private:
        QString m_key = nullptr;
};
#endif // GITHUB_H
