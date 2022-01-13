/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#include "ASNTool.h"
#include "ui_ASNTool.h"

#include "src/utils/Definitions.h"
#include "src/dialogs/PassiveConfigDialog.h"


ASNTool::ASNTool(QWidget *parent) : QWidget(parent), ui(new Ui::ASNTool),
    m_model(new AsModel)
{
    ui->setupUi(this);

    /* placeholder texts... */
    ui->lineEditFilter->setPlaceholderText("Filter...");
    ui->lineEditTarget->setPlaceholderText(PLACEHOLDERTEXT_ASN);

    /* setting model with tableView... */
    ui->treeResults->setModel(m_model->model);

    /* equally seperate the widgets... */
    ui->splitter->setSizes(QList<int>() << static_cast<int>((this->width() * 0.50))
                                        << static_cast<int>((this->width() * 0.50)));
}
ASNTool::~ASNTool(){
    delete ui;
}

void ASNTool::onResultsAsn(AsModelStruct results){
    /* info */
    m_model->info_asn->setText(results.info_asn);
    m_model->info_name->setText(results.info_name);
    m_model->info_description->setText(results.info_description);
    m_model->info_country->setText(results.info_country);
    m_model->info_website->setText(results.info_website);
    m_model->info_ownerAddress->setText(results.info_ownerAddress);

    /* rir */
    m_model->rir_name->setText(results.rir_name);
    m_model->rir_country->setText(results.rir_country);
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
}

void ASNTool::onResultsAsnPeers(AsModelStruct results){
    int peers = 0;
    foreach(const QString &value, results.peers){
        m_model->peers->appendRow({new QStandardItem(QString::number(peers)), new QStandardItem(value)});
        peers++;
    }
}

void ASNTool::onResultsAsnPrefixes(AsModelStruct results){
    int prefixes = 0;
    foreach(const QString &value, results.prefixes){
        m_model->prefixes->appendRow({new QStandardItem(QString::number(prefixes)), new QStandardItem(value)});
        prefixes++;
    }
}

void ASNTool::onEnumerationComplete(){
    ui->buttonStart->setEnabled(true);
    ui->buttonStop->setDisabled(true);
}

void ASNTool::onErrorLog(ScanLog log){
    QString message("<font color=\"red\">"+log.message+"</font>");
    QString module("<font color=\"red\">"+log.moduleName+"</font>");
    QString status("<font color=\"red\">"+QString::number(log.statusCode)+"</font>");
    ui->plainTextEditLogs->appendHtml("[Module]        :"+module);
    ui->plainTextEditLogs->appendHtml("[Status Code]   :"+status);
    ui->plainTextEditLogs->appendHtml("[Error message] :"+message);
    ui->plainTextEditLogs->appendPlainText("");
}

void ASNTool::onInfoLog(ScanLog log){
    QString module("<font color=\"green\">"+log.moduleName+"</font>");
    QString status("<font color=\"green\">"+QString::number(log.statusCode)+"</font>");
    ui->plainTextEditLogs->appendHtml("[Module]        :"+module);
    ui->plainTextEditLogs->appendHtml("[Status Code]   :"+status);
    ui->plainTextEditLogs->appendPlainText("");
}

void ASNTool::onRateLimitLog(ScanLog log){
    QString message("<font color=\"yellow\">"+log.message+"</font>");
    QString module("<font color=\"yellow\">"+log.moduleName+"</font>");
    QString status("<font color=\"yellow\">"+QString::number(log.statusCode)+"</font>");
    ui->plainTextEditLogs->appendHtml("[Module]        :"+module);
    ui->plainTextEditLogs->appendHtml("[Status Code]   :"+status);
    ui->plainTextEditLogs->appendHtml("[Error message] :"+message);
    ui->plainTextEditLogs->appendPlainText("");
}

void ASNTool::on_buttonStart_clicked(){
    ScanArgs scanArgs;

    ///
    /// getting arguments...
    ///
    if(ui->checkBoxMultipleTargets->isChecked()){
        // for multiple targets...
    }else{
        scanArgs.targets.push(ui->lineEditTarget->text());
    }

    switch(ui->comboBoxOption->currentIndex()){
    case 0:
        scanArgs.outputInfoAsn = true;
        scanArgs.outputInfoAsnPeers = false;
        scanArgs.outputInfoAsnPrefixes = false;
        break;
    case 1:
        scanArgs.outputInfoAsn = false;
        scanArgs.outputInfoAsnPeers = true;
        scanArgs.outputInfoAsnPrefixes = false;
        break;
    case 2:
        scanArgs.outputInfoAsn = false;
        scanArgs.outputInfoAsnPeers = false;
        scanArgs.outputInfoAsnPrefixes = true;
        break;
    }

    ///
    /// enabling/disabling widgets...
    ///
    ui->buttonStop->setEnabled(true);
    ui->buttonStart->setDisabled(true);

    ///
    /// start scan-thread...
    ///
    QThread *cThread = new QThread;
    int engineToUse = ui->comboBoxEngine->currentIndex();

    if(engineToUse == 0)
    {
        Bgpview *bgpview = new Bgpview(scanArgs);
        bgpview->startScan(cThread);
        bgpview->moveToThread(cThread);

        switch(ui->comboBoxOption->currentIndex()){
        case 0:
            connect(bgpview, &IpInfo::infoASN, this, &ASNTool::onResultsAsn);
            break;
        case 1:
            connect(bgpview, &IpInfo::infoASN, this, &ASNTool::onResultsAsnPeers);
            break;
        case 2:
            connect(bgpview, &IpInfo::infoASN, this, &ASNTool::onResultsAsnPrefixes);
            break;
        }
        connect(bgpview, &IpInfo::infoLog, this, &ASNTool::onInfoLog);
        connect(bgpview, &IpInfo::errorLog, this, &ASNTool::onErrorLog);
        connect(bgpview, &IpInfo::rateLimitLog, this, &ASNTool::onRateLimitLog);
        /* ... */
        connect(this, &ASNTool::stopScanThread, bgpview, &AbstractOsintModule::onStop);
        connect(this, &ASNTool::pauseScanThread, bgpview, &AbstractOsintModule::onPause);
        /* ... */
        connect(cThread, &QThread::finished, this, &ASNTool::onEnumerationComplete);
        connect(cThread, &QThread::finished, bgpview, &Bgpview::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        cThread->start();
    }
}

void ASNTool::on_buttonConfig_clicked(){
    PassiveConfigDialog *scanConfig = new PassiveConfigDialog(this);
    scanConfig->setAttribute(Qt::WA_DeleteOnClose, true);
    scanConfig->show();
}
