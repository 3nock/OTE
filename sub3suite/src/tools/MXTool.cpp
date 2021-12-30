#include "MXTool.h"
#include "ui_MXTool.h"

#include "src/dialogs/PassiveConfigDialog.h"


#define DNSLYTICS 0


MXTool::MXTool(QWidget *parent) : QDialog(parent), ui(new Ui::MXTool),
    m_model(new MXModel)
{
    ui->setupUi(this);

    /* enter used modules */
    ui->comboBoxEngine->addItem("DNSLytics");
    ui->comboBoxOption->addItem("Domains");

    /* placeholder texts... */
    ui->lineEditFilter->setPlaceholderText("Filter...");
    ui->lineEditTarget->setPlaceholderText(PLACEHOLDERTEXT_MX);

    /* setting model with tableView... */
    ui->treeResults->setModel(m_model->model);

    /* registering meta-objects */
    qRegisterMetaType<MXModelStruct>("MXModelStruct");
}
MXTool::~MXTool(){
    delete ui;
}

void MXTool::onResultsMX(MXModelStruct results){
    /* info */
    m_model->info_mx->setText(results.info_mx);
    m_model->info_ip->setText(results.info_ip);

    /* domains */
    int domains = 0;
    foreach(const QString &value, results.domains){
        m_model->domains->appendRow({new QStandardItem(QString::number(domains)), new QStandardItem(value)});
        domains++;
    }
}

void MXTool::onEnumerationComplete(){
    ui->buttonStart->setEnabled(true);
    ui->buttonStop->setDisabled(true);
}

void MXTool::onErrorLog(ScanLog log){
    QString message("<font color=\"red\">"+log.message+"</font>");
    QString module("<font color=\"red\">"+log.moduleName+"</font>");
    QString status("<font color=\"red\">"+QString::number(log.statusCode)+"</font>");
    ui->plainTextEditLogs->appendHtml("[Module]        :"+module);
    ui->plainTextEditLogs->appendHtml("[Status Code]   :"+status);
    ui->plainTextEditLogs->appendHtml("[Error message] :"+message);
    ui->plainTextEditLogs->appendPlainText("");
}

void MXTool::onInfoLog(ScanLog log){
    QString module("<font color=\"green\">"+log.moduleName+"</font>");
    QString status("<font color=\"green\">"+QString::number(log.statusCode)+"</font>");
    ui->plainTextEditLogs->appendHtml("[Module]        :"+module);
    ui->plainTextEditLogs->appendHtml("[Status Code]   :"+status);
    ui->plainTextEditLogs->appendPlainText("");
}

void MXTool::onRateLimitLog(ScanLog log){
    QString message("<font color=\"yellow\">"+log.message+"</font>");
    QString module("<font color=\"yellow\">"+log.moduleName+"</font>");
    QString status("<font color=\"yellow\">"+QString::number(log.statusCode)+"</font>");
    ui->plainTextEditLogs->appendHtml("[Module]        :"+module);
    ui->plainTextEditLogs->appendHtml("[Status Code]   :"+status);
    ui->plainTextEditLogs->appendHtml("[Error message] :"+message);
    ui->plainTextEditLogs->appendPlainText("");
}

void MXTool::on_checkBoxExpand_clicked(bool checked){
    if(checked)
        ui->treeResults->expandAll();
    else
        ui->treeResults->collapseAll();
}

void MXTool::on_buttonStart_clicked(){
    /* scan argumemts */
    ScanArgs scanArgs;

    /* getting the targets */
    if(ui->checkBoxMultipleTargets->isChecked()){

    }else
        scanArgs.targets.push(ui->lineEditTarget->text());

    scanArgs.outputInfoMX = true;

    ui->buttonStop->setEnabled(true);
    ui->buttonStart->setDisabled(true);

    /* starting appropriate engine enumeration thread */
    QThread *cThread = new QThread;
    switch (ui->comboBoxEngine->currentIndex()) {
    case DNSLYTICS:
        Dnslytics *dnslytics = new Dnslytics(scanArgs);
        dnslytics->startScan(cThread);
        dnslytics->moveToThread(cThread);
        connect(dnslytics, &IpInfo::infoMX, this, &MXTool::onResultsMX);
        connect(dnslytics, &IpInfo::infoLog, this, &MXTool::onInfoLog);
        connect(dnslytics, &IpInfo::errorLog, this, &MXTool::onErrorLog);
        connect(dnslytics, &IpInfo::rateLimitLog, this, &MXTool::onRateLimitLog);
        connect(cThread, &QThread::finished, this, &MXTool::onEnumerationComplete);
        connect(cThread, &QThread::finished, dnslytics, &Bgpview::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        cThread->start();
    }
}

void MXTool::on_buttonConfig_clicked(){
    PassiveConfigDialog *scanConfig = new PassiveConfigDialog(this);
    scanConfig->setAttribute(Qt::WA_DeleteOnClose, true);
    scanConfig->show();
}
