/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#include "DNSTool.h"
#include "ui_DNSTool.h"


void DNSTool::onScanLog(scan::Log log){
    ui->plainTextEditLogs->appendHtml("[Error]      : <font color=\"red\">"+log.message+"</font>");
    ui->plainTextEditLogs->appendHtml("[Target]     : <font color=\"red\">"+log.target+"</font>");
    ui->plainTextEditLogs->appendHtml("[Record]     : <font color=\"red\">"+log.recordType+"</font>");
    ui->plainTextEditLogs->appendHtml("[Nameserver] : <font color=\"red\">"+log.nameserver+"</font>");
    /* add a new line */
    ui->plainTextEditLogs->appendPlainText("");

    m_failedScans.insert(log.target, log.message);
    m_scanStats->failed++;
}

void DNSTool::onScanResult_srv(s3s_struct::DNS dns){
    if(m_resultSet.contains(dns.dns)){
        s3s_item::DNS *item = m_resultSet.value(dns.dns);
        item->addSRV(dns);
        return;
    }

    s3s_item::DNS *item = new s3s_item::DNS;
    item->setValues(dns);
    m_model->appendRow(item);
    m_resultSet.insert(dns.dns, item);

    if(m_scanConfig->autoSaveToProject)
        project->addActiveDNS(dns);

    ui->labelResultsCount->setNum(proxyModel->rowCount());
    m_scanStats->resolved++;
}

void DNSTool::onScanResult(s3s_struct::DNS dns){
    if(m_resultSet.contains(dns.dns)){
        s3s_item::DNS *item = m_resultSet.value(dns.dns);
        item->setValues(dns);
        return;
    }

    s3s_item::DNS *item = new s3s_item::DNS;
    item->setValues(dns);
    m_model->appendRow(item);
    m_resultSet.insert(dns.dns, item);

    if(m_scanConfig->autoSaveToProject)
        project->addActiveDNS(dns);

    ui->labelResultsCount->setNum(proxyModel->rowCount());
    m_scanStats->resolved++;
}
