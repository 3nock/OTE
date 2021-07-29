#include "Base.h"

BaseClass::BaseClass(QWidget *parent, ResultsModel *resultsModel) : QWidget(parent),
    m_resultsModel(resultsModel),
    widgets(new Widgets)
{

}
BaseClass::~BaseClass(){
    delete widgets;
}

void BaseClass::logs(QString log){
    ///
    /// send the log to the main logger class...
    ///
    sendLog(log);
    ///
    /// adding the log to local log widget and setting color depending on error...
    ///
    widgets->listWidget_logs->addItem(log);
    if(log.startsWith("[ERROR]") || log.startsWith("[Error]"))
    {
        widgets->listWidget_logs->item(widgets->listWidget_logs->count()-1)->setForeground(Qt::red);
        return;
    }
    if(log.startsWith("[START]") || log.startsWith("[END]"))
    {
        widgets->listWidget_logs->item(widgets->listWidget_logs->count()-1)->setFont(QFont("MS Shell Dlg 2", 8, QFont::Bold));
        return;
    }
}

/***************************************************************************
                        WIDGETS - TARGETS
****************************************************************************/
void BaseClass::addTargets(){
    ///
    /// check...
    ///
    if(widgets->lineEdit_targetInput->text() == EMPTY){
        return;
    }
    ///
    /// adding target from lineEdit to widget and to set...
    ///
    QString item = widgets->lineEdit_targetInput->text();
    if(!m_targetSet.contains(item)){
        m_targetSet.insert(widgets->lineEdit_targetInput->text());
        widgets->listWidget_targets->addItem(item);
    }
    widgets->lineEdit_targetInput->clear();
    widgets->label_targetsCount->setNum(widgets->listWidget_targets->count());
}

void BaseClass::clearTargets(){
    widgets->listWidget_targets->clear();
    widgets->label_targetsCount->clear();
    m_targetSet.clear();
}

void BaseClass::removeTargets(){
    ///
    /// check...
    ///
    if(!widgets->listWidget_targets->selectedItems().count()){
        return;
    }
    ///
    /// iterating and deleting every selected item from widget and from set...
    ///
    foreach(QListWidgetItem *item, widgets->listWidget_targets->selectedItems())
    {
        m_targetSet.remove(item->text());
        widgets->listWidget_targets->removeItemWidget(item);
        delete item;
    }
    widgets->label_targetsCount->setNum(widgets->listWidget_targets->count());
}

void BaseClass::loadTargetsFromFile(){
    ///
    /// check...
    ///
    QString filename = QFileDialog::getOpenFileName(this, INFO_LOADFILE, CURRENT_PATH);
    if(filename.isEmpty()){
        return;
    }
    QFile file(filename);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        QMessageBox::warning(this, TITLE_ERROR, "Failed To Open the File!");
        return;
    }
    ///
    /// loading the file contents...
    ///
    QString item;
    QTextStream in(&file);
    while (!in.atEnd())
    {
        item = in.readLine();
        if(!m_targetSet.contains(item)){
            m_targetSet.insert(item);
            widgets->listWidget_targets->addItem(item);
        }
    }
    widgets->label_targetsCount->setNum(widgets->listWidget_targets->count());
    file.close();
}


