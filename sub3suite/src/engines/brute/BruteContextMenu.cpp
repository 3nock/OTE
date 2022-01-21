/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#include "Brute.h"
#include "ui_Brute.h"

#include <QClipboard>
#include <QFileDialog>
#include <QDesktopServices>


void Brute::m_initActions(){
    connect(&a_ClearResults, &QAction::triggered, this, [=](){this->m_clearResults();});
    connect(&a_RemoveResults, &QAction::triggered, this, [=](){this->m_removeResults(selectionModel);});
    connect(&a_OpenInBrowser, &QAction::triggered, this, [=](){this->m_openInBrowser(selectionModel);});
    /* ... */
    connect(&a_ExtractAll, &QAction::triggered, this, [=](){this->m_extract();});
    connect(&a_ExtractSelected, &QAction::triggered, this, [=](){this->m_extract(selectionModel);});
    /* ... */
    connect(&a_SendAllToProject, &QAction::triggered, this, [=](){this->m_sendToProject();});
    connect(&a_SendAllIpToOsint, &QAction::triggered, this, [=](){this->m_sendIpToEngine(ENGINE::OSINT);});
    connect(&a_SendAllIpToRaw, &QAction::triggered, this, [=](){this->m_sendIpToEngine(ENGINE::RAW);});
    connect(&a_SendAllHostToOsint, &QAction::triggered, this, [=](){this->m_sendSubdomainToEngine(ENGINE::OSINT);});
    connect(&a_SendAllHostToRaw, &QAction::triggered, this, [=](){this->m_sendSubdomainToEngine(ENGINE::RAW);});
    connect(&a_SendAllHostToBrute, &QAction::triggered, this, [=](){this->m_sendSubdomainToEngine(ENGINE::BRUTE);});
    connect(&a_SendAllHostToActive, &QAction::triggered, this, [=](){this->m_sendSubdomainToEngine(ENGINE::ACTIVE);});
    connect(&a_SendAllHostToDNS, &QAction::triggered, this, [=](){this->m_sendSubdomainToEngine(ENGINE::DNS);});
    connect(&a_SendAllHostToSSL, &QAction::triggered, this, [=](){this->m_sendSubdomainToEngine(ENGINE::CERT);});
    connect(&a_SendAllIpToIPTool, &QAction::triggered, this, [=](){this->m_sendIpToTool(TOOL::IP);});
    connect(&a_SendAllHostToSSLTool, &QAction::triggered, this, [=](){this->m_sendSubdomainToTool(TOOL::CERT);});
    connect(&a_SendAllHostToDomainTool, &QAction::triggered, this, [=](){this->m_sendSubdomainToTool(TOOL::DOMAINTOOL);});
    /* ... */
    connect(&a_SendSelectedToProject, &QAction::triggered, this, [=](){this->m_sendToProject(selectionModel);});
    connect(&a_SendSelectedIpToOsint, &QAction::triggered, this, [=](){this->m_sendIpToEngine(ENGINE::OSINT, selectionModel);});
    connect(&a_SendSelectedIpToRaw, &QAction::triggered, this, [=](){this->m_sendIpToEngine(ENGINE::RAW, selectionModel);});
    connect(&a_SendSelectedHostToOsint, &QAction::triggered, this, [=](){this->m_sendSubdomainToEngine(ENGINE::OSINT, selectionModel);});
    connect(&a_SendSelectedHostToRaw, &QAction::triggered, this, [=](){this->m_sendSubdomainToEngine(ENGINE::RAW, selectionModel);});
    connect(&a_SendSelectedHostToBrute, &QAction::triggered, this, [=](){this->m_sendSubdomainToEngine(ENGINE::BRUTE, selectionModel);});
    connect(&a_SendSelectedHostToActive, &QAction::triggered, this, [=](){this->m_sendSubdomainToEngine(ENGINE::ACTIVE, selectionModel);});
    connect(&a_SendSelectedHostToDNS, &QAction::triggered, this, [=](){this->m_sendSubdomainToEngine(ENGINE::DNS, selectionModel);});
    connect(&a_SendSelectedHostToSSL, &QAction::triggered, this, [=](){this->m_sendSubdomainToEngine(ENGINE::CERT, selectionModel);});
    connect(&a_SendSelectedIpToIPTool, &QAction::triggered, this, [=](){this->m_sendIpToTool(TOOL::IP, selectionModel);});
    connect(&a_SendSelectedHostToSSLTool, &QAction::triggered, this, [=](){this->m_sendSubdomainToTool(TOOL::CERT, selectionModel);});
    connect(&a_SendSelectedHostToDomainTool, &QAction::triggered, this, [=](){this->m_sendSubdomainToTool(TOOL::DOMAINTOOL, selectionModel);});
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

void Brute::on_buttonAction_clicked(){
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
    saveMenu.addAction(&a_SaveSubdomainIp);
    saveMenu.addAction(&a_SaveSubdomain);
    saveMenu.addAction(&a_SaveIp);
    copyMenu.addAction(&a_CopySubdomainIp);
    copyMenu.addAction(&a_CopySubdomain);
    copyMenu.addAction(&a_CopyIp);
    /* adding to mainMenu */
    menu.addAction(&a_ClearResults);
    menu.addSeparator();
    menu.addMenu(&saveMenu);
    menu.addMenu(&copyMenu);
    menu.addAction(&a_ExtractAll);
    menu.addSeparator();
    menu.addAction(&a_SendAllToProject);
    menu.addSeparator();
    menu.addAction(&a_SendAllIpToOsint);
    menu.addAction(&a_SendAllIpToRaw);
    menu.addSeparator();
    menu.addAction(&a_SendAllHostToOsint);
    menu.addAction(&a_SendAllHostToRaw);
    menu.addAction(&a_SendAllHostToBrute);
    menu.addAction(&a_SendAllHostToActive);
    menu.addAction(&a_SendAllHostToDNS);
    menu.addAction(&a_SendAllHostToSSL);
    menu.addSeparator();
    menu.addAction(&a_SendAllIpToIPTool);
    menu.addAction(&a_SendAllHostToSSLTool);
    menu.addAction(&a_SendAllHostToDomainTool);

    /* showing the context menu... */
    menu.exec(pos);
}

void Brute::on_tableViewResults_customContextMenuRequested(const QPoint &pos){
    Q_UNUSED(pos);

    /* check if user right clicked on items else dont show the context menu... */
    if(!ui->tableViewResults->selectionModel()->isSelected(ui->tableViewResults->currentIndex()))
        return;

    /* getting the selected items... */
    selectionModel = ui->tableViewResults->selectionModel();

    /* creating the context menu... */
    QMenu menu(this);

    /* adding to mainMenu */
    menu.addAction(&a_RemoveResults);
    menu.addAction(&a_OpenInBrowser);
    menu.addSeparator();
    menu.addAction(&a_Save);
    menu.addAction(&a_Copy);
    menu.addAction(&a_ExtractSelected);
    menu.addSeparator();
    menu.addAction(&a_SendSelectedToProject);
    menu.addSeparator();
    menu.addAction(&a_SendSelectedIpToOsint);
    menu.addAction(&a_SendSelectedIpToRaw);
    menu.addSeparator();
    menu.addAction(&a_SendSelectedHostToOsint);
    menu.addAction(&a_SendSelectedHostToRaw);
    menu.addAction(&a_SendSelectedHostToBrute);
    menu.addAction(&a_SendSelectedHostToActive);
    menu.addAction(&a_SendSelectedHostToDNS);
    menu.addAction(&a_SendSelectedHostToSSL);
    menu.addSeparator();
    menu.addAction(&a_SendSelectedIpToIPTool);
    menu.addAction(&a_SendSelectedHostToSSLTool);
    menu.addAction(&a_SendSelectedHostToDomainTool);

    /* showing the context menu... */
    menu.exec(QCursor::pos());
}

void Brute::m_clearResults(){
    /* clear all containers containing results */
    switch (ui->comboBoxOutput->currentIndex()) {
    case brute::OUTPUT::SUBDOMAIN:
        m_subdomainSet.clear();
        m_resultModelSubdomain->clear();
        m_resultModelSubdomain->setHorizontalHeaderLabels({"Subdomain", "IpAddress"});
        break;

    case brute::OUTPUT::TLD:
        m_tldSet.clear();
        m_resultModelTld->clear();
        m_resultModelTld->setHorizontalHeaderLabels({"TLD", "IpAddress"});
    }
    ui->labelResultsCount->clear();

    /* clear the progressbar... */
    ui->progressBar->clearMask();
    ui->progressBar->reset();
    ui->progressBar->hide();
}

void Brute::m_openInBrowser(QItemSelectionModel *selectionModel){
    foreach(const QModelIndex &index, selectionModel->selectedIndexes())
        QDesktopServices::openUrl(QUrl("https://"+index.data().toString(), QUrl::TolerantMode));
}

void Brute::m_removeResults(QItemSelectionModel *selectionModel){
    /* findout from what results model */
    QStandardItemModel *model = nullptr;
    QSet<QString> set;
    switch (ui->comboBoxOutput->currentIndex()) {
    case brute::OUTPUT::SUBDOMAIN:
        model = m_resultModelSubdomain;
        set = m_subdomainSet;
        break;
    case brute::OUTPUT::TLD:
        model = m_resultModelTld;
        set = m_tldSet;
    }

    QModelIndex index;
    foreach(const QModelIndex &proxyIndex, selectionModel->selectedIndexes()){
        index = m_resultProxyModel->mapToSource(proxyIndex);
        set.remove(index.data().toString());
        model->removeRow(index.row());
    }
    ui->labelResultsCount->setNum(m_resultProxyModel->rowCount());
}

void Brute::m_saveResults(RESULT_TYPE resultType){
    /* checks... */
    QString filename = QFileDialog::getSaveFileName(this, "Save To File", "./");
    if(filename.isEmpty())
        return;

    QFile file(filename);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    if(!file.isOpen())
        return;

    /* writing to file */
    QString item;
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
}

void Brute::m_saveResults(QItemSelectionModel *selectionModel){
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


void Brute::m_copyResults(RESULT_TYPE resultType){
    QClipboard *clipboard = QGuiApplication::clipboard();
    QString clipboardData;
    QString item;
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
            item = m_resultProxyModel->data(m_resultProxyModel->index(i, 0)).toString()+"|"+m_resultProxyModel->data(m_resultProxyModel->index(i, 1)).toString().append(NEWLINE);
            clipboardData.append(item);
        }
        break;

    default:
        break;
    }

    clipboard->setText(clipboardData.trimmed());
}

