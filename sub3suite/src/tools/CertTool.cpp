#include "CertTool.h"
#include "ui_CertTool.h"
//...
#include <QSslCertificate>
#include <QStandardItem>
#include <QSslKey>
#include "src/utils/Definitions.h"
#include "src/modules/scan/CertScanner.h"
#include "src/dialogs/PassiveConfigDialog.h"

#define TARGET_SSLCERT 0
#define TARGET_HOSTNAME 1

enum PROTOCAL{
    HTTPS = 0,
    SSH = 1,
    FTP = 2
};

enum MODULE{
    CRTSH = 0,
    CERTSPOTTER = 1
};

/* group the certificates into; expired, valid, all
 *
 * just active and crtsh for now...
 *
 * articles on ssl-cert osint
 * https://osintcurio.us/2019/03/12/certificates-the-osint-gift-that-keeps-on-giving/
 */
CertTool::CertTool(QWidget *parent) : QDialog(parent), ui(new Ui::CertTool),
    m_model(new CertModel),
    m_proxyModel(new QSortFilterProxyModel)
{
    ui->setupUi(this);

    /* setting the placeholdertxt */
    ui->lineEditFilter->setPlaceholderText("Filter...");
    ui->lineEditTarget->setPlaceholderText(PLACEHOLDERTEXT_SSLCERT);

    /* setting the models */
    m_model->initModel();
    m_proxyModel->setSourceModel(m_model->mainModel);
    ui->treeResults->setModel(m_proxyModel);
}
CertTool::~CertTool(){
    delete ui;
    delete m_model;
    delete m_proxyModel;
}

void CertTool::on_buttonStart_clicked(){
    ui->buttonStop->setEnabled(true);
    ui->buttonStart->setDisabled(true);

    /* creating and runing the enumeration thread */
    QThread *cThread = new QThread;
    switch (ui->comboBoxTargetType->currentIndex()) {
    case TARGET_SSLCERT:
    {
        ScanArgs scanArgs;
        ///
        /// acquire scan arguments...
        ///
        scanArgs.outputInfo = true;
        if(ui->checkBoxMultipleTargets->isChecked()){
            // for multpile targets...
        }
        else{
            scanArgs.targets.push(ui->lineEditTarget->text());
        }

        ///
        /// enumeration module...
        ///
        switch (ui->comboBoxOption->currentIndex()) {
        case MODULE::CRTSH:
        {
            Crtsh *crtsh = new Crtsh(scanArgs);
            crtsh->startScan(cThread);
            crtsh->moveToThread(cThread);

            connect(crtsh, &Crtsh::rawCert, this, &CertTool::onResult);
            /* ... */
            connect(crtsh, &Crtsh::infoLog, this, &CertTool::onInfoLog);
            connect(crtsh, &Crtsh::errorLog, this, &CertTool::onErrorLog);
            connect(crtsh, &Crtsh::rateLimitLog, this, &CertTool::onRateLimitLog);
            /* ... */
            connect(this, &CertTool::stopScanThread, crtsh, &AbstractOsintModule::onStop);
            connect(this, &CertTool::pauseScanThread, crtsh, &AbstractOsintModule::onPause);
            /* ... */
            connect(cThread, &QThread::finished, this, &CertTool::onEnumerationComplete);
            connect(cThread, &QThread::finished, crtsh, &Crtsh::deleteLater);
            connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);

            cThread->start();
        }
        }
        break;
    }
    case TARGET_HOSTNAME:
    {
        /* getting target, and determining target type */
        certificate::ScanArguments *args = new certificate::ScanArguments;
        args->target = ui->lineEditTarget->text();
        args->singleTarget = true;
        args->raw = true;

        /* getting protocal to use */
        switch(ui->comboBoxOption->currentIndex()){
        case PROTOCAL::HTTPS:
            args->https = true;
            break;
        case PROTOCAL::SSH:
            args->ssh = true;
            break;
        case PROTOCAL::FTP:
            args->ftp = true;
            break;
        }

        /* enumerating */
        certificate::Scanner *scanner = new certificate::Scanner(args);
        scanner->startScan(cThread);
        scanner->moveToThread(cThread);
        connect(scanner, &certificate::Scanner::resultRaw, this, &CertTool::onResult);
        connect(scanner, &certificate::Scanner::errorLog, this, &CertTool::onErrorLogTxt);
        connect(scanner, &certificate::Scanner::infoLog, this, &CertTool::onInfoLogTxt);
        connect(cThread, &QThread::finished, this, &CertTool::onEnumerationComplete);
        connect(cThread, &QThread::finished, scanner, &certificate::Scanner::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        connect(cThread, &QThread::finished, this, [=](){delete args;});
        cThread->start();
        break;
    }
    }
}

