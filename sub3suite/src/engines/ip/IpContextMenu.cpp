#include "Ip.h"
#include "ui_Ip.h"

#include <QClipboard>


void Ip::m_initActions(){
    connect(&a_ClearResults, &QAction::triggered, this, [=](){this->onClearResults();});
    connect(&a_RemoveResults, &QAction::triggered, this, [=](){this->onRemoveResults(selectionModel);});
    connect(&a_OpenInBrowser, &QAction::triggered, this, [=](){this->onOpenInBrowser(selectionModel);});
    /* ... */
    connect(&a_SendIpToIp, &QAction::triggered, this, [=](){emit sendResultsToIp(ENGINE::IP, RESULT_TYPE::IP, RESULT_MODEL_TYPE::SUBDOMAINIP); emit changeTabToIp();});
    connect(&a_SendIpToOsint, &QAction::triggered, this, [=](){emit sendResultsToOsint(ENGINE::IP, RESULT_TYPE::IP, RESULT_MODEL_TYPE::SUBDOMAINIP); emit changeTabToOsint();});
    connect(&a_SendIpToRaw, &QAction::triggered, this, [=](){emit sendResultsToRaw(ENGINE::IP, RESULT_TYPE::IP, RESULT_MODEL_TYPE::SUBDOMAINIP); emit changeTabToRaw();});
    connect(&a_SendHostToOsint, &QAction::triggered, this, [=](){emit sendResultsToOsint(ENGINE::IP, RESULT_TYPE::SUBDOMAIN, RESULT_MODEL_TYPE::SUBDOMAINIP); emit changeTabToOsint();});
    connect(&a_SendHostToRaw, &QAction::triggered, this, [=](){emit sendResultsToRaw(ENGINE::IP, RESULT_TYPE::SUBDOMAIN, RESULT_MODEL_TYPE::SUBDOMAINIP); emit changeTabToRaw();});
    connect(&a_SendHostToBrute, &QAction::triggered, this, [=](){emit sendResultsToBrute(ENGINE::IP, RESULT_TYPE::SUBDOMAIN, RESULT_MODEL_TYPE::SUBDOMAINIP); emit changeTabToBrute();});
    connect(&a_SendHostToActive, &QAction::triggered, this, [=](){emit sendResultsToActive(ENGINE::IP, RESULT_TYPE::SUBDOMAIN, RESULT_MODEL_TYPE::SUBDOMAINIP); emit changeTabToActive();});
    connect(&a_SendHostToDns, &QAction::triggered, this, [=](){emit sendResultsToDns(ENGINE::IP, RESULT_TYPE::SUBDOMAIN, RESULT_MODEL_TYPE::SUBDOMAINIP); emit changeTabToDns();});
    connect(&a_SendHostToCert, &QAction::triggered, this, [=](){emit sendResultsToCert(ENGINE::IP, RESULT_TYPE::SUBDOMAIN, RESULT_MODEL_TYPE::SUBDOMAINIP); emit changeTabToCert();});
    connect(&a_SendIpToIpTool, &QAction::triggered, this, [=](){emit sendResultsToIpTool(ENGINE::IP, RESULT_TYPE::IP, RESULT_MODEL_TYPE::SUBDOMAINIP);});
    connect(&a_SendHostToCertTool, &QAction::triggered, this, [=](){emit sendResultsToCertTool(ENGINE::IP, RESULT_TYPE::SUBDOMAIN, RESULT_MODEL_TYPE::SUBDOMAINIP);});
    connect(&a_SendHostToDomainTool, &QAction::triggered, this, [=](){emit sendResultsToDomainTool(ENGINE::IP, RESULT_TYPE::SUBDOMAIN, RESULT_MODEL_TYPE::SUBDOMAINIP);});
    /* ... */
    connect(&a_SendSelectedIpToIp, &QAction::triggered, this, [=](){emit sendResultsToIp(selectionModel, RESULT_TYPE::IP, RESULT_MODEL_TYPE::SUBDOMAINIP); emit changeTabToIp();});
    connect(&a_SendSelectedIpToOsint, &QAction::triggered, this, [=](){emit sendResultsToOsint(selectionModel, RESULT_TYPE::IP, RESULT_MODEL_TYPE::SUBDOMAINIP); emit changeTabToOsint();});
    connect(&a_SendSelectedIpToRaw, &QAction::triggered, this, [=](){emit sendResultsToRaw(selectionModel, RESULT_TYPE::IP, RESULT_MODEL_TYPE::SUBDOMAINIP); emit changeTabToRaw();});
    connect(&a_SendSelectedHostToOsint, &QAction::triggered, this, [=](){emit sendResultsToOsint(selectionModel, RESULT_TYPE::SUBDOMAIN, RESULT_MODEL_TYPE::SUBDOMAINIP); emit changeTabToOsint();});
    connect(&a_SendSelectedHostToRaw, &QAction::triggered, this, [=](){emit sendResultsToRaw(selectionModel, RESULT_TYPE::SUBDOMAIN, RESULT_MODEL_TYPE::SUBDOMAINIP); emit changeTabToRaw();});
    connect(&a_SendSelectedHostToBrute, &QAction::triggered, this, [=](){emit sendResultsToBrute(selectionModel, RESULT_TYPE::SUBDOMAIN, RESULT_MODEL_TYPE::SUBDOMAINIP); emit changeTabToBrute();});
    connect(&a_SendSelectedHostToActive, &QAction::triggered, this, [=](){emit sendResultsToActive(selectionModel, RESULT_TYPE::SUBDOMAIN, RESULT_MODEL_TYPE::SUBDOMAINIP); emit changeTabToActive();});
    connect(&a_SendSelectedHostToDns, &QAction::triggered, this, [=](){emit sendResultsToDns(selectionModel, RESULT_TYPE::SUBDOMAIN, RESULT_MODEL_TYPE::SUBDOMAINIP); emit changeTabToDns();});
    connect(&a_SendSelectedHostToCert, &QAction::triggered, this, [=](){emit sendResultsToCert(selectionModel, RESULT_TYPE::SUBDOMAIN, RESULT_MODEL_TYPE::SUBDOMAINIP); emit changeTabToCert();});
    connect(&a_SendSelectedIpToIpTool, &QAction::triggered, this, [=](){emit sendResultsToIpTool(selectionModel, RESULT_TYPE::IP, RESULT_MODEL_TYPE::SUBDOMAINIP);});
    connect(&a_SendSelectedHostToCertTool, &QAction::triggered, this, [=](){emit sendResultsToCertTool(selectionModel, RESULT_TYPE::SUBDOMAIN, RESULT_MODEL_TYPE::SUBDOMAINIP);});
    connect(&a_SendSelectedHostToDomainTool, &QAction::triggered, this, [=](){emit sendResultsToDomainTool(selectionModel, RESULT_TYPE::SUBDOMAIN, RESULT_MODEL_TYPE::SUBDOMAINIP);});
    /* ... */
    connect(&a_Save, &QAction::triggered, this, [=](){this->onSaveResults(selectionModel);});
    connect(&a_SaveSubdomainIp, &QAction::triggered, this, [=](){this->onSaveResults(RESULT_TYPE::SUBDOMAINIP);});
    connect(&a_SaveSubdomain, &QAction::triggered, this, [=](){this->onSaveResults(RESULT_TYPE::SUBDOMAIN);});
    connect(&a_SaveIp, &QAction::triggered, this, [=](){this->onSaveResults(RESULT_TYPE::IP);});
    connect(&a_Copy, &QAction::triggered, this, [=](){this->onCopyResults(selectionModel);});
    connect(&a_CopySubdomainIp, &QAction::triggered, this, [=](){this->onCopyResults(RESULT_TYPE::SUBDOMAINIP);});
    connect(&a_CopySubdomain, &QAction::triggered, this, [=](){this->onCopyResults(RESULT_TYPE::SUBDOMAIN);});
    connect(&a_CopyIp, &QAction::triggered, this, [=](){this->onCopyResults(RESULT_TYPE::IP);});
}

