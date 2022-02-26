/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#include "Dns.h"
#include "ui_Dns.h"


void Dns::on_buttonAction_clicked(){
    /* check if there are results available else dont show the context menu */
    if(proxyModel->rowCount() < 1)
        return;

    /* getting the position of the action button to place the context menu and
       showing the context menu right by the side of the action button... */
    QPoint pos = ui->buttonAction->mapToGlobal(QPoint(0,0));
    pos = QPoint(pos.x()+65, pos.y());

    /* save menu */
    QMenu saveMenu(this);
    saveMenu.setTitle(tr("Save"));
    saveMenu.setIcon(QIcon(":/img/res/icons/save.png"));
    saveMenu.addAction(tr("DNS"), this, [=](){this->saveResults(RESULT_TYPE::DNS);});
    saveMenu.addSeparator();
    saveMenu.addAction(tr("A"), this, [=](){this->saveResults(RESULT_TYPE::A);});
    saveMenu.addAction(tr("AAAA"), this, [=](){this->saveResults(RESULT_TYPE::AAAA);});
    saveMenu.addAction(tr("MX"), this, [=](){this->saveResults(RESULT_TYPE::MX);});
    saveMenu.addAction(tr("NS"), this, [=](){this->saveResults(RESULT_TYPE::NS);});
    saveMenu.addAction(tr("CNAME"), this, [=](){this->saveResults(RESULT_TYPE::CNAME);});
    saveMenu.addAction(tr("TXT"), this, [=](){this->saveResults(RESULT_TYPE::TXT);});
    saveMenu.addAction(tr("SRV"), this, [=](){this->saveResults(RESULT_TYPE::SRV);});
    saveMenu.addSeparator();
    saveMenu.addAction(tr("Hostnames"), this, [=](){this->saveResults(RESULT_TYPE::SUBDOMAIN);});
    saveMenu.addAction(tr("IpAddresses"), this, [=](){this->saveResults(RESULT_TYPE::IP);});
    /* copy menu */
    QMenu copyMenu(this);
    copyMenu.setTitle(tr("Copy"));
    copyMenu.setIcon(QIcon(":/img/res/icons/copy.png"));
    copyMenu.addAction(tr("DNS"), this, [=](){this->copyResults(RESULT_TYPE::DNS);});
    copyMenu.addSeparator();
    copyMenu.addAction(tr("A"), this, [=](){this->copyResults(RESULT_TYPE::A);});
    copyMenu.addAction(tr("AAAA"), this, [=](){this->copyResults(RESULT_TYPE::AAAA);});
    copyMenu.addAction(tr("MX"), this, [=](){this->copyResults(RESULT_TYPE::MX);});
    copyMenu.addAction(tr("NS"), this, [=](){this->copyResults(RESULT_TYPE::NS);});
    copyMenu.addAction(tr("CNAME"), this, [=](){this->copyResults(RESULT_TYPE::CNAME);});
    copyMenu.addAction(tr("TXT"), this, [=](){this->copyResults(RESULT_TYPE::TXT);});
    copyMenu.addAction(tr("SRV"), this, [=](){this->copyResults(RESULT_TYPE::SRV);});
    copyMenu.addSeparator();
    copyMenu.addAction(tr("Hostnames"), this, [=](){this->copyResults(RESULT_TYPE::SUBDOMAIN);});
    copyMenu.addAction(tr("IpAddresses"), this, [=](){this->copyResults(RESULT_TYPE::IP);});

    /* main menu */
    QMenu menu(this);

    menu.addAction(tr("Clear"), this, [=](){this->clearResults();})->setIcon(QIcon(":/img/res/icons/delete.png"));
    menu.addAction(tr("Expand"), this, [=](){ui->treeViewResults->expandAll();})->setIcon(QIcon(":/img/res/icons/expand.png"));
    menu.addAction(tr("Collapse"), this, [=](){ui->treeViewResults->collapseAll();})->setIcon(QIcon(":/img/res/icons/collapse.png"));
    menu.addSeparator();
    menu.addMenu(&saveMenu);
    menu.addMenu(&copyMenu);
    menu.addSeparator();
    menu.addAction(tr("Send To Project"), this, [=](){this->sendToProject();})->setIcon(QIcon(":/img/res/icons/project.png"));
    menu.addSeparator();
    menu.addAction(tr("Send IpAddress to OSINT"), this, [=](){this->sendToEngine(ENGINE::OSINT, RESULT_TYPE::IP);})->setIcon(QIcon(":/img/res/icons/ip.png"));
    menu.addAction(tr("Send IpAddress to RAW"), this, [=](){this->sendToEngine(ENGINE::RAW, RESULT_TYPE::IP);})->setIcon(QIcon(":/img/res/icons/ip.png"));
    menu.addSeparator();
    menu.addAction(tr("Send Hostname to OSINT"), this, [=](){this->sendToEngine(ENGINE::OSINT, RESULT_TYPE::SUBDOMAIN);})->setIcon(QIcon(":/img/res/icons/domain.png"));
    menu.addAction(tr("Send Hostname to RAW"), this, [=](){this->sendToEngine(ENGINE::RAW, RESULT_TYPE::SUBDOMAIN);})->setIcon(QIcon(":/img/res/icons/domain.png"));
    menu.addAction(tr("Send Hostname to BRUTE"), this, [=](){this->sendToEngine(ENGINE::BRUTE, RESULT_TYPE::SUBDOMAIN);})->setIcon(QIcon(":/img/res/icons/domain.png"));
    menu.addAction(tr("Send Hostname to ACTIVE"), this, [=](){this->sendToEngine(ENGINE::ACTIVE, RESULT_TYPE::SUBDOMAIN);})->setIcon(QIcon(":/img/res/icons/domain.png"));
    menu.addAction(tr("Send Hostname to DNS"), this, [=](){this->sendToEngine(ENGINE::DNS, RESULT_TYPE::SUBDOMAIN);})->setIcon(QIcon(":/img/res/icons/domain.png"));
    menu.addAction(tr("Send Hostname to SSL"), this, [=](){this->sendToEngine(ENGINE::CERT, RESULT_TYPE::SUBDOMAIN);})->setIcon(QIcon(":/img/res/icons/domain.png"));
    menu.addSeparator();
    menu.addAction(tr("Send IpAddress to IP-Enum"), this, [=](){this->sendToEnum(TOOL::IP);})->setIcon(QIcon(":/img/res/icons/ip.png"));
    menu.addAction(tr("Send NS to NS-Enum"), this, [=](){this->sendToEnum(TOOL::NS);})->setIcon(QIcon(":/img/res/icons/ns.png"));
    menu.addAction(tr("Send MX to MX-Enum"), this, [=](){this->sendToEnum(TOOL::MX);})->setIcon(QIcon(":/img/res/icons/mx.png"));

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

    /* main menu */
    QMenu menu(this);

    menu.addAction(tr("Remove"), this, [=](){this->removeResults();})->setIcon(QIcon(":/img/res/icons/delete.png"));
    menu.addAction(tr("Open in Browser"), this, [=](){this->openInBrowser();})->setIcon(QIcon(":/img/res/icons/browser.png"));
    menu.addSeparator();
    menu.addAction(tr("Save"), this, [=](){this->saveSelectedResults();})->setIcon(QIcon(":/img/res/icons/save.png"));
    menu.addAction(tr("Copy"), this, [=](){this->copySelectedResults();})->setIcon(QIcon(":/img/res/icons/copy.png"));
    menu.addSeparator();
    menu.addAction(tr("Send To Project"), this, [=](){this->sendSelectedToProject();})->setIcon(QIcon(":/img/res/icons/project.png"));
    menu.addSeparator();
    menu.addAction(tr("Send IpAddress to OSINT"), this, [=](){this->sendSelectedToEngine(ENGINE::OSINT, RESULT_TYPE::IP);})->setIcon(QIcon(":/img/res/icons/ip.png"));
    menu.addAction(tr("Send IpAddress to RAW"), this, [=](){this->sendSelectedToEngine(ENGINE::RAW, RESULT_TYPE::IP);})->setIcon(QIcon(":/img/res/icons/ip.png"));
    menu.addSeparator();
    menu.addAction(tr("Send Hostname to OSINT"), this, [=](){this->sendSelectedToEngine(ENGINE::OSINT, RESULT_TYPE::SUBDOMAIN);})->setIcon(QIcon(":/img/res/icons/domain.png"));
    menu.addAction(tr("Send Hostname to RAW"), this, [=](){this->sendSelectedToEngine(ENGINE::RAW, RESULT_TYPE::SUBDOMAIN);})->setIcon(QIcon(":/img/res/icons/domain.png"));
    menu.addAction(tr("Send Hostname to BRUTE"), this, [=](){this->sendSelectedToEngine(ENGINE::BRUTE, RESULT_TYPE::SUBDOMAIN);})->setIcon(QIcon(":/img/res/icons/domain.png"));
    menu.addAction(tr("Send Hostname to ACTIVE"), this, [=](){this->sendSelectedToEngine(ENGINE::ACTIVE, RESULT_TYPE::SUBDOMAIN);})->setIcon(QIcon(":/img/res/icons/domain.png"));
    menu.addAction(tr("Send Hostname to DNS"), this, [=](){this->sendSelectedToEngine(ENGINE::DNS, RESULT_TYPE::SUBDOMAIN);})->setIcon(QIcon(":/img/res/icons/domain.png"));
    menu.addAction(tr("Send Hostname to SSL"), this, [=](){this->sendSelectedToEngine(ENGINE::CERT, RESULT_TYPE::SUBDOMAIN);})->setIcon(QIcon(":/img/res/icons/domain.png"));
    menu.addSeparator();
    menu.addAction(tr("Send IpAddress to IP-Enum"), this, [=](){this->sendSelectedToEnum(TOOL::IP);})->setIcon(QIcon(":/img/res/icons/ip.png"));
    menu.addAction(tr("Send MX to MX-Enum"), this, [=](){this->sendSelectedToEnum(TOOL::MX);})->setIcon(QIcon(":/img/res/icons/mx.png"));
    menu.addAction(tr("Send NS to NS-Enum"), this, [=](){this->sendSelectedToEnum(TOOL::NS);})->setIcon(QIcon(":/img/res/icons/ns.png"));

    /* showing the context menu... */
    menu.exec(QCursor::pos());
}
