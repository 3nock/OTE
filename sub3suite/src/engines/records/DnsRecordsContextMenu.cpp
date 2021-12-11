#include "DnsRecords.h"
#include "ui_DnsRecords.h"
//...
#include <QClipboard>

void DnsRecords::connectActions(){
    connect(&actionClearResults, &QAction::triggered, this, [=](){this->onClearResultsSrvRecords();});
    connect(&actionShowFilter, &QAction::triggered, this, [=](){this->onShowFilter(true);});
    connect(&actionHideFilter, &QAction::triggered, this, [=](){this->onShowFilter(false);});
    ///
    /// SAVE...
    ///
    connect(&actionSaveSRV, &QAction::triggered, this, [=](){this->onSaveResultsSrvRecords(CHOICE::srv);});
    ///
    /// COPY...
    ///
    connect(&actionCopySRV, &QAction::triggered, this, [=](){this->onCopyResultsSrvRecords(CHOICE::srv);});
    ///
    /// ...
    ///
    connect(&actionClearResults, &QAction::triggered, this, [=](){this->onClearResultsDnsRecords();});
    connect(&actionExpandResults, &QAction::triggered, this, [=](){this->onExpandResultsDnsRecords();});
    connect(&actionCollapseResults, &QAction::triggered, this, [=](){this->onCollapseResultsDnsRecords();});
    ///
    /// SAVE...
    ///
    connect(&actionSaveA, &QAction::triggered, this, [=](){this->onSaveResultsDnsRecords(CHOICE::A);});
    connect(&actionSaveAAAA, &QAction::triggered, this, [=](){this->onSaveResultsDnsRecords(CHOICE::AAAA);});
    connect(&actionSaveMX, &QAction::triggered, this, [=](){this->onSaveResultsDnsRecords(CHOICE::MX);});
    connect(&actionSaveNS, &QAction::triggered, this, [=](){this->onSaveResultsDnsRecords(CHOICE::NS);});
    connect(&actionSaveCNAME, &QAction::triggered, this, [=](){this->onSaveResultsDnsRecords(CHOICE::CNAME);});
    connect(&actionSaveTXT, &QAction::triggered, this, [=](){this->onSaveResultsDnsRecords(CHOICE::TXT);});
    ///
    /// COPY...
    ///
    connect(&actionCopyA, &QAction::triggered, this, [=](){this->onCopyResultsDnsRecords(CHOICE::A);});
    connect(&actionCopyAAAA, &QAction::triggered, this, [=](){this->onCopyResultsDnsRecords(CHOICE::AAAA);});
    connect(&actionCopyMX, &QAction::triggered, this, [=](){this->onCopyResultsDnsRecords(CHOICE::MX);});
    connect(&actionCopyNS, &QAction::triggered, this, [=](){this->onCopyResultsDnsRecords(CHOICE::NS);});
    connect(&actionCopyCNAME, &QAction::triggered, this, [=](){this->onCopyResultsDnsRecords(CHOICE::CNAME);});
    connect(&actionCopyTXT, &QAction::triggered, this, [=](){this->onCopyResultsDnsRecords(CHOICE::TXT);});
    ///
    /// SUBDOMAINS AND IPS...
    ///
    connect(&actionSendToIp, &QAction::triggered, this, [=](){emit sendIpAddressesToIp(ENGINE::RECORDS, CHOICE::ip, PROXYMODEL_TYPE::dnsProxy); emit changeTabToIp();});
    //...
    connect(&actionSendToOsint_dns, &QAction::triggered, this, [=](){emit sendSubdomainsToOsint(ENGINE::RECORDS, CHOICE::subdomain, PROXYMODEL_TYPE::dnsProxy); emit changeTabToOsint();});
    connect(&actionSendToBrute_dns, &QAction::triggered, this, [=](){emit sendSubdomainsToBrute(ENGINE::RECORDS, CHOICE::subdomain, PROXYMODEL_TYPE::dnsProxy); emit changeTabToBrute();});
    connect(&actionSendToActive_dns, &QAction::triggered, this, [=](){emit sendSubdomainsToActive(ENGINE::RECORDS, CHOICE::subdomain, PROXYMODEL_TYPE::dnsProxy); emit changeTabToActive();});
    connect(&actionSendToRecords_dns, &QAction::triggered, this, [=](){emit sendSubdomainsToRecord(ENGINE::RECORDS, CHOICE::subdomain, PROXYMODEL_TYPE::dnsProxy); emit changeTabToRecords();});
    //...
    connect(&actionSendToOsint_srv, &QAction::triggered, this, [=](){emit sendSubdomainsToOsint(ENGINE::RECORDS, CHOICE::subdomain, PROXYMODEL_TYPE::srvProxy); emit changeTabToOsint();});
    connect(&actionSendToBrute_srv, &QAction::triggered, this, [=](){emit sendSubdomainsToBrute(ENGINE::RECORDS, CHOICE::subdomain, PROXYMODEL_TYPE::srvProxy); emit changeTabToBrute();});
    connect(&actionSendToActive_srv, &QAction::triggered, this, [=](){emit sendSubdomainsToActive(ENGINE::RECORDS, CHOICE::subdomain, PROXYMODEL_TYPE::srvProxy); emit changeTabToActive();});
    connect(&actionSendToRecords_srv, &QAction::triggered, this, [=](){emit sendSubdomainsToRecord(ENGINE::RECORDS, CHOICE::subdomain, PROXYMODEL_TYPE::srvProxy); emit changeTabToRecords();});

    /**** For Right-Click Context Menu ****/
    ///
    /// ...
    ///
    connect(&actionSave, &QAction::triggered, this, [=](){this->onSaveResultsDnsRecords(selectionModel);});
    connect(&actionCopy, &QAction::triggered, this, [=](){this->onCopyResultsDnsRecords(selectionModel);});
    //...
    connect(&actionOpenInBrowser, &QAction::triggered, this, [=](){this->openInBrowser(selectionModel);});
    //...
    connect(&actionSendToOsint_c, &QAction::triggered, this, [=](){emit sendSubdomainsToOsint(selectionModel); emit changeTabToOsint();});
    connect(&actionSendToIp_c, &QAction::triggered, this, [=](){emit sendIpAddressesToIp(selectionModel); emit changeTabToIp();});
    connect(&actionSendToBrute_c, &QAction::triggered, this, [=](){emit sendSubdomainsToBrute(selectionModel); emit changeTabToBrute();});
    connect(&actionSendToActive_c, &QAction::triggered, this, [=](){emit sendSubdomainsToActive(selectionModel); emit changeTabToActive();});
    connect(&actionSendToRecords_c, &QAction::triggered, this, [=](){emit sendSubdomainsToRecord(selectionModel); emit changeTabToRecords();});
}

