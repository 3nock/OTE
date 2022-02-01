/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#include "Dns.h"
#include "ui_Dns.h"

#include <QClipboard>
#include <QDesktopServices>


void Dns::m_clearResults(){
    /* clear the results... */
    m_resultModel->clear();
    ui->labelResultsCount->clear();
    m_resultSet.clear();

    /* clear the progressbar... */
    ui->progressBar->clearMask();
    ui->progressBar->reset();
    ui->progressBar->hide();
}

void Dns::m_expandResults(){
    ui->treeViewResults->expandAll();
}

void Dns::m_collapseResults(){
    ui->treeViewResults->collapseAll();
}

void Dns::m_openInBrowser(QItemSelectionModel *selectionModel){
    foreach(const QModelIndex &index, selectionModel->selectedIndexes())
        QDesktopServices::openUrl(QUrl("https://"+index.data().toString(), QUrl::TolerantMode));
}

void Dns::m_removeResults(QItemSelectionModel *selectionModel){
    QModelIndex index;
    foreach(const QModelIndex &proxyIndex, selectionModel->selectedIndexes()){
        index = m_resultProxyModel->mapToSource(proxyIndex);
        m_resultSet.remove(index.data().toString());
        m_resultModel->removeRow(index.row());
    }

    ui->labelResultsCount->setNum(m_resultProxyModel->rowCount());
}

void Dns::m_saveResultsAll(){
    /* save as json */
}

void Dns::m_saveResults(RESULT_TYPE resultType){
    /* checks... */
    QString filename = QFileDialog::getSaveFileName(this, "Save To File", "./");
    if(filename.isEmpty())
        return;

    QFile file(filename);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    if(!file.isOpen())
        return;

    /* getting the types */
    QStringList choosenTypes;
    switch(resultType){
    case RESULT_TYPE::SUBDOMAIN:
        choosenTypes.append({"CNAME", "MX", "NS"});
        break;
    case RESULT_TYPE::IP:
        choosenTypes.append({"A", "AAAA"});
        break;
    case RESULT_TYPE::A:
        choosenTypes.append("A");
        break;
    case RESULT_TYPE::AAAA:
        choosenTypes.append("AAAA");
        break;
    case RESULT_TYPE::CNAME:
        choosenTypes.append("CNAME");
        break;
    case RESULT_TYPE::TXT:
        choosenTypes.append("TXT");
        break;
    case RESULT_TYPE::NS:
        choosenTypes.append("NS");
        break;
    case RESULT_TYPE::MX:
        choosenTypes.append("MX");
        break;

    default:
        break;
    }

    /* writing to file */
    QModelIndexList domains;
    for(int i = 0; i < m_resultProxyModel->rowCount(); ++i)
        domains << m_resultProxyModel->index( i, 0 );

    for ( int i = 0; i < domains.size(); ++i )
    {
        for ( int j = 0; j < m_resultProxyModel->rowCount( domains[i] ); ++j )
        {
            QString type = domains[i].child(j, 0).data().toString();
            if(choosenTypes.contains(type))
            {
                for(int k = 0; k < m_resultProxyModel->rowCount(domains[i].child(j, 0)); k++)
                {
                    QString domain = domains[i].child(j, 0).child(k, 0).data().toString();
                    file.write(domain.toUtf8());
                }
            }
        }
    }

    /* finished */
    file.close();
}

