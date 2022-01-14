/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#include "Dns.h"
#include "ui_Dns.h"

#include <QClipboard>
#include <QDesktopServices>


void Dns::m_initActions(){
    connect(&a_ClearResults, &QAction::triggered, this, [=](){this->m_clearResults();});
    connect(&a_RemoveResults, &QAction::triggered, this, [=](){this->m_removeResults(selectionModel);});
    connect(&a_ExpandResults, &QAction::triggered, this, [=](){this->m_expandResults();});
    connect(&a_CollapseResults, &QAction::triggered, this, [=](){this->m_collapseResults();});
    connect(&a_OpenInBrowser, &QAction::triggered, this, [=](){this->m_openInBrowser(selectionModel);});
    /* ... */
    connect(&a_SendIpToIp, &QAction::triggered, this, [=](){this->m_sendIpToEngine(ENGINE::IP);});
    connect(&a_SendIpToOsint, &QAction::triggered, this, [=](){this->m_sendIpToEngine(ENGINE::OSINT);});
    connect(&a_SendIpToRaw, &QAction::triggered, this, [=](){this->m_sendIpToEngine(ENGINE::RAW);});
    connect(&a_SendHostToOsint, &QAction::triggered, this, [=](){this->m_sendSubdomainToEngine(ENGINE::OSINT);});
    connect(&a_SendHostToRaw, &QAction::triggered, this, [=](){this->m_sendSubdomainToEngine(ENGINE::RAW);});
    connect(&a_SendHostToBrute, &QAction::triggered, this, [=](){this->m_sendSubdomainToEngine(ENGINE::BRUTE);});
    connect(&a_SendHostToActive, &QAction::triggered, this, [=](){this->m_sendSubdomainToEngine(ENGINE::ACTIVE);});
    connect(&a_SendHostToDns, &QAction::triggered, this, [=](){this->m_sendSubdomainToEngine(ENGINE::DNS);});
    connect(&a_SendHostToCert, &QAction::triggered, this, [=](){this->m_sendSubdomainToEngine(ENGINE::CERT);});
    connect(&a_SendIpToIpTool, &QAction::triggered, this, [=](){this->m_sendIpToTool(TOOL::IP);});
    connect(&a_SendHostToCertTool, &QAction::triggered, this, [=](){this->m_sendSubdomainToTool(TOOL::CERT);});
    connect(&a_SendHostToDomainTool, &QAction::triggered, this, [=](){this->m_sendSubdomainToTool(TOOL::DOMAINTOOL);});
    connect(&a_SendNSToNSTool, &QAction::triggered, this, [=](){this->m_sendNSToTool(TOOL::NS);});
    connect(&a_SendMXToMXTool, &QAction::triggered, this, [=](){this->m_sendMXToTool(TOOL::MX);});
    /* ... */
    connect(&a_SendSelectedIpToIp, &QAction::triggered, this, [=](){this->m_sendIpToEngine(ENGINE::IP, selectionModel);});
    connect(&a_SendSelectedIpToOsint, &QAction::triggered, this, [=](){this->m_sendIpToEngine(ENGINE::OSINT, selectionModel);});
    connect(&a_SendSelectedIpToRaw, &QAction::triggered, this, [=](){this->m_sendIpToEngine(ENGINE::RAW, selectionModel);});
    connect(&a_SendSelectedHostToOsint, &QAction::triggered, this, [=](){this->m_sendSubdomainToEngine(ENGINE::OSINT, selectionModel);});
    connect(&a_SendSelectedHostToRaw, &QAction::triggered, this, [=](){this->m_sendSubdomainToEngine(ENGINE::RAW, selectionModel);});
    connect(&a_SendSelectedHostToBrute, &QAction::triggered, this, [=](){this->m_sendSubdomainToEngine(ENGINE::BRUTE, selectionModel);});
    connect(&a_SendSelectedHostToActive, &QAction::triggered, this, [=](){this->m_sendSubdomainToEngine(ENGINE::ACTIVE, selectionModel);});
    connect(&a_SendSelectedHostToDns, &QAction::triggered, this, [=](){this->m_sendSubdomainToEngine(ENGINE::DNS, selectionModel);});
    connect(&a_SendSelectedHostToCert, &QAction::triggered, this, [=](){this->m_sendSubdomainToEngine(ENGINE::CERT, selectionModel);});
    connect(&a_SendSelectedIpToIpTool, &QAction::triggered, this, [=](){this->m_sendIpToTool(TOOL::IP, selectionModel);});
    connect(&a_SendSelectedHostToCertTool, &QAction::triggered, this, [=](){this->m_sendSubdomainToTool(TOOL::CERT, selectionModel);});
    connect(&a_SendSelectedHostToDomainTool, &QAction::triggered, this, [=](){this->m_sendSubdomainToTool(TOOL::DOMAINTOOL, selectionModel);});
    connect(&a_SendSelectedNSToNSTool, &QAction::triggered, this, [=](){this->m_sendNSToTool(TOOL::NS, selectionModel);});
    connect(&a_SendSelectedMXToMXTool, &QAction::triggered, this, [=](){this->m_sendMXToTool(TOOL::MX, selectionModel);});
    /* ... */
    connect(&a_SaveAll, &QAction::triggered, this, [=](){this->m_saveResultsAll();});
    connect(&a_Save, &QAction::triggered, this, [=](){this->m_saveResults(selectionModel);});
    connect(&a_SaveIp, &QAction::triggered, this, [=](){this->m_saveResults(RESULT_TYPE::IP);});
    connect(&a_SaveHostnames, &QAction::triggered, this, [=](){this->m_saveResults(RESULT_TYPE::SUBDOMAIN);});
    connect(&a_SaveA, &QAction::triggered, this, [=](){this->m_saveResults(RESULT_TYPE::A);});
    connect(&a_SaveAAAA, &QAction::triggered, this, [=](){this->m_saveResults(RESULT_TYPE::AAAA);});
    connect(&a_SaveCNAME, &QAction::triggered, this, [=](){this->m_saveResults(RESULT_TYPE::CNAME);});
    connect(&a_SaveNS, &QAction::triggered, this, [=](){this->m_saveResults(RESULT_TYPE::NS);});
    connect(&a_SaveMX, &QAction::triggered, this, [=](){this->m_saveResults(RESULT_TYPE::MX);});
    connect(&a_SaveTXT, &QAction::triggered, this, [=](){this->m_saveResults(RESULT_TYPE::TXT);});
    connect(&a_CopyAll, &QAction::triggered, this, [=](){this->m_copyResultsAll();});
    connect(&a_Copy, &QAction::triggered, this, [=](){this->m_copyResults(selectionModel);});
    connect(&a_CopyIp, &QAction::triggered, this, [=](){this->m_copyResults(RESULT_TYPE::IP);});
    connect(&a_CopyHostnames, &QAction::triggered, this, [=](){this->m_copyResults(RESULT_TYPE::SUBDOMAIN);});
    connect(&a_CopyA, &QAction::triggered, this, [=](){this->m_copyResults(RESULT_TYPE::A);});
    connect(&a_CopyAAAA, &QAction::triggered, this, [=](){this->m_copyResults(RESULT_TYPE::AAAA);});
    connect(&a_CopyCNAME, &QAction::triggered, this, [=](){this->m_copyResults(RESULT_TYPE::CNAME);});
    connect(&a_CopyNS, &QAction::triggered, this, [=](){this->m_copyResults(RESULT_TYPE::NS);});
    connect(&a_CopyMX, &QAction::triggered, this, [=](){this->m_copyResults(RESULT_TYPE::MX);});
    connect(&a_CopyTXT, &QAction::triggered, this, [=](){this->m_copyResults(RESULT_TYPE::TXT);});
}

