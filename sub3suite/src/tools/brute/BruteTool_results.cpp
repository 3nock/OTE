/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief : slots for receiving resolved subdomains and TLDs from the brute-module.
*/

#include "BruteTool.h"
#include "ui_BruteTool.h"


void BruteTool::onScanLog(scan::Log log){
    ui->plainTextEditLogs->appendHtml("[Error]      : <font color=\"red\">"+log.message+"</font>");
    ui->plainTextEditLogs->appendHtml("[Target]     : <font color=\"red\">"+log.target+"</font>");
    ui->plainTextEditLogs->appendHtml("[Nameserver] : <font color=\"red\">"+log.nameserver+"</font>");
    /* add a new line... */
    ui->plainTextEditLogs->appendPlainText("");

    m_failedScans.insert(log.target, log.message);
    m_scanStats->failed++;
}

void BruteTool::onWildcard(s3s_struct::Wildcard wildcard){
    /* send the found wildcard to project */
    project->addActiveWildcard(wildcard);

    /* log the found wildcard */
    ui->plainTextEditLogs->appendHtml("[Wildcard Found]: <font color=\"white\">"+wildcard.wildcard+"</font>");
}

void BruteTool::onResultSubdomain(s3s_struct::HOST host){
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
        m_model_subdomain->appendRow({item, item->ipv4, item->ipv6});
        set_subdomain.insert(host.host, item);

        ui->labelResultsCount->setNum(proxyModel->rowCount());
        m_scanStats->resolved++;
    }

    /* save to Project model */
    if(m_scanConfig->autoSaveToProject)
        project->addActiveHost(host);
}

void BruteTool::onResultTLD(s3s_struct::HOST host){
    if(set_tld.contains(host.host)) // for existing entry...
    {
        s3s_item::HOST *item = set_tld.value(host.host);
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
        m_model_tld->appendRow({item, item->ipv4, item->ipv6});
        set_tld.insert(host.host, item);

        ui->labelResultsCount->setNum(proxyModel->rowCount());
        m_scanStats->resolved++;
    }

    /* save to Project model */
    if(m_scanConfig->autoSaveToProject)
        project->addActiveHost(host);
}
