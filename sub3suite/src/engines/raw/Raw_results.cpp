/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#include "Raw.h"
#include "ui_Raw.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>


void Raw::onScanLog(ScanLog log){
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

void Raw::onResults(s3s_struct::RAW raw){
    if(raw.results.isEmpty())
        return;

    s3s_item::RAW *item = new s3s_item::RAW;
    item->setValues(raw);
    m_model->appendRow(item);
    ui->labelResultsCountTree->setNum(proxyModel->rowCount());

    QJsonDocument document = QJsonDocument::fromJson(raw.results);
    ui->plainTextEditResults->appendPlainText(document.toJson());
    m_resultsCountJson++;
    ui->labelResultsCount->setNum(m_resultsCountJson);

    if(m_scanConfig->autosaveToProject)
        project->addRaw(raw);
}

void Raw::onResultsTxt(s3s_struct::RAW raw){
    if(raw.results.isEmpty())
        return;

    ui->plainTextEditResults->appendPlainText(raw.results);
    m_resultsCountJson++;
    ui->labelResultsCount->setNum(m_resultsCountJson);
}
