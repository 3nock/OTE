#include "SSLEnum.h"
#include "ui_SSLEnum.h"


void SSLEnum::onErrorLog(ScanLog log){
    QString message("<font color=\"red\">"+log.message+"</font>");
    QString module("<font color=\"red\">"+log.moduleName+"</font>");
    QString status("<font color=\"red\">"+QString::number(log.statusCode)+"</font>");
    ui->plainTextEditLogs->appendHtml("[Module]        :"+module);
    ui->plainTextEditLogs->appendHtml("[Status Code]   :"+status);
    ui->plainTextEditLogs->appendHtml("[Error message] :"+message);
    ui->plainTextEditLogs->appendPlainText("");
}

void SSLEnum::onInfoLog(ScanLog log){
    QString module("<font color=\"green\">"+log.moduleName+"</font>");
    QString status("<font color=\"green\">"+QString::number(log.statusCode)+"</font>");
    ui->plainTextEditLogs->appendHtml("[Module]        :"+module);
    ui->plainTextEditLogs->appendHtml("[Status Code]   :"+status);
    ui->plainTextEditLogs->appendPlainText("");
}

void SSLEnum::onRateLimitLog(ScanLog log){
    QString message("<font color=\"yellow\">"+log.message+"</font>");
    QString module("<font color=\"yellow\">"+log.moduleName+"</font>");
    QString status("<font color=\"yellow\">"+QString::number(log.statusCode)+"</font>");
    ui->plainTextEditLogs->appendHtml("[Module]        :"+module);
    ui->plainTextEditLogs->appendHtml("[Status Code]   :"+status);
    ui->plainTextEditLogs->appendHtml("[Error message] :"+message);
    ui->plainTextEditLogs->appendPlainText("");
}

void SSLEnum::onResult(QByteArray rawCert){
    foreach(const QSslCertificate &cert, QSslCertificate::fromData(rawCert, QSsl::Pem))
    {
        s3s_item::SSL *item = new s3s_item::SSL;
        item->setValues(cert.digest(QCryptographicHash::Sha1), cert);
        m_model->appendRow(item);

        if(m_scanConfig->autosaveToProject)
            project->addEnumSSL(cert.digest(QCryptographicHash::Sha1), cert);
    }

    ui->labelResultsCount->setNum(proxyModel->rowCount());
}

/*

30 81 9F             ;30=SEQUENCE (0x9F = 159 bytes)
|  30 0D             ;30=SEQUENCE (0x0D = 13 bytes)
|  |  06 09          ;06=OBJECT_IDENTIFIER (0x09 = 9 bytes)
|  |  2A 86 48 86    ;Hex encoding of 1.2.840.113549.1.1
|  |  F7 0D 01 01 01
|  |  05 00          ;05=NULL (0 bytes)
|  03 81 8D 00       ;03=BIT STRING (0x8d = 141 bytes)
|  |  30 81 89       ;30=SEQUENCE (0x89 = 137 bytes)
|  |  |  02 81 81    ;02=INTEGER (0x81 = 129 bytes) the modulus
|  |  |  00          ;leading zero of INTEGER
|  |  |  AA 18 AB A4 3B 50 DE EF  38 59 8F AF 87 D2 AB 63
|  |  |  4E 45 71 C1 30 A9 BC A7  B8 78 26 74 14 FA AB 8B
|  |  |  47 1B D8 96 5F 5C 9F C3  81 84 85 EA F5 29 C2 62
|  |  |  46 F3 05 50 64 A8 DE 19  C8 C3 38 BE 54 96 CB AE
|  |  |  B0 59 DC 0B 35 81 43 B4  4A 35 44 9E B2 64 11 31
|  |  |  21 A4 55 BD 7F DE 3F AC  91 9E 94 B5 6F B9 BB 4F
|  |  |  65 1C DB 23 EA D4 39 D6  CD 52 3E B0 81 91 E7 5B
|  |  |  35 FD 13 A7 41 9B 30 90  F2 47 87 BD 4F 4E 19 67
|  |  02 03          ;02=INTEGER (0x03 = 3 bytes) - the exponent
|  |  |  01 00 01    ;hex for 65537

*/
