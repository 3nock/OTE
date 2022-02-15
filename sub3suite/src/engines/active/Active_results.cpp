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
    if(m_resultSet.contains(host.host)) // for existing entry...
    {
        s3s_item::HOST *item = m_resultSet.value(host.host);
        item->setValue_ports(host);
    }
    else // for new entry...
    {
        s3s_item::HOST *item = new s3s_item::HOST;
        item->setValue_ports(host);
        m_resultModel->appendRow({item, item->ipv4, item->ipv6, item->ports});
        m_resultSet.insert(host.host, item);

        ui->labelResultsCount->setNum(m_resultProxyModel->rowCount());
        m_scanStats->resolved++;
    }

    /* save to Project model */
    if(m_scanConfig->autoSaveToProject)
        project->addActiveHost(host);
}

void Active::onScanResult_dns(s3s_struct::HOST host){
    if(m_resultSet.contains(host.host)) // for existing entry...
    {
        s3s_item::HOST *item = m_resultSet.value(host.host);
        if(m_scanConfig->recordType == QDnsLookup::A)
            item->setValue_ipv4(host.ipv4);
        else
            item->setValue_ipv6(host.ipv6);
    }
    else // for new entry...
    {
        s3s_item::HOST *item = new s3s_item::HOST;
        item->setValues(host);
        m_resultModel->appendRow({item, item->ipv4, item->ipv6, item->ports});
        m_resultSet.insert(host.host, item);

        ui->labelResultsCount->setNum(m_resultProxyModel->rowCount());
        m_scanStats->resolved++;
    }

    /* save to Project model */
    if(m_scanConfig->autoSaveToProject)
        project->addActiveHost(host);
}
