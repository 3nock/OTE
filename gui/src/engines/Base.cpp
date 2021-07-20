#include "Base.h"

BaseClass::BaseClass(QWidget *parent) : QWidget(parent)
{

}

/***************************************************************************
                            CONTEXT MENU
****************************************************************************/
void BaseClass::contextMenu(ENGINE engineName, QPoint &pos){
    Q_UNUSED(engineName);
    //...
    QMenu *contextMenu_actionButton = new QMenu(this);
    contextMenu_actionButton->setAttribute( Qt::WA_DeleteOnClose, true );
    contextMenu_actionButton->setObjectName("actionButtonMenu");
    //...
    QAction actionSendToIp("Send IpAddresses To Ip");
    QAction actionSendToActive("Send Subdomains To Active");
    QAction actionSendToBrute("Send Subdomains To Brute");
    QAction actionSendToSave("Send Subdomains To Save");
    QAction actionSendToLevel("Send Subdomains To Level");
    QAction actionSendToRecords("Send Subdomains To Records");
    //...
    connect(&actionSendToIp, SIGNAL(triggered()), this, SLOT(actionSendToIp(ENGINE::BRUTE)));
    connect(&actionSendToSave, SIGNAL(triggered()), this, SLOT(actionSendToSave(ENGINE::BRUTE)));
    connect(&actionSendToBrute, SIGNAL(triggered()), this, SLOT(actionSendToBrute(ENGINE::BRUTE)));
    connect(&actionSendToActive, SIGNAL(triggered()), this, SLOT(actionSendToActive(ENGINE::BRUTE)));
    connect(&actionSendToRecords, SIGNAL(triggered()), this, SLOT(actionSendToRecords(ENGINE::BRUTE)));
    connect(&actionSendToLevel, SIGNAL(triggered()), this, SLOT(actionSendToLevel(ENGINE::BRUTE)));
    //...
    contextMenu_actionButton->addAction(&actionSendToIp);
    contextMenu_actionButton->addAction(&actionSendToBrute);
    contextMenu_actionButton->addAction(&actionSendToActive);
    contextMenu_actionButton->addAction(&actionSendToRecords);
    contextMenu_actionButton->addAction(&actionSendToLevel);
    contextMenu_actionButton->addAction(&actionSendToSave);
    //...
    contextMenu_actionButton->move(QPoint(pos.x()+76, pos.y()));
    contextMenu_actionButton->exec();;
}

/****************************** Send All Results ***************************/
void BaseClass::actionSendToSave(ENGINE engineName){
    emit sendToSave(engineName);
}
void BaseClass::actionSendToRecords(ENGINE engineName){
    emit sendToRecord(engineName);
}
void BaseClass::actionSendToIp(ENGINE engineName){
    emit sendToIp(engineName);
}
void BaseClass::actionSendToActive(ENGINE engineName){
    emit sendToActive(engineName);
    emit changeTabToActive();
}
void BaseClass::actionSendToLevel(ENGINE engineName){
    emit sendToLevel(engineName);
}

/**************************** Send Selected Results *************************/
void BaseClass::cursorSendToActive(){

}
void BaseClass::cursorSendToSave(){

}
void BaseClass::cursorSendToIp(){

}
void BaseClass::cursorSendToRecords(){

}
