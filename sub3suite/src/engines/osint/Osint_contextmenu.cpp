/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief : Creating result's treeview contextmenu & actions to perform on the results.
*/

#include "Osint.h"
#include "ui_Osint.h"

#include <QMenu>


void Osint::on_buttonAction_clicked(){
    /* check if there are results available else dont show the context menu */
    if(proxyModel->rowCount() < 1)
        return;

    /* getting the position of the action button to place the context menu and
       showing the context menu right by the side of the action button... */
    QPoint pos = ui->buttonAction->mapToGlobal(QPoint(0,0));
    pos = QPoint(pos.x()+60, pos.y());

    /* creating the context menu... */
    QMenu menu(this);

    /* adding actions */
    menu.addAction(tr("Clear"), this, [=](){this->clearResults();})->setIcon(QIcon(":/img/res/icons/delete.png"));
    menu.addSeparator();

    /* extract menu */
    QMenu extractMenu(this);
    extractMenu.setTitle(tr("Extract"));
    extractMenu.setIcon(QIcon(":/img/res/icons/extract.png"));
    extractMenu.addAction(tr("Subdomain(*.)"), this, [=](){this->extract(true, false);});
    extractMenu.addAction(tr("Top level domain(.*)"), this, [=](){this->extract(false, true);});

    switch(ui->comboBoxOutput->currentIndex()){
    case osint::OUTPUT::SUBDOMAINIP:
    {
        QMenu saveMenu(this);
        QMenu copyMenu(this);
        saveMenu.setIcon(QIcon(":/img/res/icons/save.png"));
        copyMenu.setIcon(QIcon(":/img/res/icons/copy.png"));
        saveMenu.setTitle(tr("Save"));
        copyMenu.setTitle(tr("Copy"));
        saveMenu.addAction(tr("Subdomain,Ip"), this, [=](){this->saveResults(RESULT_TYPE::SUBDOMAINIP);});
        saveMenu.addAction(tr("Subdomain"), this, [=](){this->saveResults(RESULT_TYPE::SUBDOMAIN);});
        saveMenu.addAction(tr("IpAddress"), this, [=](){this->saveResults(RESULT_TYPE::IP);});
        copyMenu.addAction(tr("Subdomain,Ip"), this, [=](){this->copyResults(RESULT_TYPE::SUBDOMAINIP);});
        copyMenu.addAction(tr("Subdomain"), this, [=](){this->saveResults(RESULT_TYPE::SUBDOMAIN);});
        copyMenu.addAction(tr("IpAddress"), this, [=](){this->saveResults(RESULT_TYPE::IP);});
        menu.addMenu(&saveMenu);
        menu.addMenu(&copyMenu);
        menu.addMenu(&extractMenu);
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
        menu.addAction(tr("Send Hostname to SSL"), this, [=](){this->sendToEngine(ENGINE::SSL, RESULT_TYPE::SUBDOMAIN);})->setIcon(QIcon(":/img/res/icons/domain.png"));
        menu.addSeparator();
        menu.addAction(tr("Send IpAddress to IP-Enum"), this, [=](){this->sendToEnum(TOOL::IP, RESULT_TYPE::IP);})->setIcon(QIcon(":/img/res/icons/ip.png"));
    }
        break;
    case osint::OUTPUT::SUBDOMAIN:
        menu.addAction(tr("Save"), this, [=](){this->saveResults(RESULT_TYPE::SUBDOMAIN);})->setIcon(QIcon(":/img/res/icons/save.png"));
        menu.addAction(tr("Copy"), this, [=](){this->copyResults(RESULT_TYPE::SUBDOMAIN);})->setIcon(QIcon(":/img/res/icons/copy.png"));
        menu.addMenu(&extractMenu);
        menu.addSeparator();
        menu.addAction(tr("Send To Project"), this, [=](){this->sendToProject();})->setIcon(QIcon(":/img/res/icons/project.png"));
        menu.addSeparator();
        menu.addAction(tr("Send Hostname to OSINT"), this, [=](){this->sendToEngine(ENGINE::OSINT, RESULT_TYPE::SUBDOMAIN);})->setIcon(QIcon(":/img/res/icons/domain.png"));
        menu.addAction(tr("Send Hostname to RAW"), this, [=](){this->sendToEngine(ENGINE::RAW, RESULT_TYPE::SUBDOMAIN);})->setIcon(QIcon(":/img/res/icons/domain.png"));
        menu.addAction(tr("Send Hostname to BRUTE"), this, [=](){this->sendToEngine(ENGINE::BRUTE, RESULT_TYPE::SUBDOMAIN);})->setIcon(QIcon(":/img/res/icons/domain.png"));
        menu.addAction(tr("Send Hostname to ACTIVE"), this, [=](){this->sendToEngine(ENGINE::ACTIVE, RESULT_TYPE::SUBDOMAIN);})->setIcon(QIcon(":/img/res/icons/domain.png"));
        menu.addAction(tr("Send Hostname to DNS"), this, [=](){this->sendToEngine(ENGINE::DNS, RESULT_TYPE::SUBDOMAIN);})->setIcon(QIcon(":/img/res/icons/domain.png"));
        menu.addAction(tr("Send Hostname to SSL"), this, [=](){this->sendToEngine(ENGINE::SSL, RESULT_TYPE::SUBDOMAIN);})->setIcon(QIcon(":/img/res/icons/domain.png"));
        break;
    case osint::OUTPUT::IP:
        menu.addAction(tr("Save"), this, [=](){this->saveResults(RESULT_TYPE::IP);})->setIcon(QIcon(":/img/res/icons/save.png"));
        menu.addAction(tr("Copy"), this, [=](){this->copyResults(RESULT_TYPE::IP);})->setIcon(QIcon(":/img/res/icons/copy.png"));
        menu.addSeparator();
        menu.addAction(tr("Send To Project"), this, [=](){this->sendToProject();})->setIcon(QIcon(":/img/res/icons/project.png"));
        menu.addSeparator();
        menu.addAction(tr("Send IpAddress to OSINT"), this, [=](){this->sendToEngine(ENGINE::OSINT, RESULT_TYPE::IP);})->setIcon(QIcon(":/img/res/icons/ip.png"));
        menu.addAction(tr("Send IpAddress to RAW"), this, [=](){this->sendToEngine(ENGINE::RAW, RESULT_TYPE::IP);})->setIcon(QIcon(":/img/res/icons/ip.png"));
        menu.addSeparator();
        menu.addAction(tr("Send IpAddress to IP-Enum"), this, [=](){this->sendToEnum(TOOL::IP, RESULT_TYPE::IP);})->setIcon(QIcon(":/img/res/icons/ip.png"));
        break;
    case osint::OUTPUT::EMAIL:
        menu.addAction(tr("Save"), this, [=](){this->saveResults(RESULT_TYPE::EMAIL);})->setIcon(QIcon(":/img/res/icons/save.png"));
        menu.addAction(tr("Copy"), this, [=](){this->copyResults(RESULT_TYPE::EMAIL);})->setIcon(QIcon(":/img/res/icons/copy.png"));
        menu.addSeparator();
        menu.addAction(tr("Send To Project"), this, [=](){this->sendToProject();})->setIcon(QIcon(":/img/res/icons/project.png"));
        menu.addSeparator();
        menu.addAction(tr("Send Email to OSINT"), this, [=](){this->sendToEngine(ENGINE::OSINT, RESULT_TYPE::EMAIL);})->setIcon(QIcon(":/img/res/icons/email.png"));
        menu.addAction(tr("Send Email to RAW"), this, [=](){this->sendToEngine(ENGINE::RAW, RESULT_TYPE::EMAIL);})->setIcon(QIcon(":/img/res/icons/email.png"));
        menu.addSeparator();
        menu.addAction(tr("Send Email to Email-Enum"), this, [=](){this->sendToEnum(TOOL::EMAIL, RESULT_TYPE::EMAIL);})->setIcon(QIcon(":/img/res/icons/email.png"));
        break;
    case osint::OUTPUT::URL:
        menu.addAction(tr("Save"), this, [=](){this->saveResults(RESULT_TYPE::URL);})->setIcon(QIcon(":/img/res/icons/save.png"));
        menu.addAction(tr("Copy"), this, [=](){this->copyResults(RESULT_TYPE::URL);})->setIcon(QIcon(":/img/res/icons/copy.png"));
        menu.addSeparator();
        menu.addAction(tr("Send To Project"), this, [=](){this->sendToProject();})->setIcon(QIcon(":/img/res/icons/project.png"));
        menu.addSeparator();
        menu.addAction(tr("Send URL to URL"), this, [=](){this->sendToEngine(ENGINE::URL, RESULT_TYPE::URL);})->setIcon(QIcon(":/img/res/icons/url.png"));
        menu.addAction(tr("Send URL to RAW"), this, [=](){this->sendToEngine(ENGINE::RAW, RESULT_TYPE::URL);})->setIcon(QIcon(":/img/res/icons/url.png"));
        break;
    case osint::OUTPUT::ASN:
        menu.addAction(tr("Save"), this, [=](){this->saveResults(RESULT_TYPE::ASN);})->setIcon(QIcon(":/img/res/icons/save.png"));
        menu.addAction(tr("Copy"), this, [=](){this->copyResults(RESULT_TYPE::ASN);})->setIcon(QIcon(":/img/res/icons/copy.png"));
        menu.addSeparator();
        menu.addAction(tr("Send To Project"), this, [=](){this->sendToProject();})->setIcon(QIcon(":/img/res/icons/project.png"));
        menu.addSeparator();
        menu.addAction(tr("Send ASN to OSINT"), this, [=](){this->sendToEngine(ENGINE::OSINT, RESULT_TYPE::ASN);})->setIcon(QIcon(":/img/res/icons/asn.png"));
        menu.addAction(tr("Send ASN to RAW"), this, [=](){this->sendToEngine(ENGINE::RAW, RESULT_TYPE::ASN);})->setIcon(QIcon(":/img/res/icons/asn.png"));
        menu.addSeparator();
        menu.addAction(tr("Send ASN to ASN-Enum"), this, [=](){this->sendToEnum(TOOL::ASN, RESULT_TYPE::ASN);})->setIcon(QIcon(":/img/res/icons/asn.png"));
        break;
    case osint::OUTPUT::CERT:
        menu.addAction(tr("Save"), this, [=](){this->saveResults(RESULT_TYPE::CERT_ID);})->setIcon(QIcon(":/img/res/icons/save.png"));
        menu.addAction(tr("Copy"), this, [=](){this->copyResults(RESULT_TYPE::CERT_ID);})->setIcon(QIcon(":/img/res/icons/copy.png"));
        menu.addSeparator();
        menu.addAction(tr("Send To Project"), this, [=](){this->sendToProject();})->setIcon(QIcon(":/img/res/icons/project.png"));
        menu.addSeparator();
        menu.addAction(tr("Send SSL to OSINT"), this, [=](){this->sendToEngine(ENGINE::OSINT, RESULT_TYPE::CERT_ID);})->setIcon(QIcon(":/img/res/icons/ssl.png"));
        menu.addAction(tr("Send SSL to RAW"), this, [=](){this->sendToEngine(ENGINE::RAW, RESULT_TYPE::CERT_ID);})->setIcon(QIcon(":/img/res/icons/ssl.png"));
        menu.addSeparator();
        menu.addAction(tr("Send SSL to SSL-Enum"), this, [=](){this->sendToEnum(TOOL::SSL, RESULT_TYPE::CERT_ID);})->setIcon(QIcon(":/img/res/icons/ssl.png"));
        break;
    case osint::OUTPUT::CIDR:
        menu.addAction(tr("Save"), this, [=](){this->saveResults(RESULT_TYPE::CIDR);})->setIcon(QIcon(":/img/res/icons/save.png"));
        menu.addAction(tr("Copy"), this, [=](){this->copyResults(RESULT_TYPE::CIDR);})->setIcon(QIcon(":/img/res/icons/copy.png"));
        menu.addSeparator();
        menu.addAction(tr("Send To Project"), this, [=](){this->sendToProject();})->setIcon(QIcon(":/img/res/icons/project.png"));
        menu.addSeparator();
        menu.addAction(tr("Send CIDR to OSINT"), this, [=](){this->sendToEngine(ENGINE::OSINT, RESULT_TYPE::CIDR);})->setIcon(QIcon(":/img/res/icons/cidr.png"));
        menu.addAction(tr("Send CIDR to RAW"), this, [=](){this->sendToEngine(ENGINE::RAW, RESULT_TYPE::CIDR);})->setIcon(QIcon(":/img/res/icons/cidr.png"));
        menu.addSeparator();
        menu.addAction(tr("Send CIDR to CIDR-Enum"), this, [=](){this->sendToEnum(TOOL::CIDR, RESULT_TYPE::CIDR);})->setIcon(QIcon(":/img/res/icons/cidr.png"));
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

    /* extract menu */
    QMenu extractMenu(this);
    extractMenu.setTitle(tr("Extract"));
    extractMenu.setIcon(QIcon(":/img/res/icons/extract.png"));
    extractMenu.addAction(tr("Subdomain(*.)"), this, [=](){this->extractSelected(true, false);});
    extractMenu.addAction(tr("Top level domain(.*)"), this, [=](){this->extractSelected(false, true);});

    /* adding actions */
    menu.addAction(tr("Remove"), this, [=](){this->removeResults();})->setIcon(QIcon(":/img/res/icons/delete.png"));
    menu.addAction(tr("Open In Browser"), this, [=](){this->openInBrowser();})->setIcon(QIcon(":/img/res/icons/browser.png"));
    menu.addSeparator();
    menu.addAction(tr("Save"), this, [=](){this->saveSelectedResults();})->setIcon(QIcon(":/img/res/icons/save.png"));
    menu.addAction(tr("Copy"), this, [=](){this->copySelectedResults();})->setIcon(QIcon(":/img/res/icons/copy.png"));
    if(ui->comboBoxOutput->currentIndex() == osint::OUTPUT::SUBDOMAINIP ||
       ui->comboBoxOutput->currentIndex() == osint::OUTPUT::SUBDOMAIN)
        menu.addMenu(&extractMenu);
    menu.addSeparator();
    menu.addAction(tr("Send To Project"), this, [=](){this->sendSelectedToProject();})->setIcon(QIcon(":/img/res/icons/project.png"));
    menu.addSeparator();

    switch(ui->comboBoxOutput->currentIndex()){
    case osint::OUTPUT::SUBDOMAINIP:
        if(selectionModel->columnIntersectsSelection(1, selectionModel->currentIndex().parent())){
            menu.addAction(tr("Send IpAddress To OSINT"), this, [=](){this->sendSelectedToEngine(ENGINE::OSINT, RESULT_TYPE::IP);})->setIcon(QIcon(":/img/res/icons/ip.png"));
            menu.addAction(tr("Send IpAddress To RAW"), this, [=](){this->sendSelectedToEngine(ENGINE::RAW, RESULT_TYPE::IP);})->setIcon(QIcon(":/img/res/icons/ip.png"));
            menu.addSeparator();
        }
        if(selectionModel->columnIntersectsSelection(0, selectionModel->currentIndex().parent())){
            menu.addAction(tr("Send Hostname To OSINT"), this, [=](){this->sendSelectedToEngine(ENGINE::OSINT, RESULT_TYPE::SUBDOMAIN);})->setIcon(QIcon(":/img/res/icons/domain.png"));
            menu.addAction(tr("Send Hostname To RAW"), this, [=](){this->sendSelectedToEngine(ENGINE::RAW, RESULT_TYPE::SUBDOMAIN);})->setIcon(QIcon(":/img/res/icons/domain.png"));
            menu.addAction(tr("Send Hostname To BRUTE"), this, [=](){this->sendSelectedToEngine(ENGINE::BRUTE, RESULT_TYPE::SUBDOMAIN);})->setIcon(QIcon(":/img/res/icons/domain.png"));
            menu.addAction(tr("Send Hostname To ACTIVE"), this, [=](){this->sendSelectedToEngine(ENGINE::ACTIVE, RESULT_TYPE::SUBDOMAIN);})->setIcon(QIcon(":/img/res/icons/domain.png"));
            menu.addAction(tr("Send Hostname To DNS"), this, [=](){this->sendSelectedToEngine(ENGINE::DNS, RESULT_TYPE::SUBDOMAIN);})->setIcon(QIcon(":/img/res/icons/domain.png"));
            menu.addAction(tr("Send Hostname To SSL"), this, [=](){this->sendSelectedToEngine(ENGINE::SSL, RESULT_TYPE::SUBDOMAIN);})->setIcon(QIcon(":/img/res/icons/domain.png"));
            menu.addSeparator();
        }
        if(selectionModel->columnIntersectsSelection(1, selectionModel->currentIndex().parent()))
            menu.addAction(tr("Send IpAddress To IP-Enum"), this, [=](){this->sendSelectedToEnum(TOOL::IP, RESULT_TYPE::IP);})->setIcon(QIcon(":/img/res/icons/ip.png"));
        break;
    case osint::OUTPUT::SUBDOMAIN:
        menu.addAction(tr("Send Hostname To OSINT"), this, [=](){this->sendSelectedToEngine(ENGINE::OSINT, RESULT_TYPE::SUBDOMAIN);})->setIcon(QIcon(":/img/res/icons/domain.png"));
        menu.addAction(tr("Send Hostname To RAW"), this, [=](){this->sendSelectedToEngine(ENGINE::RAW, RESULT_TYPE::SUBDOMAIN);})->setIcon(QIcon(":/img/res/icons/domain.png"));
        menu.addAction(tr("Send Hostname To BRUTE"), this, [=](){this->sendSelectedToEngine(ENGINE::BRUTE, RESULT_TYPE::SUBDOMAIN);})->setIcon(QIcon(":/img/res/icons/domain.png"));
        menu.addAction(tr("Send Hostname To ACTIVE"), this, [=](){this->sendSelectedToEngine(ENGINE::ACTIVE, RESULT_TYPE::SUBDOMAIN);})->setIcon(QIcon(":/img/res/icons/domain.png"));
        menu.addAction(tr("Send Hostname To DNS"), this, [=](){this->sendSelectedToEngine(ENGINE::DNS, RESULT_TYPE::SUBDOMAIN);})->setIcon(QIcon(":/img/res/icons/domain.png"));
        menu.addAction(tr("Send Hostname To SSL"), this, [=](){this->sendSelectedToEngine(ENGINE::SSL, RESULT_TYPE::SUBDOMAIN);})->setIcon(QIcon(":/img/res/icons/domain.png"));
        break;
    case osint::OUTPUT::IP:
        menu.addAction(tr("Send IpAddress To OSINT"), this, [=](){this->sendSelectedToEngine(ENGINE::OSINT, RESULT_TYPE::IP);})->setIcon(QIcon(":/img/res/icons/ip.png"));
        menu.addAction(tr("Send IpAddress To RAW"), this, [=](){this->sendSelectedToEngine(ENGINE::OSINT, RESULT_TYPE::IP);})->setIcon(QIcon(":/img/res/icons/ip.png"));
        menu.addSeparator();
        menu.addAction(tr("Send IpAddress To IP-Enum"), this, [=](){this->sendSelectedToEnum(TOOL::IP, RESULT_TYPE::IP);})->setIcon(QIcon(":/img/res/icons/ip.png"));
        break;
    case osint::OUTPUT::EMAIL:
        menu.addAction(tr("Send Email To OSINT"), this, [=](){this->sendSelectedToEngine(ENGINE::OSINT, RESULT_TYPE::EMAIL);})->setIcon(QIcon(":/img/res/icons/email.png"));
        menu.addAction(tr("Send Email To RAW"), this, [=](){this->sendSelectedToEngine(ENGINE::OSINT, RESULT_TYPE::EMAIL);})->setIcon(QIcon(":/img/res/icons/email.png"));
        menu.addSeparator();
        menu.addAction(tr("Send Email To EMAIL-Enum"), this, [=](){this->sendSelectedToEnum(TOOL::EMAIL, RESULT_TYPE::EMAIL);})->setIcon(QIcon(":/img/res/icons/email.png"));
        break;
    case osint::OUTPUT::URL:
        menu.addAction(tr("Send URL To URL"), this, [=](){this->sendSelectedToEngine(ENGINE::URL, RESULT_TYPE::URL);})->setIcon(QIcon(":/img/res/icons/url.png"));
        menu.addAction(tr("Send URL To RAW"), this, [=](){this->sendSelectedToEngine(ENGINE::RAW, RESULT_TYPE::URL);})->setIcon(QIcon(":/img/res/icons/url.png"));
        break;
    case osint::OUTPUT::ASN:
        menu.addAction(tr("Send ASN To OSINT"), this, [=](){this->sendSelectedToEngine(ENGINE::OSINT, RESULT_TYPE::ASN);})->setIcon(QIcon(":/img/res/icons/asn.png"));
        menu.addAction(tr("Send ASN To RAW"), this, [=](){this->sendSelectedToEngine(ENGINE::OSINT, RESULT_TYPE::ASN);})->setIcon(QIcon(":/img/res/icons/asn.png"));
        menu.addSeparator();
        menu.addAction(tr("Send ASN To ASN-Enum"), this, [=](){this->sendSelectedToEnum(TOOL::ASN, RESULT_TYPE::ASN);})->setIcon(QIcon(":/img/res/icons/asn.png"));
        break;
    case osint::OUTPUT::CERT:
        menu.addAction(tr("Send SSL To OSINT"), this, [=](){this->sendSelectedToEngine(ENGINE::OSINT, RESULT_TYPE::CERT_ID);})->setIcon(QIcon(":/img/res/icons/ssl.png"));
        menu.addAction(tr("Send SSL To RAW"), this, [=](){this->sendSelectedToEngine(ENGINE::OSINT, RESULT_TYPE::CERT_ID);})->setIcon(QIcon(":/img/res/icons/ssl.png"));
        menu.addSeparator();
        menu.addAction(tr("Send SSL To SSL-Enum"), this, [=](){this->sendSelectedToEnum(TOOL::SSL, RESULT_TYPE::CERT_ID);})->setIcon(QIcon(":/img/res/icons/ssl.png"));
        break;
    case osint::OUTPUT::CIDR:
        menu.addAction(tr("Send CIDR To OSINT"), this, [=](){this->sendSelectedToEngine(ENGINE::OSINT, RESULT_TYPE::CIDR);})->setIcon(QIcon(":/img/res/icons/cidr.png"));
        menu.addAction(tr("Send CIDR To RAW"), this, [=](){this->sendSelectedToEngine(ENGINE::OSINT, RESULT_TYPE::CIDR);})->setIcon(QIcon(":/img/res/icons/cidr.png"));
        menu.addSeparator();
        menu.addAction(tr("Send CIDR To CIDR-Enum"), this, [=](){this->sendSelectedToEnum(TOOL::CIDR, RESULT_TYPE::CIDR);})->setIcon(QIcon(":/img/res/icons/cidr.png"));
    }

    /* showing the context menu... */
    menu.exec(QCursor::pos());
}
