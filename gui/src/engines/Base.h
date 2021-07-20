#ifndef BASECLASS_H
#define BASECLASS_H

#include "src/core.h"

class BaseClass : public QWidget{
        Q_OBJECT

    public:
        explicit BaseClass(QWidget *parent = nullptr);
        //...
        void contextMenu(ENGINE engineName, QPoint &pos);

    signals:
        void sendLog(QString log);
        void sendStatus(QString status);
        //...
        void sendToActive(ENGINE engineName);
        void sendToBrute(ENGINE engineName);
        void sendToRecord(ENGINE engineName);
        void sendToIp(ENGINE engineName);
        void sendToLevel(ENGINE engineName);
        void sendToSave(ENGINE engineName);
        void sendToProject(ENGINE engineName);
        //...
        void changeTabToOsint();
        void changeTabToBrute();
        void changeTabToActive();
        void changeTabToIp();
        void changeTabToRecord();
        void changeTabToLevel();
        void changeTabToProject();
        void changeTabToSave();

    public slots:
        void actionSendToActive(ENGINE engineName);
        void actionSendToLevel(ENGINE engineName);
        void actionSendToSave(ENGINE engineName);
        void actionSendToRecords(ENGINE engineName);
        void actionSendToIp(ENGINE engineName);
        //...
        void cursorSendToActive();
        void cursorSendToSave();
        void cursorSendToRecords();
        void cursorSendToIp();
};

#endif // BASECLASS_H
