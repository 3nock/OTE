#include "ASNTool.h"
#include "ui_ASNTool.h"

#include "src/utils/Definitions.h"


ASNTool::ASNTool(QWidget *parent) : QDialog(parent), ui(new Ui::ASNTool),
    asModel(new AsModel)
{
    ui->setupUi(this);

    /* placeholder texts... */
    ui->lineEditFilter->setPlaceholderText("Filter...");
    ui->lineEditTarget->setPlaceholderText(PLACEHOLDERTEXT_ASN);

    /* setting model with tableView... */
    ui->treeResults->setModel(asModel->model);

    /* for scan... */
    m_scanArgs = new ScanArgs;

    /* registering meta-objects */
    qRegisterMetaType<AsModelStruct>("AsModelStruct");
}
ASNTool::~ASNTool(){
    delete ui;
}

void ASNTool::onResultsAsn(AsModelStruct results){
    /* info */
    asModel->info_asn->setText(results.info_asn);
    asModel->info_name->setText(results.info_name);
    asModel->info_description->setText(results.info_description);
    asModel->info_country->setText(results.info_country);
    asModel->info_website->setText(results.info_website);
    asModel->info_ownerAddress->setText(results.info_ownerAddress);

    /* rir */
    asModel->rir_name->setText(results.rir_name);
    asModel->rir_country->setText(results.rir_country);
    asModel->rir_dateAllocated->setText(results.rir_dateallocated);

    /* email contacts */
    int emailContacts = 0;
    foreach(const QString &value, results.emailcontacts){
        asModel->emailContacts->appendRow({new QStandardItem(QString::number(emailContacts)), new QStandardItem(value)});
        emailContacts++;
    }

    /* abuse contacts */
    int abuseContacts = 0;
    foreach(const QString &value, results.abusecontacts){
        asModel->abuseContacts->appendRow({new QStandardItem(QString::number(abuseContacts)), new QStandardItem(value)});
        abuseContacts++;
    }
}

void ASNTool::onResultsAsnPeers(AsModelStruct results){
    int peers = 0;
    foreach(const QString &value, results.peers){
        asModel->peers->appendRow({new QStandardItem(QString::number(peers)), new QStandardItem(value)});
        peers++;
    }
}

void ASNTool::onResultsAsnPrefixes(AsModelStruct results){
    int prefixes = 0;
    foreach(const QString &value, results.prefixes){
        asModel->prefixes->appendRow({new QStandardItem(QString::number(prefixes)), new QStandardItem(value)});
        prefixes++;
    }
}

void ASNTool::onEnumerationComplete(){
    ui->buttonAnalyze->setEnabled(true);
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

void ASNTool::on_checkBoxExpand_clicked(bool checked){
    if(checked)
        ui->treeResults->expandAll();
    else
        ui->treeResults->collapseAll();
}

void ASNTool::on_buttonAnalyze_clicked(){
    m_scanArgs->target = ui->lineEditTarget->text();
    switch(ui->comboBoxOption->currentIndex()){
    case 0:
        m_scanArgs->outputInfoAsn = true;
        m_scanArgs->outputInfoAsnPeers = false;
        m_scanArgs->outputInfoAsnPrefixes = false;
        break;
    case 1:
        m_scanArgs->outputInfoAsn = false;
        m_scanArgs->outputInfoAsnPeers = true;
        m_scanArgs->outputInfoAsnPrefixes = false;
        break;
    case 2:
        m_scanArgs->outputInfoAsn = false;
        m_scanArgs->outputInfoAsnPeers = false;
        m_scanArgs->outputInfoAsnPrefixes = true;
        break;
    }

    ui->buttonStop->setEnabled(true);
    ui->buttonAnalyze->setDisabled(true);

    QThread *cThread = new QThread;
    int engineToUse = ui->comboBoxEngine->currentIndex();

    if(engineToUse == 0)
    {
        Bgpview *bgpview = new Bgpview(m_scanArgs);
        bgpview->Enumerator(cThread);
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
        connect(cThread, &QThread::finished, this, &ASNTool::onEnumerationComplete);
        connect(cThread, &QThread::finished, bgpview, &Bgpview::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        cThread->start();
    }
}
