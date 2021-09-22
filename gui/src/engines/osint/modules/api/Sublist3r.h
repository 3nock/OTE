#ifndef SUBLIST3R_H
#define SUBLIST3R_H


#include "src/engines/osint/modules/AbstractOsintModule.h"

class Sublist3r: public AbstractOsintModule{

    public:
        Sublist3r(QString target = nullptr, QObject *parent = nullptr);
        ~Sublist3r() override;

    public slots:
        void start() override;
        void replyFinished(QNetworkReply *) override;
};

#endif // SUBLIST3R_H
