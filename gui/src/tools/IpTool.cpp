#include "IpTool.h"
#include "ui_IpTool.h"

#define ALL 0
#define IPINFO 1
#define IPAPI 2

IpTool::IpTool(QWidget *parent) : QDialog(parent), ui(new Ui::IpTool),
    ipModel(new IpModel)
{
    ui->setupUi(this);
    ///
    /// placeholder texts...
    ///
    ui->lineEditFilter->setPlaceholderText("Filter...");
    ui->lineEditTarget->setPlaceholderText("Target eg 1.1.1.1");
    ///
    /// setting model with tableView...
    ///
    ui->treeResults->setModel(ipModel->model);
    ///
    /// for scan...
    ///
    m_scanArgs = new ScanArgs;
    m_scanArgs->outputInfo = true;
}
IpTool::~IpTool(){
    delete ui;
}

void IpTool::onEnumerationComplete(){
    ui->buttonAnalyze->setEnabled(true);
    ui->buttonStop->setDisabled(true);
}

void IpTool::onErrorLog(QString log){
    QString fontedLog;
    fontedLog.append("<font color=\"red\">").append(log).append("</font>");
    QString logTime = QDateTime::currentDateTime().toString("[hh:mm]  ");
    ui->plainTextEditLogs->appendHtml(logTime.append(fontedLog));
}

void IpTool::onInfoLog(QString log){
    QString logTime = QDateTime::currentDateTime().toString("[hh:mm]  ");
    ui->plainTextEditLogs->appendPlainText(logTime.append(log));
}

void IpTool::on_checkBoxExpand_clicked(bool checked){
    if(checked)
        ui->treeResults->expandAll();
    else
        ui->treeResults->collapseAll();
}

void IpTool::on_buttonAnalyze_clicked(){
    m_scanArgs->target = ui->lineEditTarget->text();
    ///
    /// ....
    ///
    ui->buttonStop->setEnabled(true);
    ui->buttonAnalyze->setDisabled(true);

    QThread *cThread = new QThread;
    int engineToUse = ui->comboBoxEngine->currentIndex();

    if(engineToUse == ALL || engineToUse == IPINFO)
    {
        IpInfo *ipinfo = new IpInfo(m_scanArgs);
        ipinfo->Enumerator(cThread);
        ipinfo->moveToThread(cThread);
        //...
        //connect(ipinfo, &IpInfo::errorLog, this, &IpTool::onErrorLog);
        //connect(ipinfo, &IpInfo::infoLog, this, &IpTool::onInfoLog);
        connect(cThread, &QThread::finished, this, &IpTool::onEnumerationComplete);
        connect(cThread, &QThread::finished, ipinfo, &IpInfo::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
    }

    if(engineToUse == ALL || engineToUse == IPAPI)
    {
        IpApi *ipApi = new IpApi(m_scanArgs);
        ipApi->Enumerator(cThread);
        ipApi->moveToThread(cThread);
        //...
        //connect(ipApi, &IpInfo::errorLog, this, &IpTool::onErrorLog);
        //connect(ipApi, &IpInfo::infoLog, this, &IpTool::onInfoLog);
        connect(cThread, &QThread::finished, this, &IpTool::onEnumerationComplete);
        connect(cThread, &QThread::finished, ipApi, &IpInfo::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
    }
}
