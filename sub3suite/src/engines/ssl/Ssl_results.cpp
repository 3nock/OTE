/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#include "Ssl.h"
#include "ui_Ssl.h"

#include <QSslKey>
#include "src/models/SSLModel.h"


void Ssl::onScanLog(scan::Log log){
    ui->plainTextEditLogs->appendHtml("[Error]      : <font color=\"red\">"+log.message+"</font>");
    ui->plainTextEditLogs->appendHtml("[Target]     : <font color=\"red\">"+log.target+"</font>");
    /* add a new line... */
    ui->plainTextEditLogs->appendPlainText("");

    m_failedScans.insert(log.target, log.message);
    m_scanStats->failed++;
}

void Ssl::onScanResultSHA1(QString sha1){
    m_resultModelCertId->appendRow(new QStandardItem(sha1));
    ui->labelResultsCount->setNum(m_resultModelCertId->rowCount());
    m_scanStats->resolved++;

    if(m_scanConfig->autoSaveToProject)
        project->addActiveSSL_sha1(sha1);
}

void Ssl::onScanResultSHA256(QString sha256){
    m_resultModelCertId->appendRow(new QStandardItem(sha256));
    ui->labelResultsCount->setNum(m_resultModelCertId->rowCount());
    m_scanStats->resolved++;

    if(m_scanConfig->autoSaveToProject)
        project->addActiveSSL_sha256(sha256);
}

void Ssl::onScanResultRaw(QString target, QSslCertificate ssl){
    s3s_item::SSL *item = new s3s_item::SSL;
    item->setValues(target, ssl);

    m_resultModelCertInfo->invisibleRootItem()->appendRow(item);

    ui->labelResultsCount->setNum(m_resultModelCertInfo->rowCount());
    m_scanStats->resolved++;

    if(m_scanConfig->autoSaveToProject)
        project->addActiveSSL(target, ssl);
}

void Ssl::onScanResultSubdomain(QString target, QStringList subdomains){
    /*
    QStandardItem *targetItem = new QStandardItem(target);
    foreach(const QString &subdomain, subdomains)
        targetItem->appendRow(new QStandardItem(subdomain));

    m_resultModelSubdomain->appendRow(targetItem);
    ui->labelResultsCount->setNum(m_resultModelSubdomain->rowCount());
    m_scanStats->resolved++;
    */
    Q_UNUSED(target);

    foreach(const QString &subdomain, subdomains){
        m_resultModelSubdomain->appendRow(new QStandardItem(subdomain));
        project->addActiveSSL_altNames(subdomain);
    }

    ui->labelResultsCount->setNum(m_resultModelSubdomain->rowCount());
    m_scanStats->resolved++;
}
