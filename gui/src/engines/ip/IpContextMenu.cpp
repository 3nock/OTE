#include "Ip.h"
#include "ui_Ip.h"
//...
#include <QClipboard>


void Ip::connectActions(){
    connect(&actionClearResults, &QAction::triggered, this, [=](){this->onClearResults();});
    connect(&actionShowFilter, &QAction::triggered, this, [=](){this->onShowFilter(true);});
    connect(&actionHideFilter, &QAction::triggered, this, [=](){this->onShowFilter(false);});
    ///
    /// SAVE...
    ///
    connect(&actionSaveSubdomains, &QAction::triggered, this, [=](){this->onSaveResults(CHOICE::subdomain, PROXYMODEL_TYPE::subdomainIpProxy);});
    connect(&actionSaveIpAddresses, &QAction::triggered, this, [=](){this->onSaveResults(CHOICE::ip, PROXYMODEL_TYPE::subdomainIpProxy);});
    connect(&actionSaveAll, &QAction::triggered, this, [=](){this->onSaveResults(CHOICE::subdomainIp, PROXYMODEL_TYPE::subdomainIpProxy);});
    ///
    /// COPY...
    ///
    connect(&actionCopySubdomains, &QAction::triggered, this, [=](){this->onCopyResults(CHOICE::subdomain, PROXYMODEL_TYPE::subdomainIpProxy);});
    connect(&actionCopyIpAddresses, &QAction::triggered, this, [=](){this->onCopyResults(CHOICE::ip, PROXYMODEL_TYPE::subdomainIpProxy);});
    connect(&actionCopyAll, &QAction::triggered, this, [=](){this->onCopyResults(CHOICE::subdomainIp, PROXYMODEL_TYPE::subdomainIpProxy);});
    ///
    /// SUBDOMAINS AND IPS...
    ///
    connect(&actionSendToIp, &QAction::triggered, this, [=](){emit sendIpAddressesToIp(ENGINE::IP, CHOICE::ip, PROXYMODEL_TYPE::subdomainIpProxy); emit changeTabToIp();});
    connect(&actionSendToOsint, &QAction::triggered, this, [=](){emit sendSubdomainsToOsint(ENGINE::IP, CHOICE::subdomain, PROXYMODEL_TYPE::subdomainIpProxy); emit changeTabToOsint();});
    connect(&actionSendToBrute, &QAction::triggered, this, [=](){emit sendSubdomainsToBrute(ENGINE::IP, CHOICE::subdomain, PROXYMODEL_TYPE::subdomainIpProxy); emit changeTabToBrute();});
    connect(&actionSendToActive, &QAction::triggered, this, [=](){emit sendSubdomainsToActive(ENGINE::IP, CHOICE::subdomain, PROXYMODEL_TYPE::subdomainIpProxy); emit changeTabToActive();});
    connect(&actionSendToRecords, &QAction::triggered, this, [=](){emit sendSubdomainsToRecord(ENGINE::IP, CHOICE::subdomain, PROXYMODEL_TYPE::subdomainIpProxy); emit changeTabToRecords();});

    /**** For Right-Click Context-Menu ****/
    connect(&actionSave, &QAction::triggered, this, [=](){this->onSaveResults(selectionModel);});
    connect(&actionCopy, &QAction::triggered, this, [=](){this->onCopyResults(selectionModel);});
    //...
    connect(&actionOpenInBrowser, &QAction::triggered, this, [=](){this->openInBrowser(selectionModel);});
    //...
    connect(&actionSendToOsint_c, &QAction::triggered, this, [=](){emit sendSubdomainsToOsint(selectionModel); emit changeTabToOsint();});
    connect(&actionSendToIp_c, &QAction::triggered, this, [=](){emit sendIpAddressesToIp(selectionModel); emit changeTabToIp();});
    connect(&actionSendToBrute_c, &QAction::triggered, this, [=](){emit sendSubdomainsToBrute(selectionModel); emit changeTabToBrute();});
    connect(&actionSendToActive_c, &QAction::triggered, this, [=](){emit sendSubdomainsToActive(selectionModel); emit changeTabToActive();});
    connect(&actionSendToRecords_c, &QAction::triggered, this, [=](){emit sendSubdomainsToRecord(selectionModel); emit changeTabToRecords();});
}

