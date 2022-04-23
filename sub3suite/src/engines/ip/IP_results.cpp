/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#include "IP.h"
#include "ui_IP.h"


void IP::onScanLog(scan::Log log){
    ui->plainTextEditLogs->appendHtml("[Error]      : <font color=\"red\">"+log.message+"</font>");
    ui->plainTextEditLogs->appendHtml("[Target]     : <font color=\"red\">"+log.target+"</font>");
    ui->plainTextEditLogs->appendHtml("[Nameserver] : <font color=\"red\">"+log.nameserver+"</font>");
    /* add a new line... */
    ui->plainTextEditLogs->appendPlainText("");

    m_failedScans.insert(log.target, log.message);
    m_scanStats->failed++;
}

void IP::onScanResult_ping(QString ip, int bytes, int time, int ttl){
    m_model_ping->appendRow({new QStandardItem(ip),
                             new QStandardItem(QString::number(bytes)),
                             new QStandardItem(QString::number(time)),
                             new QStandardItem(QString::number(ttl))});

    ui->labelResultsCount->setNum(proxyModel->rowCount());
    m_scanStats->resolved++;
}

void IP::onScanResult_port(QString ip, QList<u_short> ports){
    QString port_list = nullptr;
    foreach(const u_short &port, ports)
        port_list.append(port).append(",");
    port_list.chop(1);

    m_model_port->appendRow({new QStandardItem(ip),
                             new QStandardItem(port_list)});

    ui->labelResultsCount->setNum(proxyModel->rowCount());
    m_scanStats->resolved++;
}

void IP::onScanResult_dns(QString ip, QString hostname){
    m_model_dns->appendRow({new QStandardItem(ip),
                             new QStandardItem(hostname)});

    ui->labelResultsCount->setNum(proxyModel->rowCount());
    m_scanStats->resolved++;
}
