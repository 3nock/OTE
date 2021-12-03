#include "Osint.h"
#include "ui_Osint.h"
//...
#include <QDateTime>
#include <QClipboard>
#include "src/utils/Config.h"
#include "src/utils/Definitions.h"
#include "src/dialogs/OsintConfigDialog.h"


/*
 * cidr not implemented in many modules...
 *
 * clear proxyModel to delete certain results...
 *
 * write the ultimate article on subdomain enumeration
 *
 * fix the regex for filter, it breaks on special chars
 *
 * log on every scan each module its results count
 *
 * http://ipv4info.com/account/register/
 * https://riddler.io/auth/register
 * https://dnschecker.org/subdomainIp-tools.php
 * https://suip.biz/
 * https://viewdns.info/api/
 * https://rapiddns.io/tools
 * pkey.in
 * https://skrapp.io/api
 *
 * https://tools.epieos.com/email.php ->email-lookup
 *
 * you can crawl the enumerated urls subdomainIp eg from archives
 * and return only the status codes of each url and show a graph
 * of status codes eg like screaming frog crawler analysis
 * eg. how many 200,301,404,501
 * create its own seperate analysis tool...
 *
 * add option for certificates and on comboBox and their own
 * modelView which show subdomainIp cert n subdomain info
 * graph of certificates
 *
 * create a dialog(program properties) that shows subdomainIp api-csubdomainIps url so
 * user can be able to fix them without compiling..
 *
 * emit a QStringList/QSet instead of individual list
 * automaticsubdomainIpy group the subdmains with stars into one group, from cert scans
 * for later multilevel scanning...
 *
 * create a Map structure that has a subdomain name, ip-address, banners & other info
 * then only link the structure name to the modelview more additional info are to be
 * added on the structure by other scan types...
 *
 * use treeView instead of tableView to display osint results..
 * so every additional info on the structure adds on the tree view, which
 * can be saved as a html file tree or json data.
 *
 * same data different model views eg general treeView, subdomain-ip table view, subdomain-ListView
 * ip-listView
 * subdomainIp updated at the same time...
 *
 * osint config should contain every osint module with its options of what you
 * want to enumerate
 *
 * active services eg HTTP should be boolean on the structure...
 *
 *  avoiding emmiting empty results
 *
 * reverse lookups for mx, ns, ip.. eq viewdns..
 * dont count txt
 *
 * countries resources eg asns : ripe
 */


Osint::Osint(QWidget *parent, ResultsModel *resultsModel, ProjectDataModel *project, Status *status):
    AbstractEngine(parent, resultsModel, project, status),
    ui(new Ui::Osint),
    m_scanArguments(new osint::ScanArguments)
{
    ui->setupUi(this);
    ///
    /// init...
    ///
    ui->targets->init("Targets");
    targets = ui->targets;
    scanConfig->name = tr("ScanConfig-Osint");
    ///
    /// ...
    ///
    m_currentPath = QDir::currentPath();
    ui->lineEditTarget->setPlaceholderText(PLACEHOLDERTEXT_DOMAIN);
    ui->lineEditFilter->setPlaceholderText("Filter...");
    //...
    result->osint->subdomainIp->setHorizontalHeaderLabels({"Subdomains", "IpAddresses"});
    result->osint->subdomain->setHorizontalHeaderLabels({"Subdomains"});
    result->osint->ip->setHorizontalHeaderLabels({"IpAddresses"});
    result->osint->email->setHorizontalHeaderLabels({"Emails"});
    result->osint->url->setHorizontalHeaderLabels({"Urls"});
    result->osint->asn->setHorizontalHeaderLabels({"Asn", "Name"});
    result->osint->sslCert->setHorizontalHeaderLabels({"SSL-Cert FingerPrint"});
    result->osint->cidr->setHorizontalHeaderLabels({"Cidr"});
    //...
    ui->tableViewResults->setModel(result->osint->subdomainProxy);
    //...
    ui->buttonStop->setDisabled(true);
    ///
    /// hide widgets...
    ///
    ui->progressBar->hide();
    ui->targets->hide();
    ui->comboBoxFilter->hide();

    ui->splitter->setSizes(QList<int>() << static_cast<int>((this->width() * 0.50))
                                        << static_cast<int>((this->width() * 0.50)));
    this->connectActions();
    ///
    /// ....
    ///
    ui->lineEditNewProfile->setPlaceholderText("Enter New Profile's Name...");
    ui->frameProfiles->hide();
    this->initProfiles();
    this->initModules();

    /* registering meta-objects */
    qRegisterMetaType<ScanLog>("ScanLog");
}
Osint::~Osint(){
    delete m_scanArguments;
    delete m_scanResults;
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
    this->startScan();
}

void Osint::on_lineEditTarget_returnPressed(){
    on_buttonStart_clicked();
}

void Osint::on_buttonStop_clicked(){
    emit stopScan();
    sendStatus("[*] Stopping...");
}

void Osint::stopScan(){

}

void Osint::ResumeScan(){

}

void Osint::onScanThreadEnded(){
    ///
    /// check if no active thread...
    ///
    status->osint->activeThreads--;
    if(status->osint->activeThreads)
        return;
    ///
    /// reanabling the widgets...
    ///
    ui->buttonStart->setEnabled(true);
    ui->buttonStop->setDisabled(true);
    ///
    /// status...
    ///
    //...
    this->m_infoLog("------------------ End ----------------");
    emit sendStatus("[END] Enumeration Complete!");
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
    OsintConfigDialog *scanConfig = new OsintConfigDialog(this);
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
    this->initModules();
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
    this->initModules();
}
