/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#include "Ssl.h"
#include "ui_Ssl.h"

#include <QSslKey>
#include "src/items/SSLItem.h"


void Ssl::onScanLog(scan::Log log){
    ui->plainTextEditLogs->appendHtml("[Error]      : <font color=\"red\">"+log.message+"</font>");
    ui->plainTextEditLogs->appendHtml("[Target]     : <font color=\"red\">"+log.target+"</font>");
    /* add a new line... */
    ui->plainTextEditLogs->appendPlainText("");

    m_failedScans.insert(log.target, log.message);
    m_scanStats->failed++;
}

void Ssl::onScanResultSHA1(QString sha1){
    if(set_hash.contains(sha1))
        return;

    m_model_hash->appendRow(new QStandardItem(sha1));
    set_hash.insert(sha1);

    if(m_scanConfig->autoSaveToProject)
        project->addActiveSSL_sha1(sha1);

    ui->labelResultsCount->setNum(m_model_hash->rowCount());
    m_scanStats->resolved++;
}

void Ssl::onScanResultSHA256(QString sha256){
    if(set_hash.contains(sha256))
        return;

    m_model_hash->appendRow(new QStandardItem(sha256));
    set_hash.insert(sha256);

    if(m_scanConfig->autoSaveToProject)
        project->addActiveSSL_sha256(sha256);

    ui->labelResultsCount->setNum(m_model_hash->rowCount());
    m_scanStats->resolved++;
}

void Ssl::onScanResultRaw(QString target, QSslCertificate ssl){
    if(set_ssl.contains(target))
        return;

    s3s_item::SSL *item = new s3s_item::SSL;
    item->setValues(target, ssl);

    m_model_ssl->invisibleRootItem()->appendRow(item);
    set_ssl.insert(target, item);

    if(m_scanConfig->autoSaveToProject)
        project->addActiveSSL(target, ssl);

    ui->labelResultsCount->setNum(m_model_ssl->rowCount());
    m_scanStats->resolved++;
}

void Ssl::onScanResultSubdomain(QStringList subdomains){
    foreach(const QString &subdomain, subdomains)
    {
        if(set_subdomain.contains(subdomain))
            continue;

        m_model_subdomain->appendRow(new QStandardItem(subdomain));
        set_subdomain.insert(subdomain);

        if(m_scanConfig->autoSaveToProject)
            project->addActiveSSL_altNames(subdomain);
    }

    ui->labelResultsCount->setNum(m_model_subdomain->rowCount());
    m_scanStats->resolved++;
}
