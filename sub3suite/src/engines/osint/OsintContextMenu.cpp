/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#include "Osint.h"
#include "ui_Osint.h"

#include <QClipboard>
#include <QDesktopServices>


void Osint::m_initActions(){
    connect(&a_ClearResults, &QAction::triggered, this, [=](){this->m_clearResults();});
    connect(&a_RemoveResults, &QAction::triggered, this, [=](){this->m_removeResults(selectionModel);});
    connect(&a_OpenInBrowser, &QAction::triggered, this, [=](){this->m_openInBrowser(selectionModel);});
    /* ... */
    connect(&a_SendAllIpToOsint, &QAction::triggered, this, [=](){this->m_sendIpToEngine(ENGINE::OSINT);});
    connect(&a_SendAllIpToRaw, &QAction::triggered, this, [=](){this->m_sendIpToEngine(ENGINE::RAW);});
    connect(&a_SendAllHostToOsint, &QAction::triggered, this, [=](){this->m_sendSubdomainToEngine(ENGINE::OSINT);});
    connect(&a_SendAllHostToRaw, &QAction::triggered, this, [=](){this->m_sendSubdomainToEngine(ENGINE::RAW);});
    connect(&a_SendAllHostToBrute, &QAction::triggered, this, [=](){this->m_sendSubdomainToEngine(ENGINE::BRUTE);});
    connect(&a_SendAllHostToActive, &QAction::triggered, this, [=](){this->m_sendSubdomainToEngine(ENGINE::ACTIVE);});
    connect(&a_SendAllHostToDns, &QAction::triggered, this, [=](){this->m_sendSubdomainToEngine(ENGINE::DNS);});
    connect(&a_SendAllHostToCert, &QAction::triggered, this, [=](){this->m_sendSubdomainToEngine(ENGINE::CERT);});
    connect(&a_SendAllEmailToRaw, &QAction::triggered, this, [=](){this->m_sendToEngine(ENGINE::RAW);});
    connect(&a_SendAllUrlToRaw, &QAction::triggered, this, [=](){this->m_sendToEngine(ENGINE::RAW);});
    connect(&a_SendAllAsnToRaw, &QAction::triggered, this, [=](){this->m_sendToEngine(ENGINE::RAW);});
    connect(&a_SendAllCidrToRaw, &QAction::triggered, this, [=](){this->m_sendToEngine(ENGINE::RAW);});
    connect(&a_SendAllCertToRaw, &QAction::triggered, this, [=](){this->m_sendToEngine(ENGINE::RAW);});
    connect(&a_SendAllEmailToOsint, &QAction::triggered, this, [=](){this->m_sendToEngine(ENGINE::OSINT);});
    connect(&a_SendAllAsnToOsint, &QAction::triggered, this, [=](){this->m_sendToEngine(ENGINE::OSINT);});
    connect(&a_SendAllCidrToOsint, &QAction::triggered, this, [=](){this->m_sendToEngine(ENGINE::OSINT);});
    connect(&a_SendAllCertToOsint, &QAction::triggered, this, [=](){this->m_sendToEngine(ENGINE::OSINT);});
    connect(&a_SendAllIpToIpTool, &QAction::triggered, this, [=](){this->m_sendIpToTool(TOOL::IP);});
    connect(&a_SendAllHostToCertTool, &QAction::triggered, this, [=](){this->m_sendToTool(TOOL::DOMAINTOOL);});
    connect(&a_SendAllHostToDomainTool, &QAction::triggered, this, [=](){this->m_sendToTool(TOOL::CERT);});
    connect(&a_SendAllAsnToAsnTool, &QAction::triggered, this, [=](){this->m_sendToTool(TOOL::ASN);});
    connect(&a_SendAllEmailToEmailTool, &QAction::triggered, this, [=](){this->m_sendToTool(TOOL::EMAIL);});
    connect(&a_SendAllCidrToCidrTool, &QAction::triggered, this, [=](){this->m_sendToTool(TOOL::CIDR);});
    connect(&a_SendAllCertToCertTool, &QAction::triggered, this, [=](){this->m_sendToTool(TOOL::CERT);});
    /* ... */
    connect(&a_SendSelectedIpToOsint, &QAction::triggered, this, [=](){this->m_sendIpToEngine(ENGINE::OSINT, selectionModel);});
    connect(&a_SendSelectedIpToRaw, &QAction::triggered, this, [=](){this->m_sendIpToEngine(ENGINE::RAW, selectionModel);});
    connect(&a_SendSelectedHostToOsint, &QAction::triggered, this, [=](){this->m_sendSubdomainToEngine(ENGINE::OSINT, selectionModel);});
    connect(&a_SendSelectedHostToRaw, &QAction::triggered, this, [=](){this->m_sendSubdomainToEngine(ENGINE::RAW, selectionModel);});
    connect(&a_SendSelectedHostToBrute, &QAction::triggered, this, [=](){this->m_sendSubdomainToEngine(ENGINE::BRUTE, selectionModel);});
    connect(&a_SendSelectedHostToActive, &QAction::triggered, this, [=](){this->m_sendSubdomainToEngine(ENGINE::ACTIVE, selectionModel);});
    connect(&a_SendSelectedHostToDns, &QAction::triggered, this, [=](){this->m_sendSubdomainToEngine(ENGINE::DNS, selectionModel);});
    connect(&a_SendSelectedHostToCert, &QAction::triggered, this, [=](){this->m_sendSubdomainToEngine(ENGINE::CERT, selectionModel);});
    connect(&a_SendSelectedEmailToRaw, &QAction::triggered, this, [=](){this->m_sendToEngine(ENGINE::RAW, selectionModel);});
    connect(&a_SendSelectedUrlToRaw, &QAction::triggered, this, [=](){this->m_sendToEngine(ENGINE::RAW, selectionModel);});
    connect(&a_SendSelectedAsnToRaw, &QAction::triggered, this, [=](){this->m_sendToEngine(ENGINE::RAW, selectionModel);});
    connect(&a_SendSelectedCidrToRaw, &QAction::triggered, this, [=](){this->m_sendToEngine(ENGINE::RAW, selectionModel);});
    connect(&a_SendSelectedCertToRaw, &QAction::triggered, this, [=](){this->m_sendToEngine(ENGINE::RAW, selectionModel);});
    connect(&a_SendSelectedEmailToOsint, &QAction::triggered, this, [=](){this->m_sendToEngine(ENGINE::OSINT, selectionModel);});
    connect(&a_SendSelectedAsnToOsint, &QAction::triggered, this, [=](){this->m_sendToEngine(ENGINE::OSINT, selectionModel);});
    connect(&a_SendSelectedCidrToOsint, &QAction::triggered, this, [=](){this->m_sendToEngine(ENGINE::OSINT, selectionModel);});
    connect(&a_SendSelectedCertToOsint, &QAction::triggered, this, [=](){this->m_sendToEngine(ENGINE::OSINT, selectionModel);});
    connect(&a_SendSelectedIpToIpTool, &QAction::triggered, this, [=](){this->m_sendIpToTool(TOOL::IP, selectionModel);});
    connect(&a_SendSelectedHostToCertTool, &QAction::triggered, this, [=](){this->m_sendToTool(TOOL::DOMAINTOOL, selectionModel);});
    connect(&a_SendSelectedHostToDomainTool, &QAction::triggered, this, [=](){this->m_sendToTool(TOOL::CERT, selectionModel);});
    connect(&a_SendSelectedAsnToAsnTool, &QAction::triggered, this, [=](){this->m_sendToTool(TOOL::ASN, selectionModel);});
    connect(&a_SendSelectedEmailToEmailTool, &QAction::triggered, this, [=](){this->m_sendToTool(TOOL::EMAIL, selectionModel);});
    connect(&a_SendSelectedCidrToCidrTool, &QAction::triggered, this, [=](){this->m_sendToTool(TOOL::CIDR, selectionModel);});
    connect(&a_SendSelectedCertToCertTool, &QAction::triggered, this, [=](){this->m_sendToTool(TOOL::CERT, selectionModel);});
    /* ... */
    connect(&a_Save, &QAction::triggered, this, [=](){this->m_saveResults(selectionModel);});
    connect(&a_SaveSubdomainIp, &QAction::triggered, this, [=](){this->m_saveResults(RESULT_TYPE::SUBDOMAINIP);});
    connect(&a_SaveSubdomain, &QAction::triggered, this, [=](){this->m_saveResults(RESULT_TYPE::SUBDOMAIN);});
    connect(&a_SaveIp, &QAction::triggered, this, [=](){this->m_saveResults(RESULT_TYPE::IP);});
    connect(&a_Copy, &QAction::triggered, this, [=](){this->m_copyResults(selectionModel);});
    connect(&a_CopySubdomainIp, &QAction::triggered, this, [=](){this->m_copyResults(RESULT_TYPE::SUBDOMAINIP);});
    connect(&a_CopySubdomain, &QAction::triggered, this, [=](){this->m_copyResults(RESULT_TYPE::SUBDOMAIN);});
    connect(&a_CopyIp, &QAction::triggered, this, [=](){this->m_copyResults(RESULT_TYPE::IP);});
}

