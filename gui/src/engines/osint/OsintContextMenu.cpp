#include "Osint.h"
#include "ui_Osint.h"
//...
#include <QClipboard>


void Osint::connectActions(){
    connect(&actionClearResults, &QAction::triggered, this, [=](){this->onClearResults();});
    ///
    /// SAVE...
    ///
    connect(&actionSaveSubdomains, &QAction::triggered, this, [=](){this->onSaveResults(CHOICE::subdomain, PROXYMODEL_TYPE::subdomainIpProxy);});
    connect(&actionSaveIpAddresses, &QAction::triggered, this, [=](){this->onSaveResults(CHOICE::ip, PROXYMODEL_TYPE::subdomainIpProxy);});
    connect(&actionSaveAll, &QAction::triggered, this, [=](){this->onSaveResults(CHOICE::subdomainIp, PROXYMODEL_TYPE::subdomainIpProxy);});
    connect(&actionSaveEmails, &QAction::triggered, this, [=](){this->onSaveResults(CHOICE::email, PROXYMODEL_TYPE::emailProxy);});
    connect(&actionSaveUrls, &QAction::triggered, this, [=](){this->onSaveResults(CHOICE::url, PROXYMODEL_TYPE::urlProxy);});
    connect(&actionSaveAsns, &QAction::triggered, this, [=](){this->onSaveResults(CHOICE::asn, PROXYMODEL_TYPE::asnProxy);});
    connect(&actionSaveCerts, &QAction::triggered, this, [=](){this->onSaveResults(CHOICE::cert, PROXYMODEL_TYPE::sslCertProxy);});
    //...
    connect(&actionSaveSubdomains_subdomain, &QAction::triggered, this, [=](){this->onSaveResults(CHOICE::subdomain, PROXYMODEL_TYPE::subdomainProxy);});
    connect(&actionSaveIpAddresses_ip, &QAction::triggered, this, [=](){this->onSaveResults(CHOICE::ip, PROXYMODEL_TYPE::ipProxy);});

    ///
    /// COPY...
    ///
    connect(&actionCopySubdomains, &QAction::triggered, this, [=](){this->onCopyResults(CHOICE::subdomain, PROXYMODEL_TYPE::subdomainIpProxy);});
    connect(&actionCopyIpAddresses, &QAction::triggered, this, [=](){this->onCopyResults(CHOICE::ip, PROXYMODEL_TYPE::subdomainIpProxy);});
    connect(&actionCopyAll, &QAction::triggered, this, [=](){this->onCopyResults(CHOICE::subdomainIp, PROXYMODEL_TYPE::subdomainIpProxy);});
    connect(&actionCopyEmails, &QAction::triggered, this, [=](){this->onCopyResults(CHOICE::email, PROXYMODEL_TYPE::emailProxy);});
    connect(&actionCopyUrls, &QAction::triggered, this, [=](){this->onCopyResults(CHOICE::url, PROXYMODEL_TYPE::urlProxy);});
    connect(&actionCopyAsns, &QAction::triggered, this, [=](){this->onCopyResults(CHOICE::asn, PROXYMODEL_TYPE::asnProxy);});
    connect(&actionCopyCerts, &QAction::triggered, this, [=](){this->onCopyResults(CHOICE::cert, PROXYMODEL_TYPE::sslCertProxy);});
    //...
    connect(&actionCopySubdomains_subdomain, &QAction::triggered, this, [=](){this->onCopyResults(CHOICE::subdomain, PROXYMODEL_TYPE::subdomainProxy);});
    connect(&actionCopyIpAddresses_ip, &QAction::triggered, this, [=](){this->onCopyResults(CHOICE::ip, PROXYMODEL_TYPE::ipProxy);});
    ///
    /// SUBDOMAINS AND IPS...
    ///
    connect(&actionSendToIp, &QAction::triggered, this, [=](){emit sendIpAddressesToIp(ENGINE::OSINT, CHOICE::ip, PROXYMODEL_TYPE::subdomainIpProxy); emit changeTabToIp();});
    connect(&actionSendToOsint, &QAction::triggered, this, [=](){emit sendSubdomainsToOsint(ENGINE::OSINT, CHOICE::subdomain, PROXYMODEL_TYPE::subdomainIpProxy); emit changeTabToOsint();});
    connect(&actionSendToBrute, &QAction::triggered, this, [=](){emit sendSubdomainsToBrute(ENGINE::OSINT, CHOICE::subdomain, PROXYMODEL_TYPE::subdomainIpProxy); emit changeTabToBrute();});
    connect(&actionSendToActive, &QAction::triggered, this, [=](){emit sendSubdomainsToActive(ENGINE::OSINT, CHOICE::subdomain, PROXYMODEL_TYPE::subdomainIpProxy); emit changeTabToActive();});
    connect(&actionSendToRecords, &QAction::triggered, this, [=](){emit sendSubdomainsToRecord(ENGINE::OSINT, CHOICE::subdomain, PROXYMODEL_TYPE::subdomainIpProxy); emit changeTabToRecords();});
    //...
    connect(&actionSendToIp_ip, &QAction::triggered, this, [=](){emit sendIpAddressesToIp(ENGINE::OSINT, CHOICE::ip, PROXYMODEL_TYPE::ipProxy); emit changeTabToIp();});
    connect(&actionSendToOsint_subdomain, &QAction::triggered, this, [=](){emit sendSubdomainsToOsint(ENGINE::OSINT, CHOICE::subdomain, PROXYMODEL_TYPE::subdomainProxy); emit changeTabToOsint();});
    connect(&actionSendToBrute_subdomain, &QAction::triggered, this, [=](){emit sendSubdomainsToBrute(ENGINE::OSINT, CHOICE::subdomain, PROXYMODEL_TYPE::subdomainProxy); emit changeTabToBrute();});
    connect(&actionSendToActive_subdomain, &QAction::triggered, this, [=](){emit sendSubdomainsToActive(ENGINE::OSINT, CHOICE::subdomain, PROXYMODEL_TYPE::subdomainProxy); emit changeTabToActive();});
    connect(&actionSendToRecords_subdomain, &QAction::triggered, this, [=](){emit sendSubdomainsToRecord(ENGINE::OSINT, CHOICE::subdomain, PROXYMODEL_TYPE::subdomainProxy); emit changeTabToRecords();});
    /**** For Right-Click ContextMenu ****/
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

void Osint::on_buttonAction_clicked(){
    int option = ui->comboBoxOption->currentIndex();

    switch(option){
    case SUBDOMAINIP:
        if(result->osint->subdomainIpProxy->rowCount() < 1)
            return;
        break;
    case SUBDOMAIN:
        if(result->osint->subdomainProxy->rowCount() < 1)
            return;
        break;
    case IP:
        if(result->osint->ipProxy->rowCount() < 1)
            return;
        break;
    case EMAIL:
        if(result->osint->emailProxy->rowCount() < 1)
            return;
        break;
    case URL:
        if(result->osint->urlProxy->rowCount() < 1)
            return;
        break;
    case ASN:
        if(result->osint->asnProxy->rowCount() < 1)
            return;
        break;
    case SSLCERT:
        if(result->osint->sslCertProxy->rowCount() < 1)
            return;
        break;
    }

    QPoint pos = ui->buttonAction->mapToGlobal(QPoint(0,0));
    pos = QPoint(pos.x()+65, pos.y());

    QMenu *Menu = new QMenu(this);
    QMenu *saveMenu = new QMenu(this);
    QMenu *copyMenu = new QMenu(this);
    Menu->setAttribute(Qt::WA_DeleteOnClose, true);

    saveMenu->setTitle("Save");
    copyMenu->setTitle("Copy");

    ///
    /// ADDING ACTIONS TO THE CONTEXT MENU...
    ///
    if(option == SUBDOMAINIP){
        saveMenu->addAction(&actionSaveSubdomains);
        copyMenu->addAction(&actionCopySubdomains);
        saveMenu->addAction(&actionSaveIpAddresses);
        copyMenu->addAction(&actionCopyIpAddresses);
        saveMenu->addAction(&actionSaveAll);
        copyMenu->addAction(&actionCopyAll);
    }
    if(option == SUBDOMAIN){
        saveMenu->addAction(&actionSaveSubdomains_subdomain);
        copyMenu->addAction(&actionCopySubdomains_subdomain);
    }
    if(option == IP){
        saveMenu->addAction(&actionSaveIpAddresses_ip);
        copyMenu->addAction(&actionCopyIpAddresses_ip);
    }
    if(option == EMAIL){
        saveMenu->addAction(&actionSaveEmails);
        copyMenu->addAction(&actionCopyEmails);
    }
    if(option == URL){
        saveMenu->addAction(&actionSaveUrls);
        copyMenu->addAction(&actionCopyUrls);
    }
    if(option == ASN){
        saveMenu->addAction(&actionSaveAsns);
        copyMenu->addAction(&actionCopyAsns);
    }
    if(option == SSLCERT){
        saveMenu->addAction(&actionSaveCerts);
        copyMenu->addAction(&actionCopyCerts);
    }
    Menu->addAction(&actionClearResults);
    Menu->addSeparator();
    Menu->addMenu(copyMenu);
    Menu->addMenu(saveMenu);
    Menu->addSeparator();
    if(option == IP)
        Menu->addAction(&actionSendToIp_ip);
    if(option == SUBDOMAINIP){
        Menu->addAction(&actionSendToIp);
        Menu->addAction(&actionSendToOsint);
        Menu->addAction(&actionSendToBrute);
        Menu->addAction(&actionSendToActive);
        Menu->addAction(&actionSendToRecords);
    }
    if(option == SUBDOMAIN){
        Menu->addAction(&actionSendToOsint_subdomain);
        Menu->addAction(&actionSendToBrute_subdomain);
        Menu->addAction(&actionSendToActive_subdomain);
        Menu->addAction(&actionSendToRecords_subdomain);
    }
    ///
    /// showing the context menu...
    ///
    Menu->exec(pos);
}

void Osint::on_tableViewResults_customContextMenuRequested(const QPoint &pos){
    Q_UNUSED(pos);
    int option = ui->comboBoxOption->currentIndex();

    if(!ui->tableViewResults->selectionModel()->isSelected(ui->tableViewResults->currentIndex())){
        return;
    }
    selectionModel = ui->tableViewResults->selectionModel();

    QMenu *Menu = new QMenu(this);
    Menu->setAttribute(Qt::WA_DeleteOnClose, true);

    Menu->addAction(&actionCopy);
    Menu->addAction(&actionSave);
    Menu->addSeparator();
    if(option != EMAIL){
        Menu->addAction(&actionOpenInBrowser);
        Menu->addSeparator();
    }
    if(option == SUBDOMAINIP || option == IP)
        Menu->addAction(&actionSendToIp_c);
    if(option == SUBDOMAINIP || option == SUBDOMAIN){
        Menu->addAction(&actionSendToOsint_c);
        Menu->addAction(&actionSendToBrute_c);
        Menu->addAction(&actionSendToActive_c);
        Menu->addAction(&actionSendToRecords_c);
    }
    ///
    /// showing the menu...
    ///
    Menu->exec(QCursor::pos());
}


/********************************************************************************

*********************************************************************************/

void Osint::onSaveResults(CHOICE choice, PROXYMODEL_TYPE proxyType){
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

    QString item;
    ui->plainTextEditLogs->setPlainText("hi there...");
    switch(proxyType){
    case PROXYMODEL_TYPE::subdomainIpProxy:
    {
        switch(choice){
        case CHOICE::subdomain:
            for(int i = 0; i != result->osint->subdomainIpProxy->rowCount(); ++i)
            {
                item = result->osint->subdomainIpProxy->data(result->osint->subdomainIpProxy->index(i, 0)).toString().append(NEWLINE);
                file.write(item.toUtf8());
            }
            break;
        case CHOICE::ip:
        {
            QSet<QString> itemSet;
            for(int i = 0; i != result->osint->subdomainIpProxy->rowCount(); ++i)
            {
                item = result->osint->subdomainIpProxy->data(result->osint->subdomainIpProxy->index(i, 1)).toString().append(NEWLINE);
                if(!itemSet.contains(item)){
                    itemSet.insert(item);
                    file.write(item.toUtf8());
                }
            }
            break;
        }
        case CHOICE::subdomainIp:
            for(int i = 0; i != result->osint->subdomainIpProxy->rowCount(); ++i)
            {
                item = result->osint->subdomainIpProxy->data(result->osint->subdomainIpProxy->index(i, 0)).toString()+":"+result->osint->subdomainIpProxy->data(result->osint->subdomainIpProxy->index(i, 1)).toString().append(NEWLINE);
                file.write(item.toUtf8());
            }
            break;
        default:
            break;
        }
        break;
    }

    case PROXYMODEL_TYPE::subdomainProxy:
        ui->plainTextEditLogs->setPlainText("hello there...");
        for(int i = 0; i != result->osint->subdomainProxy->rowCount(); ++i)
        {
            item = result->osint->subdomainProxy->data(result->osint->subdomainProxy->index(i, 0)).toString().append(NEWLINE);
            file.write(item.toUtf8());
        }
        break;

    case PROXYMODEL_TYPE::ipProxy:
        for(int i = 0; i != result->osint->ipProxy->rowCount(); ++i)
        {
            item = result->osint->ipProxy->data(result->osint->ipProxy->index(i, 0)).toString().append(NEWLINE);
            file.write(item.toUtf8());
        }
        break;

    case PROXYMODEL_TYPE::emailProxy:
        for(int i = 0; i != result->osint->emailProxy->rowCount(); ++i)
        {
            item = result->osint->emailProxy->data(result->osint->emailProxy->index(i, 0)).toString().append(NEWLINE);
            file.write(item.toUtf8());
        }
        break;

    case PROXYMODEL_TYPE::urlProxy:
        for(int i = 0; i != result->osint->urlProxy->rowCount(); ++i)
        {
            item = result->osint->urlProxy->data(result->osint->urlProxy->index(i, 0)).toString().append(NEWLINE);
            file.write(item.toUtf8());
        }
        break;

    case PROXYMODEL_TYPE::asnProxy:
        for(int i = 0; i != result->osint->asnProxy->rowCount(); ++i)
        {
            item = result->osint->asnProxy->data(result->osint->asnProxy->index(i, 0)).toString().append(NEWLINE);
            file.write(item.toUtf8());
        }
        break;

    case PROXYMODEL_TYPE::sslCertProxy:
        for(int i = 0; i != result->osint->sslCertProxy->rowCount(); ++i)
        {
            item = result->osint->sslCertProxy->data(result->osint->sslCertProxy->index(i, 0)).toString().append(NEWLINE);
            file.write(item.toUtf8());
        }
        break;

    default:
        break;
    }
    file.close();
}

void Osint::onSaveResults(QItemSelectionModel *selectionModel){
    QString filename = QFileDialog::getSaveFileName(this, "Save To File", "./");
    if(filename.isEmpty()){
        return;
    }
    QString data;
    QString item;
    QFile file(filename);

    if(file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        foreach(const QModelIndex &index, selectionModel->selectedIndexes()){
            item = index.data().toString();
            data.append(item.append(NEWLINE));
        }
        file.write(data.toUtf8());
        file.close();
    }
}

void Osint::onCopyResults(CHOICE choice, PROXYMODEL_TYPE proxyType){
    QClipboard *clipboard = QGuiApplication::clipboard();
    QString clipboardData;
    QString item;

    switch(proxyType){
    case PROXYMODEL_TYPE::subdomainIpProxy:
    {
        switch(choice){
        case CHOICE::subdomain:
            for(int i = 0; i != result->osint->subdomainIpProxy->rowCount(); ++i)
            {
                item = result->osint->subdomainIpProxy->data(result->osint->subdomainIpProxy->index(i, 0)).toString().append(NEWLINE);
                clipboardData.append(item);
            }
            break;
        case CHOICE::ip:
        {
            QSet<QString> itemSet;
            for(int i = 0; i != result->osint->subdomainIpProxy->rowCount(); ++i)
            {
                item = result->osint->subdomainIpProxy->data(result->osint->subdomainIpProxy->index(i, 1)).toString().append(NEWLINE);
                if(!itemSet.contains(item)){
                    itemSet.insert(item);
                    clipboardData.append(item);
                }
            }
            break;
        }
        case CHOICE::subdomainIp:
            for(int i = 0; i != result->osint->subdomainIpProxy->rowCount(); ++i)
            {
                item = result->osint->subdomainIpProxy->data(result->osint->subdomainIpProxy->index(i, 0)).toString()+":"+result->osint->subdomainIpProxy->data(result->osint->subdomainIpProxy->index(i, 1)).toString().append(NEWLINE);
                clipboardData.append(item);
            }
            break;
        default:
            break;
        }
        break;
    }

    case PROXYMODEL_TYPE::subdomainProxy:
        for(int i = 0; i != result->osint->subdomainProxy->rowCount(); ++i)
        {
            item = result->osint->subdomainProxy->data(result->osint->subdomainProxy->index(i, 0)).toString().append(NEWLINE);
            clipboardData.append(item);
        }
        break;

    case PROXYMODEL_TYPE::ipProxy:
        for(int i = 0; i != result->osint->ipProxy->rowCount(); ++i)
        {
            item = result->osint->ipProxy->data(result->osint->ipProxy->index(i, 0)).toString().append(NEWLINE);
            clipboardData.append(item);
        }
        break;

    case PROXYMODEL_TYPE::emailProxy:
        for(int i = 0; i != result->osint->emailProxy->rowCount(); ++i)
        {
            item = result->osint->emailProxy->data(result->osint->emailProxy->index(i, 0)).toString().append(NEWLINE);
            clipboardData.append(item);
        }
        break;

    case PROXYMODEL_TYPE::urlProxy:
        for(int i = 0; i != result->osint->urlProxy->rowCount(); ++i)
        {
            item = result->osint->urlProxy->data(result->osint->urlProxy->index(i, 0)).toString().append(NEWLINE);
            clipboardData.append(item);
        }
        break;

    case PROXYMODEL_TYPE::asnProxy:
        for(int i = 0; i != result->osint->asnProxy->rowCount(); ++i)
        {
            item = result->osint->asnProxy->data(result->osint->asnProxy->index(i, 0)).toString().append(NEWLINE);
            clipboardData.append(item);
        }
        break;

    case PROXYMODEL_TYPE::sslCertProxy:
        for(int i = 0; i != result->osint->sslCertProxy->rowCount(); ++i)
        {
            item = result->osint->sslCertProxy->data(result->osint->sslCertProxy->index(i, 0)).toString().append(NEWLINE);
            clipboardData.append(item);
        }
        break;

    default:
        break;
    }
    clipboard->setText(clipboardData);
}

void Osint::onCopyResults(QItemSelectionModel *selectionModel){
    QClipboard *clipboard = QGuiApplication::clipboard();
    QString data, item;

    foreach(const QModelIndex &index, selectionModel->selectedIndexes())
    {
        item = index.data().toString();
        data.append(item.append(NEWLINE));
    }
    clipboard->setText(data);
}
