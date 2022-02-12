/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#include "Dns.h"
#include "ui_Dns.h"


void Dns::onScanLog(scan::Log log){
    ui->plainTextEditLogs->appendHtml("[Error]      : <font color=\"red\">"+log.message+"</font>");
    ui->plainTextEditLogs->appendHtml("[Target]     : <font color=\"red\">"+log.target+"</font>");
    ui->plainTextEditLogs->appendHtml("[Record]     : <font color=\"red\">"+log.recordType+"</font>");
    ui->plainTextEditLogs->appendHtml("[Nameserver] : <font color=\"red\">"+log.nameserver+"</font>");
    /* add a new line */
    ui->plainTextEditLogs->appendPlainText("");

    m_failedScans.insert(log.target, log.message);
    m_scanStats->failed++;
}

void Dns::onScanResult(s3s_struct::DNS results){
    if(m_resultSet.contains(results.dns)){
        s3s_item::DNS *item = m_resultSet.value(results.dns);
        item->setValues(results);
    }
    else{
        s3s_item::DNS *item = new s3s_item::DNS;
        item->setValues(results);
        m_resultModel->appendRow(item);
        m_resultSet.insert(results.dns, item);
    }

    m_scanStats->resolved++;
}
