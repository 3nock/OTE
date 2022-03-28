/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#include "Raw.h"
#include "ui_Raw.h"


void Raw::on_buttonActionJson_clicked(){
    /* check if there are results available else dont show the context menu */
    if(proxyModel->rowCount() < 1)
        return;

    /* getting the position of the action button to place the context menu and
       showing the context menu right by the side of the action button... */
    QPoint pos = ui->buttonActionJson->mapToGlobal(QPoint(0,0));
    pos = QPoint(pos.x()+60, pos.y());

    /* creating the context menu... */
    QMenu menu(this);

    /* adding actions */
    menu.addAction(tr("Clear"), this, [=](){this->clearResults_txt();})->setIcon(QIcon(":/img/res/icons/delete.png"));
    menu.addSeparator();
    menu.addAction(tr("Save"), this, [=](){this->saveResults_txt();})->setIcon(QIcon(":/img/res/icons/save.png"));
    menu.addAction(tr("Copy"), this, [=](){this->copyResults_txt();})->setIcon(QIcon(":/img/res/icons/copy.png"));

    /* showing the context menu... */
    menu.exec(pos);
}

void Raw::on_buttonActionTree_clicked(){
    /* check if there are results available else dont show the context menu */
    if(proxyModel->rowCount() < 1)
        return;

    /* getting the position of the action button to place the context menu and
       showing the context menu right by the side of the action button... */
    QPoint pos = ui->buttonActionTree->mapToGlobal(QPoint(0,0));
    pos = QPoint(pos.x()+60, pos.y());

    /* creating the context menu... */
    QMenu menu(this);

    /* adding actions */
    menu.addAction(tr("Clear"), this, [=](){this->clearResults();})->setIcon(QIcon(":/img/res/icons/delete.png"));
    menu.addAction(tr("Expand"), this, [=](){ui->treeViewResults->expandAll();})->setIcon(QIcon(":/img/res/icons/expand.png"));
    menu.addAction(tr("Collapse"), this, [=](){ui->treeViewResults->collapseAll();})->setIcon(QIcon(":/img/res/icons/collapse.png"));
    menu.addSeparator();
    menu.addAction(tr("Save"), this, [=](){this->saveResults();})->setIcon(QIcon(":/img/res/icons/save.png"));
    menu.addAction(tr("Copy"), this, [=](){this->copyResults();})->setIcon(QIcon(":/img/res/icons/copy.png"));
    menu.addSeparator();
    menu.addAction(tr("Send To Project"), this, [=](){this->sendToProject();})->setIcon(QIcon(":/img/res/icons/project.png"));

    /* showing the context menu */
    menu.exec(pos);
}