void Osint::on_buttonAction_clicked(){
    /* check if there are results available else dont show the context menu */
    if(m_resultProxyModel->rowCount() < 1)
        return;

    /* getting the position of the action button to place the context menu and
       showing the context menu right by the side of the action button... */
    QPoint pos = ui->buttonAction->mapToGlobal(QPoint(0,0));
    pos = QPoint(pos.x()+65, pos.y());

    /* creating the context menu... */
    QMenu menu(this);
    QMenu saveMenu(this);
    QMenu copyMenu(this);
    saveMenu.setTitle("Save");
    copyMenu.setTitle("Copy");

    /* adding actions */
    menu.addAction(&a_ClearResults);
    menu.addSeparator();

    switch(ui->comboBoxOutput->currentIndex()){
    case osint::OUTPUT::SUBDOMAINIP:
        saveMenu.addAction(&a_SaveSubdomainIp);
        saveMenu.addAction(&a_SaveSubdomain);
        saveMenu.addAction(&a_SaveIp);
        copyMenu.addAction(&a_CopySubdomainIp);
        copyMenu.addAction(&a_CopySubdomain);
        copyMenu.addAction(&a_CopyIp);
        menu.addMenu(&saveMenu);
        menu.addMenu(&copyMenu);
        menu.addSeparator();
        menu.addAction(&a_SendAllIpToOsint);
        menu.addAction(&a_SendAllIpToRaw);
        menu.addSeparator();
        menu.addAction(&a_SendAllHostToOsint);
        menu.addAction(&a_SendAllHostToRaw);
        menu.addAction(&a_SendAllHostToBrute);
        menu.addAction(&a_SendAllHostToActive);
        menu.addAction(&a_SendAllHostToDns);
        menu.addAction(&a_SendAllHostToCert);
        menu.addSeparator();
        menu.addAction(&a_SendAllIpToIpTool);
        menu.addAction(&a_SendAllHostToCertTool);
        menu.addAction(&a_SendAllHostToDomainTool);
        break;
    case osint::OUTPUT::SUBDOMAIN:
        menu.addAction(&a_Save);
        menu.addAction(&a_Copy);
        menu.addSeparator();
        menu.addAction(&a_SendAllHostToOsint);
        menu.addAction(&a_SendAllHostToRaw);
        menu.addAction(&a_SendAllHostToBrute);
        menu.addAction(&a_SendAllHostToActive);
        menu.addAction(&a_SendAllHostToDns);
        menu.addAction(&a_SendAllHostToCert);
        menu.addSeparator();
        menu.addAction(&a_SendAllHostToCertTool);
        menu.addAction(&a_SendAllHostToDomainTool);
        break;
    case osint::OUTPUT::IP:
        menu.addAction(&a_Save);
        menu.addAction(&a_Copy);
        menu.addSeparator();
        menu.addAction(&a_SendAllIpToOsint);
        menu.addAction(&a_SendAllIpToRaw);
        menu.addSeparator();
        menu.addAction(&a_SendAllIpToIpTool);
        break;
    case osint::OUTPUT::EMAIL:
        menu.addAction(&a_Save);
        menu.addAction(&a_Copy);
        menu.addSeparator();
        menu.addAction(&a_SendAllEmailToOsint);
        menu.addAction(&a_SendAllEmailToRaw);
        menu.addSeparator();
        menu.addAction(&a_SendAllEmailToEmailTool);
        break;
    case osint::OUTPUT::URL:
        menu.addAction(&a_Save);
        menu.addAction(&a_Copy);
        menu.addSeparator();
        menu.addAction(&a_SendAllUrlToRaw);
        break;
    case osint::OUTPUT::ASN:
        menu.addAction(&a_Save);
        menu.addAction(&a_Copy);
        menu.addSeparator();
        menu.addAction(&a_SendAllAsnToOsint);
        menu.addAction(&a_SendAllAsnToRaw);
        menu.addSeparator();
        menu.addAction(&a_SendAllAsnToAsnTool);
        break;
    case osint::OUTPUT::CERT:
        menu.addAction(&a_Save);
        menu.addAction(&a_Copy);
        menu.addSeparator();
        menu.addAction(&a_SendAllCertToOsint);
        menu.addAction(&a_SendAllCertToRaw);
        menu.addSeparator();
        menu.addAction(&a_SendAllCertToCertTool);
        break;
    case osint::OUTPUT::CIDR:
        menu.addAction(&a_Save);
        menu.addAction(&a_Copy);
        menu.addSeparator();
        menu.addAction(&a_SendAllCidrToOsint);
        menu.addAction(&a_SendAllCidrToRaw);
        menu.addSeparator();
        menu.addAction(&a_SendAllCidrToCidrTool);
    }

    /* showing the context menu... */
    menu.exec(pos);
}

