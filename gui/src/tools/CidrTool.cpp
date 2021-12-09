#include "CidrTool.h"
#include "ui_CidrTool.h"

#include "src/utils/Definitions.h"


CidrTool::CidrTool(QWidget *parent) : QDialog(parent), ui(new Ui::CidrTool),
    cidrModel(new CidrModel)
{
    ui->setupUi(this);

    /* placeholder texts... */
    ui->lineEditFilter->setPlaceholderText("Filter...");
    ui->lineEditTarget->setPlaceholderText(PLACEHOLDERTEXT_CIDR);

    /* setting model with tableView... */
    ui->treeResults->setModel(cidrModel->model);

    /* for scan... */
    m_scanArgs = new ScanArgs;

    /* registering meta-objects */
    qRegisterMetaType<CidrModelStruct>("CidrModelStruct");
}
CidrTool::~CidrTool(){
    delete ui;
}

void CidrTool::onResultsCidr(CidrModelStruct results){
    /* info */
    cidrModel->info_prefix->setText(results.info_prefix);
    cidrModel->info_ip->setText(results.info_ip);
    cidrModel->info_cidr->setText(results.info_cidr);
    cidrModel->info_name->setText(results.info_name);
    cidrModel->info_description->setText(results.info_description);
    cidrModel->info_website->setText(results.info_website);
    cidrModel->info_ownerAddress->setText(results.info_ownerAddress);

    /* rir */
    cidrModel->rir_name->setText(results.rir_name);
    cidrModel->rir_country->setText(results.rir_country);
    cidrModel->rir_prefix->setText(results.rir_prefix);
    cidrModel->rir_dateAllocated->setText(results.rir_dateallocated);

    /* email contacts */
    int emailContacts = 0;
    foreach(const QString &value, results.emailcontacts){
        cidrModel->emailContacts->appendRow({new QStandardItem(QString::number(emailContacts)), new QStandardItem(value)});
        emailContacts++;
    }

    /* abuse contacts */
    int abuseContacts = 0;
    foreach(const QString &value, results.abusecontacts){
        cidrModel->abuseContacts->appendRow({new QStandardItem(QString::number(abuseContacts)), new QStandardItem(value)});
        abuseContacts++;
    }

    /* asns */
    int asns = 0;
    foreach(const QString &value, results.asns){
        cidrModel->asns->appendRow({new QStandardItem(QString::number(asns)), new QStandardItem(value)});
        asns++;
    }
}

void CidrTool::onEnumerationComplete(){
    ui->buttonAnalyze->setEnabled(true);
    ui->buttonStop->setDisabled(true);
}

void CidrTool::onErrorLog(ScanLog log){
    QString message("<font color=\"red\">"+log.message+"</font>");
    QString module("<font color=\"red\">"+log.moduleName+"</font>");
    QString status("<font color=\"red\">"+QString::number(log.statusCode)+"</font>");
    ui->plainTextEditLogs->appendHtml("[Module]        :"+module);
    ui->plainTextEditLogs->appendHtml("[Status Code]   :"+status);
    ui->plainTextEditLogs->appendHtml("[Error message] :"+message);
    ui->plainTextEditLogs->appendPlainText("");
}

void CidrTool::onInfoLog(ScanLog log){
    QString module("<font color=\"green\">"+log.moduleName+"</font>");
    QString status("<font color=\"green\">"+QString::number(log.statusCode)+"</font>");
    ui->plainTextEditLogs->appendHtml("[Module]        :"+module);
    ui->plainTextEditLogs->appendHtml("[Status Code]   :"+status);
    ui->plainTextEditLogs->appendPlainText("");
}

void CidrTool::onRateLimitLog(ScanLog log){
    QString message("<font color=\"yellow\">"+log.message+"</font>");
    QString module("<font color=\"yellow\">"+log.moduleName+"</font>");
    QString status("<font color=\"yellow\">"+QString::number(log.statusCode)+"</font>");
    ui->plainTextEditLogs->appendHtml("[Module]        :"+module);
    ui->plainTextEditLogs->appendHtml("[Status Code]   :"+status);
    ui->plainTextEditLogs->appendHtml("[Error message] :"+message);
    ui->plainTextEditLogs->appendPlainText("");
}

void CidrTool::on_checkBoxExpand_clicked(bool checked){
    if(checked)
        ui->treeResults->expandAll();
    else
        ui->treeResults->collapseAll();
}

void CidrTool::on_buttonAnalyze_clicked(){
    m_scanArgs->target = ui->lineEditTarget->text();
    m_scanArgs->outputInfoCidr = true;

    ui->buttonStop->setEnabled(true);
    ui->buttonAnalyze->setDisabled(true);

    QThread *cThread = new QThread;
    int engineToUse = ui->comboBoxEngine->currentIndex();

    if(engineToUse == 0)
    {
        Bgpview *bgpview = new Bgpview(m_scanArgs);
        bgpview->Enumerator(cThread);
        bgpview->moveToThread(cThread);
        connect(bgpview, &IpInfo::infoCidr, this, &CidrTool::onResultsCidr);
        connect(bgpview, &IpInfo::infoLog, this, &CidrTool::onInfoLog);
        connect(bgpview, &IpInfo::errorLog, this, &CidrTool::onErrorLog);
        connect(bgpview, &IpInfo::rateLimitLog, this, &CidrTool::onRateLimitLog);
        connect(cThread, &QThread::finished, this, &CidrTool::onEnumerationComplete);
        connect(cThread, &QThread::finished, bgpview, &Bgpview::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        cThread->start();
    }
}
