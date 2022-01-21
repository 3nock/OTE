/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#include "NSEnum.h"
#include "ui_NSEnum.h"

#include "src/dialogs/PassiveConfigDialog.h"


#define DNSLYTICS 0


NSEnum::NSEnum(QWidget *parent, ProjectModel *project) : AbstractEnum(parent, project),
    ui(new Ui::NSEnum),
    m_model(new NSModel)
{
    ui->setupUi(this);

    /* enter used modules */
    ui->comboBoxEngine->addItem("DNSLytics");
    ui->comboBoxOption->addItem("Domains");

    /* placeholder texts... */
    ui->lineEditFilter->setPlaceholderText("Filter...");
    ui->lineEditTarget->setPlaceholderText(PLACEHOLDERTEXT_NS);

    /* setting model with tableView... */
    ui->treeResults->setModel(m_model->model);

    /* equally seperate the widgets... */
    ui->splitter->setSizes(QList<int>() << static_cast<int>((this->width() * 0.50))
                                        << static_cast<int>((this->width() * 0.50)));
}
NSEnum::~NSEnum(){
    delete m_model;
    delete ui;
}

void NSEnum::onResultsNS(NSModelStruct results){
    /* info */
    m_model->info_ns->setText(results.info_ns);
    m_model->info_ip->setText(results.info_ip);

    /* domains */
    int domains = 0;
    foreach(const QString &value, results.domains){
        m_model->domains->appendRow({new QStandardItem(QString::number(domains)), new QStandardItem(value)});
        domains++;
    }
}

void NSEnum::onEnumerationComplete(){
    ui->buttonStart->setEnabled(true);
    ui->buttonStop->setDisabled(true);
}

void NSEnum::onErrorLog(ScanLog log){
    QString message("<font color=\"red\">"+log.message+"</font>");
    QString module("<font color=\"red\">"+log.moduleName+"</font>");
    QString status("<font color=\"red\">"+QString::number(log.statusCode)+"</font>");
    ui->plainTextEditLogs->appendHtml("[Module]        :"+module);
    ui->plainTextEditLogs->appendHtml("[Status Code]   :"+status);
    ui->plainTextEditLogs->appendHtml("[Error message] :"+message);
    ui->plainTextEditLogs->appendPlainText("");
}

void NSEnum::onInfoLog(ScanLog log){
    QString module("<font color=\"green\">"+log.moduleName+"</font>");
    QString status("<font color=\"green\">"+QString::number(log.statusCode)+"</font>");
    ui->plainTextEditLogs->appendHtml("[Module]        :"+module);
    ui->plainTextEditLogs->appendHtml("[Status Code]   :"+status);
    ui->plainTextEditLogs->appendPlainText("");
}

void NSEnum::onRateLimitLog(ScanLog log){
    QString message("<font color=\"yellow\">"+log.message+"</font>");
    QString module("<font color=\"yellow\">"+log.moduleName+"</font>");
    QString status("<font color=\"yellow\">"+QString::number(log.statusCode)+"</font>");
    ui->plainTextEditLogs->appendHtml("[Module]        :"+module);
    ui->plainTextEditLogs->appendHtml("[Status Code]   :"+status);
    ui->plainTextEditLogs->appendHtml("[Error message] :"+message);
    ui->plainTextEditLogs->appendPlainText("");
}

void NSEnum::on_buttonStart_clicked(){
    /* scan argumemts */
    ScanArgs scanArgs;

    /* getting the targets */
    if(ui->checkBoxMultipleTargets->isChecked()){

    }else
        scanArgs.targets.push(ui->lineEditTarget->text());

    scanArgs.outputInfoNS = true;

    ui->buttonStop->setEnabled(true);
    ui->buttonStart->setDisabled(true);

    /* starting appropriate engine enumeration thread */
    QThread *cThread = new QThread;
    switch (ui->comboBoxEngine->currentIndex()) {
    case DNSLYTICS:
        Dnslytics *dnslytics = new Dnslytics(scanArgs);
        dnslytics->startScan(cThread);
        dnslytics->moveToThread(cThread);
        connect(dnslytics, &IpInfo::infoNS, this, &NSEnum::onResultsNS);
        connect(dnslytics, &IpInfo::infoLog, this, &NSEnum::onInfoLog);
        connect(dnslytics, &IpInfo::errorLog, this, &NSEnum::onErrorLog);
        connect(dnslytics, &IpInfo::rateLimitLog, this, &NSEnum::onRateLimitLog);
        connect(cThread, &QThread::finished, this, &NSEnum::onEnumerationComplete);
        connect(cThread, &QThread::finished, dnslytics, &Bgpview::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        cThread->start();
    }
}

void NSEnum::on_buttonConfig_clicked(){
    PassiveConfigDialog *scanConfig = new PassiveConfigDialog(this);
    scanConfig->setAttribute(Qt::WA_DeleteOnClose, true);
    scanConfig->show();
}
