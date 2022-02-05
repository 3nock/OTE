#include "ASNEnum.h"
#include "ui_ASNEnum.h"


void ASNEnum::onResultsAsn(s3s_struct::ASN results){
    if(m_resultsSet.contains(results.asn))
        return;

    s3s_item::ASN *item = new s3s_item::ASN;
    item->setValues(results);
    m_model->appendRow(item);
    m_resultsSet.insert(results.asn, item);

    if(m_scanConfig->autosaveToProject)
        project->addEnumASN(results);
}

void ASNEnum::onResultsAsnPeers(s3s_struct::ASN results){
    if(m_resultsSet.contains(results.asn)){
        s3s_item::ASN *item = m_resultsSet.value(results.asn);
        item->setPeers(results.peers);
    }
    else{
        s3s_item::ASN *item = new s3s_item::ASN;
        item->setValues(results);
        m_model->appendRow(item);
        m_resultsSet.insert(results.asn, item);
    }

    if(m_scanConfig->autosaveToProject)
        project->addEnumASN(results);
}

void ASNEnum::onResultsAsnPrefixes(s3s_struct::ASN results){
    if(m_resultsSet.contains(results.asn)){
        s3s_item::ASN *item = m_resultsSet.value(results.asn);
        item->setPrefixes(results.prefixes);
    }
    else{
        s3s_item::ASN *item = new s3s_item::ASN;
        item->setValues(results);
        m_model->appendRow(item);
        m_resultsSet.insert(results.asn, item);
    }

    if(m_scanConfig->autosaveToProject)
        project->addEnumASN(results);
}

void ASNEnum::onErrorLog(ScanLog log){
    QString message("<font color=\"red\">"+log.message+"</font>");
    QString module("<font color=\"red\">"+log.moduleName+"</font>");
    QString status("<font color=\"red\">"+QString::number(log.statusCode)+"</font>");
    ui->plainTextEditLogs->appendHtml("[Module]        :"+module);
    ui->plainTextEditLogs->appendHtml("[Status Code]   :"+status);
    ui->plainTextEditLogs->appendHtml("[Error message] :"+message);
    ui->plainTextEditLogs->appendPlainText("");
}

void ASNEnum::onInfoLog(ScanLog log){
    QString module("<font color=\"green\">"+log.moduleName+"</font>");
    QString status("<font color=\"green\">"+QString::number(log.statusCode)+"</font>");
    ui->plainTextEditLogs->appendHtml("[Module]        :"+module);
    ui->plainTextEditLogs->appendHtml("[Status Code]   :"+status);
    ui->plainTextEditLogs->appendPlainText("");
}

void ASNEnum::onRateLimitLog(ScanLog log){
    QString message("<font color=\"yellow\">"+log.message+"</font>");
    QString module("<font color=\"yellow\">"+log.moduleName+"</font>");
    QString status("<font color=\"yellow\">"+QString::number(log.statusCode)+"</font>");
    ui->plainTextEditLogs->appendHtml("[Module]        :"+module);
    ui->plainTextEditLogs->appendHtml("[Status Code]   :"+status);
    ui->plainTextEditLogs->appendHtml("[Error message] :"+message);
    ui->plainTextEditLogs->appendPlainText("");
}