/***************************************************************************
                            CONTEXT MENU
****************************************************************************/
void BaseClass::contextMenu_actionButton(ENGINE engineName, QPoint &pos){
    QMenu *contextMenu_save = new QMenu(this);
    QMenu *contextMenu_copy = new QMenu(this);
    QMenu *contextMenu_main = new QMenu(this);
    ///
    /// ...
    ///
    contextMenu_main->setAttribute( Qt::WA_DeleteOnClose, true );
    //...
    contextMenu_save->setTitle("Save");
    contextMenu_copy->setTitle("Copy");
    //...
    contextMenu_save->setObjectName("saveMenu");
    contextMenu_copy->setObjectName("saveMenu");
    contextMenu_main->setObjectName("rightClickMenu");
    ///
    /// ...
    ///
    QAction actionSaveAll("subdomain | ip");
    QAction actionSaveSubdomains("subdomains");
    QAction actionSaveIpAddresses("ip-addresses");
    // for dns-records...
    QAction actionSaveA("A Records");
    QAction actionSaveAAAA("AAAA Records");
    QAction actionSaveMX("MX Records");
    QAction actionSaveNS("NS Records");
    QAction actionSaveCNAME("CNAME Records");
    QAction actionSaveTXT("TXT Records");
    QAction actionSaveSRVTarget("SRV Targets");
    QAction actionSaveSRVName("SRV Names");
    //...
    QAction actionCopyAll("subdomain | ip");
    QAction actionCopySubdomains("subdomains");
    QAction actionCopyIpAddresses("ip-addresses");
    // for dns-records...
    QAction actionCopyA("A Records");
    QAction actionCopyAAAA("AAAA Records");
    QAction actionCopyMX("MX Records");
    QAction actionCopyNS("NS Records");
    QAction actionCopyCNAME("CNAME Records");
    QAction actionCopyTXT("TXT Records");
    QAction actionCopySRVTarget("SRV Targets");
    QAction actionCopySRVName("SRV Names");
    //...
    QAction actionSendToIp("Send ip-addresses to Ip");
    QAction actionSendToOsint("Send subdomains to Osint");
    QAction actionSendToActive("Send subdomains to Active");
    QAction actionSendToBrute("Send subdomains to Brute");
    QAction actionSendToLevel("Send subdomains to Level");
    QAction actionSendToRecords("Send subdomains to Records");
    ///
    /// SAVE...
    ///
    connect(&actionSaveSubdomains, &QAction::triggered, this, [=](){this->actionSave(engineName, CHOICE::susbdomains);});
    connect(&actionSaveIpAddresses, &QAction::triggered, this, [=](){this->actionSave(engineName, CHOICE::ipaddress);});
    connect(&actionSaveAll, &QAction::triggered, this, [=](){this->actionSave(engineName, CHOICE::all);});
    //...
    connect(&actionSaveA, &QAction::triggered, this, [=](){this->actionSave(engineName, CHOICE::A);});
    connect(&actionSaveAAAA, &QAction::triggered, this, [=](){this->actionSave(engineName, CHOICE::AAAA);});
    connect(&actionSaveMX, &QAction::triggered, this, [=](){this->actionSave(engineName, CHOICE::MX);});
    connect(&actionSaveNS, &QAction::triggered, this, [=](){this->actionSave(engineName, CHOICE::NS);});
    connect(&actionSaveCNAME, &QAction::triggered, this, [=](){this->actionSave(engineName, CHOICE::CNAME);});
    connect(&actionSaveTXT, &QAction::triggered, this, [=](){this->actionSave(engineName, CHOICE::TXT);});
    //...
    connect(&actionSaveSRVName, &QAction::triggered, this, [=](){this->actionSave(engineName, CHOICE::srvName);});
    connect(&actionSaveSRVTarget, &QAction::triggered, this, [=](){this->actionSave(engineName, CHOICE::srvTarget);});
    ///
    /// COPY...
    ///
    connect(&actionCopySubdomains, &QAction::triggered, this, [=](){this->actionCopy(engineName, CHOICE::susbdomains);});
    connect(&actionCopyIpAddresses, &QAction::triggered, this, [=](){this->actionCopy(engineName, CHOICE::ipaddress);});
    connect(&actionCopyAll, &QAction::triggered, this, [=](){this->actionCopy(engineName, CHOICE::all);});
    //...
    connect(&actionCopyA, &QAction::triggered, this, [=](){this->actionCopy(engineName, CHOICE::A);});
    connect(&actionCopyAAAA, &QAction::triggered, this, [=](){this->actionCopy(engineName, CHOICE::AAAA);});
    connect(&actionCopyMX, &QAction::triggered, this, [=](){this->actionCopy(engineName, CHOICE::MX);});
    connect(&actionCopyNS, &QAction::triggered, this, [=](){this->actionCopy(engineName, CHOICE::NS);});
    connect(&actionCopyCNAME, &QAction::triggered, this, [=](){this->actionCopy(engineName, CHOICE::CNAME);});
    connect(&actionCopyTXT, &QAction::triggered, this, [=](){this->actionCopy(engineName, CHOICE::TXT);});
    //...
    connect(&actionCopySRVName, &QAction::triggered, this, [=](){this->actionCopy(engineName, CHOICE::srvName);});
    connect(&actionCopySRVTarget, &QAction::triggered, this, [=](){this->actionCopy(engineName, CHOICE::srvTarget);});
    ///
    /// SUBDOMAINS AND IPS...
    ///
    connect(&actionSendToIp, &QAction::triggered, this, [=](){this->actionSendToIp(engineName, CHOICE::ipaddress);});
    connect(&actionSendToOsint, &QAction::triggered, this, [=](){this->actionSendToOsint(engineName, CHOICE::susbdomains);});
    connect(&actionSendToBrute, &QAction::triggered, this, [=](){this->actionSendToBrute(engineName, CHOICE::susbdomains);});
    connect(&actionSendToActive, &QAction::triggered, this, [=](){this->actionSendToActive(engineName, CHOICE::susbdomains);});
    connect(&actionSendToRecords, &QAction::triggered, this, [=](){this->actionSendToRecords(engineName, CHOICE::susbdomains);});
    connect(&actionSendToLevel, &QAction::triggered, this, [=](){this->actionSendToLevel(engineName, CHOICE::susbdomains);});
    ///
    /// ADDING ACTIONS TO THE CONTEXT MENU...
    ///
    if(engineName == ENGINE::DNSRECORDS){
        contextMenu_save->addSeparator();
        contextMenu_save->addAction(&actionSaveA);
        contextMenu_save->addAction(&actionSaveAAAA);
        contextMenu_save->addAction(&actionSaveNS);
        contextMenu_save->addAction(&actionSaveMX);
        contextMenu_save->addAction(&actionSaveTXT);
        contextMenu_save->addAction(&actionSaveCNAME);
        //...
        contextMenu_copy->addSeparator();
        contextMenu_copy->addAction(&actionCopyA);
        contextMenu_copy->addAction(&actionCopyAAAA);
        contextMenu_copy->addAction(&actionCopyNS);
        contextMenu_copy->addAction(&actionCopyMX);
        contextMenu_copy->addAction(&actionCopyTXT);
        contextMenu_copy->addAction(&actionCopyCNAME);
    }
    if(engineName == ENGINE::OSINT || engineName == ENGINE::BRUTE || engineName == ENGINE::ACTIVE || engineName == ENGINE::LEVEL || engineName == ENGINE::IP){
        contextMenu_save->addAction(&actionSaveSubdomains);
        contextMenu_save->addAction(&actionSaveIpAddresses);
        contextMenu_save->addAction(&actionSaveAll);
        //...
        contextMenu_copy->addAction(&actionCopySubdomains);
        contextMenu_copy->addAction(&actionCopyIpAddresses);
        contextMenu_copy->addAction(&actionCopyAll);
    }
    if(engineName == ENGINE::SRVRECORDS){
        contextMenu_save->addAction(&actionSaveSRVName);
        contextMenu_save->addAction(&actionSaveSRVTarget);
        //...
        contextMenu_copy->addAction(&actionCopySRVName);
        contextMenu_copy->addAction(&actionCopySRVTarget);
    }
    contextMenu_main->addMenu(contextMenu_copy);
    contextMenu_main->addMenu(contextMenu_save);
    //...
    contextMenu_main->addSeparator();
    if(engineName != ENGINE::SRVRECORDS){
        contextMenu_main->addAction(&actionSendToIp);
    }
    //...
    contextMenu_main->addSeparator();
    contextMenu_main->addAction(&actionSendToOsint);
    contextMenu_main->addAction(&actionSendToBrute);
    contextMenu_main->addAction(&actionSendToActive);
    contextMenu_main->addAction(&actionSendToRecords);
    contextMenu_main->addAction(&actionSendToLevel);
    //...
    contextMenu_main->exec(QPoint(pos.x()+76, pos.y()));
}

