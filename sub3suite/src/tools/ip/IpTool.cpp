/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#include "IpTool.h"
#include "ui_IpTool.h"

#include "src/dialogs/PassiveConfigDialog.h"

#define ALL 0
#define IPINFO 1
#define IPAPI 2


IpTool::IpTool(QWidget *parent) : QWidget(parent), ui(new Ui::IpTool),
    m_model(new IpModel)
{
    ui->setupUi(this);

    /* placeholder texts... */
    ui->lineEditFilter->setPlaceholderText("Filter...");
    ui->lineEditTarget->setPlaceholderText(PLACEHOLDERTEXT_IP);

    /* setting model with tableView...*/
    ui->treeResults->setModel(m_model->model);

    /* equally seperate the widgets... */
    ui->splitter->setSizes(QList<int>() << static_cast<int>((this->width() * 0.50))
                                        << static_cast<int>((this->width() * 0.50)));
}
IpTool::~IpTool(){
    delete ui;
}

void IpTool::onResult(){
    /* NOT YET IMPLEMENTED */
}

void IpTool::onEnumerationComplete(){
    ui->buttonStart->setEnabled(true);
    ui->buttonStop->setDisabled(true);
}

void IpTool::onErrorLog(ScanLog log){
    QString message("<font color=\"red\">"+log.message+"</font>");
    QString module("<font color=\"red\">"+log.moduleName+"</font>");
    QString status("<font color=\"red\">"+QString::number(log.statusCode)+"</font>");
    ui->plainTextEditLogs->appendHtml("[Module]        :"+module);
    ui->plainTextEditLogs->appendHtml("[Status Code]   :"+status);
    ui->plainTextEditLogs->appendHtml("[Error message] :"+message);
    ui->plainTextEditLogs->appendPlainText("");
}

void IpTool::onInfoLog(ScanLog log){
    QString module("<font color=\"green\">"+log.moduleName+"</font>");
    QString status("<font color=\"green\">"+QString::number(log.statusCode)+"</font>");
    ui->plainTextEditLogs->appendHtml("[Module]        :"+module);
    ui->plainTextEditLogs->appendHtml("[Status Code]   :"+status);
    ui->plainTextEditLogs->appendPlainText("");
}

void IpTool::onRateLimitLog(ScanLog log){
    QString message("<font color=\"yellow\">"+log.message+"</font>");
    QString module("<font color=\"yellow\">"+log.moduleName+"</font>");
    QString status("<font color=\"yellow\">"+QString::number(log.statusCode)+"</font>");
    ui->plainTextEditLogs->appendHtml("[Module]        :"+module);
    ui->plainTextEditLogs->appendHtml("[Status Code]   :"+status);
    ui->plainTextEditLogs->appendHtml("[Error message] :"+message);
    ui->plainTextEditLogs->appendPlainText("");
}

void IpTool::on_checkBoxExpand_clicked(bool checked){
    if(checked)
        ui->treeResults->expandAll();
    else
        ui->treeResults->collapseAll();
}

void IpTool::on_buttonStart_clicked(){
    ScanArgs scanArgs;

    ///
    /// getting arguments...
    ///
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

    if(engineToUse == ALL || engineToUse == IPINFO)
    {
        IpInfo *ipinfo = new IpInfo(scanArgs);
        ipinfo->startScan(cThread);
        ipinfo->moveToThread(cThread);

        connect(ipinfo, &IpInfo::infoLog, this, &IpTool::onInfoLog);
        connect(ipinfo, &IpInfo::errorLog, this, &IpTool::onErrorLog);
        connect(ipinfo, &IpInfo::rateLimitLog, this, &IpTool::onRateLimitLog);
        /* ... */
        connect(this, &IpTool::stopScanThread, ipinfo, &AbstractOsintModule::onStop);
        connect(this, &IpTool::pauseScanThread, ipinfo, &AbstractOsintModule::onPause);
        /* ... */
        connect(cThread, &QThread::finished, this, &IpTool::onEnumerationComplete);
        connect(cThread, &QThread::finished, ipinfo, &IpInfo::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);

        cThread->start();
    }

    if(engineToUse == ALL || engineToUse == IPAPI)
    {
        IpApi *ipApi = new IpApi(scanArgs);
        ipApi->startScan(cThread);
        ipApi->moveToThread(cThread);

        connect(ipApi, &IpInfo::infoLog, this, &IpTool::onInfoLog);
        connect(ipApi, &IpInfo::errorLog, this, &IpTool::onErrorLog);
        connect(ipApi, &IpInfo::rateLimitLog, this, &IpTool::onRateLimitLog);
        /* ... */
        connect(this, &IpTool::stopScanThread, ipApi, &AbstractOsintModule::onStop);
        connect(this, &IpTool::pauseScanThread, ipApi, &AbstractOsintModule::onPause);
        /* ... */
        connect(cThread, &QThread::finished, this, &IpTool::onEnumerationComplete);
        connect(cThread, &QThread::finished, ipApi, &IpInfo::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);

        cThread->start();
    }
}

void IpTool::on_buttonConfig_clicked(){
    PassiveConfigDialog *scanConfig = new PassiveConfigDialog(this);
    scanConfig->setAttribute(Qt::WA_DeleteOnClose, true);
    scanConfig->show();
}