void Ip::on_buttonAction_clicked(){
    /* check if there are results available else dont show the context menu */

    /* getting the position of the action button to place the context menu and
       showing the context menu right by the side of the action button... */
    QPoint pos = ui->buttonAction->mapToGlobal(QPoint(0,0));
    pos = QPoint(pos.x()+65, pos.y());

    /* creating the context menu... */
    QMenu *mainMenu = new QMenu(this);
    QMenu *saveMenu = new QMenu(this);
    QMenu *copyMenu = new QMenu(this);
    QMenu *sendToToolMenu = new QMenu(this);
    QMenu *sendToEngineMenu = new QMenu(this);
    saveMenu->setTitle("Save");
    copyMenu->setTitle("Copy");
    sendToToolMenu->setTitle("Send To Tool");
    sendToEngineMenu->setTitle("Send To Engine");
    mainMenu->setAttribute(Qt::WA_DeleteOnClose, true);

    /* adding actions */
    saveMenu->addAction(&a_SaveSubdomainIp);
    saveMenu->addAction(&a_SaveSubdomain);
    saveMenu->addAction(&a_SaveIp);
    copyMenu->addAction(&a_CopySubdomainIp);
    copyMenu->addAction(&a_CopySubdomain);
    copyMenu->addAction(&a_CopyIp);
    sendToEngineMenu->addAction(&a_SendIpToIp);
    sendToEngineMenu->addAction(&a_SendIpToOsint);
    sendToEngineMenu->addAction(&a_SendIpToRaw);
    sendToEngineMenu->addSeparator();
    sendToEngineMenu->addAction(&a_SendHostToOsint);
    sendToEngineMenu->addAction(&a_SendHostToRaw);
    sendToEngineMenu->addAction(&a_SendHostToBrute);
    sendToEngineMenu->addAction(&a_SendHostToActive);
    sendToEngineMenu->addAction(&a_SendHostToDns);
    sendToEngineMenu->addAction(&a_SendHostToCert);
    sendToToolMenu->addAction(&a_SendIpToIpTool);
    sendToToolMenu->addAction(&a_SendHostToCertTool);
    sendToToolMenu->addAction(&a_SendHostToDomainTool);
    /* adding to mainMenu */
    mainMenu->addAction(&a_ClearResults);
    mainMenu->addSeparator();
    mainMenu->addMenu(saveMenu);
    mainMenu->addMenu(copyMenu);
    mainMenu->addSeparator();
    mainMenu->addMenu(sendToEngineMenu);
    mainMenu->addMenu(sendToToolMenu);

    /* showing the context menu... */
    mainMenu->exec(pos);
}

