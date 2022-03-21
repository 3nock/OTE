/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#include "MXEnum.h"
#include "ui_MXEnum.h"


void MXEnum::onResult(s3s_struct::MX results){
    if(m_resultsSet.contains(results.info_mx))
    {
        s3s_item::MX *item = m_resultsSet.value(results.info_mx);
        item->setValues(results);
    }else {
        s3s_item::MX *item = new s3s_item::MX;
        item->setValues(results);
        m_model->appendRow(item);
        m_resultsSet.insert(results.info_mx, item);
        ui->labelResultsCount->setNum(proxyModel->rowCount());
    }

    if(m_scanConfig->autosaveToProject)
        project->addEnumMX(results);
}

void MXEnum::onErrorLog(ScanLog log){
    QString message("<font color=\"red\">"+log.message+"</font>");
    QString module("<font color=\"red\">"+log.moduleName+"</font>");
    QString status("<font color=\"red\">"+QString::number(log.statusCode)+"</font>");
    ui->plainTextEditLogs->appendHtml("[Module]        :"+module);
    ui->plainTextEditLogs->appendHtml("[Status Code]   :"+status);
    ui->plainTextEditLogs->appendHtml("[Error message] :"+message);
    ui->plainTextEditLogs->appendPlainText("");

    m_failedScans.insert(log.target, log.message);
}

void MXEnum::onInfoLog(ScanLog log){
    QString module("<font color=\"green\">"+log.moduleName+"</font>");
    QString status("<font color=\"green\">"+QString::number(log.statusCode)+"</font>");
    ui->plainTextEditLogs->appendHtml("[Module]        :"+module);
    ui->plainTextEditLogs->appendHtml("[Status Code]   :"+status);
    ui->plainTextEditLogs->appendPlainText("");
}
