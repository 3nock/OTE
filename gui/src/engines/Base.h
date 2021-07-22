#ifndef BASECLASS_H
#define BASECLASS_H

#include "src/core.h"

class BaseClass : public QWidget{
        Q_OBJECT

    public:
        explicit BaseClass(QWidget *parent = nullptr);
        //...
        void contextMenu_actionButton(ENGINE engineName, QPoint &pos);
        void contextMenu_rightClick(QItemSelectionModel* selectionModel, QPoint &pos);

    signals:
        void sendLog(QString log);
        void sendStatus(QString status);
        //...
        void a_sendToOsint(ENGINE engineName);
        void a_sendToActive(ENGINE engineName);
        void a_sendToBrute(ENGINE engineName);
        void a_sendToRecord(ENGINE engineName);
        void a_sendToIp(ENGINE engineName);
        void a_sendToLevel(ENGINE engineName);
        void a_sendToSave(ENGINE engineName);
        void a_sendToProject(ENGINE engineName);
        //...
        void c_sendToOsint(QItemSelectionModel* selectionModel);
        void c_sendToActive(QItemSelectionModel* selectionModel);
        void c_sendToBrute(QItemSelectionModel* selectionModel);
        void c_sendToRecord(QItemSelectionModel* selectionModel);
        void c_sendToIp(QItemSelectionModel* selectionModel);
        void c_sendToLevel(QItemSelectionModel* selectionModel);
        void c_sendToSave(QItemSelectionModel* selectionModel);
        void c_sendToProject(QItemSelectionModel* selectionModel);
        //...
        void changeTabToOsint();
        void changeTabToBrute();
        void changeTabToActive();
        void changeTabToIp();
        void changeTabToRecords();
        void changeTabToLevel();
        void changeTabToProject();
        void changeTabToSave();

    public slots:
        void actionSendToOsint(ENGINE engineName);
        void actionSendToActive(ENGINE engineName);
        void actionSendToBrute(ENGINE engineName);
        void actionSendToLevel(ENGINE engineName);
        void actionSendToSave(ENGINE engineName);
        void actionSendToRecords(ENGINE engineName);
        void actionSendToIp(ENGINE engineName);
        //...
        void cursorOpenInBrowser(QItemSelectionModel *selectionModel);
        void cursorSendToOsint(QItemSelectionModel *selectionModel);
        void cursorSendToActive(QItemSelectionModel *selectionModel);
        void cursorSendToSave(QItemSelectionModel *selectionModel);
        void cursorSendToRecords(QItemSelectionModel *selectionModel);
        void cursorSendToIp(QItemSelectionModel *selectionModel);
        void cursorSendToBrute(QItemSelectionModel *selectionModel);
        void cursorSendToLevel(QItemSelectionModel *selectionModel);
};

#endif // BASECLASS_H
