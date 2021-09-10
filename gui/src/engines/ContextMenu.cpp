#include "ContextMenu.h"
//...
#include <QGuiApplication>
#include <QClipboard>
#include <QDesktopServices>
#include <QFileDialog>
#include <QUrl>

ContextMenu::ContextMenu(ENGINE engineName, ResultsModel *resultsModel, QWidget *parent)
    : AbstractClass(engineName, resultsModel, parent)
{
    initMenuC();
    initMenuA();
}
ContextMenu::~ContextMenu(){
    delete a_Menu;
    delete c_Menu;
    delete saveMenu;
    delete copyMenu;
}

void ContextMenu::initMenuA(){
    a_Menu = new QMenu(this);
    saveMenu = new QMenu(this);
    copyMenu = new QMenu(this);
    //...
    saveMenu->setTitle("Save");
    copyMenu->setTitle("Copy");
    ///
    /// SAVE...
    ///
    connect(&actionSaveSubdomains, &QAction::triggered, this, [=](){this->saveResults(CHOICE::susbdomains);});
    connect(&actionSaveIpAddresses, &QAction::triggered, this, [=](){this->saveResults(CHOICE::ipaddress);});
    connect(&actionSaveAll, &QAction::triggered, this, [=](){this->saveResults(CHOICE::all);});
    //...
    connect(&actionSaveA, &QAction::triggered, this, [=](){this->saveResults(CHOICE::A);});
    connect(&actionSaveAAAA, &QAction::triggered, this, [=](){this->saveResults(CHOICE::AAAA);});
    connect(&actionSaveMX, &QAction::triggered, this, [=](){this->saveResults(CHOICE::MX);});
    connect(&actionSaveNS, &QAction::triggered, this, [=](){this->saveResults(CHOICE::NS);});
    connect(&actionSaveCNAME, &QAction::triggered, this, [=](){this->saveResults(CHOICE::CNAME);});
    connect(&actionSaveTXT, &QAction::triggered, this, [=](){this->saveResults(CHOICE::TXT);});
    connect(&actionSaveSRVName, &QAction::triggered, this, [=](){this->saveResults(CHOICE::srvName);});
    connect(&actionSaveSRVTarget, &QAction::triggered, this, [=](){this->saveResults(CHOICE::srvTarget);});
    ///
    /// COPY...
    ///
    connect(&actionCopySubdomains, &QAction::triggered, this, [=](){this->copyResults(CHOICE::susbdomains);});
    connect(&actionCopyIpAddresses, &QAction::triggered, this, [=](){this->copyResults(CHOICE::ipaddress);});
    connect(&actionCopyAll, &QAction::triggered, this, [=](){this->copyResults(CHOICE::all);});
    //...
    connect(&actionCopyA, &QAction::triggered, this, [=](){this->copyResults(CHOICE::A);});
    connect(&actionCopyAAAA, &QAction::triggered, this, [=](){this->copyResults(CHOICE::AAAA);});
    connect(&actionCopyMX, &QAction::triggered, this, [=](){this->copyResults(CHOICE::MX);});
    connect(&actionCopyNS, &QAction::triggered, this, [=](){this->copyResults(CHOICE::NS);});
    connect(&actionCopyCNAME, &QAction::triggered, this, [=](){this->copyResults(CHOICE::CNAME);});
    connect(&actionCopyTXT, &QAction::triggered, this, [=](){this->copyResults(CHOICE::TXT);});
    connect(&actionCopySRVName, &QAction::triggered, this, [=](){this->copyResults(CHOICE::srvName);});
    connect(&actionCopySRVTarget, &QAction::triggered, this, [=](){this->copyResults(CHOICE::srvTarget);});
    ///
    /// SUBDOMAINS AND IPS...
    ///
    connect(&a_actionSendToIp, &QAction::triggered, this, [=](){emit sendResultsToIp(engineName, CHOICE::ipaddress); emit changeTabToIp();});
    connect(&a_actionSendToOsint, &QAction::triggered, this, [=](){emit sendResultsToOsint(engineName, CHOICE::susbdomains); emit changeTabToOsint();});
    connect(&a_actionSendToBrute, &QAction::triggered, this, [=](){emit sendResultsToBrute(engineName, CHOICE::susbdomains); emit changeTabToBrute();});
    connect(&a_actionSendToActive, &QAction::triggered, this, [=](){emit sendResultsToActive(engineName, CHOICE::susbdomains); emit changeTabToActive();});
    connect(&a_actionSendToRecords, &QAction::triggered, this, [=](){emit sendResultsToRecord(engineName, CHOICE::susbdomains); emit changeTabToRecords();});
    connect(&a_actionSendToLevel, &QAction::triggered, this, [=](){emit sendResultsToLevel(engineName, CHOICE::susbdomains); emit changeTabToLevel();});
    ///
    /// ADDING ACTIONS TO THE CONTEXT MENU...
    ///
    if(engineName == ENGINE::RECORDS){
        saveMenu->addSeparator();
        saveMenu->addAction(&actionSaveA);
        saveMenu->addAction(&actionSaveAAAA);
        saveMenu->addAction(&actionSaveNS);
        saveMenu->addAction(&actionSaveMX);
        saveMenu->addAction(&actionSaveTXT);
        saveMenu->addAction(&actionSaveCNAME);
        saveMenu->addAction(&actionSaveSRVName);
        saveMenu->addAction(&actionSaveSRVTarget);
        //...
        copyMenu->addSeparator();
        copyMenu->addAction(&actionCopyA);
        copyMenu->addAction(&actionCopyAAAA);
        copyMenu->addAction(&actionCopyNS);
        copyMenu->addAction(&actionCopyMX);
        copyMenu->addAction(&actionCopyTXT);
        copyMenu->addAction(&actionCopyCNAME);
        copyMenu->addAction(&actionCopySRVName);
        copyMenu->addAction(&actionCopySRVTarget);
    }
    if(engineName == ENGINE::OSINT || engineName == ENGINE::BRUTE || engineName == ENGINE::ACTIVE || engineName == ENGINE::LEVEL || engineName == ENGINE::IP){
        saveMenu->addAction(&actionSaveSubdomains);
        saveMenu->addAction(&actionSaveIpAddresses);
        saveMenu->addAction(&actionSaveAll);
        //...
        copyMenu->addAction(&actionCopySubdomains);
        copyMenu->addAction(&actionCopyIpAddresses);
        copyMenu->addAction(&actionCopyAll);
    }
    a_Menu->addMenu(copyMenu);
    a_Menu->addMenu(saveMenu);
    //...
    a_Menu->addSeparator();
    a_Menu->addAction(&a_actionSendToIp);
    a_Menu->addAction(&a_actionSendToOsint);
    a_Menu->addAction(&a_actionSendToBrute);
    a_Menu->addAction(&a_actionSendToActive);
    a_Menu->addAction(&a_actionSendToRecords);
    a_Menu->addAction(&a_actionSendToLevel);
}