void CertTool::on_checkBoxExpand_clicked(bool checked){
    if(checked)
        ui->treeResults->expandAll();
    else
        ui->treeResults->collapseAll();
}

void CertTool::on_comboBoxTargetType_currentIndexChanged(int index){
    ui->lineEditTarget->clear();

    /* set new placeholdertext */
    switch(index){
    case TARGET_SSLCERT:
        ui->lineEditTarget->setPlaceholderText(PLACEHOLDERTEXT_SSLCERT);
        break;
    case TARGET_HOSTNAME:
        ui->lineEditTarget->setPlaceholderText(PLACEHOLDERTEXT_DOMAIN);
        break;
    }

    /* set new options on option combobox */
    switch(index){
    case TARGET_SSLCERT:
        ui->labelOption->setText("*Osint Module");
        ui->comboBoxOption->clear();
        ui->comboBoxOption->addItems(m_osintModules);
        break;
    case TARGET_HOSTNAME:
        ui->labelOption->setText("*Protocal To Use");
        ui->comboBoxOption->clear();
        ui->comboBoxOption->addItems(m_protocals);
        break;
    }
}


/**********************************************************************
                        scan slots
***********************************************************************/

void CertTool::onEnumerationComplete(){
    ui->buttonStart->setEnabled(true);
    ui->buttonStop->setDisabled(true);
}

void CertTool::onInfoLogTxt(QString log){
    ui->plainTextEditLogs->appendPlainText(log);
}

void CertTool::onErrorLogTxt(QString log){
    QString message("<font color=\"red\">"+log+"</font>");
    ui->plainTextEditLogs->appendHtml(message);
}

void CertTool::onErrorLog(ScanLog log){
    QString message("<font color=\"red\">"+log.message+"</font>");
    QString module("<font color=\"red\">"+log.moduleName+"</font>");
    QString status("<font color=\"red\">"+QString::number(log.statusCode)+"</font>");
    ui->plainTextEditLogs->appendHtml("[Module]        :"+module);
    ui->plainTextEditLogs->appendHtml("[Status Code]   :"+status);
    ui->plainTextEditLogs->appendHtml("[Error message] :"+message);
    ui->plainTextEditLogs->appendPlainText("");
}

void CertTool::onInfoLog(ScanLog log){
    QString module("<font color=\"green\">"+log.moduleName+"</font>");
    QString status("<font color=\"green\">"+QString::number(log.statusCode)+"</font>");
    ui->plainTextEditLogs->appendHtml("[Module]        :"+module);
    ui->plainTextEditLogs->appendHtml("[Status Code]   :"+status);
    ui->plainTextEditLogs->appendPlainText("");
}

void CertTool::onRateLimitLog(ScanLog log){
    QString message("<font color=\"yellow\">"+log.message+"</font>");
    QString module("<font color=\"yellow\">"+log.moduleName+"</font>");
    QString status("<font color=\"yellow\">"+QString::number(log.statusCode)+"</font>");
    ui->plainTextEditLogs->appendHtml("[Module]        :"+module);
    ui->plainTextEditLogs->appendHtml("[Status Code]   :"+status);
    ui->plainTextEditLogs->appendHtml("[Error message] :"+message);
    ui->plainTextEditLogs->appendPlainText("");
}

