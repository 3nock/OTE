/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#include "CidrEnum.h"
#include "ui_CidrEnum.h"

#include "src/utils/Definitions.h"
#include "src/dialogs/PassiveConfigDialog.h"


CidrEnum::CidrEnum(QWidget *parent, ProjectModel *project) : AbstractEnum(parent, project),
    ui(new Ui::CidrEnum),
    m_model(new CidrModel)
{
    ui->setupUi(this);

    ui->frame->setProperty("default_frame", true);

    /* placeholder texts... */
    ui->lineEditFilter->setPlaceholderText("Filter...");
    ui->lineEditTarget->setPlaceholderText(PLACEHOLDERTEXT_CIDR);

    /* setting model with tableView... */
    ui->treeResults->setModel(m_model->model);

    /* equally seperate the widgets... */
    ui->splitter->setSizes(QList<int>() << static_cast<int>((this->width() * 0.50))
                                        << static_cast<int>((this->width() * 0.50)));
}
CidrEnum::~CidrEnum(){
    delete m_model;
    delete ui;
}

void CidrEnum::onResult(CidrModelStruct results){
    /* info */
    m_model->info_prefix->setText(results.info_prefix);
    m_model->info_ip->setText(results.info_ip);
    m_model->info_cidr->setText(results.info_cidr);
    m_model->info_name->setText(results.info_name);
    m_model->info_description->setText(results.info_description);
    m_model->info_website->setText(results.info_website);
    m_model->info_ownerAddress->setText(results.info_ownerAddress);

    /* rir */
    m_model->rir_name->setText(results.rir_name);
    m_model->rir_country->setText(results.rir_country);
    m_model->rir_prefix->setText(results.rir_prefix);
    m_model->rir_dateAllocated->setText(results.rir_dateallocated);

    /* email contacts */
    int emailContacts = 0;
    foreach(const QString &value, results.emailcontacts){
        m_model->emailContacts->appendRow({new QStandardItem(QString::number(emailContacts)), new QStandardItem(value)});
        emailContacts++;
    }

    /* abuse contacts */
    int abuseContacts = 0;
    foreach(const QString &value, results.abusecontacts){
        m_model->abuseContacts->appendRow({new QStandardItem(QString::number(abuseContacts)), new QStandardItem(value)});
        abuseContacts++;
    }

    /* asns */
    int asns = 0;
    foreach(const QString &value, results.asns){
        m_model->asns->appendRow({new QStandardItem(QString::number(asns)), new QStandardItem(value)});
        asns++;
    }
}

void CidrEnum::onEnumerationComplete(){
    ui->buttonStart->setEnabled(true);
    ui->buttonStop->setDisabled(true);
}

void CidrEnum::onErrorLog(ScanLog log){
    QString message("<font color=\"red\">"+log.message+"</font>");
    QString module("<font color=\"red\">"+log.moduleName+"</font>");
    QString status("<font color=\"red\">"+QString::number(log.statusCode)+"</font>");
    ui->plainTextEditLogs->appendHtml("[Module]        :"+module);
    ui->plainTextEditLogs->appendHtml("[Status Code]   :"+status);
    ui->plainTextEditLogs->appendHtml("[Error message] :"+message);
    ui->plainTextEditLogs->appendPlainText("");
}

void CidrEnum::onInfoLog(ScanLog log){
    QString module("<font color=\"green\">"+log.moduleName+"</font>");
    QString status("<font color=\"green\">"+QString::number(log.statusCode)+"</font>");
    ui->plainTextEditLogs->appendHtml("[Module]        :"+module);
    ui->plainTextEditLogs->appendHtml("[Status Code]   :"+status);
    ui->plainTextEditLogs->appendPlainText("");
}

void CidrEnum::onRateLimitLog(ScanLog log){
    QString message("<font color=\"yellow\">"+log.message+"</font>");
    QString module("<font color=\"yellow\">"+log.moduleName+"</font>");
    QString status("<font color=\"yellow\">"+QString::number(log.statusCode)+"</font>");
    ui->plainTextEditLogs->appendHtml("[Module]        :"+module);
    ui->plainTextEditLogs->appendHtml("[Status Code]   :"+status);
    ui->plainTextEditLogs->appendHtml("[Error message] :"+message);
    ui->plainTextEditLogs->appendPlainText("");
}

void CidrEnum::on_buttonStart_clicked(){
    ScanArgs scanArgs;

    ///
    /// getting scan arguments...
    ///
    scanArgs.outputInfoCidr = true;
    if(ui->checkBoxMultipleTargets->isChecked()){
        // for multiple targets...
    }else{
        scanArgs.targets.push(ui->lineEditTarget->text());
    }

    ui->buttonStop->setEnabled(true);
    ui->buttonStart->setDisabled(true);

    ///
    /// starting the scan thread...
    ///
    QThread *cThread = new QThread;
    int engineToUse = ui->comboBoxEngine->currentIndex();

    if(engineToUse == 0)
    {
        Bgpview *bgpview = new Bgpview(scanArgs);
        bgpview->startScan(cThread);
        bgpview->moveToThread(cThread);

        connect(bgpview, &IpInfo::infoCidr, this, &CidrEnum::onResult);
        /* ... */
        connect(bgpview, &IpInfo::infoLog, this, &CidrEnum::onInfoLog);
        connect(bgpview, &IpInfo::errorLog, this, &CidrEnum::onErrorLog);
        connect(bgpview, &IpInfo::rateLimitLog, this, &CidrEnum::onRateLimitLog);
        /* ... */
        connect(this, &CidrEnum::stopScanThread, bgpview, &AbstractOsintModule::onStop);
        connect(this, &CidrEnum::pauseScanThread, bgpview, &AbstractOsintModule::onPause);
        /* ... */
        connect(cThread, &QThread::finished, this, &CidrEnum::onEnumerationComplete);
        connect(cThread, &QThread::finished, bgpview, &Bgpview::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);

        cThread->start();
    }
}

void CidrEnum::on_buttonConfig_clicked(){
    PassiveConfigDialog *scanConfig = new PassiveConfigDialog(this);
    scanConfig->setAttribute(Qt::WA_DeleteOnClose, true);
    scanConfig->show();
}