void Raw::on_treeViewResults_customContextMenuRequested(const QPoint &pos){
    Q_UNUSED(pos);

    /* check if user right clicked on items else dont show the context menu... */
    if(!ui->treeViewResults->selectionModel()->isSelected(ui->treeViewResults->currentIndex()))
        return;

    /* getting the selected items... */
    selectionModel = ui->treeViewResults->selectionModel();

    QMenu send_domain_menu(this);
    send_domain_menu.setTitle(tr("Hostname"));
    send_domain_menu.setIcon(QIcon(":/img/res/icons/domain.png"));
    send_domain_menu.addAction(tr("Send Hostname to OSINT"), this, [=](){this->sendToEngine(ENGINE::OSINT, RESULT_TYPE::SUBDOMAIN);});
    send_domain_menu.addAction(tr("Send Hostname to RAW"), this, [=](){this->sendToEngine(ENGINE::RAW, RESULT_TYPE::SUBDOMAIN);});
    send_domain_menu.addAction(tr("Send Hostname to BRUTE"), this, [=](){this->sendToEngine(ENGINE::BRUTE, RESULT_TYPE::SUBDOMAIN);});
    send_domain_menu.addAction(tr("Send Hostname to ACTIVE"), this, [=](){this->sendToEngine(ENGINE::ACTIVE, RESULT_TYPE::SUBDOMAIN);});
    send_domain_menu.addAction(tr("Send Hostname to DNS"), this, [=](){this->sendToEngine(ENGINE::DNS, RESULT_TYPE::SUBDOMAIN);});
    send_domain_menu.addAction(tr("Send Hostname to SSL"), this, [=](){this->sendToEngine(ENGINE::SSL, RESULT_TYPE::SUBDOMAIN);});

    QMenu send_ip_menu(this);
    send_ip_menu.setTitle(tr("IpAddress"));
    send_ip_menu.setIcon(QIcon(":/img/res/icons/ip.png"));
    send_ip_menu.addAction(tr("Send IpAddress to OSINT"), this, [=](){this->sendToEngine(ENGINE::OSINT, RESULT_TYPE::IP);});
    send_ip_menu.addAction(tr("Send IpAddress to RAW"), this, [=](){this->sendToEngine(ENGINE::RAW, RESULT_TYPE::IP);});
    send_ip_menu.addSeparator();
    send_ip_menu.addAction(tr("Send IpAddress to IP-Enum"), this, [=](){this->sendToEnum(ENUMERATOR::IP, RESULT_TYPE::IP);});

    QMenu send_url_menu(this);
    send_url_menu.setTitle(tr("URL"));
    send_url_menu.setIcon(QIcon(":/img/res/icons/url.png"));
    send_url_menu.addAction(tr("Send URL to OSINT"), this, [=](){this->sendToEngine(ENGINE::OSINT, RESULT_TYPE::URL);});
    send_url_menu.addAction(tr("Send URL to RAW"), this, [=](){this->sendToEngine(ENGINE::RAW, RESULT_TYPE::URL);});
    send_url_menu.addAction(tr("Send URL to URL"), this, [=](){this->sendToEngine(ENGINE::URL, RESULT_TYPE::URL);});

    QMenu send_email_menu(this);
    send_email_menu.setTitle(tr("Email"));
    send_email_menu.setIcon(QIcon(":/img/res/icons/email.png"));
    send_email_menu.addAction(tr("Send Email to OSINT"), this, [=](){this->sendToEngine(ENGINE::OSINT, RESULT_TYPE::EMAIL);});
    send_email_menu.addAction(tr("Send Email to RAW"), this, [=](){this->sendToEngine(ENGINE::RAW, RESULT_TYPE::EMAIL);});
    send_email_menu.addSeparator();
    send_email_menu.addAction(tr("Send Email to Email-Enum"), this, [=](){this->sendToEnum(ENUMERATOR::EMAIL, RESULT_TYPE::EMAIL);});

    QMenu send_asn_menu(this);
    send_asn_menu.setTitle(tr("ASN"));
    send_asn_menu.setIcon(QIcon(":/img/res/icons/asn.png"));
    send_asn_menu.addAction(tr("Send ASN to OSINT"), this, [=](){this->sendToEngine(ENGINE::OSINT, RESULT_TYPE::ASN);});
    send_asn_menu.addAction(tr("Send ASN to RAW"), this, [=](){this->sendToEngine(ENGINE::RAW, RESULT_TYPE::ASN);});
    send_asn_menu.addSeparator();
    send_asn_menu.addAction(tr("Send ASN to ASN-Enum"), this, [=](){this->sendToEnum(ENUMERATOR::ASN, RESULT_TYPE::ASN);});

    QMenu send_cidr_menu(this);
    send_cidr_menu.setTitle(tr("CIDR"));
    send_cidr_menu.setIcon(QIcon(":/img/res/icons/cidr.png"));
    send_cidr_menu.addAction(tr("Send CIDR to OSINT"), this, [=](){this->sendToEngine(ENGINE::OSINT, RESULT_TYPE::CIDR);});
    send_cidr_menu.addAction(tr("Send CIDR to RAW"), this, [=](){this->sendToEngine(ENGINE::RAW, RESULT_TYPE::CIDR);});
    send_cidr_menu.addSeparator();
    send_cidr_menu.addAction(tr("Send CIDR to CIDR-Enum"), this, [=](){this->sendToEnum(ENUMERATOR::CIDR, RESULT_TYPE::CIDR);});

    QMenu send_ssl_menu(this);
    send_ssl_menu.setTitle(tr("SSL"));
    send_ssl_menu.setIcon(QIcon(":/img/res/icons/ssl.png"));
    send_ssl_menu.addAction(tr("Send SSL to OSINT"), this, [=](){this->sendToEngine(ENGINE::OSINT, RESULT_TYPE::CERT_ID);});
    send_ssl_menu.addAction(tr("Send SSL to RAW"), this, [=](){this->sendToEngine(ENGINE::RAW, RESULT_TYPE::CERT_ID);});
    send_ssl_menu.addSeparator();
    send_ssl_menu.addAction(tr("Send SSL to SSL-Enum"), this, [=](){this->sendToEnum(ENUMERATOR::SSL, RESULT_TYPE::CERT_ID);});

    QMenu send_ns_menu(this);
    send_ns_menu.setTitle(tr("NameServer"));
    send_ns_menu.setIcon(QIcon(":/img/res/icons/ns.png"));
    send_ns_menu.addAction(tr("Send NS to NS-Enum"), this, [=](){this->sendToEnum(ENUMERATOR::NS, RESULT_TYPE::NS);});

    QMenu send_mx_menu(this);
    send_mx_menu.setTitle(tr("MailServer"));
    send_mx_menu.setIcon(QIcon(":/img/res/icons/mx.png"));
    send_mx_menu.addAction(tr("Send MX to MX-Enum"), this, [=](){this->sendToEnum(ENUMERATOR::MX, RESULT_TYPE::MX);});

    QMenu send_menu(this);
    send_menu.setTitle(tr("Send Item"));
    send_menu.setIcon(QIcon(":/img/res/icons/send.png"));
    send_menu.addMenu(&send_domain_menu);
    send_menu.addMenu(&send_ip_menu);
    send_menu.addMenu(&send_url_menu);
    send_menu.addMenu(&send_email_menu);
    send_menu.addMenu(&send_asn_menu);
    send_menu.addMenu(&send_cidr_menu);
    send_menu.addMenu(&send_ssl_menu);
    send_menu.addMenu(&send_ns_menu);
    send_menu.addMenu(&send_mx_menu);

    QMenu menu(this);
    menu.addAction(tr("Remove"), this, [=](){this->removeResults();})->setIcon(QIcon(":/img/res/icons/delete.png"));
    menu.addAction(tr("Open In Browser"), this, [=](){this->openInBrowser();})->setIcon(QIcon(":/img/res/icons/browser.png"));
    menu.addSeparator();
    menu.addAction(tr("Save"), this, [=](){this->saveSelectedResults();})->setIcon(QIcon(":/img/res/icons/save.png"));
    menu.addAction(tr("Copy"), this, [=](){this->copySelectedResults();})->setIcon(QIcon(":/img/res/icons/copy.png"));
    menu.addSeparator();
    menu.addAction(tr("Send To Project"), this, [=](){this->sendSelectedToProject();})->setIcon(QIcon(":/img/res/icons/project.png"));
    menu.addSeparator();
    menu.addMenu(&send_menu);

    /* showing the context menu */
    menu.exec(QCursor::pos());
}
