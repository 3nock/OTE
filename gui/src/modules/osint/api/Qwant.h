#ifndef QWANT_H
#define QWANT_H

#include "../AbstractOsintModule.h"

namespace ModuleInfo {
    struct Qwant{
        QString name = "Qwant";
        QString url = "https://www.qwant.com/";
        QString summary = "Qwant";
        QMap<QString, QString> flags = {{"web", "domain name"}};
    };
}

class Qwant: public AbstractOsintModule{

    public:
        Qwant(ScanArgs *args);
        ~Qwant() override;

    public slots:
        void start() override;
        void replyFinished(QNetworkReply *) override;

    private:
        int m_offset = 0;
};

#endif // QWANT_H