void Osint::on_tableViewResults_customContextMenuRequested(const QPoint &pos){
    Q_UNUSED(pos);

    /* check if user right clicked on items else dont show the context menu... */
    if(!ui->tableViewResults->selectionModel()->isSelected(ui->tableViewResults->currentIndex())){
        return;
    }

    /* getting the selected items... */
    selectionModel = ui->tableViewResults->selectionModel();

    /* creating the context menu... */
    QMenu menu(this);

    /* adding actions */
    menu.addAction(&a_Save);
    menu.addAction(&a_Copy);
    menu.addSeparator();
    menu.addAction(&a_RemoveResults);
    menu.addAction(&a_OpenInBrowser);
    menu.addSeparator();

    switch(ui->comboBoxOutput->currentIndex()){
    case osint::OUTPUT::SUBDOMAINIP:
        menu.addAction(&a_SendSelectedIpToOsint);
        menu.addAction(&a_SendSelectedIpToRaw);
        menu.addSeparator();
        menu.addAction(&a_SendSelectedHostToOsint);
        menu.addAction(&a_SendSelectedHostToRaw);
        menu.addAction(&a_SendSelectedHostToBrute);
        menu.addAction(&a_SendSelectedHostToActive);
        menu.addAction(&a_SendSelectedHostToDns);
        menu.addAction(&a_SendSelectedHostToCert);
        menu.addSeparator();
        menu.addAction(&a_SendSelectedIpToIpTool);
        menu.addAction(&a_SendSelectedHostToCertTool);
        menu.addAction(&a_SendSelectedHostToDomainTool);
        break;
    case osint::OUTPUT::SUBDOMAIN:
        menu.addAction(&a_SendSelectedHostToOsint);
        menu.addAction(&a_SendSelectedHostToRaw);
        menu.addAction(&a_SendSelectedHostToBrute);
        menu.addAction(&a_SendSelectedHostToActive);
        menu.addAction(&a_SendSelectedHostToDns);
        menu.addAction(&a_SendSelectedHostToCert);
        menu.addSeparator();
        menu.addAction(&a_SendSelectedHostToCertTool);
        menu.addAction(&a_SendSelectedHostToDomainTool);
        break;
    case osint::OUTPUT::IP:
        menu.addAction(&a_SendSelectedIpToOsint);
        menu.addAction(&a_SendSelectedIpToRaw);
        menu.addSeparator();
        menu.addAction(&a_SendSelectedIpToIpTool);
        break;
    case osint::OUTPUT::EMAIL:
        menu.addAction(&a_SendSelectedEmailToOsint);
        menu.addAction(&a_SendSelectedEmailToRaw);
        menu.addSeparator();
        menu.addAction(&a_SendSelectedEmailToEmailTool);
        break;
    case osint::OUTPUT::URL:
        menu.addAction(&a_SendSelectedUrlToRaw);
        break;
    case osint::OUTPUT::ASN:
        menu.addAction(&a_SendSelectedAsnToOsint);
        menu.addAction(&a_SendSelectedAsnToRaw);
        menu.addSeparator();
        menu.addAction(&a_SendSelectedAsnToAsnTool);
        break;
    case osint::OUTPUT::CERT:
        menu.addAction(&a_SendSelectedCertToOsint);
        menu.addAction(&a_SendSelectedCertToRaw);
        menu.addSeparator();
        menu.addAction(&a_SendSelectedCertToCertTool);
        break;
    case osint::OUTPUT::CIDR:
        menu.addAction(&a_SendSelectedCidrToOsint);
        menu.addAction(&a_SendSelectedCidrToRaw);
        menu.addSeparator();
        menu.addAction(&a_SendSelectedCidrToCidrTool);
    }

    /* showing the context menu... */
    menu.exec(QCursor::pos());
}