void CertTool::onResult(QByteArray rawCert){

    foreach(const QSslCertificate &cert, QSslCertificate::fromData(rawCert, QSsl::Pem))
    {
        /* ... */
        m_model->info_verison->setText(cert.version());
        m_model->info_serialNumber->setText(cert.serialNumber());
        m_model->info_signatureAlgorithm->setText(""); // none yet

        /* fingerprint */
        m_model->fingerprint_md5->setText(cert.digest(QCryptographicHash::Md5).toHex());
        m_model->fingerprint_sha1->setText(cert.digest(QCryptographicHash::Sha1).toHex());
        m_model->fingerprint_sha256->setText(cert.digest(QCryptographicHash::Sha256).toHex());

        /* validity */
        m_model->validity_notBefore->setText(cert.effectiveDate().toString());
        m_model->validity_notAfter->setText(cert.expiryDate().toString());

        /* issuer Info */
        if(cert.issuerInfo(QSslCertificate::CommonName).length() > 0)
            m_model->issuer_commonName->setText(cert.issuerInfo(QSslCertificate::CommonName)[0]);
        if(cert.issuerInfo(QSslCertificate::Organization).length() > 0)
            m_model->issuer_organizationName->setText(cert.issuerInfo(QSslCertificate::Organization)[0]);
        if(cert.issuerInfo(QSslCertificate::CountryName).length() > 0)
            m_model->issuer_countryName->setText(cert.issuerInfo(QSslCertificate::CountryName)[0]);

        /* subject info */
        if(cert.subjectInfo(QSslCertificate::CommonName).length() > 0)
            m_model->subject_commonName->setText(cert.subjectInfo(QSslCertificate::CommonName)[0]);
        if(cert.subjectInfo(QSslCertificate::CountryName).length() > 0)
            m_model->subject_countryName->setText(cert.subjectInfo(QSslCertificate::CountryName)[0]);
        if(cert.subjectInfo(QSslCertificate::LocalityName).length() > 0)
            m_model->subject_localityName->setText(cert.subjectInfo(QSslCertificate::LocalityName)[0]);
        if(cert.subjectInfo(QSslCertificate::Organization).length() > 0)
            m_model->subject_organizationName->setText(cert.subjectInfo(QSslCertificate::Organization)[0]);
        if(cert.subjectInfo(QSslCertificate::StateOrProvinceName).length() > 0)
            m_model->subject_stateOrProvinceName->setText(cert.subjectInfo(QSslCertificate::StateOrProvinceName)[0]);
        if(cert.subjectInfo(QSslCertificate::EmailAddress).length() > 0)
            m_model->subject_email->setText(cert.subjectInfo(QSslCertificate::EmailAddress)[0]);

        // key type...
        if(cert.publicKey().type() == QSsl::PrivateKey)
            m_model->key_type->setText("Private Key");
        if(cert.publicKey().type() == QSsl::PublicKey)
            m_model->key_type->setText("Public Key");

        // algorithm type...
        if(cert.publicKey().algorithm() == QSsl::Rsa)
            m_model->key_algorithm->setText("RSA algorithm.");
        if(cert.publicKey().algorithm() == QSsl::Dsa)
            m_model->key_algorithm->setText("DSA algorithm.");
        if(cert.publicKey().algorithm() == QSsl::Ec)
            m_model->key_algorithm->setText("Elliptic Curve algorithm.");
        if(cert.publicKey().algorithm() == QSsl::Dh)
            m_model->key_algorithm->setText("Diffie-Hellman algorithm.");
        if(cert.publicKey().algorithm() == QSsl::Opaque)
            m_model->key_algorithm->setText("BlackBox");

        /* raw cert */
        ui->plainTextEditRawCert->setPlainText(cert.toPem());
        /* raw key */
        ui->plainTextEditRawKey->setPlainText(cert.publicKey().toPem());

        /* alternative names */
        int alternativeName = 0;
        foreach(const QString &value, cert.subjectAlternativeNames()){
            m_model->subjectAltNames->appendRow({new QStandardItem(QString::number(alternativeName)), new QStandardItem(value)});
            alternativeName++;
        }
    }
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

void CertTool::on_buttonConfig_clicked(){
    PassiveConfigDialog *scanConfig = new PassiveConfigDialog(this);
    scanConfig->setAttribute(Qt::WA_DeleteOnClose, true);
    scanConfig->show();
}
