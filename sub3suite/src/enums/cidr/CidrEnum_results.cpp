#include "CIDREnum.h"
#include "ui_CIDREnum.h"


void CIDREnum::onResult(s3s_struct::CIDR results){
    if(m_resultsSet.contains(results.cidr))
        return;

    s3s_item::CIDR *item = new s3s_item::CIDR;
    item->setValues(results);
    m_model->appendRow(item);
    m_resultsSet.insert(results.cidr, item);

    if(m_scanConfig->autosaveToProject)
        project->addEnumCIDR(results);
}

void CIDREnum::onScanLog(ScanLog log){
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