void Dns::m_saveResults(QItemSelectionModel *selectionModel){
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

void Dns::m_copyResultsAll(){
    /* copy as json */
}


void Dns::m_copyResults(RESULT_TYPE resultType){
    QClipboard *clipboard = QGuiApplication::clipboard();
    QString clipboardData;

    /* getting the types */
    QStringList choosenTypes;
    switch(resultType){
    case RESULT_TYPE::SUBDOMAIN:
        choosenTypes.append({"CNAME", "MX", "NS"});
        break;
    case RESULT_TYPE::IP:
        choosenTypes.append({"A", "AAAA"});
        break;
    case RESULT_TYPE::A:
        choosenTypes.append("A");
        break;
    case RESULT_TYPE::AAAA:
        choosenTypes.append("AAAA");
        break;
    case RESULT_TYPE::CNAME:
        choosenTypes.append("CNAME");
        break;
    case RESULT_TYPE::TXT:
        choosenTypes.append("TXT");
        break;
    case RESULT_TYPE::NS:
        choosenTypes.append("NS");
        break;
    case RESULT_TYPE::MX:
        choosenTypes.append("MX");
        break;

    default:
        break;
    }

    /* writing to file */
    QModelIndexList domains;
    for(int i = 0; i < m_resultProxyModel->rowCount(); ++i)
        domains << m_resultProxyModel->index( i, 0 );

    for ( int i = 0; i < domains.size(); ++i )
    {
        for ( int j = 0; j < m_resultProxyModel->rowCount( domains[i] ); ++j )
        {
            QString type = domains[i].child(j, 0).data().toString();
            if(choosenTypes.contains(type))
            {
                for(int k = 0; k < m_resultProxyModel->rowCount(domains[i].child(j, 0)); k++)
                {
                    QString domain = domains[i].child(j, 0).child(k, 0).data().toString();
                    clipboardData.append(domain);
                }
            }
        }
    }

    clipboard->setText(clipboardData.trimmed());
}

void Dns::m_copyResults(QItemSelectionModel *selectionModel){
    QClipboard *clipboard = QGuiApplication::clipboard();
    QString data;
    QString item;

    foreach(const QModelIndex &index, selectionModel->selectedIndexes()){
        item = index.data().toString();
        data.append(item.append(NEWLINE));
    }

    clipboard->setText(data.trimmed());
}

void Dns::onReceiveTargets(QString target, RESULT_TYPE resultType){
    if(resultType == RESULT_TYPE::SUBDOMAIN){
        ui->targets->add(target);
    }

    /* set multiple targets checkbox checked */
    ui->checkBoxMultipleTargets->setChecked(true);
}

/*****************************************************************************
                            SENDING RESULTS
******************************************************************************/
void Dns::m_sendSubdomainToEngine(ENGINE engine){
    QString item;
    switch (engine) {
    case ENGINE::OSINT:
    {
        QModelIndexList domains;
        for(int i = 0; i < m_resultProxyModel->rowCount(); ++i)
            domains << m_resultProxyModel->index( i, 0 );
        for ( int i = 0; i < domains.size(); ++i ){
            for ( int j = 0; j < m_resultProxyModel->rowCount( domains[i] ); ++j ){
                QString type = domains[i].child(j, 0).data().toString();
                if(type == "NS" || type == "MX" || type == "CNAME"){
                    for(int k = 0; k < m_resultProxyModel->rowCount(domains[i].child(j, 0)); k++){
                        QString item = domains[i].child(j, 0).child(k, 0).data().toString();
                        emit sendResultsToOsint(item, RESULT_TYPE::SUBDOMAIN);
                    }
                }
            }
        }
        emit changeTabToOsint();
    }
        break;
    case ENGINE::RAW:
    {
        QModelIndexList domains;
        for(int i = 0; i < m_resultProxyModel->rowCount(); ++i)
            domains << m_resultProxyModel->index( i, 0 );
        for ( int i = 0; i < domains.size(); ++i ){
            for ( int j = 0; j < m_resultProxyModel->rowCount( domains[i] ); ++j ){
                QString type = domains[i].child(j, 0).data().toString();
                if(type == "NS" || type == "MX" || type == "CNAME"){
                    for(int k = 0; k < m_resultProxyModel->rowCount(domains[i].child(j, 0)); k++){
                        QString item = domains[i].child(j, 0).child(k, 0).data().toString();
                        emit sendResultsToRaw(item, RESULT_TYPE::SUBDOMAIN);
                    }
                }
            }
        }
        emit changeTabToRaw();
    }
        break;
    case ENGINE::BRUTE:
    {
        QModelIndexList domains;
        for(int i = 0; i < m_resultProxyModel->rowCount(); ++i)
            domains << m_resultProxyModel->index( i, 0 );
        for ( int i = 0; i < domains.size(); ++i ){
            for ( int j = 0; j < m_resultProxyModel->rowCount( domains[i] ); ++j ){
                QString type = domains[i].child(j, 0).data().toString();
                if(type == "NS" || type == "MX" || type == "CNAME"){
                    for(int k = 0; k < m_resultProxyModel->rowCount(domains[i].child(j, 0)); k++){
                        QString item = domains[i].child(j, 0).child(k, 0).data().toString();
                        emit sendResultsToBrute(item, RESULT_TYPE::SUBDOMAIN);
                    }
                }
            }
        }
        emit changeTabToBrute();
    }
        break;
    case ENGINE::ACTIVE:
    {
        QModelIndexList domains;
        for(int i = 0; i < m_resultProxyModel->rowCount(); ++i)
            domains << m_resultProxyModel->index( i, 0 );
        for ( int i = 0; i < domains.size(); ++i ){
            for ( int j = 0; j < m_resultProxyModel->rowCount( domains[i] ); ++j ){
                QString type = domains[i].child(j, 0).data().toString();
                if(type == "NS" || type == "MX" || type == "CNAME"){
                    for(int k = 0; k < m_resultProxyModel->rowCount(domains[i].child(j, 0)); k++){
                        QString item = domains[i].child(j, 0).child(k, 0).data().toString();
                        emit sendResultsToActive(item, RESULT_TYPE::SUBDOMAIN);
                    }
                }
            }
        }
        emit changeTabToActive();
    }
        break;
    case ENGINE::DNS:
    {
        QModelIndexList domains;
        for(int i = 0; i < m_resultProxyModel->rowCount(); ++i)
            domains << m_resultProxyModel->index( i, 0 );
        for ( int i = 0; i < domains.size(); ++i ){
            for ( int j = 0; j < m_resultProxyModel->rowCount( domains[i] ); ++j ){
                QString type = domains[i].child(j, 0).data().toString();
                if(type == "NS" || type == "MX" || type == "CNAME"){
                    for(int k = 0; k < m_resultProxyModel->rowCount(domains[i].child(j, 0)); k++){
                        QString item = domains[i].child(j, 0).child(k, 0).data().toString();
                        emit sendResultsToDns(item, RESULT_TYPE::SUBDOMAIN);
                    }
                }
            }
        }
        emit changeTabToDns();
    }
        break;
    case ENGINE::CERT:
    {
        QModelIndexList domains;
        for(int i = 0; i < m_resultProxyModel->rowCount(); ++i)
            domains << m_resultProxyModel->index( i, 0 );
        for ( int i = 0; i < domains.size(); ++i ){
            for ( int j = 0; j < m_resultProxyModel->rowCount( domains[i] ); ++j ){
                QString type = domains[i].child(j, 0).data().toString();
                if(type == "NS" || type == "MX" || type == "CNAME"){
                    for(int k = 0; k < m_resultProxyModel->rowCount(domains[i].child(j, 0)); k++){
                        QString item = domains[i].child(j, 0).child(k, 0).data().toString();
                        emit sendResultsToCert(item, RESULT_TYPE::SUBDOMAIN);
                    }
                }
            }
        }
        emit changeTabToSSL();
    }
        break;
    default:
        break;
    }
}

void Dns::m_sendIpToEngine(ENGINE engine){
    QString item;

    switch (engine) {
    case ENGINE::OSINT:
    {
        QModelIndexList domains;
        for(int i = 0; i < m_resultProxyModel->rowCount(); ++i)
            domains << m_resultProxyModel->index( i, 0 );
        for ( int i = 0; i < domains.size(); ++i ){
            for ( int j = 0; j < m_resultProxyModel->rowCount( domains[i] ); ++j ){
                QString type = domains[i].child(j, 0).data().toString();
                if(type == "A" || type == "AAAA"){
                    for(int k = 0; k < m_resultProxyModel->rowCount(domains[i].child(j, 0)); k++){
                        QString item = domains[i].child(j, 0).child(k, 0).data().toString();
                        emit sendResultsToOsint(item, RESULT_TYPE::IP);
                    }
                }
            }
        }
        emit changeTabToOsint();
    }
        break;
    case ENGINE::RAW:
    {
        QModelIndexList domains;
        for(int i = 0; i < m_resultProxyModel->rowCount(); ++i)
            domains << m_resultProxyModel->index( i, 0 );
        for ( int i = 0; i < domains.size(); ++i ){
            for ( int j = 0; j < m_resultProxyModel->rowCount( domains[i] ); ++j ){
                QString type = domains[i].child(j, 0).data().toString();
                if(type == "A" || type == "AAAA"){
                    for(int k = 0; k < m_resultProxyModel->rowCount(domains[i].child(j, 0)); k++){
                        QString item = domains[i].child(j, 0).child(k, 0).data().toString();
                        emit sendResultsToRaw(item, RESULT_TYPE::IP);
                    }
                }
            }
        }
        emit changeTabToRaw();
    }
        break;

    default:
        break;
    }
}

void Dns::m_sendSubdomainToEngine(ENGINE engine, QItemSelectionModel *selection){
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

void Dns::m_sendIpToEngine(ENGINE engine, QItemSelectionModel *selection){
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

void Dns::m_sendSubdomainToTool(TOOL){
    /* not yet */
}

void Dns::m_sendIpToTool(TOOL){
    /* not yet */
}

void Dns::m_sendNSToTool(TOOL){
    /* not yet */
}

void Dns::m_sendMXToTool(TOOL){
    /* not yet */
}

void Dns::m_sendSubdomainToTool(TOOL, QItemSelectionModel *){
    /* not yet */
}

void Dns::m_sendIpToTool(TOOL, QItemSelectionModel *){
    /* not yet */
}

void Dns::m_sendNSToTool(TOOL, QItemSelectionModel *){
    /* not yet */
}

void Dns::m_sendMXToTool(TOOL, QItemSelectionModel *){
    /* not yet */
}
