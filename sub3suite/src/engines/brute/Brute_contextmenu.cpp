/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief : Creating result's tableview contextmenu & actions to perform on the results.
*/

#include "Brute.h"
#include "ui_Brute.h"


void Brute::on_buttonAction_clicked(){
    /* check if there are results available else dont show the context menu */
    if(proxyModel->rowCount() < 1)
        return;

    /* getting the position of the action button to place the context menu and
       showing the context menu right by the side of the action button... */
    QPoint pos = ui->buttonAction->mapToGlobal(QPoint(0,0));
    pos = QPoint(pos.x()+65, pos.y());

    /* creating the context menu... */
    QMenu menu(this);
    QMenu saveMenu(this);
    QMenu copyMenu(this);
    saveMenu.setTitle(tr("Save"));
    copyMenu.setTitle(tr("Copy"));
    saveMenu.setIcon(QIcon(":/img/res/icons/save.png"));
    copyMenu.setIcon(QIcon(":/img/res/icons/copy.png"));

    /* adding actions */
    saveMenu.addAction(tr("Subdomain,Ip"), this, [=](){this->saveResults(RESULT_TYPE::SUBDOMAINIP);});
    saveMenu.addAction(tr("Subdomain"), this, [=](){this->saveResults(RESULT_TYPE::SUBDOMAIN);});
    saveMenu.addAction(tr("Ip"), this, [=](){this->saveResults(RESULT_TYPE::IP);});
    copyMenu.addAction(tr("Subdomain,Ip"), this, [=](){this->copyResults(RESULT_TYPE::SUBDOMAINIP);});
    copyMenu.addAction(tr("Subdomain"), this, [=](){this->copyResults(RESULT_TYPE::SUBDOMAIN);});
    copyMenu.addAction(tr("Ip"), this, [=](){this->copyResults(RESULT_TYPE::IP);});

    /* adding to mainMenu */
    menu.addAction(tr("Clear"), this, [=](){this->clearResults();})->setIcon(QIcon(":/img/res/icons/delete.png"));
    menu.addSeparator();
    menu.addMenu(&saveMenu);
    menu.addMenu(&copyMenu);
    menu.addAction(tr("Extract subdomain"), this, [=](){this->extract();})->setIcon(QIcon(":/img/res/icons/extract.png"));
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
    menu.addAction(tr("Remove"), this, [=](){this->removeResults();})->setIcon(QIcon(":/img/res/icons/delete.png"));
    menu.addAction(tr("Open in Browser"), this, [=](){this->openInBrowser();})->setIcon(QIcon(":/img/res/icons/browser.png"));
    if(selectionModel->columnIntersectsSelection(0, selectionModel->currentIndex().parent()))
        menu.addAction(tr("Extract subdomain"), this, [=](){this->extractSelected();})->setIcon(QIcon(":/img/res/icons/extract.png"));
    menu.addSeparator();
    menu.addAction(tr("Save"), this, [=](){this->saveSelectedResults();})->setIcon(QIcon(":/img/res/icons/save.png"));
    menu.addAction(tr("Copy"), this, [=](){this->copySelectedResults();})->setIcon(QIcon(":/img/res/icons/copy.png"));
    menu.addSeparator();
    menu.addAction(tr("Send To Project"), this, [=](){this->sendSelectedToProject();})->setIcon(QIcon(":/img/res/icons/project.png"));
    menu.addSeparator();
    if(selectionModel->columnIntersectsSelection(1, selectionModel->currentIndex().parent()) ||
       selectionModel->columnIntersectsSelection(2, selectionModel->currentIndex().parent())){
        menu.addAction(tr("Send IpAddress to OSINT"), this, [=](){this->sendSelectedToEngine(ENGINE::OSINT, RESULT_TYPE::IP);})->setIcon(QIcon(":/img/res/icons/ip.png"));
        menu.addAction(tr("Send IpAddress to RAW"), this, [=](){this->sendSelectedToEngine(ENGINE::RAW, RESULT_TYPE::IP);})->setIcon(QIcon(":/img/res/icons/ip.png"));
        menu.addSeparator();
    }
    if(selectionModel->columnIntersectsSelection(0, selectionModel->currentIndex().parent())){
        menu.addAction(tr("Send Hostname to OSINT"), this, [=](){this->sendSelectedToEngine(ENGINE::OSINT, RESULT_TYPE::SUBDOMAIN);})->setIcon(QIcon(":/img/res/icons/domain.png"));
        menu.addAction(tr("Send Hostname to RAW"), this, [=](){this->sendSelectedToEngine(ENGINE::RAW, RESULT_TYPE::SUBDOMAIN);})->setIcon(QIcon(":/img/res/icons/domain.png"));
        menu.addAction(tr("Send Hostname to BRUTE"), this, [=](){this->sendSelectedToEngine(ENGINE::BRUTE, RESULT_TYPE::SUBDOMAIN);})->setIcon(QIcon(":/img/res/icons/domain.png"));
        menu.addAction(tr("Send Hostname to ACTIVE"), this, [=](){this->sendSelectedToEngine(ENGINE::ACTIVE, RESULT_TYPE::SUBDOMAIN);})->setIcon(QIcon(":/img/res/icons/domain.png"));
        menu.addAction(tr("Send Hostname to DNS"), this, [=](){this->sendSelectedToEngine(ENGINE::DNS, RESULT_TYPE::SUBDOMAIN);})->setIcon(QIcon(":/img/res/icons/domain.png"));
        menu.addAction(tr("Send Hostname to SSL"), this, [=](){this->sendSelectedToEngine(ENGINE::CERT, RESULT_TYPE::SUBDOMAIN);})->setIcon(QIcon(":/img/res/icons/domain.png"));
        menu.addSeparator();
    }
    if(selectionModel->columnIntersectsSelection(1, selectionModel->currentIndex().parent()) ||
       selectionModel->columnIntersectsSelection(2, selectionModel->currentIndex().parent()))
        menu.addAction(tr("Send IpAddress to IP-Enum"), this, [=](){this->sendSelectedToEnum(TOOL::IP);})->setIcon(QIcon(":/img/res/icons/ip.png"));

    /* showing the context menu... */
    menu.exec(QCursor::pos());
}
