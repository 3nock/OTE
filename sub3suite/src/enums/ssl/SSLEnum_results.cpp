#include "SSLEnum.h"
#include "ui_SSLEnum.h"


void SSLEnum::onResult(QString target, QSslCertificate ssl){
    if(m_resultsSet.contains(target))
        return;

    s3s_item::SSL *item = new s3s_item::SSL;
    item->setValues(target, ssl);

    m_model->invisibleRootItem()->appendRow(item);
    m_resultsSet.insert(target, item);

    if(m_scanConfig->autosaveToProject)
        project->addEnumSSL(target, ssl);

    ui->labelResultsCount->setNum(proxyModel->rowCount());
}

void SSLEnum::onScanLog(ScanLog log){
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
