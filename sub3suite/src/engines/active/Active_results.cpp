/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#include "Active.h"
#include "ui_Active.h"


void Active::onScanLog(scan::Log log){
    ui->plainTextEditLogs->appendHtml("[Error]      : <font color=\"red\">"+log.message+"</font>");
    ui->plainTextEditLogs->appendHtml("[Target]     : <font color=\"red\">"+log.target+"</font>");
    ui->plainTextEditLogs->appendHtml("[Nameserver] : <font color=\"red\">"+log.nameserver+"</font>");
    /* add a new line... */
    ui->plainTextEditLogs->appendPlainText("");

    m_failedScans.insert(log.target, log.message);
    m_scanStats->failed++;
}

void Active::onScanResult_port(s3s_struct::HOST host){
    if(set_subdomain.contains(host.host)) // for existing entry...
    {
        s3s_item::HOST *item = set_subdomain.value(host.host);
        item->setValue_ports(host);
    }
    else // for new entry...
    {
        s3s_item::HOST *item = new s3s_item::HOST;
        item->setValue_ports(host);
        m_model->appendRow({item, item->ipv4, item->ipv6, item->ports});
        set_subdomain.insert(host.host, item);

        ui->labelResultsCount->setNum(proxyModel->rowCount());
        m_scanStats->resolved++;
    }

    /* save to Project model */
    if(m_scanConfig->autoSaveToProject)
        project->addActiveHost(host);
}

void Active::onScanResult_dns(s3s_struct::HOST host){
    if(set_subdomain.contains(host.host)) // for existing entry...
    {
        s3s_item::HOST *item = set_subdomain.value(host.host);
        switch (m_scanConfig->recordType) {
        case QDnsLookup::A:
            item->setValue_ipv4(host.ipv4);
            break;
        case QDnsLookup::AAAA:
            item->setValue_ipv6(host.ipv6);
            break;
        case QDnsLookup::ANY:
            item->setValue_ipv4(host.ipv4);
            item->setValue_ipv6(host.ipv6);
            break;
        default:
            break;
        }
    }
    else // for new entry...
    {
        s3s_item::HOST *item = new s3s_item::HOST;
        item->setValues(host);
        m_model->appendRow({item, item->ipv4, item->ipv6, item->ports});
        set_subdomain.insert(host.host, item);

        ui->labelResultsCount->setNum(proxyModel->rowCount());
        m_scanStats->resolved++;
    }

    /* save to Project model */
    if(m_scanConfig->autoSaveToProject)
        project->addActiveHost(host);
}