void Osint::m_openInBrowser(QItemSelectionModel *selectionModel){
    foreach(const QModelIndex &index, selectionModel->selectedIndexes())
        QDesktopServices::openUrl(QUrl("https://"+index.data().toString(), QUrl::TolerantMode));
}

void Osint::m_removeResults(QItemSelectionModel *selectionModel){
    QStandardItemModel *model = nullptr;
    QSet<QString> set;
    switch(ui->comboBoxOutput->currentIndex()){
    case osint::OUTPUT::SUBDOMAINIP:
        model = m_resultModelSubdomainIp;
        set = m_subdomainIpSet;
        break;
    case osint::OUTPUT::SUBDOMAIN:
        model = m_resultModelSubdomain;
        set = m_subdomainSet;
        break;
    case osint::OUTPUT::IP:
        model = m_resultModelIp;
        set = m_ipSet;
        break;
    case osint::OUTPUT::EMAIL:
        model = m_resultModelEmail;
        set = m_emailSet;
        break;
    case osint::OUTPUT::URL:
        model = m_resultModelUrl;
        set = m_urlSet;
        break;
    case osint::OUTPUT::ASN:
        model = m_resultModelAsn;
        set = m_asnSet;
        break;
    case osint::OUTPUT::CERT:
        model = m_resultModelCert;
        set = m_sslCertSet;
        break;
    case osint::OUTPUT::CIDR:
        model = m_resultModelCidr;
        set = m_cidrSet;
    }

    QModelIndex index;
    foreach(const QModelIndex &proxyIndex, selectionModel->selectedIndexes()){
        index = m_resultProxyModel->mapToSource(proxyIndex);
        set.remove(index.data().toString());
        model->removeRow(index.row());
    }
    ui->labelResultsCount->setNum(m_resultProxyModel->rowCount());
}