void BaseClass::contextMenu_rightClick(QItemSelectionModel* selectionModel){
    QMenu *contextMenu_main = new QMenu(this);
    contextMenu_main->setAttribute( Qt::WA_DeleteOnClose, true );
    contextMenu_main->setObjectName("rightClickMenu");
    ///
    /// ...
    ///
    QAction actionSave("Save");
    QAction actionCopy("Copy");
    //...
    QAction actionOpenInBrowser("Open selected in Browser");
    QAction actionSendToOsint("Send selected to Osint");
    QAction actionSendToIp("Send selected to Ip");
    QAction actionSendToActive("Send selected to Active");
    QAction actionSendToBrute("Send selected to Brute");
    QAction actionSendToLevel("Send selected to Level");
    QAction actionSendToRecords("Send selected to Records");
    ///
    /// ...
    ///
    connect(&actionSave, &QAction::triggered, this, [=](){this->cursorSave(selectionModel);});
    connect(&actionCopy, &QAction::triggered, this, [=](){this->cursorCopy(selectionModel);});
    //...
    connect(&actionOpenInBrowser, &QAction::triggered, this, [=](){this->cursorOpenInBrowser(selectionModel);});
    //...
    connect(&actionSendToOsint, &QAction::triggered, this, [=](){this->cursorSendToOsint(selectionModel);});
    connect(&actionSendToIp, &QAction::triggered, this, [=](){this->cursorSendToIp(selectionModel);});
    connect(&actionSendToBrute, &QAction::triggered, this, [=](){this->cursorSendToBrute(selectionModel);});
    connect(&actionSendToActive, &QAction::triggered, this, [=](){this->cursorSendToActive(selectionModel);});
    connect(&actionSendToRecords, &QAction::triggered, this, [=](){this->cursorSendToRecords(selectionModel);});
    connect(&actionSendToLevel, &QAction::triggered, this, [=](){this->cursorSendToLevel(selectionModel);});
    ///
    /// ...
    ///
    contextMenu_main->addAction(&actionCopy);
    contextMenu_main->addAction(&actionSave);
    contextMenu_main->addSeparator();
    contextMenu_main->addAction(&actionOpenInBrowser);
    contextMenu_main->addSeparator();
    contextMenu_main->addAction(&actionSendToIp);
    contextMenu_main->addAction(&actionSendToOsint);
    contextMenu_main->addAction(&actionSendToBrute);
    contextMenu_main->addAction(&actionSendToActive);
    contextMenu_main->addAction(&actionSendToRecords);
    contextMenu_main->addAction(&actionSendToLevel);
    ///
    /// ...
    ///
    contextMenu_main->exec(QCursor::pos());
}