void Dns::on_buttonAction_clicked(){
    /* check if there are results available else dont show the context menu */
    if(m_resultProxyModel->rowCount() < 1)
        return;

    /* getting the position of the action button to place the context menu and
       showing the context menu right by the side of the action button... */
    QPoint pos = ui->buttonAction->mapToGlobal(QPoint(0,0));
    pos = QPoint(pos.x()+65, pos.y());

    /* creating the context menu... */
    QMenu *mainMenu = new QMenu(this);
    QMenu *saveMenu = new QMenu(this);
    QMenu *copyMenu = new QMenu(this);
    saveMenu->setTitle("Save");
    copyMenu->setTitle("Copy");
    mainMenu->setAttribute(Qt::WA_DeleteOnClose, true);

    /* adding actions */
    saveMenu->addAction(&a_SaveAll);
    saveMenu->addAction(&a_SaveHostnames);
    saveMenu->addAction(&a_SaveIp);
    saveMenu->addSeparator();
    saveMenu->addAction(&a_SaveA);
    saveMenu->addAction(&a_SaveAAAA);
    saveMenu->addAction(&a_SaveCNAME);
    saveMenu->addAction(&a_SaveMX);
    saveMenu->addAction(&a_SaveNS);
    saveMenu->addAction(&a_SaveTXT);
    copyMenu->addAction(&a_CopyAll);
    copyMenu->addAction(&a_CopyHostnames);
    copyMenu->addAction(&a_CopyIp);
    copyMenu->addSeparator();
    copyMenu->addAction(&a_CopyA);
    copyMenu->addAction(&a_CopyAAAA);
    copyMenu->addAction(&a_CopyCNAME);
    copyMenu->addAction(&a_CopyMX);
    copyMenu->addAction(&a_CopyNS);
    copyMenu->addAction(&a_CopyTXT);

    /* adding to mainMenu */
    mainMenu->addAction(&a_ClearResults);
    mainMenu->addAction(&a_ExpandResults);
    mainMenu->addAction(&a_CollapseResults);
    mainMenu->addSeparator();
    mainMenu->addMenu(saveMenu);
    mainMenu->addMenu(copyMenu);
    mainMenu->addSeparator();
    mainMenu->addAction(&a_SendIpToIp);
    mainMenu->addAction(&a_SendIpToOsint);
    mainMenu->addAction(&a_SendIpToRaw);
    mainMenu->addSeparator();
    mainMenu->addAction(&a_SendHostToOsint);
    mainMenu->addAction(&a_SendHostToRaw);
    mainMenu->addAction(&a_SendHostToBrute);
    mainMenu->addAction(&a_SendHostToActive);
    mainMenu->addAction(&a_SendHostToDns);
    mainMenu->addAction(&a_SendHostToCert);
    mainMenu->addSeparator();
    mainMenu->addAction(&a_SendNSToNSTool);
    mainMenu->addAction(&a_SendMXToMXTool);
    mainMenu->addAction(&a_SendIpToIpTool);
    mainMenu->addAction(&a_SendHostToCertTool);
    mainMenu->addAction(&a_SendHostToDomainTool);

    /* showing the context menu... */
    mainMenu->exec(pos);
}

