#ifndef CENSYSFREE_H
#define CENSYSFREE_H

#include "src/engines/osint/modules/AbstractOsintModule.h"
#include "gumbo-parser/src/gumbo.h"

class CensysFree: public AbstractOsintModule{

    public:
        CensysFree(QString target = nullptr, QObject *parent = nullptr);
        ~CensysFree() override;

    public slots:
        void start() override;
        void replyFinished(QNetworkReply *) override;

    private:
        void getSubdomains(GumboNode *node);
};

#endif // CENSYSFREE_H