void Brute::m_copyResults(QItemSelectionModel *selectionModel){
    QClipboard *clipboard = QGuiApplication::clipboard();
    QString data;
    QString item;

    foreach(const QModelIndex &index, selectionModel->selectedIndexes()){
        item = index.data().toString();
        data.append(item.append(NEWLINE));
    }

    clipboard->setText(data.trimmed());
}

void Brute::m_extract(){
    QClipboard *clipboard = QGuiApplication::clipboard();
    QString data;
    QString item;

    for(int i = 0; i != m_resultProxyModel->rowCount(); ++i){
        item = m_resultProxyModel->data(m_resultProxyModel->index(i, 0)).toString().split(".").at(0);
        data.append(item.append(NEWLINE));
    }

    clipboard->setText(data.trimmed());
}

void Brute::m_extract(QItemSelectionModel *selectionModel){
    QClipboard *clipboard = QGuiApplication::clipboard();
    QString data;
    QString item;
    foreach(const QModelIndex &index, selectionModel->selectedIndexes()){
        if(index.column())
            continue;
        item = index.data().toString().split(".").at(0);
        data.append(item.append(NEWLINE));
    }

    clipboard->setText(data.trimmed());
}

/* TODO:
 *      obtain as items and send them to project
 */
void Brute::onReceiveTargets(QString target, RESULT_TYPE resultType){
    if(resultType == RESULT_TYPE::SUBDOMAIN){
        ui->targets->add(target);
    }

    /* set multiple targets checkbox checked */
    ui->checkBoxMultipleTargets->setChecked(true);
}