void Ip::on_tableViewResults_customContextMenuRequested(const QPoint &pos){
    Q_UNUSED(pos);

    /* check if user right clicked on items else dont show the context menu... */
    if(!ui->tableViewResults->selectionModel()->isSelected(ui->tableViewResults->currentIndex())){
        return;
    }

    /* getting the selected items... */
    selectionModel = ui->tableViewResults->selectionModel();

    /* creating the context menu... */
    QMenu *mainMenu = new QMenu(this);
    QMenu *saveMenu = new QMenu(this);
    QMenu *copyMenu = new QMenu(this);
    QMenu *sendToToolMenu = new QMenu(this);
    QMenu *sendToEngineMenu = new QMenu(this);
    saveMenu->setTitle("Save");
    copyMenu->setTitle("Copy");
    sendToToolMenu->setTitle("Send To Tool");
    sendToEngineMenu->setTitle("Send To Engine");
    mainMenu->setAttribute(Qt::WA_DeleteOnClose, true);

    /* adding actions */
    saveMenu->addAction(&a_Save);
    copyMenu->addAction(&a_Copy);
    sendToEngineMenu->addAction(&a_SendSelectedIpToIp);
    sendToEngineMenu->addAction(&a_SendSelectedIpToOsint);
    sendToEngineMenu->addAction(&a_SendSelectedIpToRaw);
    sendToEngineMenu->addSeparator();
    sendToEngineMenu->addAction(&a_SendSelectedHostToOsint);
    sendToEngineMenu->addAction(&a_SendSelectedHostToRaw);
    sendToEngineMenu->addAction(&a_SendSelectedHostToBrute);
    sendToEngineMenu->addAction(&a_SendSelectedHostToActive);
    sendToEngineMenu->addAction(&a_SendSelectedHostToDns);
    sendToEngineMenu->addAction(&a_SendSelectedHostToCert);
    sendToToolMenu->addAction(&a_SendSelectedIpToIpTool);
    sendToToolMenu->addAction(&a_SendSelectedHostToCertTool);
    sendToToolMenu->addAction(&a_SendSelectedHostToDomainTool);
    /* adding to mainMenu */
    mainMenu->addAction(&a_RemoveResults);
    mainMenu->addAction(&a_OpenInBrowser);
    mainMenu->addSeparator();
    mainMenu->addMenu(saveMenu);
    mainMenu->addMenu(copyMenu);
    mainMenu->addSeparator();
    mainMenu->addMenu(sendToEngineMenu);
    mainMenu->addMenu(sendToToolMenu);

    /* showing the context menu... */
    mainMenu->exec(QCursor::pos());
}

void Ip::onOpenInBrowser(QItemSelectionModel *){

}

void Ip::onRemoveResults(QItemSelectionModel *selectionModel){
    Q_UNUSED(selectionModel);
}