void Ip::on_buttonAction_clicked(){
    ///
    /// check if there are results available else dont show the context menu...
    ///
    if(result->ip->subdomainIp->rowCount() < 1){
        return;
    }
    ///
    /// getting the position of the action button to place the context menu and
    /// showing the context menu right by the side of the action button...
    ///
    QPoint pos = ui->buttonAction->mapToGlobal(QPoint(0,0));
    pos = QPoint(pos.x()+65, pos.y());
    ///
    /// creating the context menu...
    ///
    QMenu *Menu = new QMenu(this);
    QMenu *saveMenu = new QMenu(this);
    QMenu *copyMenu = new QMenu(this);
    Menu->setAttribute(Qt::WA_DeleteOnClose, true);
    //...
    saveMenu->setTitle("Save");
    copyMenu->setTitle("Copy");
    ///
    /// ADDING ACTIONS TO THE CONTEXT MENU...
    ///
    saveMenu->addAction(&actionSaveSubdomains);
    saveMenu->addAction(&actionSaveIpAddresses);
    saveMenu->addAction(&actionSaveAll);
    //...
    copyMenu->addAction(&actionCopySubdomains);
    copyMenu->addAction(&actionCopyIpAddresses);
    copyMenu->addAction(&actionCopyAll);
    ///
    /// ....
    ///
    Menu->addAction(&actionClearResults);
    if(ui->lineEditFilter->isHidden() && ui->buttonFilter->isHidden()){
        Menu->addAction(&actionShowFilter);
    }else{
        Menu->addAction(&actionHideFilter);
    }
    Menu->addSeparator();
    //...
    Menu->addMenu(copyMenu);
    Menu->addMenu(saveMenu);
    //...
    Menu->addSeparator();
    Menu->addAction(&actionSendToIp);
    Menu->addAction(&actionSendToOsint);
    Menu->addAction(&actionSendToBrute);
    Menu->addAction(&actionSendToActive);
    Menu->addAction(&actionSendToRecords);
    ///
    /// showing the context menu...
    ///
    Menu->exec(pos);
}

void Ip::on_tableViewResults_customContextMenuRequested(const QPoint &pos){
    Q_UNUSED(pos);
    ///
    /// check if user right clicked on items else dont show the context menu...
    ///
    if(!ui->tableViewResults->selectionModel()->isSelected(ui->tableViewResults->currentIndex())){
        return;
    }
    selectionModel = ui->tableViewResults->selectionModel();
    ///
    /// creating the context menu...
    ///
    QMenu *Menu = new QMenu(this);
    Menu->setAttribute(Qt::WA_DeleteOnClose, true);
    ///
    /// ...
    ///
    Menu->addAction(&actionCopy);
    Menu->addAction(&actionSave);
    Menu->addSeparator();
    Menu->addAction(&actionOpenInBrowser);
    Menu->addSeparator();
    Menu->addAction(&actionSendToIp_c);
    Menu->addAction(&actionSendToOsint_c);
    Menu->addAction(&actionSendToBrute_c);
    Menu->addAction(&actionSendToActive_c);
    Menu->addAction(&actionSendToRecords_c);
    ///
    /// showing the menu...
    ///
    Menu->exec(QCursor::pos());
}

/********************************************************************************

*********************************************************************************/

void Ip::onSaveResults(CHOICE choice, PROXYMODEL_TYPE proxyType){
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
    switch(choice){
    case CHOICE::subdomain:
        for(int i = 0; i != result->ip->subdomainIpProxy->rowCount(); ++i)
        {
            item = result->ip->subdomainIpProxy->data(result->ip->subdomainIpProxy->index(i, 0)).toString().append(NEWLINE);
            if(!itemSet.contains(item)){
                itemSet.insert(item);
                file.write(item.toUtf8());
            }
        }
        break;
    case CHOICE::ip:
        for(int i = 0; i != result->ip->subdomainIpProxy->rowCount(); ++i)
        {
            item = result->ip->subdomainIpProxy->data(result->ip->subdomainIpProxy->index(i, 1)).toString().append(NEWLINE);
            if(!itemSet.contains(item)){
                itemSet.insert(item);
                file.write(item.toUtf8());
            }
        }
        break;
    case CHOICE::subdomainIp:
        for(int i = 0; i != result->ip->subdomainIpProxy->rowCount(); ++i)
        {
            item = result->ip->subdomainIpProxy->data(result->ip->subdomainIpProxy->index(i, 0)).toString()+":"+result->ip->subdomainIpProxy->data(result->ip->subdomainIpProxy->index(i, 1)).toString().append(NEWLINE);
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
    ///
    /// ...
    ///
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

void Ip::onCopyResults(CHOICE choice, PROXYMODEL_TYPE proxyType){
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
    switch(choice){
    case CHOICE::subdomain:
        for(int i = 0; i != result->ip->subdomainIpProxy->rowCount(); ++i)
        {
            item = result->ip->subdomainIpProxy->data(result->ip->subdomainIpProxy->index(i, 0)).toString().append(NEWLINE);
            if(!itemSet.contains(item)){
                itemSet.insert(item);
                clipboardData.append(item);
            }
        }
        break;
    case CHOICE::ip:
        for(int i = 0; i != result->ip->subdomainIpProxy->rowCount(); ++i)
        {
            item = result->ip->subdomainIpProxy->data(result->ip->subdomainIpProxy->index(i, 1)).toString().append(NEWLINE);
            if(!itemSet.contains(item)){
                itemSet.insert(item);
                clipboardData.append(item);
            }
        }
        break;
    case CHOICE::subdomainIp:
        for(int i = 0; i != result->ip->subdomainIpProxy->rowCount(); ++i)
        {
            item = result->ip->subdomainIpProxy->data(result->ip->subdomainIpProxy->index(i, 0)).toString()+"|"+result->ip->subdomainIpProxy->data(result->ip->subdomainIpProxy->index(i, 1)).toString().append(NEWLINE);
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
    ///
    /// ...
    ///
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
