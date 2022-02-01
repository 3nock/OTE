/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#include "Active.h"
#include "ui_Active.h"


void Active::m_initActions(){
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

void Active::on_buttonAction_clicked(){
    /* check if there are results available else dont show the context menu */
    if(m_resultProxyModel->rowCount() < 1)
        return;

    /* getting the position of the action button to place the context menu and
       showing the context menu right by the side of the action button... */
    QPoint pos = ui->buttonAction->mapToGlobal(QPoint(0,0));
    pos = QPoint(pos.x()+63, pos.y());

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

void Active::on_tableViewResults_customContextMenuRequested(const QPoint &pos){
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
