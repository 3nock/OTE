#ifndef SUBLIST3R_H
#define SUBLIST3R_H


#include "../AbstractOsintModule.h"

class Sublist3r: public AbstractOsintModule{

    public:
        Sublist3r(ScanArgs *args);
        ~Sublist3r() override;

    public slots:
        void start() override;
        void replyFinished(QNetworkReply *) override;
};

#endif // SUBLIST3R_H
