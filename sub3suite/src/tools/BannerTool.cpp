#include "BannerTool.h"
#include "ui_BannerTool.h"

#include <QDateTime>
#include "src/utils/Definitions.h"

#define SERVICE_HTTP 0
#define SERVICE_HTTPS 1
#define SERVICE_FTP 2
#define SERVICE_SSH 3
#define SERVICE_SMTP 4

#define TARGET_HOSTNAME 0
#define TARGET_IP 1


BannerTool::BannerTool(QWidget *parent) : QDialog(parent),
    ui(new Ui::BannerTool),
    scanConfig(new banner::ScanConfig)
{
    ui->setupUi(this);
    ///
    /// placeholdertxt..
    ///
    ui->lineEditTarget->setPlaceholderText(PLACEHOLDERTEXT_DOMAIN);
}
BannerTool::~BannerTool(){
    delete ui;
}

void BannerTool::on_buttonStart_clicked(){
    this->m_startScan();
}

void BannerTool::onScanResultsBanner(QString banner){
    ui->plainTextEditResults->appendPlainText(banner);
}

void BannerTool::onEnumerationComplete(){
    ui->buttonStart->setEnabled(true);
    ui->buttonStop->setDisabled(true);
}

void BannerTool::onErrorLog(QString log){
    QString fontedLog;
    fontedLog.append("<font color=\"red\">").append(log).append("</font>");
    QString logTime = QDateTime::currentDateTime().toString("[hh:mm]  ");
    ui->plainTextEditLog->appendHtml(logTime.append(fontedLog));
}

void BannerTool::onInfoLog(QString log){
    QString logTime = QDateTime::currentDateTime().toString("[hh:mm]  ");
    ui->plainTextEditLog->appendPlainText(logTime.append(log));
}

void BannerTool::m_startScan(){
    banner::ScanArgs *scanArgs = new banner::ScanArgs;
    scanArgs->config = scanConfig;
    ///
    /// getting target...
    ///
    scanArgs->target = ui->lineEditTarget->text();

    switch (ui->comboBoxTargetType->currentIndex()) {
    case TARGET_HOSTNAME:
        scanArgs->hostname = true;
        break;
    case TARGET_IP:
        scanArgs->ipaddress = true;
        break;
    }

    ///
    /// getting service type to check...
    ///
    switch (ui->comboBoxService->currentIndex()) {
    case SERVICE_HTTP:
        scanArgs->http = true;
        break;
    case SERVICE_HTTPS:
        scanArgs->https = true;
        break;
    case SERVICE_FTP:
        scanArgs->ftp = true;
        break;
    case SERVICE_SSH:
        scanArgs->ssh = true;
        break;
    case SERVICE_SMTP:
        scanArgs->smtp = true;
        break;
    }

    ///
    /// ...
    ///
    ui->buttonStop->setEnabled(true);
    ui->buttonStart->setDisabled(true);

    ///
    /// starting the scan...
    ///
    banner::Scanner *scanner = new banner::Scanner(scanArgs);
    QThread *cThread = new QThread;
    scanner->startScan(cThread);
    scanner->moveToThread(cThread);
    connect(scanner, &banner::Scanner::scanResultBanner, this, &BannerTool::onScanResultsBanner);
    connect(scanner, &banner::Scanner::errorLog, this, &BannerTool::onErrorLog);
    connect(scanner, &banner::Scanner::infoLog, this, &BannerTool::onInfoLog);
    connect(cThread, &QThread::finished, this, &BannerTool::onEnumerationComplete);
    connect(cThread, &QThread::finished, scanner, &banner::Scanner::deleteLater);
    connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
    connect(cThread, &QThread::finished, this, [=](){delete scanArgs;});
    cThread->start();
}

void BannerTool::on_comboBoxTargetType_currentIndexChanged(int index){
    switch (index) {
    case TARGET_HOSTNAME:
        ui->lineEditTarget->setPlaceholderText(PLACEHOLDERTEXT_DOMAIN);
        break;
    case TARGET_IP:
        ui->lineEditTarget->setPlaceholderText(PLACEHOLDERTEXT_IP);
        break;
    }
}
