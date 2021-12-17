#include "Osint.h"
#include "ui_Osint.h"

#include <QDateTime>
#include <QClipboard>
#include "src/utils/Config.h"
#include "src/utils/Definitions.h"
#include "src/dialogs/PassiveConfigDialog.h"


Osint::Osint(QWidget *parent, ResultsModel *resultsModel, ProjectDataModel *project, Status *status):
    AbstractEngine(parent, resultsModel, project, status),
    ui(new Ui::Osint),
    m_targetListModelHostname(new QStringListModel),
    m_targetListModelIp(new QStringListModel),
    m_targetListModelAsn(new QStringListModel),
    m_targetListModelCidr(new QStringListModel),
    m_targetListModelCert(new QStringListModel),
    m_targetListModelEmail(new QStringListModel)
{
    ui->setupUi(this);
    /* init... */
    ui->targets->setListName("Targets");
    ui->targets->setListModel(m_targetListModelHostname);

    /* ... */
    m_currentPath = QDir::currentPath();
    ui->lineEditTarget->setPlaceholderText(PLACEHOLDERTEXT_DOMAIN);
    ui->lineEditFilter->setPlaceholderText("Filter...");

    /* results models */
    result->osint->subdomainIp->setHorizontalHeaderLabels({"Subdomains", "IpAddresses"});
    result->osint->subdomain->setHorizontalHeaderLabels({"Subdomains"});
    result->osint->ip->setHorizontalHeaderLabels({"IpAddresses"});
    result->osint->email->setHorizontalHeaderLabels({"Emails"});
    result->osint->url->setHorizontalHeaderLabels({"Urls"});
    result->osint->asn->setHorizontalHeaderLabels({"Asn", "Name"});
    result->osint->sslCert->setHorizontalHeaderLabels({"SSL-Cert FingerPrint"});
    result->osint->cidr->setHorizontalHeaderLabels({"Cidr"});
    ui->tableViewResults->setModel(result->osint->subdomainProxy);

    /* hide widgets... */
    ui->buttonStop->setDisabled(true);
    ui->progressBar->hide();
    ui->targets->hide();
    ui->comboBoxFilter->hide();

    /* profiles */
    ui->lineEditNewProfile->setPlaceholderText("Enter New Profile's Name...");
    ui->frameProfiles->hide();
    this->m_initProfiles();
    this->m_initModules();

    /* registering meta-objects */
    qRegisterMetaType<ScanLog>("ScanLog");

    /* equally seperate the widgets... */
    ui->splitter->setSizes(QList<int>() << static_cast<int>((this->width() * 0.50))
                                        << static_cast<int>((this->width() * 0.50)));

    /* initiate all actions for the context menus */
    this->m_initActions();

    /* syntax higlighting... */
    m_notesSyntaxHighlighter = new NotesSyntaxHighlighter(ui->plainTextEditNotes->document());

}
Osint::~Osint(){
    delete m_targetListModelIp;
    delete m_targetListModelAsn;
    delete m_targetListModelCert;
    delete m_targetListModelCidr;
    delete m_targetListModelEmail;
    delete m_targetListModelHostname;
    delete ui;
}

void Osint::m_infoLog(QString log){
    QString logTime = QDateTime::currentDateTime().toString("hh:mm:ss  ");
    ui->plainTextEditLogs->appendPlainText("\n"+logTime+log+"\n");
}

void Osint::m_errorLog(QString log){
    QString fontedLog("<font color=\"red\">"+log+"</font>");
    QString logTime = QDateTime::currentDateTime().toString("hh:mm:ss  ");
    ui->plainTextEditLogs->appendHtml("\n"+logTime+fontedLog+"\n");
}

void Osint::on_buttonStart_clicked(){
    if(ui->lineEditTarget->text().isEmpty()){
        QMessageBox::warning(this, "Error!", "Please Target Domain For Enumerations!");
        return;
    }

    this->m_startScan();
}

void Osint::on_lineEditTarget_returnPressed(){
    on_buttonStart_clicked();
}

void Osint::on_buttonStop_clicked(){
    emit stopScanThread();
}

void Osint::m_stopScan(){

}

void Osint::m_resumeScan(){

}

void Osint::onScanThreadEnded(){
    /* check if no active thread... */
    status->osint->activeScanThreads--;
    if(status->osint->activeScanThreads)
        return;

    /* reanabling the widgets... */
    ui->buttonStart->setEnabled(true);
    ui->buttonStop->setDisabled(true);

    this->m_infoLog("------------------ End ----------------");
}

