#include "Base.h"

BaseClass::BaseClass(QWidget *parent) : QWidget(parent)
{

}

/***************************************************************************
                            CONTEXT MENU
****************************************************************************/
void BaseClass::contextMenu_actionButton(ENGINE engineName, QPoint &pos){
    Q_UNUSED(engineName);
    //...
    QMenu *contextMenu_actionButton = new QMenu(this);
    contextMenu_actionButton->setAttribute( Qt::WA_DeleteOnClose, true );
    contextMenu_actionButton->setObjectName("actionButtonMenu");
    //...
    QAction actionSendToIp("Send IpAddresses To Ip");
    QAction actionSendToOsint("Send Subdomains To Osint");
    QAction actionSendToActive("Send Subdomains To Active");
    QAction actionSendToBrute("Send Subdomains To Brute");
    QAction actionSendToSave("Send Subdomains To Save");
    QAction actionSendToLevel("Send Subdomains To Level");
    QAction actionSendToRecords("Send Subdomains To Records");
    //...
    connect(&actionSendToIp, &QAction::triggered, this, [=](){this->actionSendToIp(engineName);});
    connect(&actionSendToOsint, &QAction::triggered, this, [=](){this->actionSendToOsint(engineName);});
    connect(&actionSendToSave, &QAction::triggered, this, [=](){this->actionSendToSave(engineName);});
    connect(&actionSendToBrute, &QAction::triggered, this, [=](){this->actionSendToBrute(engineName);});
    connect(&actionSendToActive, &QAction::triggered, this, [=](){this->actionSendToActive(engineName);});
    connect(&actionSendToRecords, &QAction::triggered, this, [=](){this->actionSendToRecords(engineName);});
    connect(&actionSendToLevel, &QAction::triggered, this, [=](){this->actionSendToLevel(engineName);});
    //...
    contextMenu_actionButton->addAction(&actionSendToIp);
    contextMenu_actionButton->addSeparator();
    contextMenu_actionButton->addAction(&actionSendToOsint);
    contextMenu_actionButton->addAction(&actionSendToBrute);
    contextMenu_actionButton->addAction(&actionSendToActive);
    contextMenu_actionButton->addAction(&actionSendToRecords);
    contextMenu_actionButton->addAction(&actionSendToLevel);
    contextMenu_actionButton->addAction(&actionSendToSave);
    //...
    contextMenu_actionButton->move(QPoint(pos.x()+76, pos.y()));
    contextMenu_actionButton->exec();;
}

void BaseClass::contextMenu_rightClick(QItemSelectionModel* selectionModel, QPoint &pos){
    //...
    QMenu *contextMenu_rightClick = new QMenu(this);
    contextMenu_rightClick->setAttribute( Qt::WA_DeleteOnClose, true );
    contextMenu_rightClick->setObjectName("rightClickMenu");
    //...
    QAction actionOpenInBrowser("Open selected in Browser");
    QAction actionSendToOsint("Send Selection To Osint");
    QAction actionSendToIp("Send Selected To Ip");
    QAction actionSendToActive("Send selected To Active");
    QAction actionSendToBrute("Send selected To Brute");
    QAction actionSendToSave("Send selected To Save");
    QAction actionSendToLevel("Send selected To Level");
    QAction actionSendToRecords("Send selected To Records");
    //...
    connect(&actionOpenInBrowser, &QAction::triggered, this, [=](){this->cursorOpenInBrowser(selectionModel);});
    connect(&actionSendToOsint, &QAction::triggered, this, [=](){this->cursorSendToOsint(selectionModel);});
    connect(&actionSendToIp, &QAction::triggered, this, [=](){this->cursorSendToIp(selectionModel);});
    connect(&actionSendToSave, &QAction::triggered, this, [=](){this->cursorSendToSave(selectionModel);});
    connect(&actionSendToBrute, &QAction::triggered, this, [=](){this->cursorSendToBrute(selectionModel);});
    connect(&actionSendToActive, &QAction::triggered, this, [=](){this->cursorSendToActive(selectionModel);});
    connect(&actionSendToRecords, &QAction::triggered, this, [=](){this->cursorSendToRecords(selectionModel);});
    connect(&actionSendToLevel, &QAction::triggered, this, [=](){this->cursorSendToLevel(selectionModel);});
    //...
    contextMenu_rightClick->addAction(&actionOpenInBrowser);
    contextMenu_rightClick->addSeparator();
    contextMenu_rightClick->addAction(&actionSendToOsint);
    contextMenu_rightClick->addAction(&actionSendToIp);
    contextMenu_rightClick->addAction(&actionSendToBrute);
    contextMenu_rightClick->addAction(&actionSendToActive);
    contextMenu_rightClick->addAction(&actionSendToRecords);
    contextMenu_rightClick->addAction(&actionSendToLevel);
    contextMenu_rightClick->addAction(&actionSendToSave);
    //...
    contextMenu_rightClick->move(pos);
    contextMenu_rightClick->exec();
}

/****************************** Send All Results ***************************/
void BaseClass::actionSendToOsint(ENGINE engineName){
    emit a_sendToOsint(engineName);
    emit changeTabToOsint();
}
void BaseClass::actionSendToBrute(ENGINE engineName){
    emit a_sendToBrute(engineName);
    emit changeTabToBrute();
}
void BaseClass::actionSendToSave(ENGINE engineName){
    emit a_sendToSave(engineName);
    emit changeTabToSave();
}
void BaseClass::actionSendToRecords(ENGINE engineName){
    emit a_sendToRecord(engineName);
    emit changeTabToRecords();
}
void BaseClass::actionSendToIp(ENGINE engineName){
    emit a_sendToIp(engineName);
    emit changeTabToIp();
}
void BaseClass::actionSendToActive(ENGINE engineName){
    emit a_sendToActive(engineName);
    emit changeTabToActive();
}
void BaseClass::actionSendToLevel(ENGINE engineName){
    emit a_sendToLevel(engineName);
    emit changeTabToLevel();
}

/**************************** Send Selected Results *************************/

void BaseClass::cursorOpenInBrowser(QItemSelectionModel *selectionModel){
    // iterate and open each selected item in a browser...
    foreach(const QModelIndex &index, selectionModel->selectedIndexes()){
        QDesktopServices::openUrl(QUrl("https://"+index.data().toString(), QUrl::TolerantMode));
    }
}
//...
void BaseClass::cursorSendToOsint(QItemSelectionModel *selectionModel){
    emit c_sendToOsint(selectionModel);
    emit changeTabToOsint();
}
void BaseClass::cursorSendToActive(QItemSelectionModel *selectionModel){
    emit c_sendToActive(selectionModel);
    emit changeTabToActive();
}
void BaseClass::cursorSendToSave(QItemSelectionModel *selectionModel){
    emit c_sendToSave(selectionModel);
    emit changeTabToSave();
}
void BaseClass::cursorSendToRecords(QItemSelectionModel *selectionModel){
    emit c_sendToRecord(selectionModel);
    emit changeTabToRecords();
}
void BaseClass::cursorSendToIp(QItemSelectionModel *selectionModel){
    emit c_sendToIp(selectionModel);
    emit changeTabToIp();
}
void BaseClass::cursorSendToBrute(QItemSelectionModel *selectionModel){
    emit c_sendToBrute(selectionModel);
    emit changeTabToBrute();
}
void BaseClass::cursorSendToLevel(QItemSelectionModel *selectionModel){
    emit c_sendToLevel(selectionModel);
    emit changeTabToLevel();
}