/*****************************************************************************
                            SENDING RESULTS
******************************************************************************/

void Brute::m_sendToProject(){
    QString host, ip;
    switch (ui->comboBoxOutput->currentIndex()) {
    case brute::OUTPUT::SUBDOMAIN:
        for(int i = 0; i != m_resultProxyModel->rowCount(); ++i){
            host = m_resultProxyModel->data(m_resultProxyModel->index(i, 0)).toString();
            ip = m_resultProxyModel->data(m_resultProxyModel->index(i, 1)).toString();
            project->addActiveSubdomainIp(host, ip);
        }
        break;
    case brute::OUTPUT::TLD:
        for(int i = 0; i != m_resultProxyModel->rowCount(); ++i){
            host = m_resultProxyModel->data(m_resultProxyModel->index(i, 0)).toString();
            ip = m_resultProxyModel->data(m_resultProxyModel->index(i, 1)).toString();
            project->addActiveTLD(host, ip);
        }
    }
}

/*
 * TODO:
 *      current way still crude.
 *      Fix. selection can be host, ip or both
 */
void Brute::m_sendToProject(QItemSelectionModel *selection){
    int row;
    QString host(""), ip("");
    QModelIndexList indexList(selection->selectedIndexes());

    switch (ui->comboBoxOutput->currentIndex()) {
    case brute::OUTPUT::SUBDOMAIN:
        for(int i = 0; i < indexList.size();){
            host = indexList.at(i).data().toString();
            row = indexList.at(i).row();
            i++;
            if((i < indexList.size()) && (row == indexList.at(i).row())){
                ip = indexList.at(i).data().toString();
                project->addActiveSubdomainIp(host, ip);
                i++;
            }
        }
        break;
    case brute::OUTPUT::TLD:
        for(int i = 0; i < indexList.size();){
            host = indexList.at(i).data().toString();
            row = indexList.at(i).row();
            i++;
            if((i < indexList.size()) && (row == indexList.at(i).row())){
                ip = indexList.at(i).data().toString();
                project->addActiveTLD(host, ip);
                i++;
            }
        }
    }
}