/**************************** Open in Browser **************************/
void BaseClass::cursorOpenInBrowser(QItemSelectionModel *selectionModel){
    QSet<QString> itemSet;
    QString item;
    ///
    /// ...
    ///
    foreach(const QModelIndex &index, selectionModel->selectedIndexes())
    {
        item = index.data().toString();
        if(!itemSet.contains(item) && item != "A" && item != "AAAA" && item != "NS" && item != "MX" && item != "CNAME" && item != "TXT"){
            itemSet.insert(item);
            QDesktopServices::openUrl(QUrl("https://"+item, QUrl::TolerantMode));
        }
    }
}

/***************************** Send Results *****************************/
void BaseClass::actionSendToOsint(ENGINE engineName, CHOICE choice){
    emit a_sendToOsint(engineName, choice);
    emit changeTabToOsint();
}
void BaseClass::actionSendToBrute(ENGINE engineName, CHOICE choice){
    emit a_sendToBrute(engineName, choice);
    emit changeTabToBrute();
}
void BaseClass::actionSendToRecords(ENGINE engineName, CHOICE choice){
    emit a_sendToRecord(engineName, choice);
    emit changeTabToRecords();
}
void BaseClass::actionSendToIp(ENGINE engineName, CHOICE choice){
    emit a_sendToIp(engineName, choice);
    emit changeTabToIp();
}
void BaseClass::actionSendToActive(ENGINE engineName, CHOICE choice){
    emit a_sendToActive(engineName, choice);
    emit changeTabToActive();
}
void BaseClass::actionSendToLevel(ENGINE engineName, CHOICE choice){
    emit a_sendToLevel(engineName, choice);
    emit changeTabToLevel();
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

/********************************** COPY & SAVE ************************************/

void BaseClass::actionSave(ENGINE engineName, CHOICE choice){
    ///
    /// checks...
    ///
    QString filename = QFileDialog::getSaveFileName(this, INFO_LOADFILE, CURRENT_PATH);
    if(filename.isEmpty()){
        return;
    }
    QFile file(filename);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    if(!file.isOpen()){
        return;
    }
    ///
    /// variable declarations...
    ///
    QStandardItemModel *model = nullptr;
    QSet<QString> itemSet;
    QString item;
    ///
    /// Engine the results are from...
    ///
    switch(engineName){
    case ENGINE::IP:
        model = m_resultsModel->ip;
        break;
    case ENGINE::BRUTE:
        model = m_resultsModel->brute;
        break;
    case ENGINE::LEVEL:
        model = m_resultsModel->level;
        break;
    case ENGINE::OSINT:
        model = m_resultsModel->osint;
        break;
    case ENGINE::ACTIVE:
        model = m_resultsModel->active;
        break;
    case ENGINE::DNSRECORDS:
        model = m_resultsModel->records->model_records;
        break;
    case ENGINE::SRVRECORDS:
        model = m_resultsModel->records->model_srv;
        break;
    default:
        break;
    }
    ///
    /// choice of item to save...
    ///
    switch(choice){
    case CHOICE::susbdomains:
        for(int i = 0; i != model->rowCount(); ++i)
        {
            item = model->item(i, 0)->text().append(NEWLINE);
            if(!itemSet.contains(item)){
                itemSet.insert(item);
                file.write(item.toUtf8());
            }
        }
        break;
    case CHOICE::ipaddress:
        for(int i = 0; i != model->rowCount(); ++i)
        {
            item = model->item(i, 1)->text().append(NEWLINE);
            if(!itemSet.contains(item)){
                itemSet.insert(item);
                file.write(item.toUtf8());
            }
        }
        break;
    case CHOICE::all:
        for(int i = 0; i != model->rowCount(); ++i)
        {
            item = model->item(i, 0)->text()+":"+model->item(i, 1)->text().append(NEWLINE);
            if(!itemSet.contains(item)){
                itemSet.insert(item);
                file.write(item.toUtf8());
            }
        }
        break;
    case CHOICE::srvName:
        for(int i = 0; i != model->rowCount(); ++i)
        {
            item = model->item(i, 0)->text().append(NEWLINE);
            if(!itemSet.contains(item)){
                itemSet.insert(item);
                file.write(item.toUtf8());
            }
        }
        break;
    case CHOICE::srvTarget:
        for(int i = 0; i != model->rowCount(); ++i)
        {
            item = model->item(i, 1)->text().append(NEWLINE);
            if(!itemSet.contains(item)){
                itemSet.insert(item);
                file.write(item.toUtf8());
            }
        }
        break;
    case CHOICE::MX:
        for(int i = 0; i < model->rowCount(); i++)
        {
            for(int j = 0; j < model->item(i)->rowCount(); j++)
            {
                if(model->item(i)->child(j)->text() == "MX"){
                    for(int k = 0; k < model->item(i)->child(j)->rowCount(); k++)
                    {
                        item = model->item(i)->child(j)->child(k)->text().append(NEWLINE);
                        if(!itemSet.contains(item)){
                            itemSet.insert(item);
                            file.write(item.toUtf8());
                        }
                    }
                }
            }
        }
        break;
    case CHOICE::NS:
        for(int i = 0; i < model->rowCount(); i++)
        {
            for(int j = 0; j < model->item(i)->rowCount(); j++)
            {
                if(model->item(i)->child(j)->text() == "NS"){
                    for(int k = 0; k < model->item(i)->child(j)->rowCount(); k++)
                    {
                        item = model->item(i)->child(j)->child(k)->text().append(NEWLINE);
                        if(!itemSet.contains(item)){
                            itemSet.insert(item);
                            file.write(item.toUtf8());
                        }
                    }
                }
            }
        }
        break;
    case CHOICE::TXT:
        for(int i = 0; i < model->rowCount(); i++)
        {
            for(int j = 0; j < model->item(i)->rowCount(); j++)
            {
                if(model->item(i)->child(j)->text() == "TXT"){
                    for(int k = 0; k < model->item(i)->child(j)->rowCount(); k++)
                    {
                        item = model->item(i)->child(j)->child(k)->text().append(NEWLINE);
                        if(!itemSet.contains(item)){
                            itemSet.insert(item);
                            file.write(item.toUtf8());
                        }
                    }
                }
            }
        }
        break;
    case CHOICE::CNAME:
        for(int i = 0; i < model->rowCount(); i++)
        {
            for(int j = 0; j < model->item(i)->rowCount(); j++)
            {
                if(model->item(i)->child(j)->text() == "CNAME"){
                    for(int k = 0; k < model->item(i)->child(j)->rowCount(); k++)
                    {
                        item = model->item(i)->child(j)->child(k)->text().append(NEWLINE);
                        if(!itemSet.contains(item)){
                            itemSet.insert(item);
                            file.write(item.toUtf8());
                        }
                    }
                }
            }
        }
        break;
    case CHOICE::A:
        for(int i = 0; i < model->rowCount(); i++)
        {
            for(int j = 0; j < model->item(i)->rowCount(); j++)
            {
                if(model->item(i)->child(j)->text() == "A"){
                    for(int k = 0; k < model->item(i)->child(j)->rowCount(); k++)
                    {
                        item = model->item(i)->child(j)->child(k)->text().append(NEWLINE);
                        if(!itemSet.contains(item)){
                            itemSet.insert(item);
                            file.write(item.toUtf8());
                        }
                    }
                }
            }
        }
        break;
    case CHOICE::AAAA:
        for(int i = 0; i < model->rowCount(); i++)
        {
            for(int j = 0; j < model->item(i)->rowCount(); j++)
            {
                if(model->item(i)->child(j)->text() == "AAAA"){
                    for(int k = 0; k < model->item(i)->child(j)->rowCount(); k++)
                    {
                        item = model->item(i)->child(j)->child(k)->text().append(NEWLINE);
                        if(!itemSet.contains(item)){
                            itemSet.insert(item);
                            file.write(item.toUtf8());
                        }
                    }
                }
            }
        }
        break;
    }
    file.close();
}

void BaseClass::actionCopy(ENGINE engineName, CHOICE choice){
    ///
    /// variable declaration...
    ///
    QClipboard *clipboard = QGuiApplication::clipboard();
    QStandardItemModel *model = nullptr;
    QString clipboardData;
    QSet<QString> itemSet;
    QString item;
    ///
    /// Engine to Copy from...
    ///
    switch(engineName){
    case ENGINE::IP:
        model = m_resultsModel->ip;
        break;
    case ENGINE::BRUTE:
        model = m_resultsModel->brute;
        break;
    case ENGINE::LEVEL:
        model = m_resultsModel->level;
        break;
    case ENGINE::OSINT:
        model = m_resultsModel->osint;
        break;
    case ENGINE::ACTIVE:
        model = m_resultsModel->active;
        break;
    case ENGINE::DNSRECORDS:
        model = m_resultsModel->records->model_records;
        break;
    case ENGINE::SRVRECORDS:
        model = m_resultsModel->records->model_srv;
        break;
    default:
        break;
    }
    ///
    /// type of item to save...
    ///
    switch(choice){
    case CHOICE::susbdomains:
        for(int i = 0; i != model->rowCount(); ++i)
        {
            item = model->item(i, 0)->text().append(NEWLINE);
            if(!itemSet.contains(item)){
                itemSet.insert(item);
                clipboardData.append(item);
            }
        }
        break;
    case CHOICE::ipaddress:
        for(int i = 0; i != model->rowCount(); ++i)
        {
            item = model->item(i, 1)->text().append(NEWLINE);
            if(!itemSet.contains(item)){
                itemSet.insert(item);
                clipboardData.append(item);
            }
        }
        break;
    case CHOICE::all:
        for(int i = 0; i != model->rowCount(); ++i)
        {
            item = model->item(i, 0)->text()+"|"+model->item(i, 1)->text().append(NEWLINE);
            if(!itemSet.contains(item)){
                itemSet.insert(item);
                clipboardData.append(item);
            }
        }
        break;
    case CHOICE::srvName:
        for(int i = 0; i != model->rowCount(); ++i)
        {
            item = model->item(i, 0)->text().append(NEWLINE);
            if(!itemSet.contains(item)){
                itemSet.insert(item);
                clipboardData.append(item);
            }
        }
        break;
    case CHOICE::srvTarget:
        for(int i = 0; i != model->rowCount(); ++i)
        {
            item = model->item(i, 1)->text().append(NEWLINE);
            if(!itemSet.contains(item)){
                itemSet.insert(item);
                clipboardData.append(item);
            }
        }
        break;
    case CHOICE::MX:
        for(int i = 0; i < model->rowCount(); i++)
        {
            for(int j = 0; j < model->item(i)->rowCount(); j++)
            {
                if(model->item(i)->child(j)->text() == "MX"){
                    for(int k = 0; k < model->item(i)->child(j)->rowCount(); k++)
                    {
                        item = model->item(i)->child(j)->child(k)->text().append(NEWLINE);
                        if(!itemSet.contains(item)){
                            itemSet.insert(item);
                            clipboardData.append(item);
                        }
                    }
                }
            }
        }
        break;
    case CHOICE::NS:
        for(int i = 0; i < model->rowCount(); i++)
        {
            for(int j = 0; j < model->item(i)->rowCount(); j++)
            {
                if(model->item(i)->child(j)->text() == "NS"){
                    for(int k = 0; k < model->item(i)->child(j)->rowCount(); k++)
                    {
                        item = model->item(i)->child(j)->child(k)->text().append(NEWLINE);
                        if(!itemSet.contains(item)){
                            itemSet.insert(item);
                            clipboardData.append(item);
                        }
                    }
                }
            }
        }
        break;
    case CHOICE::TXT:
        for(int i = 0; i < model->rowCount(); i++)
        {
            for(int j = 0; j < model->item(i)->rowCount(); j++)
            {
                if(model->item(i)->child(j)->text() == "TXT"){
                    for(int k = 0; k < model->item(i)->child(j)->rowCount(); k++)
                    {
                        item = model->item(i)->child(j)->child(k)->text().append(NEWLINE);
                        if(!itemSet.contains(item)){
                            itemSet.insert(item);
                            clipboardData.append(item);
                        }
                    }
                }
            }
        }
        break;
    case CHOICE::CNAME:
        for(int i = 0; i < model->rowCount(); i++)
        {
            for(int j = 0; j < model->item(i)->rowCount(); j++)
            {
                if(model->item(i)->child(j)->text() == "CNAME"){
                    for(int k = 0; k < model->item(i)->child(j)->rowCount(); k++)
                    {
                        item = model->item(i)->child(j)->child(k)->text().append(NEWLINE);
                        if(!itemSet.contains(item)){
                            itemSet.insert(item);
                            clipboardData.append(item);
                        }
                    }
                }
            }
        }
        break;
    case CHOICE::A:
        for(int i = 0; i < model->rowCount(); i++)
        {
            for(int j = 0; j < model->item(i)->rowCount(); j++)
            {
                if(model->item(i)->child(j)->text() == "A"){
                    for(int k = 0; k < model->item(i)->child(j)->rowCount(); k++)
                    {
                        item = model->item(i)->child(j)->child(k)->text().append(NEWLINE);
                        if(!itemSet.contains(item)){
                            itemSet.insert(item);
                            clipboardData.append(item);
                        }
                    }
                }
            }
        }
        break;
    case CHOICE::AAAA:
        for(int i = 0; i < model->rowCount(); i++)
        {
            for(int j = 0; j < model->item(i)->rowCount(); j++)
            {
                if(model->item(i)->child(j)->text() == "AAAA"){
                    for(int k = 0; k < model->item(i)->child(j)->rowCount(); k++)
                    {
                        item = model->item(i)->child(j)->child(k)->text().append(NEWLINE);
                        if(!itemSet.contains(item)){
                            itemSet.insert(item);
                            clipboardData.append(item);
                        }
                    }
                }
            }
        }
        break;
    }
    clipboard->setText(clipboardData);
}

//......
void BaseClass::cursorSave(QItemSelectionModel *selectionModel){
    QString filename = QFileDialog::getSaveFileName(this, INFO_LOADFILE, CURRENT_PATH);
    if(filename.isEmpty()){
        return;
    }
    QSet<QString> itemSet;
    QString data;
    QString item;
    ///
    /// ...
    ///
    QFile file(filename);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    if(file.isOpen())
    {
        foreach(const QModelIndex &index, selectionModel->selectedIndexes()){
            item = index.data().toString();
            if(!itemSet.contains(item) && item != "A" && item != "AAAA" && item != "NS" && item != "MX" && item != "CNAME" && item != "TXT"){
                itemSet.insert(item);
                data.append(item.append(NEWLINE));
            }
        }
        file.write(data.toUtf8());
        file.close();
    }
}
void BaseClass::cursorCopy(QItemSelectionModel *selectionModel){
    QClipboard *clipboard = QGuiApplication::clipboard();
    QSet<QString> itemSet;
    QString data;
    QString item;
    ///
    /// ...
    ///
    foreach(const QModelIndex &index, selectionModel->selectedIndexes())
    {
        item = index.data().toString();
        if(!itemSet.contains(item) && item != "A" && item != "AAAA" && item != "NS" && item != "MX" && item != "CNAME" && item != "TXT"){
            itemSet.insert(item);
            data.append(item.append(NEWLINE));
        }
    }
    clipboard->setText(data);
}


/***************************************************************************
                        RECEIVING TARGETS
****************************************************************************/
void BaseClass::a_receiveTargets(ENGINE engineName, CHOICE choice){
    ///
    /// varible declaration...
    ///
    QStandardItemModel *model = nullptr;
    QString item;
    ///
    /// engine targets are from...
    ///
    switch(engineName){
    case ENGINE::BRUTE:
        model = m_resultsModel->brute;
        break;
    case ENGINE::ACTIVE:
        model = m_resultsModel->active;
        break;
    case ENGINE::LEVEL:
        model = m_resultsModel->level;
        break;
    case ENGINE::IP:
        model = m_resultsModel->ip;
        break;
    case ENGINE::DNSRECORDS:
        model = m_resultsModel->records->model_records;
        for(char i = 0; i < model->rowCount(); i++)
        {
            for(char j = 0; j < model->item(i)->rowCount(); j++)
            {
                if(choice == CHOICE::susbdomains && (model->item(i)->child(j)->text() == "CNAME" || model->item(i)->child(j)->text() == "MX" || model->item(i)->child(j)->text() == "NS"))
                {
                    for(int k = 0; k < model->item(i)->child(j)->rowCount(); k++){
                        item = model->item(i)->child(j)->child(k)->text();
                        if(!m_targetSet.contains(item))
                        {
                            m_targetSet.insert(item);
                            widgets->listWidget_targets->addItem(item);
                        }
                    }
                }
                if(choice == CHOICE::ipaddress && (model->item(i)->child(j)->text() == "A" || model->item(i)->child(j)->text() == "AAAA"))
                {
                    for(int k = 0; k < model->item(i)->child(j)->rowCount(); k++){
                        item = model->item(i)->child(j)->child(k)->text();
                        if(!m_targetSet.contains(item))
                        {
                            m_targetSet.insert(item);
                            widgets->listWidget_targets->addItem(item);
                        }
                    }
                }
            }
        }
        widgets->label_targetsCount->setNum(widgets->listWidget_targets->count());
        return;
    case ENGINE::SRVRECORDS:
        if(choice == CHOICE::ipaddress){
            return;
        }
        model = m_resultsModel->records->model_srv;
        for(char i = 0; i < model->rowCount(); i++)
        {
            item = model->item(i, 1)->text();
            if(!m_targetSet.contains(item)){
                m_targetSet.insert(item);
                widgets->listWidget_targets->addItem(item);
            }
        }
        widgets->label_targetsCount->setNum(widgets->listWidget_targets->count());
        return;
    default:
        break;
    }
    ///
    /// for every other engine...
    ///
    for(char i = 0; i < model->rowCount(); i++)
    {
        if(choice == CHOICE::susbdomains)
            item = model->item(i, 0)->text();
        if(choice == CHOICE::ipaddress)
            item = model->item(i, 1)->text();
        if(!m_targetSet.contains(item)){
            m_targetSet.insert(item);
            widgets->listWidget_targets->addItem(item);
        }
    }
    widgets->label_targetsCount->setNum(widgets->listWidget_targets->count());
}

void BaseClass::c_receiveTargets(QItemSelectionModel *selectionModel){
    QString item;
    ///
    /// iterate and open each selected and append on the target's listwidget...
    ///
    foreach(const QModelIndex &index, selectionModel->selectedIndexes())
    {
        item = index.data().toString();
        if(!m_targetSet.contains(item)){
            m_targetSet.insert(item);
            widgets->listWidget_targets->addItem(item);
        }
    }
    widgets->label_targetsCount->setNum(widgets->listWidget_targets->count());
}