void Ip::onSaveResults(RESULT_TYPE resultType){
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
    QSet<QString> itemSet;
    QString item;
    ///
    /// choice of item to save...
    ///
    switch(resultType){
    case RESULT_TYPE::SUBDOMAIN:
        for(int i = 0; i != result->active->subdomainIpProxy->rowCount(); ++i)
        {
            item = result->active->subdomainIpProxy->data(result->active->subdomainIpProxy->index(i, 0)).toString().append(NEWLINE);
            if(!itemSet.contains(item)){
                itemSet.insert(item);
                file.write(item.toUtf8());
            }
        }
        break;
    case RESULT_TYPE::IP:
        for(int i = 0; i != result->active->subdomainIpProxy->rowCount(); ++i)
        {
            item = result->active->subdomainIpProxy->data(result->active->subdomainIpProxy->index(i, 1)).toString().append(NEWLINE);
            if(!itemSet.contains(item)){
                itemSet.insert(item);
                file.write(item.toUtf8());
            }
        }
        break;
    case RESULT_TYPE::SUBDOMAINIP:
        for(int i = 0; i != result->active->subdomainIpProxy->rowCount(); ++i)
        {
            item = result->active->subdomainIpProxy->data(result->active->subdomainIpProxy->index(i, 0)).toString()+":"+result->active->subdomainIpProxy->data(result->active->subdomainIpProxy->index(i, 0)).toString().append(NEWLINE);
            if(!itemSet.contains(item)){
                itemSet.insert(item);
                file.write(item.toUtf8());
            }
        }
        break;

    default:
        break;
    }
    file.close();
}

void Ip::onSaveResults(QItemSelectionModel *selectionModel){
    QString filename = QFileDialog::getSaveFileName(this, "Save To File", "./");
    if(filename.isEmpty()){
        return;
    }
    QSet<QString> itemSet;
    QString data;
    QString item;

    /* ... */
    QFile file(filename);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    if(file.isOpen())
    {
        foreach(const QModelIndex &index, selectionModel->selectedIndexes()){
            item = index.data().toString();
            if(!itemSet.contains(item)){
                itemSet.insert(item);
                data.append(item.append(NEWLINE));
            }
        }
        file.write(data.toUtf8());
        file.close();
    }
}


void Ip::onCopyResults(RESULT_TYPE resultType){
    ///
    /// variable declaration...
    ///
    QClipboard *clipboard = QGuiApplication::clipboard();
    QString clipboardData;
    QSet<QString> itemSet;
    QString item;
    ///
    /// type of item to save...
    ///
    switch(resultType){
    case RESULT_TYPE::SUBDOMAIN:
        for(int i = 0; i != result->active->subdomainIpProxy->rowCount(); ++i)
        {
            item = result->active->subdomainIpProxy->data(result->active->subdomainIpProxy->index(i, 0)).toString().append(NEWLINE);
            if(!itemSet.contains(item)){
                itemSet.insert(item);
                clipboardData.append(item);
            }
        }
        break;
    case RESULT_TYPE::IP:
        for(int i = 0; i != result->active->subdomainIpProxy->rowCount(); ++i)
        {
            item = result->active->subdomainIpProxy->data(result->active->subdomainIpProxy->index(i, 1)).toString().append(NEWLINE);
            if(!itemSet.contains(item)){
                itemSet.insert(item);
                clipboardData.append(item);
            }
        }
        break;
    case RESULT_TYPE::SUBDOMAINIP:
        for(int i = 0; i != result->active->subdomainIpProxy->rowCount(); ++i)
        {
            item = result->active->subdomainIpProxy->data(result->active->subdomainIpProxy->index(i, 0)).toString()+"|"+result->active->subdomainIpProxy->data(result->active->subdomainIpProxy->index(i, 1)).toString().append(NEWLINE);
            if(!itemSet.contains(item)){
                itemSet.insert(item);
                clipboardData.append(item);
            }
        }
        break;
    default:
        break;
    }
    clipboard->setText(clipboardData);
}

void Ip::onCopyResults(QItemSelectionModel *selectionModel){
    QClipboard *clipboard = QGuiApplication::clipboard();
    QSet<QString> itemSet;
    QString data;
    QString item;
    /* ... */
    foreach(const QModelIndex &index, selectionModel->selectedIndexes())
    {
        item = index.data().toString();
        if(!itemSet.contains(item)){
            itemSet.insert(item);
            data.append(item.append(NEWLINE));
        }
    }
    clipboard->setText(data);
}

void Ip::onReceiveTargets(ENGINE, RESULT_TYPE, RESULT_MODEL_TYPE){

}

void Ip::onReceiveTargets(QItemSelectionModel *, RESULT_TYPE, RESULT_MODEL_TYPE){

}
