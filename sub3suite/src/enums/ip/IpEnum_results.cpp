/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#include "IpEnum.h"
#include "ui_IpEnum.h"


void IpEnum::onResult(s3s_struct::IP results){
    if(m_resultsSet.contains(results.ip))
    {
        s3s_item::IP *item = m_resultsSet.value(results.ip);
        item->setValues(results);
    }else {
        s3s_item::IP *item = new s3s_item::IP;
        item->setValues(results);
        m_model->appendRow(item);
        m_resultsSet.insert(results.ip, item);
        ui->labelResultsCount->setNum(proxyModel->rowCount());
    }

    if(m_scanConfig->autosaveToProject)
        project->addEnumIP(results);
}

void IpEnum::onResult_reverse(s3s_struct::IP ip){

}

void IpEnum::onScanLog(ScanLog log){
    if(log.error){
        QString message("<font color=\"red\">"+log.message+"</font>");
        QString module("<font color=\"red\">"+log.moduleName+"</font>");
        QString target("<font color=\"red\">"+log.target+"</font>");
        QString status("<font color=\"red\">"+QString::number(log.statusCode)+"</font>");
        ui->plainTextEditLogs->appendHtml("[Module]        :"+module);
        ui->plainTextEditLogs->appendHtml("[Target]        :"+target);
        ui->plainTextEditLogs->appendHtml("[Status Code]   :"+status);
        ui->plainTextEditLogs->appendHtml("[Error message] :"+message);
        m_failedScans.insert(log.target, log.message);
    }
    else{
        QString module("<font color=\"green\">"+log.moduleName+"</font>");
        QString target("<font color=\"green\">"+log.target+"</font>");
        QString status("<font color=\"green\">"+QString::number(log.statusCode)+"</font>");
        ui->plainTextEditLogs->appendHtml("[Module]        :"+module);
        ui->plainTextEditLogs->appendHtml("[Target]        :"+target);
        ui->plainTextEditLogs->appendHtml("[Status Code]   :"+status);
    }

    ui->plainTextEditLogs->appendPlainText("");
}