void Brute::m_sendSubdomainToEngine(ENGINE engine){
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

void Brute::m_sendIpToEngine(ENGINE engine){
    QString item;
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

void Brute::m_sendSubdomainToEngine(ENGINE engine, QItemSelectionModel *selection){
    /* sending the results */
    QString item;
    switch (engine) {
    case ENGINE::OSINT:
        foreach(const QModelIndex &index, selection->selectedIndexes()){
            if(index.column())
                continue;
            item = index.data().toString();
            emit sendResultsToOsint(item, RESULT_TYPE::SUBDOMAIN);
        }
        emit changeTabToOsint();
        break;
    case ENGINE::RAW:
        foreach(const QModelIndex &index, selection->selectedIndexes()){
            if(index.column())
                continue;
            item = index.data().toString();
            emit sendResultsToRaw(item, RESULT_TYPE::SUBDOMAIN);
        }
        emit changeTabToRaw();
        break;
    case ENGINE::BRUTE:
        foreach(const QModelIndex &index, selection->selectedIndexes()){
            if(index.column())
                continue;
            item = index.data().toString();
            emit sendResultsToBrute(item, RESULT_TYPE::SUBDOMAIN);
        }
        emit changeTabToBrute();
        break;
    case ENGINE::ACTIVE:
        foreach(const QModelIndex &index, selection->selectedIndexes()){
            if(index.column())
                continue;
            item = index.data().toString();
            emit sendResultsToActive(item, RESULT_TYPE::SUBDOMAIN);
        }
        emit changeTabToActive();
        break;
    case ENGINE::DNS:
        foreach(const QModelIndex &index, selection->selectedIndexes()){
            if(index.column())
                continue;
            item = index.data().toString();
            emit sendResultsToDns(item, RESULT_TYPE::SUBDOMAIN);
        }
        emit changeTabToDns();
        break;
    case ENGINE::CERT:
        foreach(const QModelIndex &index, selection->selectedIndexes()){
            if(index.column())
                continue;
            item = index.data().toString();
            emit sendResultsToCert(item, RESULT_TYPE::SUBDOMAIN);
        }
        emit changeTabToSSL();
        break;
    default:
        break;
    }
}

void Brute::m_sendIpToEngine(ENGINE engine, QItemSelectionModel *selection){
    QString item;
    switch (engine) {
    case ENGINE::OSINT:
        foreach(const QModelIndex &index, selection->selectedIndexes()){
            if(index.column()){
                item = index.data().toString();
                emit sendResultsToOsint(item, RESULT_TYPE::IP);
            }
        }
        emit changeTabToOsint();
        break;
    case ENGINE::RAW:
        foreach(const QModelIndex &index, selection->selectedIndexes()){
            if(index.column()){
                item = index.data().toString();
                emit sendResultsToRaw(item, RESULT_TYPE::IP);
            }
        }
        emit changeTabToRaw();
        break;

    default:
        break;
    }
}

void Brute::m_sendSubdomainToTool(TOOL){
    /* not yet */
}

void Brute::m_sendIpToTool(TOOL){
    /* not yet */
}

void Brute::m_sendSubdomainToTool(TOOL, QItemSelectionModel *){
    /* not yet */
}

void Brute::m_sendIpToTool(TOOL, QItemSelectionModel *){
    /* not yet */
}
