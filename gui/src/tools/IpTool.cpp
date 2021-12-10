#include "IpTool.h"
#include "ui_IpTool.h"

#define ALL 0
#define IPINFO 1
#define IPAPI 2


IpTool::IpTool(QWidget *parent) : QDialog(parent), ui(new Ui::IpTool),
    m_model(new IpModel)
{
    ui->setupUi(this);

    /* placeholder texts... */
    ui->lineEditFilter->setPlaceholderText("Filter...");
    ui->lineEditTarget->setPlaceholderText(PLACEHOLDERTEXT_IP);

    /* setting model with tableView...*/
    ui->treeResults->setModel(m_model->model);

    /* for scan... */
    m_args = new ScanArgs;
    m_args->outputInfo = true;
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
    m_args->target = ui->lineEditTarget->text();
    ///
    /// ....
    ///
    ui->buttonStop->setEnabled(true);
    ui->buttonStart->setDisabled(true);

    QThread *cThread = new QThread;
    int engineToUse = ui->comboBoxEngine->currentIndex();

    if(engineToUse == ALL || engineToUse == IPINFO)
    {
        IpInfo *ipinfo = new IpInfo(m_args);
        ipinfo->Enumerator(cThread);
        ipinfo->moveToThread(cThread);
        connect(ipinfo, &IpInfo::infoLog, this, &IpTool::onInfoLog);
        connect(ipinfo, &IpInfo::errorLog, this, &IpTool::onErrorLog);
        connect(ipinfo, &IpInfo::rateLimitLog, this, &IpTool::onRateLimitLog);
        connect(cThread, &QThread::finished, this, &IpTool::onEnumerationComplete);
        connect(cThread, &QThread::finished, ipinfo, &IpInfo::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        cThread->start();
    }

    if(engineToUse == ALL || engineToUse == IPAPI)
    {
        IpApi *ipApi = new IpApi(m_args);
        ipApi->Enumerator(cThread);
        ipApi->moveToThread(cThread);
        connect(ipApi, &IpInfo::infoLog, this, &IpTool::onInfoLog);
        connect(ipApi, &IpInfo::errorLog, this, &IpTool::onErrorLog);
        connect(ipApi, &IpInfo::rateLimitLog, this, &IpTool::onRateLimitLog);
        connect(cThread, &QThread::finished, this, &IpTool::onEnumerationComplete);
        connect(cThread, &QThread::finished, ipApi, &IpInfo::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        cThread->start();
    }
}
