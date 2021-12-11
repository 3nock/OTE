#include "NSTool.h"
#include "ui_NSTool.h"
//...
#include "src/dialogs/PassiveConfigDialog.h"


NSTool::NSTool(QWidget *parent) : QDialog(parent), ui(new Ui::NSTool),
    m_model(new NSModel)
{
    ui->setupUi(this);

    /* placeholder texts... */
    ui->lineEditFilter->setPlaceholderText("Filter...");
    ui->lineEditTarget->setPlaceholderText(PLACEHOLDERTEXT_NS);

    /* setting model with tableView... */
    ui->treeResults->setModel(m_model->model);

    /* for scan... */
    m_args = new ScanArgs;

    /* registering meta-objects */
    qRegisterMetaType<CidrModelStruct>("CidrModelStruct");
}

NSTool::~NSTool()
{
    delete ui;
}

void NSTool::onResultsNS(NSModelStruct results){
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

void NSTool::onEnumerationComplete(){
    ui->buttonStart->setEnabled(true);
    ui->buttonStop->setDisabled(true);
}

void NSTool::onErrorLog(ScanLog log){
    QString message("<font color=\"red\">"+log.message+"</font>");
    QString module("<font color=\"red\">"+log.moduleName+"</font>");
    QString status("<font color=\"red\">"+QString::number(log.statusCode)+"</font>");
    ui->plainTextEditLogs->appendHtml("[Module]        :"+module);
    ui->plainTextEditLogs->appendHtml("[Status Code]   :"+status);
    ui->plainTextEditLogs->appendHtml("[Error message] :"+message);
    ui->plainTextEditLogs->appendPlainText("");
}

void NSTool::onInfoLog(ScanLog log){
    QString module("<font color=\"green\">"+log.moduleName+"</font>");
    QString status("<font color=\"green\">"+QString::number(log.statusCode)+"</font>");
    ui->plainTextEditLogs->appendHtml("[Module]        :"+module);
    ui->plainTextEditLogs->appendHtml("[Status Code]   :"+status);
    ui->plainTextEditLogs->appendPlainText("");
}

void NSTool::onRateLimitLog(ScanLog log){
    QString message("<font color=\"yellow\">"+log.message+"</font>");
    QString module("<font color=\"yellow\">"+log.moduleName+"</font>");
    QString status("<font color=\"yellow\">"+QString::number(log.statusCode)+"</font>");
    ui->plainTextEditLogs->appendHtml("[Module]        :"+module);
    ui->plainTextEditLogs->appendHtml("[Status Code]   :"+status);
    ui->plainTextEditLogs->appendHtml("[Error message] :"+message);
    ui->plainTextEditLogs->appendPlainText("");
}

void NSTool::on_checkBoxExpand_clicked(bool checked){
    if(checked)
        ui->treeResults->expandAll();
    else
        ui->treeResults->collapseAll();
}

void NSTool::on_buttonStart_clicked(){
    /*
    m_args->target = ui->lineEditTarget->text();
    m_args->outputInfoCidr = true;

    ui->buttonStop->setEnabled(true);
    ui->buttonStart->setDisabled(true);

    QThread *cThread = new QThread;
    int engineToUse = ui->comboBoxEngine->currentIndex();

    if(engineToUse == 0)
    {
        Bgpview *bgpview = new Bgpview(m_scanArgs);
        bgpview->Enumerator(cThread);
        bgpview->moveToThread(cThread);
        connect(bgpview, &IpInfo::infoCidr, this, &NSTool::onResultsCidr);
        connect(bgpview, &IpInfo::infoLog, this, &NSTool::onInfoLog);
        connect(bgpview, &IpInfo::errorLog, this, &NSTool::onErrorLog);
        connect(bgpview, &IpInfo::rateLimitLog, this, &NSTool::onRateLimitLog);
        connect(cThread, &QThread::finished, this, &NSTool::onEnumerationComplete);
        connect(cThread, &QThread::finished, bgpview, &Bgpview::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        cThread->start();
    }
    */
}

void NSTool::on_buttonConfig_clicked(){
    PassiveConfigDialog *scanConfig = new PassiveConfigDialog(this);
    scanConfig->setAttribute(Qt::WA_DeleteOnClose, true);
    scanConfig->show();
}