void Osint::onClearResults(){
    switch(ui->comboBoxOutput->currentIndex()){
    case OUTPUT_SUBDOMAINIP:
        result->osint->subdomainIp->clear();
        result->osint->subdomainIp->setHorizontalHeaderLabels({"Subdomains", "IpAddresses"});
        break;
    case OUTPUT_SUBDOMAIN:
        result->osint->subdomain->clear();
        result->osint->subdomain->setHorizontalHeaderLabels({"Subdomains"});
        break;
    case OUTPUT_IP:
        result->osint->ip->clear();
        result->osint->ip->setHorizontalHeaderLabels({"IpAddresses"});
        break;
    case OUTPUT_EMAIL:
        result->osint->email->clear();
        result->osint->email->setHorizontalHeaderLabels({"Emails"});
        break;
    case OUTPUT_URL:
        result->osint->url->clear();
        result->osint->url->setHorizontalHeaderLabels({"Urls"});
        break;
    case OUTPUT_ASN:
        result->osint->asn->clear();
        result->osint->asn->setHorizontalHeaderLabels({"Asn", "Name"});
        break;
    case OUTPUT_SSLCERT:
        result->osint->sslCert->clear();
        result->osint->sslCert->setHorizontalHeaderLabels({"Asn", "Name"});
        break;
    case OUTPUT_CIDR:
        result->osint->sslCert->clear();
        result->osint->sslCert->setHorizontalHeaderLabels({" Cidr"});
        break;
    }
    ui->labelResultsCount->clear();
    ///
    /// clear the progressbar...
    ///
    ui->progressBar->clearMask();
    ui->progressBar->reset();
    ui->progressBar->hide();
}


void Osint::on_buttonKeys_clicked(){
    ApiKeysDialog *apiKeysDialog = new ApiKeysDialog(this);
    apiKeysDialog->setAttribute(Qt::WA_DeleteOnClose, true);
    apiKeysDialog->show();
}

void Osint::on_buttonConfig_clicked(){
    PassiveConfigDialog *scanConfig = new PassiveConfigDialog(this);
    scanConfig->setAttribute(Qt::WA_DeleteOnClose, true);
    scanConfig->show();
}

void Osint::on_checkBoxMultipleTargets_clicked(bool checked){
    if(checked)
        ui->targets->show();
    else
        ui->targets->hide();
}

void Osint::on_lineEditFilter_textChanged(const QString &filterKeyword){
    switch(ui->comboBoxOutput->currentIndex()){
    case OUTPUT_SUBDOMAINIP:
        result->osint->subdomainIpProxy->setFilterKeyColumn(ui->comboBoxFilter->currentIndex());
        result->osint->subdomainIpProxy->setFilterRegExp(filterKeyword);
        ui->tableViewResults->setModel(result->osint->subdomainIpProxy);
        ui->labelResultsCount->setNum(result->osint->subdomainIpProxy->rowCount());
        break;
    case OUTPUT_SUBDOMAIN:
        result->osint->subdomainProxy->setFilterRegExp(filterKeyword);
        ui->tableViewResults->setModel(result->osint->subdomainProxy);
        ui->labelResultsCount->setNum(result->osint->subdomainProxy->rowCount());
        break;
    case OUTPUT_IP:
        result->osint->ipProxy->setFilterRegExp(filterKeyword);
        ui->tableViewResults->setModel(result->osint->ipProxy);
        ui->labelResultsCount->setNum(result->osint->ipProxy->rowCount());
        break;
    case OUTPUT_EMAIL:
        result->osint->emailProxy->setFilterRegExp(filterKeyword);
        ui->tableViewResults->setModel(result->osint->emailProxy);
        ui->labelResultsCount->setNum(result->osint->emailProxy->rowCount());
        break;
    case OUTPUT_URL:
        result->osint->urlProxy->setFilterRegExp(filterKeyword);
        ui->tableViewResults->setModel(result->osint->urlProxy);
        ui->labelResultsCount->setNum(result->osint->urlProxy->rowCount());
        break;
    case OUTPUT_ASN:
        result->osint->asnProxy->setFilterKeyColumn(ui->comboBoxFilter->currentIndex());
        result->osint->asnProxy->setFilterRegExp(filterKeyword);
        ui->tableViewResults->setModel(result->osint->asnProxy);
        ui->labelResultsCount->setNum(result->osint->asnProxy->rowCount());
        break;
    case OUTPUT_SSLCERT:
        result->osint->sslCertProxy->setFilterRegExp(filterKeyword);
        ui->tableViewResults->setModel(result->osint->sslCertProxy);
        ui->labelResultsCount->setNum(result->osint->sslCertProxy->rowCount());
        break;
    case OUTPUT_CIDR:
        result->osint->cidrProxy->setFilterRegExp(filterKeyword);
        ui->tableViewResults->setModel(result->osint->cidrProxy);
        ui->labelResultsCount->setNum(result->osint->cidrProxy->rowCount());
        break;
    }
}

