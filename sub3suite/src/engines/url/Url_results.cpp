/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#include "Url.h"
#include "ui_Url.h"


void Url::onScanLog(scan::Log log){
    ui->plainTextEditLogs->appendHtml("[Error]      : <font color=\"red\">"+log.message+"</font>");
    ui->plainTextEditLogs->appendHtml("[Target]     : <font color=\"red\">"+log.target+"</font>");
    /* add a new line... */
    ui->plainTextEditLogs->appendPlainText("");

    m_failedScans.insert(log.target, log.message);
    m_scanStats->failed++;
}

void Url::onScanResult(s3s_struct::URL url){
    if(set_results.contains(url.url))
    {
        s3s_item::URL *item = set_results.value(url.url);
        item->setValues(url);
        return;
    }

    s3s_item::URL *item = new s3s_item::URL;
    item->setValues(url);
    m_model->appendRow({item, item->status_code, item->title, item->banner, item->content_type});
    set_results.insert(url.url, item);

    if(m_scanConfig->autoSaveToProject)
        project->addActiveURL(url);

    ui->labelResultsCount->setNum(proxyModel->rowCount());
    m_scanStats->resolved++;
}