void Osint::m_saveResults(RESULT_TYPE resultType){
    /* checks */
    QString filename = QFileDialog::getSaveFileName(this, "Save To File", "./");
    if(filename.isEmpty())
        return;

    QFile file(filename);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    if(!file.isOpen())
        return;

    /* for subdomainIp model, since it has subdomain & ip */
    QString item;
    switch(ui->comboBoxOutput->currentIndex()){
    case osint::OUTPUT::SUBDOMAINIP:
        switch(resultType){
        case RESULT_TYPE::SUBDOMAIN:
            for(int i = 0; i != m_resultProxyModel->rowCount(); ++i)
            {
                item = m_resultProxyModel->data(m_resultProxyModel->index(i, 0)).toString().append(NEWLINE);
                file.write(item.toUtf8());
            }
            break;

        case RESULT_TYPE::IP:
            for(int i = 0; i != m_resultProxyModel->rowCount(); ++i)
            {
                item = m_resultProxyModel->data(m_resultProxyModel->index(i, 1)).toString().append(NEWLINE);
                file.write(item.toUtf8());
            }
            break;

        case RESULT_TYPE::SUBDOMAINIP:
            for(int i = 0; i != m_resultProxyModel->rowCount(); ++i)
            {
                item = m_resultProxyModel->data(m_resultProxyModel->index(i, 0)).toString()+":"+m_resultProxyModel->data(m_resultProxyModel->index(i, 0)).toString().append(NEWLINE);
                file.write(item.toUtf8());
            }
            break;

        default:
            break;
        }
        file.close();
        return;
    }

    /* for other result type */
    for(int i = 0; i != m_resultProxyModel->rowCount(); ++i)
    {
        item = m_resultProxyModel->data(m_resultProxyModel->index(i, 0)).toString()+":"+m_resultProxyModel->data(m_resultProxyModel->index(i, 0)).toString().append(NEWLINE);
        file.write(item.toUtf8());
    }

    file.close();
}

void Osint::m_saveResults(QItemSelectionModel *selectionModel){
    QString filename = QFileDialog::getSaveFileName(this, "Save To File", "./");
    if(filename.isEmpty())
        return;

    /* saving to file */
    QString data;
    QString item;
    QFile file(filename);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    if(file.isOpen())
    {
        foreach(const QModelIndex &index, selectionModel->selectedIndexes()){
            item = index.data().toString();
            data.append(item.append(NEWLINE));
        }
        file.write(data.toUtf8());
        file.close();
    }
}


