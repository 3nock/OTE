/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#include "Dns.h"
#include "ui_Dns.h"


void Dns::m_initActions(){
    connect(&a_ClearResults, &QAction::triggered, this, [=](){this->m_clearResults();});
    connect(&a_RemoveResults, &QAction::triggered, this, [=](){this->m_removeResults(selectionModel);});
    connect(&a_ExpandResults, &QAction::triggered, this, [=](){this->m_expandResults();});
    connect(&a_CollapseResults, &QAction::triggered, this, [=](){this->m_collapseResults();});
    connect(&a_OpenInBrowser, &QAction::triggered, this, [=](){this->m_openInBrowser(selectionModel);});
    /* ... */
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
    QMenu menu(this);
    QMenu saveMenu(this);
    QMenu copyMenu(this);
    saveMenu.setTitle("Save");
    copyMenu.setTitle("Copy");

    /* adding actions */
    saveMenu.addAction(&a_SaveAll);
    saveMenu.addAction(&a_SaveHostnames);
    saveMenu.addAction(&a_SaveIp);
    saveMenu.addSeparator();
    saveMenu.addAction(&a_SaveA);
    saveMenu.addAction(&a_SaveAAAA);
    saveMenu.addAction(&a_SaveCNAME);
    saveMenu.addAction(&a_SaveMX);
    saveMenu.addAction(&a_SaveNS);
    saveMenu.addAction(&a_SaveTXT);
    copyMenu.addAction(&a_CopyAll);
    copyMenu.addAction(&a_CopyHostnames);
    copyMenu.addAction(&a_CopyIp);
    copyMenu.addSeparator();
    copyMenu.addAction(&a_CopyA);
    copyMenu.addAction(&a_CopyAAAA);
    copyMenu.addAction(&a_CopyCNAME);
    copyMenu.addAction(&a_CopyMX);
    copyMenu.addAction(&a_CopyNS);
    copyMenu.addAction(&a_CopyTXT);

    /* adding to mainMenu */
    menu.addAction(&a_ClearResults);
    menu.addAction(&a_ExpandResults);
    menu.addAction(&a_CollapseResults);
    menu.addSeparator();
    menu.addMenu(&saveMenu);
    menu.addMenu(&copyMenu);
    menu.addSeparator();
    menu.addAction(&a_SendIpToOsint);
    menu.addAction(&a_SendIpToRaw);
    menu.addSeparator();
    menu.addAction(&a_SendHostToOsint);
    menu.addAction(&a_SendHostToRaw);
    menu.addAction(&a_SendHostToBrute);
    menu.addAction(&a_SendHostToActive);
    menu.addAction(&a_SendHostToDns);
    menu.addAction(&a_SendHostToCert);
    menu.addSeparator();
    menu.addAction(&a_SendNSToNSTool);
    menu.addAction(&a_SendMXToMXTool);
    menu.addAction(&a_SendIpToIpTool);
    menu.addAction(&a_SendHostToCertTool);
    menu.addAction(&a_SendHostToDomainTool);

    /* showing the context menu... */
    menu.exec(pos);
}

void Dns::on_treeViewResults_customContextMenuRequested(const QPoint &pos){
    Q_UNUSED(pos);

    /* check if user right clicked on items else dont show the context menu... */
    if(!ui->treeViewResults->selectionModel()->isSelected(ui->treeViewResults->currentIndex()))
        return;

    /* getting the selected items... */
    selectionModel = ui->treeViewResults->selectionModel();

    /* creating the context menu... */
    QMenu menu(this);

    /* adding to mainMenu */
    menu.addAction(&a_RemoveResults);
    menu.addAction(&a_OpenInBrowser);
    menu.addSeparator();
    menu.addAction(&a_Save);
    menu.addAction(&a_Copy);
    menu.addSeparator();
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
    menu.addAction(&a_SendSelectedNSToNSTool);
    menu.addAction(&a_SendSelectedMXToMXTool);
    menu.addAction(&a_SendSelectedIpToIpTool);
    menu.addAction(&a_SendSelectedHostToCertTool);
    menu.addAction(&a_SendSelectedHostToDomainTool);

    /* showing the context menu... */
    menu.exec(QCursor::pos());
}
