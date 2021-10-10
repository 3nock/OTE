#ifndef BINARYEDGE_H
#define BINARYEDGE_H

#include "../AbstractOsintModule.h"

class BinaryEdge: public AbstractOsintModule{

    public:
        BinaryEdge(ScanArgs *args);
        ~BinaryEdge() override;

    public slots:
        void start() override;
        void replyFinished(QNetworkReply *) override;

    private:
        int m_page = 1;
        QString m_key = nullptr;
};

#endif // BINARYEDGE_H
