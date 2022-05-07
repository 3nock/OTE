/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#include "HostTool.h"
#include "ui_HostTool.h"


void HostTool::onScanLog(scan::Log log){
    ui->plainTextEditLogs->appendHtml("[Error]      : <font color=\"red\">"+log.message+"</font>");
    ui->plainTextEditLogs->appendHtml("[Target]     : <font color=\"red\">"+log.target+"</font>");
    ui->plainTextEditLogs->appendHtml("[Nameserver] : <font color=\"red\">"+log.nameserver+"</font>");
    /* add a new line... */
    ui->plainTextEditLogs->appendPlainText("");

    m_failedScans.insert(log.target, log.message);
    m_scanStats->failed++;
}

void HostTool::onScanResult_ping(QString host, QString ip, unsigned long time){
    m_model_ping->appendRow({new QStandardItem(host),
                             new QStandardItem(ip),
                             new QStandardItem(QString::number(time))});

    ui->labelResultsCount->setNum(proxyModel->rowCount());
    m_scanStats->resolved++;

    /* save to Project model */
    if(m_scanConfig->autoSaveToProject){
        s3s_struct::HOST s3s_host;
        s3s_host.host = host;
        s3s_host.ipv4 = ip;
        project->addActiveHost(s3s_host);
    }
}

void HostTool::onScanResult_port(QString hostname, QString ip, u_short port){
    if(set_ports.contains(hostname)){
        s3s_item::IPTool *item = set_ports.value(hostname);
        item->addPort(QString::number(port));
        return;
    }

    s3s_item::IPTool *item = new s3s_item::IPTool;
    item->setValues(ip, QString::number(port));

    m_model_port->appendRow({new QStandardItem(hostname),
                             item, item->ports});

    set_ports.insert(hostname, item);

    ui->labelResultsCount->setNum(proxyModel->rowCount());
    m_scanStats->resolved++;

    /* save to Project model */
    if(m_scanConfig->autoSaveToProject)
        project->addActiveIP(ip, port);
}

void HostTool::onScanResult_dns(s3s_struct::HOST host){
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
        m_model_dns->appendRow({item, item->ipv4, item->ipv6});
        set_subdomain.insert(host.host, item);

        ui->labelResultsCount->setNum(proxyModel->rowCount());
        m_scanStats->resolved++;
    }

    /* save to Project model */
    if(m_scanConfig->autoSaveToProject)
        project->addActiveHost(host);
}