void Osint::m_copyResults(RESULT_TYPE resultType){
    QClipboard *clipboard = QGuiApplication::clipboard();
    QString clipboardData;
    QString item;

    /* for subdomainIp result model */
    switch(ui->comboBoxOutput->currentIndex()){
    case osint::OUTPUT::SUBDOMAINIP:
        switch(resultType){
        case RESULT_TYPE::SUBDOMAIN:
            for(int i = 0; i != m_resultProxyModel->rowCount(); ++i)
            {
                item = m_resultProxyModel->data(m_resultProxyModel->index(i, 0)).toString().append(NEWLINE);
                clipboardData.append(item);
            }
            break;

        case RESULT_TYPE::IP:
            for(int i = 0; i != m_resultProxyModel->rowCount(); ++i)
            {
                item = m_resultProxyModel->data(m_resultProxyModel->index(i, 1)).toString().append(NEWLINE);
                clipboardData.append(item);
            }
            break;

        case RESULT_TYPE::SUBDOMAINIP:
            for(int i = 0; i != m_resultProxyModel->rowCount(); ++i)
            {
                item = m_resultProxyModel->data(m_resultProxyModel->index(i, 0)).toString()+":"+m_resultProxyModel->data(m_resultProxyModel->index(i, 0)).toString().append(NEWLINE);
                clipboardData.append(item);
            }
            break;

        default:
            break;
        }
        clipboard->setText(clipboardData.trimmed());
        return;
    }

    /* for other result types */
    for(int i = 0; i != m_resultProxyModel->rowCount(); ++i)
    {
        item = m_resultProxyModel->data(m_resultProxyModel->index(i, 0)).toString()+":"+m_resultProxyModel->data(m_resultProxyModel->index(i, 0)).toString().append(NEWLINE);
        clipboardData.append(item);
    }

    clipboard->setText(clipboardData.trimmed());
}

void Osint::m_copyResults(QItemSelectionModel *selectionModel){
    QClipboard *clipboard = QGuiApplication::clipboard();
    QString data;
    QString item;

    foreach(const QModelIndex &index, selectionModel->selectedIndexes())
    {
        item = index.data().toString();
        data.append(item.append(NEWLINE));
    }

    clipboard->setText(data.trimmed());
}

void Osint::onReceiveTargets(QString target, RESULT_TYPE resultType){
    switch (resultType) {
    case RESULT_TYPE::SUBDOMAIN:
        ui->comboBoxInput->setCurrentIndex(INPUT::HOSTNAME);
        break;
    case RESULT_TYPE::IP:
        ui->comboBoxInput->setCurrentIndex(INPUT::IP);
        break;
    case RESULT_TYPE::ASN:
        ui->comboBoxInput->setCurrentIndex(INPUT::ASN);
        break;
    case RESULT_TYPE::CIDR:
        ui->comboBoxInput->setCurrentIndex(INPUT::CIDR);
        break;
    case RESULT_TYPE::CERT_ID:
        ui->comboBoxInput->setCurrentIndex(INPUT::CERT);
        break;
    default:
        break;
    }

    ui->targets->add(target);
    /* set multiple targets checkbox checked */
    ui->checkBoxMultipleTargets->setChecked(true);
}

