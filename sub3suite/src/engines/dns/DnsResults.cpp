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

void Dns::onScanResult(dns::ScanResult results){
    /* if no-duplicates is allowed, prevent duplicate results using a set */
    if(m_scanConfig->noDuplicates){
        int prevSize = m_resultSet.size();
        m_resultSet.insert(results.target);
        if(prevSize == m_resultSet.size())
            return;
    }

    /* for other record types...*/
    QStandardItem *targetItem = new QStandardItem(results.target);
    targetItem->setIcon(QIcon(":/img/res/icons/folder2.png"));
    targetItem->setForeground(Qt::white);
    m_resultModel->invisibleRootItem()->appendRow(targetItem);
    ui->labelResultsCount->setNum(m_resultModel->invisibleRootItem()->rowCount());

    if(m_scanArgs->RecordType_a && !results.A.isEmpty()){
        QStandardItem *recordItem = new QStandardItem("A");
        recordItem->setIcon(QIcon(":/img/res/icons/folder2.png"));
        recordItem->setForeground(Qt::white);
        for(const QString &item: results.A)
        {
            recordItem->appendRow(new QStandardItem(item));

            /* save to Project model */
            if(m_scanConfig->autoSaveToProject)
                project->addActiveA(QStringList()<<item<<results.target);
        }
        targetItem->appendRow(recordItem);
    }

    if(m_scanArgs->RecordType_aaaa && !results.AAAA.isEmpty()){
        QStandardItem *recordItem = new QStandardItem("AAAA");
        recordItem->setIcon(QIcon(":/img/res/icons/folder2.png"));
        recordItem->setForeground(Qt::white);
        for(QString item: results.AAAA)
        {
            recordItem->appendRow(new QStandardItem(item));

            /* save to Project model */
            if(m_scanConfig->autoSaveToProject)
                project->addActiveAAAA(QStringList()<<item<<results.target);
        }
        targetItem->appendRow(recordItem);
    }

    if(m_scanArgs->RecordType_ns  && !results.NS.isEmpty()){
        QStandardItem *recordItem = new QStandardItem("NS");
        recordItem->setIcon(QIcon(":/img/res/icons/folder2.png"));
        recordItem->setForeground(Qt::white);
        for(QString item: results.NS)
        {
            recordItem->appendRow(new QStandardItem(item));

            /* save to Project model */
            if(m_scanConfig->autoSaveToProject)
                project->addActiveNS(QStringList()<<item<<results.target);
        }
        targetItem->appendRow(recordItem);
    }

    if(m_scanArgs->RecordType_mx && !results.MX.isEmpty()){
        QStandardItem *recordItem = new QStandardItem("MX");
        recordItem->setIcon(QIcon(":/img/res/icons/folder2.png"));
        recordItem->setForeground(Qt::white);
        for(QString item: results.MX)
        {
            recordItem->appendRow(new QStandardItem(item));

            /* save to Project model */
            if(m_scanConfig->autoSaveToProject)
                project->addActiveMX(QStringList()<<item<<results.target);
        }
        targetItem->appendRow(recordItem);
    }

    if(m_scanArgs->RecordType_txt && !results.TXT.isEmpty()){
        QStandardItem *recordItem = new QStandardItem("TXT");
        recordItem->setIcon(QIcon(":/img/res/icons/folder2.png"));
        recordItem->setForeground(Qt::white);
        for(QString item: results.TXT)
        {
            recordItem->appendRow(new QStandardItem(item));

            /* save to Project model */
            if(m_scanConfig->autoSaveToProject)
                project->addActiveTXT(QStringList()<<item<<results.target);
        }
        targetItem->appendRow(recordItem);
    }

    if(m_scanArgs->RecordType_cname  && !results.CNAME.isEmpty()){
        QStandardItem *recordItem = new QStandardItem("CNAME");
        recordItem->setIcon(QIcon(":/img/res/icons/folder2.png"));
        recordItem->setForeground(Qt::white);
        for(QString item: results.CNAME)
        {
            recordItem->appendRow(new QStandardItem(item));

            /* save to Project model */
            if(m_scanConfig->autoSaveToProject)
                project->addActiveCNAME(QStringList()<<item<<results.target);
        }
        targetItem->appendRow(recordItem);
    }

    m_scanStats->resolved++;
}