void Osint::on_comboBoxInput_currentIndexChanged(int index){
    /* clearing the target first, since another type of input is introduced */
    ui->lineEditTarget->clear();

    /* setting a respective placeholdertext on the target line edit */
    switch (index) {
    case INPUT_DOMAIN:
        ui->lineEditTarget->setPlaceholderText(PLACEHOLDERTEXT_DOMAIN);
        break;
    case INPUT_IP:
        ui->lineEditTarget->setPlaceholderText(PLACEHOLDERTEXT_IP);
        break;
    case INPUT_EMAIL:
        ui->lineEditTarget->setPlaceholderText(PLACEHOLDERTEXT_EMAIL);
        break;
    case INPUT_URL:
        ui->lineEditTarget->setPlaceholderText(PLACEHOLDERTEXT_URL);
        break;
    case INPUT_ASN:
        ui->lineEditTarget->setPlaceholderText(PLACEHOLDERTEXT_ASN);
        break;
    case INPUT_SSLCERT:
        ui->lineEditTarget->setPlaceholderText(PLACEHOLDERTEXT_SSLCERT);
        break;
    case INPUT_CIDR:
        ui->lineEditTarget->setPlaceholderText(PLACEHOLDERTEXT_CIDR);
        break;
    }
    /* only show modules that supports both input-type and output-type */
    this->m_initModules();
}

void Osint::on_comboBoxOutput_currentIndexChanged(int index){
    switch(index){
    case OUTPUT_SUBDOMAINIP:
        ui->tableViewResults->setModel(result->osint->subdomainIpProxy);
        ui->labelResultsCount->setNum(result->osint->subdomainIpProxy->rowCount());
        ui->comboBoxFilter->clear();
        ui->comboBoxFilter->addItems({"Subdomain", "Ip"});
        ui->comboBoxFilter->show();
        ui->labelResultsInfo->setText("Enumerated Subdomains");
        break;
    case OUTPUT_SUBDOMAIN:
        ui->tableViewResults->setModel(result->osint->subdomainProxy);
        ui->labelResultsCount->setNum(result->osint->subdomainProxy->rowCount());
        ui->comboBoxFilter->hide();
        break;
    case OUTPUT_IP:
        ui->tableViewResults->setModel(result->osint->ipProxy);
        ui->labelResultsCount->setNum(result->osint->ipProxy->rowCount());
        ui->comboBoxFilter->hide();
        ui->labelResultsInfo->setText("Enumerated Subdomains");
        break;
    case OUTPUT_EMAIL:
        ui->tableViewResults->setModel(result->osint->emailProxy);
        ui->labelResultsCount->setNum(result->osint->emailProxy->rowCount());
        ui->comboBoxFilter->hide();
        ui->labelResultsInfo->setText("Enumerated Emails");
        break;
    case OUTPUT_URL:
        ui->tableViewResults->setModel(result->osint->urlProxy);
        ui->labelResultsCount->setNum(result->osint->urlProxy->rowCount());
        ui->comboBoxFilter->hide();
        ui->labelResultsInfo->setText("Enumerated Urls");
        break;
    case OUTPUT_ASN:
        ui->tableViewResults->setModel(result->osint->asnProxy);
        ui->labelResultsCount->setNum(result->osint->asnProxy->rowCount());
        ui->comboBoxFilter->clear();
        ui->comboBoxFilter->addItems({"ASN", "Name"});
        ui->comboBoxFilter->show();
        ui->labelResultsInfo->setText("Enumerated ASNs");
        break;
    case OUTPUT_SSLCERT:
        ui->tableViewResults->setModel(result->osint->sslCertProxy);
        ui->labelResultsCount->setNum(result->osint->sslCertProxy->rowCount());
        ui->comboBoxFilter->clear();
        ui->comboBoxFilter->hide();
        ui->labelResultsInfo->setText("Enumerated Certificates");
        break;
    case OUTPUT_CIDR:
        ui->tableViewResults->setModel(result->osint->cidrProxy);
        ui->labelResultsCount->setNum(result->osint->cidrProxy->rowCount());
        ui->comboBoxFilter->clear();
        ui->comboBoxFilter->hide();
        ui->labelResultsInfo->setText("Enumerated Cidr");
        break;
    }
    /* only show modules that supports both input-type and output-type */
    this->m_initModules();
}