/*****************************************************************************
                            SENDING RESULTS
******************************************************************************/
void Osint::m_sendSubdomainToEngine(ENGINE engine){
    QString item;
    switch (engine) {
    case ENGINE::OSINT:
        for(int i = 0; i != m_resultProxyModel->rowCount(); ++i){
            item = m_resultProxyModel->data(m_resultProxyModel->index(i, 0)).toString();
            emit sendResultsToOsint(item, RESULT_TYPE::SUBDOMAIN);
        }
        emit changeTabToOsint();
        break;
    case ENGINE::RAW:
        for(int i = 0; i != m_resultProxyModel->rowCount(); ++i){
            item = m_resultProxyModel->data(m_resultProxyModel->index(i, 0)).toString();
            emit sendResultsToRaw(item, RESULT_TYPE::SUBDOMAIN);
        }
        emit changeTabToRaw();
        break;
    case ENGINE::BRUTE:
        for(int i = 0; i != m_resultProxyModel->rowCount(); ++i){
            item = m_resultProxyModel->data(m_resultProxyModel->index(i, 0)).toString();
            emit sendResultsToBrute(item, RESULT_TYPE::SUBDOMAIN);
        }
        emit changeTabToBrute();
        break;
    case ENGINE::ACTIVE:
        for(int i = 0; i != m_resultProxyModel->rowCount(); ++i){
            item = m_resultProxyModel->data(m_resultProxyModel->index(i, 0)).toString();
            emit sendResultsToActive(item, RESULT_TYPE::SUBDOMAIN);
        }
        emit changeTabToActive();
        break;
    case ENGINE::DNS:
        for(int i = 0; i != m_resultProxyModel->rowCount(); ++i){
            item = m_resultProxyModel->data(m_resultProxyModel->index(i, 0)).toString();
            emit sendResultsToDns(item, RESULT_TYPE::SUBDOMAIN);
        }
        emit changeTabToDns();
        break;
    case ENGINE::CERT:
        for(int i = 0; i != m_resultProxyModel->rowCount(); ++i){
            item = m_resultProxyModel->data(m_resultProxyModel->index(i, 0)).toString();
            emit sendResultsToCert(item, RESULT_TYPE::SUBDOMAIN);
        }
        emit changeTabToSSL();
        break;
    default:
        break;
    }
}

void Osint::m_sendIpToEngine(ENGINE engine){
    QString item;
    switch (ui->comboBoxOutput->currentIndex()) {
    case osint::OUTPUT::IP:
        switch (engine) {
        case ENGINE::OSINT:
            for(int i = 0; i != m_resultProxyModel->rowCount(); ++i){
                item = m_resultProxyModel->data(m_resultProxyModel->index(i, 0)).toString();
                emit sendResultsToOsint(item, RESULT_TYPE::IP);
            }
            emit changeTabToOsint();
            break;
        case ENGINE::RAW:
            for(int i = 0; i != m_resultProxyModel->rowCount(); ++i){
                item = m_resultProxyModel->data(m_resultProxyModel->index(i, 0)).toString();
                emit sendResultsToRaw(item, RESULT_TYPE::IP);
            }
            emit changeTabToRaw();
            break;
        }
        break;

    case osint::OUTPUT::SUBDOMAINIP:
        switch (engine) {
        case ENGINE::OSINT:
            for(int i = 0; i != m_resultProxyModel->rowCount(); ++i){
                item = m_resultProxyModel->data(m_resultProxyModel->index(i, 1)).toString();
                emit sendResultsToOsint(item, RESULT_TYPE::IP);
            }
            emit changeTabToOsint();
            break;
        case ENGINE::RAW:
            for(int i = 0; i != m_resultProxyModel->rowCount(); ++i){
                item = m_resultProxyModel->data(m_resultProxyModel->index(i, 1)).toString();
                emit sendResultsToRaw(item, RESULT_TYPE::IP);
            }
            emit changeTabToRaw();
            break;

        default:
            break;
        }
    }
}

void Osint::m_sendToEngine(ENGINE engine){
    /* getting the result type*/
    RESULT_TYPE resultType;
    switch (ui->comboBoxOutput->currentIndex()) {
    case osint::OUTPUT::ASN:
        resultType = RESULT_TYPE::ASN;
        break;
    case osint::OUTPUT::CIDR:
        resultType = RESULT_TYPE::CIDR;
        break;
    case osint::OUTPUT::CERT:
        resultType = RESULT_TYPE::CERT_ID;
        break;
    case osint::OUTPUT::EMAIL:
        resultType = RESULT_TYPE::EMAIL;
        break;
    case osint::OUTPUT::URL:
        resultType = RESULT_TYPE::URL;
    }

    /* sending the results */
    QString item;
    switch (engine) {
    case ENGINE::OSINT:
        for(int i = 0; i != m_resultProxyModel->rowCount(); ++i){
            item = m_resultProxyModel->data(m_resultProxyModel->index(i, 0)).toString();
            emit sendResultsToOsint(item, resultType);
        }
        emit changeTabToOsint();
        break;
    case ENGINE::RAW:
        for(int i = 0; i != m_resultProxyModel->rowCount(); ++i){
            item = m_resultProxyModel->data(m_resultProxyModel->index(i, 0)).toString();
            emit sendResultsToRaw(item, resultType);
        }
        emit changeTabToRaw();
        break;
    default:
        break;
    }
}

