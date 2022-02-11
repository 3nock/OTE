/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#include "Active.h"
#include "ui_Active.h"

#include <QClipboard>
#include <QFileDialog>
#include <QDesktopServices>


void Active::m_openInBrowser(QItemSelectionModel *selectionModel){
    foreach(const QModelIndex &index, selectionModel->selectedIndexes())
        QDesktopServices::openUrl(QUrl("https://"+index.data().toString(), QUrl::TolerantMode));
}

void Active::m_clearResults(){
    /* clear the results... */
    m_resultModel->clear();
    ui->labelResultsCount->clear();
    m_resultModel->setHorizontalHeaderLabels({" Host", " Ipv4", " Ipv6"});
    m_resultSet.clear();

    /* clear the progressbar... */
    ui->progressBar->clearMask();
    ui->progressBar->reset();
    ui->progressBar->hide();
}

void Active::m_removeResults(QItemSelectionModel *selectionModel){
    QModelIndex index;
    foreach(const QModelIndex &proxyIndex, selectionModel->selectedIndexes()){
        index = m_resultProxyModel->mapToSource(proxyIndex);
        m_resultSet.remove(index.data().toString());
        m_resultModel->removeRow(index.row());
    }

    ui->labelResultsCount->setNum(m_resultProxyModel->rowCount());
}

void Active::m_saveResults(RESULT_TYPE resultType){
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

void Active::m_saveResults(QItemSelectionModel *selectionModel){
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


void Active::m_copyResults(RESULT_TYPE resultType){
    QClipboard *clipboard = QGuiApplication::clipboard();
    QString clipboardData;
    QString item;

    /* copying result type */
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

void Active::m_copyResults(QItemSelectionModel *selectionModel){
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

void Active::m_extract(){
    QClipboard *clipboard = QGuiApplication::clipboard();
    QString data;
    QString item;

    for(int i = 0; i != m_resultProxyModel->rowCount(); ++i){
        item = m_resultProxyModel->data(m_resultProxyModel->index(i, 0)).toString().split(".").at(0);
        data.append(item.append(NEWLINE));
    }

    clipboard->setText(data.trimmed());
}

void Active::m_extract(QItemSelectionModel *selectionModel){
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

void Active::onReceiveTargets(QString target, RESULT_TYPE resultType){
    if(resultType == RESULT_TYPE::SUBDOMAIN){
        ui->targets->add(target);
    }

    /* set multiple targets checkbox checked */
    ui->checkBoxMultipleTargets->setChecked(true);
}

/*****************************************************************************
                            SENDING RESULTS
******************************************************************************/
void Active::m_sendToProject(){
    for(int i = 0; i != m_resultProxyModel->rowCount(); ++i)
    {
        QModelIndex index = m_resultProxyModel->mapToSource(m_resultProxyModel->index(i ,0));
        s3s_item::HOST *item = static_cast<s3s_item::HOST*>(m_resultModel->item(index.row(), index.column()));
        project->addActiveHost(host_to_struct(item));
    }
}

void Active::m_sendToProject(QItemSelectionModel *selection){
    foreach(const QModelIndex &index, selection->selectedIndexes()){
        QModelIndex model_index = m_resultProxyModel->mapToSource(index);
        s3s_item::HOST *item = static_cast<s3s_item::HOST*>(m_resultModel->item(model_index.row(), model_index.column()));
        project->addActiveHost(host_to_struct(item));
    }
}

void Active::m_sendSubdomainToEngine(ENGINE engine){
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

void Active::m_sendIpToEngine(ENGINE engine){
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

void Active::m_sendSubdomainToEngine(ENGINE engine, QItemSelectionModel *selection){
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

void Active::m_sendIpToEngine(ENGINE engine, QItemSelectionModel *selection){
    QString item;
    switch (engine) {
    case ENGINE::OSINT:
        foreach(const QModelIndex &index, selection->selectedIndexes()){
            if(!index.column())
                continue;
            item = index.data().toString();
            emit sendResultsToOsint(item, RESULT_TYPE::IP);
        }
        emit changeTabToOsint();
        break;
    case ENGINE::RAW:
        foreach(const QModelIndex &index, selection->selectedIndexes()){
            if(!index.column())
                continue;
            item = index.data().toString();
            emit sendResultsToRaw(item, RESULT_TYPE::IP);
        }
        emit changeTabToRaw();
        break;

    default:
        break;
    }
}

void Active::m_sendSubdomainToTool(TOOL){
    /* not yet */
}

void Active::m_sendIpToTool(TOOL){
    /* not yet */
}

void Active::m_sendSubdomainToTool(TOOL, QItemSelectionModel *){
    /* not yet */
}

void Active::m_sendIpToTool(TOOL, QItemSelectionModel *){
    /* not yet */
}