void DnsRecords::on_buttonAction_clicked(){
    ///
    /// check if there are results available else dont show the context menu...
    ///
    if(result->records->dns->rowCount() < 1 && result->records->srv->rowCount() < 1){
        return;
    }
    ///
    /// getting the position of the action button to place the context menu and
    /// showing the context menu right by the side of the action button...
    ///
    QPoint pos = ui->buttonAction->mapToGlobal(QPoint(0,0));
    pos = QPoint(pos.x()+65, pos.y());
    if(ui->tabWidgetResults->currentIndex()){
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
        Menu->addAction(&actionClearResults);
        if(ui->lineEditFilter->isHidden() && ui->buttonFilter->isHidden()){
            Menu->addAction(&actionShowFilter);
        }else{
            Menu->addAction(&actionHideFilter);
        }
        Menu->addSeparator();
        //...
        saveMenu->addAction(&actionSaveSRVName);
        //saveMenu->addAction(&actionSaveSRVTarget);
        //...
        copyMenu->addAction(&actionCopySRVName);
        //copyMenu->addAction(&actionCopySRVTarget);
        ///
        /// ....
        ///
        Menu->addMenu(copyMenu);
        Menu->addMenu(saveMenu);
        //...
        Menu->addSeparator();
        Menu->addAction(&actionSendToOsint);
        Menu->addAction(&actionSendToBrute);
        Menu->addAction(&actionSendToActive);
        Menu->addAction(&actionSendToRecords);
        ///
        /// showing the context menu...
        ///
        Menu->exec(pos);
    }
    else{
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
        ///
        saveMenu->addAction(&actionSaveA);
        saveMenu->addAction(&actionSaveAAAA);
        saveMenu->addAction(&actionSaveNS);
        saveMenu->addAction(&actionSaveMX);
        saveMenu->addAction(&actionSaveTXT);
        saveMenu->addAction(&actionSaveCNAME);
        //...
        copyMenu->addAction(&actionCopyA);
        copyMenu->addAction(&actionCopyAAAA);
        copyMenu->addAction(&actionCopyNS);
        copyMenu->addAction(&actionCopyMX);
        copyMenu->addAction(&actionCopyTXT);
        copyMenu->addAction(&actionCopyCNAME);
        ///
        /// ....
        ///
        Menu->addAction(&actionClearResults);
        Menu->addAction(&actionExpandResults);
        Menu->addAction(&actionCollapseResults);
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
}

void DnsRecords::on_treeViewResults_customContextMenuRequested(const QPoint &pos){
    Q_UNUSED(pos)
    if(!ui->treeViewResults->selectionModel()->isSelected(ui->treeViewResults->currentIndex())){
        return;
    }
    selectionModel = ui->treeViewResults->selectionModel();
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

void DnsRecords::on_tableViewSRV_customContextMenuRequested(const QPoint &pos){
    Q_UNUSED(pos)
    if(!ui->tableViewSRV->selectionModel()->isSelected(ui->tableViewSRV->currentIndex())){
        return;
    }
    selectionModel = ui->tableViewSRV->selectionModel();
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
    Menu->addAction(&actionSendToOsint_c);
    Menu->addAction(&actionSendToBrute_c);
    Menu->addAction(&actionSendToActive_c);
    Menu->addAction(&actionSendToRecords_c);
    ///
    /// showing the menu...
    ///
    Menu->exec(QCursor::pos());
}


/**********************************************************************************

***********************************************************************************/

void DnsRecords::onSaveResultsDnsRecords(CHOICE choice){
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
    /*
    switch(choice){
    case CHOICE::MX:
        for(int i = 0; i < m_modelDnsRecords->rowCount(); i++)
        {
            for(int j = 0; j < m_modelDnsRecords->item(i)->rowCount(); j++)
            {
                if(m_modelDnsRecords->item(i)->child(j)->text() == "MX"){
                    for(int k = 0; k < m_modelDnsRecords->item(i)->child(j)->rowCount(); k++)
                    {
                        item = m_modelDnsRecords->item(i)->child(j)->child(k)->text().append(NEWLINE);
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
        for(int i = 0; i < m_modelDnsRecords->rowCount(); i++)
        {
            for(int j = 0; j < m_modelDnsRecords->item(i)->rowCount(); j++)
            {
                if(m_modelDnsRecords->item(i)->child(j)->text() == "NS"){
                    for(int k = 0; k < m_modelDnsRecords->item(i)->child(j)->rowCount(); k++)
                    {
                        item = m_modelDnsRecords->item(i)->child(j)->child(k)->text().append(NEWLINE);
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
        for(int i = 0; i < m_modelDnsRecords->rowCount(); i++)
        {
            for(int j = 0; j < m_modelDnsRecords->item(i)->rowCount(); j++)
            {
                if(m_modelDnsRecords->item(i)->child(j)->text() == "TXT"){
                    for(int k = 0; k < m_modelDnsRecords->item(i)->child(j)->rowCount(); k++)
                    {
                        item = m_modelDnsRecords->item(i)->child(j)->child(k)->text().append(NEWLINE);
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
        for(int i = 0; i < m_modelDnsRecords->rowCount(); i++)
        {
            for(int j = 0; j < m_modelDnsRecords->item(i)->rowCount(); j++)
            {
                if(m_modelDnsRecords->item(i)->child(j)->text() == "CNAME"){
                    for(int k = 0; k < m_modelDnsRecords->item(i)->child(j)->rowCount(); k++)
                    {
                        item = m_modelDnsRecords->item(i)->child(j)->child(k)->text().append(NEWLINE);
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
        for(int i = 0; i < m_modelDnsRecords->rowCount(); i++)
        {
            for(int j = 0; j < m_modelDnsRecords->item(i)->rowCount(); j++)
            {
                if(m_modelDnsRecords->item(i)->child(j)->text() == "A"){
                    for(int k = 0; k < m_modelDnsRecords->item(i)->child(j)->rowCount(); k++)
                    {
                        item = m_modelDnsRecords->item(i)->child(j)->child(k)->text().append(NEWLINE);
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
        for(int i = 0; i < m_modelDnsRecords->rowCount(); i++)
        {
            for(int j = 0; j < m_modelDnsRecords->item(i)->rowCount(); j++)
            {
                if(m_modelDnsRecords->item(i)->child(j)->text() == "AAAA"){
                    for(int k = 0; k < m_modelDnsRecords->item(i)->child(j)->rowCount(); k++)
                    {
                        item = m_modelDnsRecords->item(i)->child(j)->child(k)->text().append(NEWLINE);
                        if(!itemSet.contains(item)){
                            itemSet.insert(item);
                            file.write(item.toUtf8());
                        }
                    }
                }
            }
        }
        break;
    default:
        break;
    }
    file.close();
    */
}

void DnsRecords::onSaveResultsDnsRecords(QItemSelectionModel* selectionModel){
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

void DnsRecords::onCopyResultsDnsRecords(CHOICE choice){
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
    /*
    switch(choice){
    case CHOICE::MX:
        for(int i = 0; i < m_modelDnsRecords->rowCount(); i++)
        {
            for(int j = 0; j < m_modelDnsRecords->item(i)->rowCount(); j++)
            {
                if(m_modelDnsRecords->item(i)->child(j)->text() == "MX"){
                    for(int k = 0; k < m_modelDnsRecords->item(i)->child(j)->rowCount(); k++)
                    {
                        item = m_modelDnsRecords->item(i)->child(j)->child(k)->text().append(NEWLINE);
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
        for(int i = 0; i < m_modelDnsRecords->rowCount(); i++)
        {
            for(int j = 0; j < m_modelDnsRecords->item(i)->rowCount(); j++)
            {
                if(m_modelDnsRecords->item(i)->child(j)->text() == "NS"){
                    for(int k = 0; k < m_modelDnsRecords->item(i)->child(j)->rowCount(); k++)
                    {
                        item = m_modelDnsRecords->item(i)->child(j)->child(k)->text().append(NEWLINE);
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
        for(int i = 0; i < m_modelDnsRecords->rowCount(); i++)
        {
            for(int j = 0; j < m_modelDnsRecords->item(i)->rowCount(); j++)
            {
                if(m_modelDnsRecords->item(i)->child(j)->text() == "TXT"){
                    for(int k = 0; k < m_modelDnsRecords->item(i)->child(j)->rowCount(); k++)
                    {
                        item = m_modelDnsRecords->item(i)->child(j)->child(k)->text().append(NEWLINE);
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
        for(int i = 0; i < m_modelDnsRecords->rowCount(); i++)
        {
            for(int j = 0; j < m_modelDnsRecords->item(i)->rowCount(); j++)
            {
                if(m_modelDnsRecords->item(i)->child(j)->text() == "CNAME"){
                    for(int k = 0; k < m_modelDnsRecords->item(i)->child(j)->rowCount(); k++)
                    {
                        item = m_modelDnsRecords->item(i)->child(j)->child(k)->text().append(NEWLINE);
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
        for(int i = 0; i < m_modelDnsRecords->rowCount(); i++)
        {
            for(int j = 0; j < m_modelDnsRecords->item(i)->rowCount(); j++)
            {
                if(m_modelDnsRecords->item(i)->child(j)->text() == "A"){
                    for(int k = 0; k < m_modelDnsRecords->item(i)->child(j)->rowCount(); k++)
                    {
                        item = m_modelDnsRecords->item(i)->child(j)->child(k)->text().append(NEWLINE);
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
        for(int i = 0; i < m_modelDnsRecords->rowCount(); i++)
        {
            for(int j = 0; j < m_modelDnsRecords->item(i)->rowCount(); j++)
            {
                if(m_modelDnsRecords->item(i)->child(j)->text() == "AAAA"){
                    for(int k = 0; k < m_modelDnsRecords->item(i)->child(j)->rowCount(); k++)
                    {
                        item = m_modelDnsRecords->item(i)->child(j)->child(k)->text().append(NEWLINE);
                        if(!itemSet.contains(item)){
                            itemSet.insert(item);
                            clipboardData.append(item);
                        }
                    }
                }
            }
        }
        break;
    default:
        break;
    }
    clipboard->setText(clipboardData);
    */
}

void DnsRecords::onCopyResultsDnsRecords(QItemSelectionModel* selectionModel){
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

void DnsRecords::onSaveResultsSrvRecords(CHOICE choice){
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
    for(int i = 0; i != result->records->srvProxy->rowCount(); ++i)
    {
        item = result->records->srvProxy->data(result->records->srvProxy->index(i, 1)).toString().append(NEWLINE);
        if(!itemSet.contains(item)){
            itemSet.insert(item);
            file.write(item.toUtf8());
        }
    }
    file.close();
}

void DnsRecords::onSaveResultsSrvRecords(QItemSelectionModel *selectionModel){
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

void DnsRecords::onCopyResultsSrvRecords(CHOICE choice){
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
    for(int i = 0; i != result->records->srvProxy->rowCount(); ++i)
    {
        item = result->records->srvProxy->data(result->records->srvProxy->index(i, 1)).toString().append(NEWLINE);
        if(!itemSet.contains(item)){
            itemSet.insert(item);
            clipboardData.append(item);
        }
    }
}

void DnsRecords::onCopyResultsSrvRecords(QItemSelectionModel *selectionModel){
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