void Osint::m_sendSubdomainToEngine(ENGINE engine, QItemSelectionModel *selection){
    QString item;
    switch (engine) {
    case ENGINE::OSINT:
        foreach(const QModelIndex &index, selection->selectedIndexes()){
            item = index.data().toString();
            emit sendResultsToOsint(item, RESULT_TYPE::SUBDOMAIN);
        }
        emit changeTabToOsint();
        break;
    case ENGINE::RAW:
        foreach(const QModelIndex &index, selection->selectedIndexes()){
            item = index.data().toString();
            emit sendResultsToRaw(item, RESULT_TYPE::SUBDOMAIN);
        }
        emit changeTabToRaw();
        break;
    case ENGINE::BRUTE:
        foreach(const QModelIndex &index, selection->selectedIndexes()){
            item = index.data().toString();
            emit sendResultsToBrute(item, RESULT_TYPE::SUBDOMAIN);
        }
        emit changeTabToBrute();
        break;
    case ENGINE::ACTIVE:
        foreach(const QModelIndex &index, selection->selectedIndexes()){
            item = index.data().toString();
            emit sendResultsToActive(item, RESULT_TYPE::SUBDOMAIN);
        }
        emit changeTabToActive();
        break;
    case ENGINE::DNS:
        foreach(const QModelIndex &index, selection->selectedIndexes()){
            item = index.data().toString();
            emit sendResultsToDns(item, RESULT_TYPE::SUBDOMAIN);
        }
        emit changeTabToDns();
        break;
    case ENGINE::CERT:
        foreach(const QModelIndex &index, selection->selectedIndexes()){
            item = index.data().toString();
            emit sendResultsToCert(item, RESULT_TYPE::SUBDOMAIN);
        }
        emit changeTabToSSL();
        break;
    default:
        break;
    }
}

void Osint::m_sendIpToEngine(ENGINE engine, QItemSelectionModel *selection){
    QString item;
    switch (engine) {
    case ENGINE::OSINT:
        foreach(const QModelIndex &index, selection->selectedIndexes()){
            item = index.data().toString();
            emit sendResultsToOsint(item, RESULT_TYPE::IP);
        }
        emit changeTabToOsint();
        break;
    case ENGINE::RAW:
        foreach(const QModelIndex &index, selection->selectedIndexes()){
            item = index.data().toString();
            emit sendResultsToRaw(item, RESULT_TYPE::IP);
        }
        emit changeTabToRaw();
        break;

    default:
        break;
    }
}

void Osint::m_sendToEngine(ENGINE engine, QItemSelectionModel *selection){
    /* getting the result type */
    RESULT_TYPE resultType;
    switch (ui->comboBoxOutput->currentIndex()) {
    case osint::OUTPUT::ASN:
        resultType = RESULT_TYPE::ASN;
        break;
    case osint::OUTPUT::CIDR:
        resultType = RESULT_TYPE::CIDR;
        break;
    case osint::OUTPUT::CERT:
        resultType = RESULT_TYPE::CERT_ID;
        break;
    case osint::OUTPUT::EMAIL:
        resultType = RESULT_TYPE::EMAIL;
        break;
    case osint::OUTPUT::URL:
        resultType = RESULT_TYPE::URL;
    }

    /* sending the results */
    QString item;
    switch (engine) {
    case ENGINE::OSINT:
        foreach(const QModelIndex &index, selection->selectedIndexes()){
            item = index.data().toString();
            emit sendResultsToOsint(item, resultType);
        }
        emit changeTabToOsint();
        break;
    case ENGINE::RAW:
        foreach(const QModelIndex &index, selection->selectedIndexes()){
            item = index.data().toString();
            emit sendResultsToRaw(item, resultType);
        }
        emit changeTabToRaw();
        break;
    default:
        break;
    }
}

void Osint::m_sendSubdomainToTool(TOOL){
    /* not yet */
}

void Osint::m_sendIpToTool(TOOL){
    /* not yet */
}

void Osint::m_sendToTool(TOOL){
    /* not yet */
}

void Osint::m_sendSubdomainToTool(TOOL, QItemSelectionModel *){
    /* not yet */
}

void Osint::m_sendIpToTool(TOOL, QItemSelectionModel *){
    /* not yet */
}

void Osint::m_sendToTool(TOOL, QItemSelectionModel *){
    /* not yet */
}

