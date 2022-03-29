#include "SSLEnum.h"
#include "ui_SSLEnum.h"


void SSLEnum::onResult(QByteArray rawCert){
    foreach(const QSslCertificate &cert, QSslCertificate::fromData(rawCert, QSsl::Pem))
    {
        QString hash(cert.digest(QCryptographicHash::Sha1));
        if(m_resultsSet.contains(hash))
            continue;

        s3s_item::SSL *item = new s3s_item::SSL;
        item->setValues(hash, cert);
        m_model->appendRow(item);
        m_resultsSet.insert(hash, item);

        if(m_scanConfig->autosaveToProject)
            project->addEnumSSL(hash, cert);
    }

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
