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
    QAction actionSendToIp("Send ip-addresses to Ip");
    QAction actionSendToOsint("Send subdomains to Osint");
    QAction actionSendToActive("Send subdomains to Active");
    QAction actionSendToBrute("Send subdomains to Brute");
    QAction actionSendToSave("Send subdomains to Save");
    QAction actionSendToLevel("Send subdomains to Level");
    QAction actionSendToRecords("Send subdomains to Records");
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
    contextMenu_actionButton->exec(QPoint(pos.x()+76, pos.y()));
}

void BaseClass::contextMenu_rightClick(QItemSelectionModel* selectionModel){
    //...
    QMenu *contextMenu_save = new QMenu(this);
    QMenu *contextMenu_copy = new QMenu(this);
    QMenu *contextMenu_main = new QMenu(this);
    //...
    contextMenu_save->setTitle("Save");
    contextMenu_copy->setTitle("Copy");
    //contextMenu_save->setAttribute( Qt::WA_DeleteOnClose, true );
    //contextMenu_copy->setAttribute( Qt::WA_DeleteOnClose, true );
    contextMenu_main->setAttribute( Qt::WA_DeleteOnClose, true );
    contextMenu_save->setObjectName("saveMenu");
    contextMenu_copy->setObjectName("saveMenu");
    contextMenu_main->setObjectName("rightClickMenu");
    ///
    /// ...
    ///
    QAction actionSaveAll("All");
    QAction actionSaveSubdomains("Subdomains");
    QAction actionSaveIpAddresses("Ip-addresses");
    //...
    QAction actionCopyAll("All");
    QAction actionCopySubdomains("Subdomains");
    QAction actionCopyIpAddresses("Ip-addresses");
    //...
    QAction actionOpenInBrowser("Open selected in Browser");
    QAction actionSendToOsint("Send selected to Osint");
    QAction actionSendToIp("Send selected to Ip");
    QAction actionSendToActive("Send selected to Active");
    QAction actionSendToBrute("Send selected to Brute");
    QAction actionSendToSave("Send selected to Save");
    QAction actionSendToLevel("Send selected to Level");
    QAction actionSendToRecords("Send selected to Records");
    ///
    /// ...
    ///
    connect(&actionSaveSubdomains, &QAction::triggered, this, [=](){this->cursorSave(selectionModel);});
    connect(&actionSaveIpAddresses, &QAction::triggered, this, [=](){this->cursorSave(selectionModel);});
    connect(&actionSaveAll, &QAction::triggered, this, [=](){this->cursorSave(selectionModel);});
    //...
    connect(&actionCopySubdomains, &QAction::triggered, this, [=](){this->cursorCopy(selectionModel);});
    connect(&actionCopyIpAddresses, &QAction::triggered, this, [=](){this->cursorCopy(selectionModel);});
    connect(&actionCopyAll, &QAction::triggered, this, [=](){this->cursorCopy(selectionModel);});
    //...
    connect(&actionOpenInBrowser, &QAction::triggered, this, [=](){this->cursorOpenInBrowser(selectionModel);});
    connect(&actionSendToOsint, &QAction::triggered, this, [=](){this->cursorSendToOsint(selectionModel);});
    connect(&actionSendToIp, &QAction::triggered, this, [=](){this->cursorSendToIp(selectionModel);});
    connect(&actionSendToSave, &QAction::triggered, this, [=](){this->cursorSendToSave(selectionModel);});
    connect(&actionSendToBrute, &QAction::triggered, this, [=](){this->cursorSendToBrute(selectionModel);});
    connect(&actionSendToActive, &QAction::triggered, this, [=](){this->cursorSendToActive(selectionModel);});
    connect(&actionSendToRecords, &QAction::triggered, this, [=](){this->cursorSendToRecords(selectionModel);});
    connect(&actionSendToLevel, &QAction::triggered, this, [=](){this->cursorSendToLevel(selectionModel);});
    ///
    /// ...
    ///
    contextMenu_save->addAction(&actionSaveAll);
    contextMenu_save->addAction(&actionSaveSubdomains);
    contextMenu_save->addAction(&actionSaveIpAddresses);
    //...
    contextMenu_copy->addAction(&actionCopyAll);
    contextMenu_copy->addAction(&actionCopySubdomains);
    contextMenu_copy->addAction(&actionCopyIpAddresses);
    //...
    contextMenu_main->addMenu(contextMenu_copy);
    contextMenu_main->addMenu(contextMenu_save);
    contextMenu_main->addSeparator();
    contextMenu_main->addAction(&actionOpenInBrowser);
    contextMenu_main->addSeparator();
    contextMenu_main->addAction(&actionSendToOsint);
    contextMenu_main->addAction(&actionSendToIp);
    contextMenu_main->addAction(&actionSendToBrute);
    contextMenu_main->addAction(&actionSendToActive);
    contextMenu_main->addAction(&actionSendToRecords);
    contextMenu_main->addAction(&actionSendToLevel);
    contextMenu_main->addAction(&actionSendToSave);
    ///
    /// ...
    ///
    contextMenu_main->exec(QCursor::pos());
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
///
/// COPY & SAVE...
///
void BaseClass::actionSave(ENGINE engineName){

}
void BaseClass::actionCopy(ENGINE engineName){

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
///
/// COPY & SAVE
///
void BaseClass::cursorSave(QItemSelectionModel *selectionModel){

}
void BaseClass::cursorCopy(QItemSelectionModel *selectionModel){

}
