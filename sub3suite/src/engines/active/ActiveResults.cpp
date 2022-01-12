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

void Active::onScanResult(QString host, QString ip){
    /* if no-duplicates is allowed, prevent duplicate results using a set */
    if(m_scanConfig->noDuplicates){
        int prevSize = m_activeDns.size();
        m_activeDns.insert(host+ip);
        if(prevSize == m_activeDns.size())
            return;
    }

    /* save to active dns model */
    m_resultModel->appendRow(QList<QStandardItem*>() << new QStandardItem(host) << new QStandardItem(ip));
    ui->labelResultsCount->setNum(m_resultProxyModel->rowCount());

    /* save to Project model */
    if(m_scanConfig->autoSaveToProject)
        project->addActiveSubdomain(QStringList()<<host<<ip);
}