void ContextMenu::initMenuC(){
    c_Menu = new QMenu(this);
    ///
    /// ...
    ///
    connect(&actionSave, &QAction::triggered, this, [=](){this->saveResults(selectionModel);});
    connect(&actionCopy, &QAction::triggered, this, [=](){this->copyResults(selectionModel);});
    //...
    connect(&actionOpenInBrowser, &QAction::triggered, this, [=](){this->openInBrowser(selectionModel);});
    //...
    connect(&c_actionSendToOsint, &QAction::triggered, this, [=](){emit sendResultsToOsint(selectionModel); emit changeTabToOsint();});
    connect(&c_actionSendToIp, &QAction::triggered, this, [=](){emit sendResultsToIp(selectionModel); emit changeTabToIp();});
    connect(&c_actionSendToBrute, &QAction::triggered, this, [=](){emit sendResultsToBrute(selectionModel); emit changeTabToBrute();});
    connect(&c_actionSendToActive, &QAction::triggered, this, [=](){emit sendResultsToActive(selectionModel); emit changeTabToActive();});
    connect(&c_actionSendToRecords, &QAction::triggered, this, [=](){emit sendResultsToRecord(selectionModel); emit changeTabToRecords();});
    connect(&c_actionSendToLevel, &QAction::triggered, this, [=](){emit sendResultsToLevel(selectionModel); emit changeTabToLevel();});
    ///
    /// ...
    ///
    c_Menu->addAction(&actionCopy);
    c_Menu->addAction(&actionSave);
    c_Menu->addSeparator();
    c_Menu->addAction(&actionOpenInBrowser);
    c_Menu->addSeparator();
    c_Menu->addAction(&c_actionSendToIp);
    c_Menu->addAction(&c_actionSendToOsint);
    c_Menu->addAction(&c_actionSendToBrute);
    c_Menu->addAction(&c_actionSendToActive);
    c_Menu->addAction(&c_actionSendToRecords);
    c_Menu->addAction(&c_actionSendToLevel);
}

void ContextMenu::openInBrowser(QItemSelectionModel *selectionModel){
    QString item;
    foreach(const QModelIndex &index, selectionModel->selectedIndexes())
    {
        item = index.data().toString();
        if(item != "A" && item != "AAAA" && item != "NS" && item != "MX" && item != "CNAME" && item != "TXT")
            QDesktopServices::openUrl(QUrl("https://"+item, QUrl::TolerantMode));
    }
}

void ContextMenu::saveResults(CHOICE choice){
    ///
    /// checks...
    ///
    QString filename = QFileDialog::getSaveFileName(this, "Save To File", "./");
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
        model = resultsModel->ip;
        break;
    case ENGINE::BRUTE:
        model = resultsModel->brute;
        break;
    case ENGINE::LEVEL:
        model = resultsModel->level;
        break;
    case ENGINE::OSINT:
        model = resultsModel->osint;
        break;
    case ENGINE::ACTIVE:
        model = resultsModel->active;
        break;
    case ENGINE::RECORDS:
        if(choice == CHOICE::srvName || choice == CHOICE::srvTarget)
            model = resultsModel->srvrecords;
        else
            model = resultsModel->dnsrecords;
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

void ContextMenu::saveResults(QItemSelectionModel* selectionModel){
    QString filename = QFileDialog::getSaveFileName(this, "Save To File", "./");
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

void ContextMenu::copyResults(CHOICE choice){
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
        model = resultsModel->ip;
        break;
    case ENGINE::BRUTE:
        model = resultsModel->brute;
        break;
    case ENGINE::LEVEL:
        model = resultsModel->level;
        break;
    case ENGINE::OSINT:
        model = resultsModel->osint;
        break;
    case ENGINE::ACTIVE:
        model = resultsModel->active;
        break;
    case ENGINE::RECORDS:
        if(choice == CHOICE::srvName || choice == CHOICE::srvTarget)
            model = resultsModel->srvrecords;
        else
            model = resultsModel->dnsrecords;
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

void ContextMenu::copyResults(QItemSelectionModel* selectionModel){
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