void Dns::on_treeViewResults_customContextMenuRequested(const QPoint &pos){
    Q_UNUSED(pos);

    /* check if user right clicked on items else dont show the context menu... */
    if(!ui->treeViewResults->selectionModel()->isSelected(ui->treeViewResults->currentIndex()))
        return;

    /* getting the selected items... */
    selectionModel = ui->treeViewResults->selectionModel();

    /* creating the context menu... */
    QMenu *mainMenu = new QMenu(this);
    mainMenu->setAttribute(Qt::WA_DeleteOnClose, true);

    /* adding to mainMenu */
    mainMenu->addAction(&a_RemoveResults);
    mainMenu->addAction(&a_OpenInBrowser);
    mainMenu->addSeparator();
    mainMenu->addAction(&a_Save);
    mainMenu->addAction(&a_Copy);
    mainMenu->addSeparator();
    mainMenu->addAction(&a_SendSelectedIpToIp);
    mainMenu->addAction(&a_SendSelectedIpToOsint);
    mainMenu->addAction(&a_SendSelectedIpToRaw);
    mainMenu->addSeparator();
    mainMenu->addAction(&a_SendSelectedHostToOsint);
    mainMenu->addAction(&a_SendSelectedHostToRaw);
    mainMenu->addAction(&a_SendSelectedHostToBrute);
    mainMenu->addAction(&a_SendSelectedHostToActive);
    mainMenu->addAction(&a_SendSelectedHostToDns);
    mainMenu->addAction(&a_SendSelectedHostToCert);
    mainMenu->addSeparator();
    mainMenu->addAction(&a_SendSelectedNSToNSTool);
    mainMenu->addAction(&a_SendSelectedMXToMXTool);
    mainMenu->addAction(&a_SendSelectedIpToIpTool);
    mainMenu->addAction(&a_SendSelectedHostToCertTool);
    mainMenu->addAction(&a_SendSelectedHostToDomainTool);

    /* showing the context menu... */
    mainMenu->exec(QCursor::pos());
}

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
    foreach(const QModelIndex &proxyIndex, selectionModel->selectedIndexes()){
        auto index = m_resultProxyModel->mapToSource(proxyIndex);
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
        emit changeTabToCert();
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
    case ENGINE::IP:
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
                        emit sendResultsToIp(item, RESULT_TYPE::IP);
                    }
                }
            }
        }
        emit changeTabToIp();
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
        emit changeTabToCert();
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
    case ENGINE::IP:
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
