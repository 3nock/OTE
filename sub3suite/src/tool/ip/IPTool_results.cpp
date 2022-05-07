/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#include "IPTool.h"
#include "ui_IPTool.h"


void IPTool::onScanLog(scan::Log log){
    ui->plainTextEditLogs->appendHtml("[Error]      : <font color=\"red\">"+log.message+"</font>");
    ui->plainTextEditLogs->appendHtml("[Target]     : <font color=\"red\">"+log.target+"</font>");
    /* add a new line... */
    ui->plainTextEditLogs->appendPlainText("");

    m_failedScans.insert(log.target, log.message);
    m_scanStats->failed++;
}

void IPTool::onScanResult_ping(QString ip, unsigned long time){
    m_model_ping->appendRow({new QStandardItem(ip),
                             new QStandardItem(QString::number(time))});

    ui->labelResultsCount->setNum(proxyModel->rowCount());
    m_scanStats->resolved++;
}

void IPTool::onScanResult_port(QString ip, u_short port){
    if(set_ports.contains(ip)){
        s3s_item::IPTool *item = set_ports.value(ip);
        item->addPort(QString::number(port));
        return;
    }

    s3s_item::IPTool *item = new s3s_item::IPTool;
    item->setValues(ip, QString::number(port));

    m_model_port->appendRow({item, item->ports});

    set_ports.insert(ip, item);

    ui->labelResultsCount->setNum(proxyModel->rowCount());
    m_scanStats->resolved++;

    /* save to Project model */
    if(m_scanConfig->autoSaveToProject)
        project->addActiveIP(ip, port);
}

void IPTool::onScanResult_dns(QString ip, QString hostname){
    m_model_dns->appendRow({new QStandardItem(ip),
                             new QStandardItem(hostname)});

    ui->labelResultsCount->setNum(proxyModel->rowCount());
    m_scanStats->resolved++;
}
