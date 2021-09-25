#ifndef BINARYEDGE_H
#define BINARYEDGE_H

#include "src/engines/osint/modules/AbstractOsintModule.h"

class BinaryEdge: public AbstractOsintModule{

    public:
        BinaryEdge(QString target = nullptr, QObject *parent = nullptr);
        ~BinaryEdge() override;

    public slots:
        void start() override;
        void replyFinished(QNetworkReply *) override;

    private:
        int m_page = 1;
        QString m_key = nullptr;
};

#endif // BINARYEDGE_H
