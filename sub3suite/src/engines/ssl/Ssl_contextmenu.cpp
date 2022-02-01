/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#include "Ssl.h"
#include "ui_Ssl.h"


void Ssl::m_initActions(){
    connect(&a_ClearResults, &QAction::triggered, this, [=](){this->m_clearResults();});
    connect(&a_RemoveResults, &QAction::triggered, this, [=](){this->m_removeResults(selectionModel);});
    connect(&a_ExpandResults, &QAction::triggered, this, [=](){this->m_expandResults();});
    connect(&a_CollapseResults, &QAction::triggered, this, [=](){this->m_collapseResults();});
    connect(&a_OpenInBrowser, &QAction::triggered, this, [=](){this->m_openInBrowser(selectionModel);});
    /* ... */
    connect(&a_SendAllCertToOsint, &QAction::triggered, this, [=](){this->m_sendCertToEngine(ENGINE::OSINT);});
    connect(&a_SendAllCertToRaw, &QAction::triggered, this, [=](){this->m_sendCertToEngine(ENGINE::RAW);});
    connect(&a_SendAllHostToOsint, &QAction::triggered, this, [=](){this->m_sendSubdomainToEngine(ENGINE::OSINT);});
    connect(&a_SendAllHostToRaw, &QAction::triggered, this, [=](){this->m_sendSubdomainToEngine(ENGINE::RAW);});
    connect(&a_SendAllHostToBrute, &QAction::triggered, this, [=](){this->m_sendSubdomainToEngine(ENGINE::BRUTE);});
    connect(&a_SendAllHostToActive, &QAction::triggered, this, [=](){this->m_sendSubdomainToEngine(ENGINE::ACTIVE);});
    connect(&a_SendAllHostToDns, &QAction::triggered, this, [=](){this->m_sendSubdomainToEngine(ENGINE::DNS);});
    connect(&a_SendAllHostToCert, &QAction::triggered, this, [=](){this->m_sendSubdomainToEngine(ENGINE::CERT);});
    connect(&a_SendAllCertToCertTool, &QAction::triggered, this, [=](){this->m_sendCertToTool(TOOL::CERT);});
    connect(&a_SendAllHostToCertTool, &QAction::triggered, this, [=](){this->m_sendSubdomainToTool(TOOL::CERT);});
    connect(&a_SendAllHostToDomainTool, &QAction::triggered, this, [=](){this->m_sendSubdomainToTool(TOOL::DOMAINTOOL);});
    /* ... */
    connect(&a_SendSelectedCertToOsint, &QAction::triggered, this, [=](){this->m_sendCertToEngine(ENGINE::OSINT, selectionModel);});
    connect(&a_SendSelectedCertToRaw, &QAction::triggered, this, [=](){this->m_sendCertToEngine(ENGINE::RAW, selectionModel);});
    connect(&a_SendSelectedHostToOsint, &QAction::triggered, this, [=](){this->m_sendSubdomainToEngine(ENGINE::OSINT, selectionModel);});
    connect(&a_SendSelectedHostToRaw, &QAction::triggered, this, [=](){this->m_sendSubdomainToEngine(ENGINE::RAW, selectionModel);});
    connect(&a_SendSelectedHostToBrute, &QAction::triggered, this, [=](){this->m_sendSubdomainToEngine(ENGINE::BRUTE, selectionModel);});
    connect(&a_SendSelectedHostToActive, &QAction::triggered, this, [=](){this->m_sendSubdomainToEngine(ENGINE::ACTIVE, selectionModel);});
    connect(&a_SendSelectedHostToDns, &QAction::triggered, this, [=](){this->m_sendSubdomainToEngine(ENGINE::DNS, selectionModel);});
    connect(&a_SendSelectedHostToCert, &QAction::triggered, this, [=](){this->m_sendSubdomainToEngine(ENGINE::CERT, selectionModel);});
    connect(&a_SendSelectedCertToCertTool, &QAction::triggered, this, [=](){this->m_sendCertToTool(TOOL::CERT, selectionModel);});
    connect(&a_SendSelectedHostToCertTool, &QAction::triggered, this, [=](){this->m_sendSubdomainToTool(TOOL::CERT, selectionModel);});
    connect(&a_SendSelectedHostToDomainTool, &QAction::triggered, this, [=](){this->m_sendSubdomainToTool(TOOL::DOMAINTOOL, selectionModel);});
    /* ... */
    connect(&a_Save, &QAction::triggered, this, [=](){this->m_saveResults(selectionModel);});
    connect(&a_Copy, &QAction::triggered, this, [=](){this->m_copyResults(selectionModel);});
}

void Ssl::on_buttonAction_clicked(){
    /* check if there are results available else dont show the context menu */
    if(m_resultProxyModel->rowCount() < 1)
        return;

    /* getting the position of the action button to place the context menu and
       showing the context menu right by the side of the action button... */
    QPoint pos = ui->buttonAction->mapToGlobal(QPoint(0,0));
    pos = QPoint(pos.x()+65, pos.y());

    /* creating the context menu... */
    QMenu menu(this);

    /* adding actions */
    menu.addAction(&a_ClearResults);
    menu.addSeparator();
    menu.addAction(&a_Save);
    menu.addAction(&a_Copy);
    menu.addSeparator();

    switch (ui->comboBoxOutput->currentIndex()) {
    case 0: // subdomain
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
    case 1: // cert id
        menu.addAction(&a_SendAllCertToOsint);
        menu.addAction(&a_SendAllCertToRaw);
        menu.addSeparator();
        menu.addAction(&a_SendAllCertToCertTool);
        break;
    case 2: // raw cert
        menu.addAction(&a_ExpandResults);
        menu.addAction(&a_CollapseResults);
    }

    /* showing the context menu... */
    menu.exec(pos);
}

void Ssl::on_treeViewResults_customContextMenuRequested(const QPoint &pos){
    Q_UNUSED(pos);

    /* check if user right clicked on items else dont show the context menu... */
    if(!ui->treeViewResults->selectionModel()->isSelected(ui->treeViewResults->currentIndex()))
        return;

    /* getting the selected items... */
    selectionModel = ui->treeViewResults->selectionModel();

    /* creating the context menu... */
    QMenu menu(this);

    /* adding actions */
    menu.addAction(&a_RemoveResults);
    menu.addAction(&a_OpenInBrowser);
    menu.addSeparator();
    menu.addAction(&a_Save);
    menu.addAction(&a_Copy);
    menu.addSeparator();

    switch (ui->comboBoxOutput->currentIndex()) {
    case 0: // subdomain
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
    case 1: // cert id
        menu.addAction(&a_SendSelectedCertToOsint);
        menu.addAction(&a_SendSelectedCertToRaw);
        menu.addSeparator();
        menu.addAction(&a_SendSelectedCertToCertTool);
        break;
    case 2: // raw cert
        /* nothing */
        break;
    }

    /* showing the context menu... */
    menu.exec(QCursor::pos());
}
