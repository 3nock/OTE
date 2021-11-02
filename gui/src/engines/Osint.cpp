#include "Osint.h"
#include "ui_Osint.h"
//...
#include <QDateTime>
#include <QClipboard>
#include "src/utils/Config.h"
#include "src/dialogs/OsintConfigDialog.h"

/* log on every scan each module its results count
 *
 * http://ipv4info.com/account/register/
 * https://riddler.io/auth/register
 * https://dnschecker.org/subdomainIp-tools.php
 * https://suip.biz/
 * https://viewdns.info/api/
 * https://rapiddns.io/tools
 * pkey.in
 * https://skrapp.io/api
 * https://hunter.io/bulk-finders
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
    ui->lineEditTarget->setPlaceholderText("eg. example.com, 1.1.1.1, <asn number>, <SHA1 certificate fingerprint>");
    ui->lineEditFilter->setPlaceholderText("Filter...");
    //...
    result->osint->subdomainIp->setHorizontalHeaderLabels({"Subdomains", "IpAddresses"});
    result->osint->subdomain->setHorizontalHeaderLabels({"Subdomains"});
    result->osint->ip->setHorizontalHeaderLabels({"IpAddresses"});
    result->osint->email->setHorizontalHeaderLabels({"Emails"});
    result->osint->url->setHorizontalHeaderLabels({"Urls"});
    result->osint->asn->setHorizontalHeaderLabels({"Asn", "Name"});
    result->osint->sslCert->setHorizontalHeaderLabels({"SSL Certs SHA-1 FingerPrints"});
    //...
    ui->tableViewResults->setModel(result->osint->subdomainIpProxy);
    //...
    //ui->buttonPause->setDisabled(true);
    ui->buttonStop->setDisabled(true);
    ///
    /// hide widgets...
    ///
    ui->progressBar->hide();
    ui->targets->hide();

    ui->splitter->setSizes(QList<int>() << static_cast<int>((this->width() * 0.50))
                                        << static_cast<int>((this->width() * 0.50)));
    this->connectActions();
}
Osint::~Osint(){
    delete m_scanArguments;
    delete m_scanResults;
    delete ui;
}

void Osint::onInfoLog(QString log){
    QString logTime = QDateTime::currentDateTime().toString("hh:mm:ss  ");
    ui->plainTextEditLogs->appendPlainText(logTime.append(log));
}

void Osint::onErrorLog(QString log){
    QString fontedLog;
    fontedLog.append("<font color=\"red\">").append(log).append("</font>");
    QString logTime = QDateTime::currentDateTime().toString("hh:mm:ss  ");
    ui->plainTextEditLogs->appendHtml(logTime.append(fontedLog));
}

void Osint::onResultSubdomainIp(QString subdomain, QString ip){
    int prevSize = m_subdomainIpSet.count();
    m_subdomainIpSet.insert(subdomain);
    if(m_subdomainIpSet.count() > prevSize){
        result->osint->subdomainIp->appendRow({new QStandardItem(subdomain), new QStandardItem(ip)});
        project->addPassiveSubdomain({subdomain, ip});
        ui->labelResultsCount->setNum(result->osint->subdomainIpProxy->rowCount());
    }
}

void Osint::onResultSubdomain(QString subdomain){
    int prevSize = m_subdomainSet.count();
    m_subdomainSet.insert(subdomain);
    if(m_subdomainSet.count() > prevSize){
        result->osint->subdomain->appendRow(new QStandardItem(subdomain));
        project->addPassiveSubdomain({subdomain});
        ui->labelResultsCount->setNum(result->osint->subdomainProxy->rowCount());
    }
}

void Osint::onResultIp(QString ip){
    int prevSize = m_ipSet.count();
    m_ipSet.insert(ip);
    if(m_ipSet.count() > prevSize){
        result->osint->ip->appendRow(new QStandardItem(ip));
        project->addPassiveIp({ip});
        ui->labelResultsCount->setNum(result->osint->ipProxy->rowCount());
    }
}

void Osint::onResultEmail(QString email){
    int prevSize = m_emailSet.count();
    m_emailSet.insert(email);
    if(m_emailSet.count() > prevSize){
        result->osint->email->appendRow(new QStandardItem(email));
        project->addPassiveEMail({email});
        ui->labelResultsCount->setNum(result->osint->emailProxy->rowCount());
    }
}

void Osint::onResultUrl(QString url){
    int prevSize = m_urlSet.count();
    m_urlSet.insert(url);
    if(m_urlSet.count() > prevSize){
        result->osint->url->appendRow(new QStandardItem(url));
        project->addPassiveUrl({url});
        ui->labelResultsCount->setNum(result->osint->urlProxy->rowCount());
    }
}

void Osint::onResultAsn(QString asnValue, QString asnName){
    int prevSize = m_asnSet.count();
    m_asnSet.insert(asnValue);
    if(m_asnSet.count() > prevSize){
        result->osint->asn->appendRow({new QStandardItem(asnValue), new QStandardItem(asnName)});
        project->addPassiveAsn({asnValue, asnName});
        ui->labelResultsCount->setNum(result->osint->asnProxy->rowCount());
    }
}

void Osint::onResultA(QString A){
    int prevSize = m_ipSet.count();
    m_ipSet.insert(A);
    if(m_ipSet.count() > prevSize){
        result->osint->ip->appendRow(new QStandardItem(A));
        project->addPassiveA({A});
        ui->labelResultsCount->setNum(result->osint->ipProxy->rowCount());
    }
}

void Osint::onResultAAAA(QString AAAA){
    int prevSize = m_ipSet.count();
    m_ipSet.insert(AAAA);
    if(m_ipSet.count() > prevSize){
        result->osint->ip->appendRow(new QStandardItem(AAAA));
        project->addPassiveAAAA({AAAA});
        ui->labelResultsCount->setNum(result->osint->ipProxy->rowCount());
    }
}

void Osint::onResultCNAME(QString CNAME){
    int prevSize = m_subdomainSet.count();
    m_subdomainSet.insert(CNAME);
    if(m_subdomainSet.count() > prevSize){
        result->osint->subdomain->appendRow(new QStandardItem(CNAME));
        project->addPassiveCNAME({CNAME});
        ui->labelResultsCount->setNum(result->osint->subdomainProxy->rowCount());
    }
}

void Osint::onResultMX(QString MX){
    int prevSize = m_subdomainSet.count();
    m_subdomainSet.insert(MX);
    if(m_subdomainSet.count() > prevSize){
        result->osint->subdomain->appendRow(new QStandardItem(MX));
        project->addPassiveMX({MX});
        ui->labelResultsCount->setNum(result->osint->subdomainProxy->rowCount());
    }
}

void Osint::onResultNS(QString NS){
    int prevSize = m_subdomainSet.count();
    m_subdomainSet.insert(NS);
    if(m_subdomainSet.count() > prevSize){
        result->osint->subdomain->appendRow(new QStandardItem(NS));
        project->addPassiveNS({NS});
        ui->labelResultsCount->setNum(result->osint->subdomainProxy->rowCount());
    }
}

void Osint::onResultTXT(QString TXT){
    project->addPassiveTXT({TXT});
}

void Osint::onResultCertFingerprint(QString certId){
    int prevSize = m_sslCertSet.count();
    m_sslCertSet.insert(certId);
    if(m_sslCertSet.count() > prevSize){
        result->osint->sslCert->appendRow(new QStandardItem(certId));
        project->addPassiveSSLCert({certId});
        ui->labelResultsCount->setNum(result->osint->sslCertProxy->rowCount());
    }
}

void Osint::on_buttonStart_clicked(){
    if(ui->lineEditTarget->text().isEmpty()){
        QMessageBox::warning(this, "Error!", "Please Target Domain For Enumerations!");
        return;
    }
    startScan();
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
    emit sendStatus("[END] Enumeration Complete!");
}

void Osint::onClearResults(){
    switch(ui->comboBoxOption->currentIndex()){
    case SUBDOMAINIP:
        result->osint->subdomainIp->clear();
        result->osint->subdomainIp->setHorizontalHeaderLabels({"Subdomains", "IpAddresses"});
        break;
    case SUBDOMAIN:
        result->osint->subdomain->clear();
        result->osint->subdomain->setHorizontalHeaderLabels({"Subdomains"});
        break;
    case IP:
        result->osint->ip->clear();
        result->osint->ip->setHorizontalHeaderLabels({"IpAddresses"});
        break;
    case EMAIL:
        result->osint->email->clear();
        result->osint->email->setHorizontalHeaderLabels({"Emails"});
        break;
    case URL:
        result->osint->url->clear();
        result->osint->url->setHorizontalHeaderLabels({"Urls"});
        break;
    case ASN:
        result->osint->asn->clear();
        result->osint->asn->setHorizontalHeaderLabels({"Asn", "Name"});
        break;
    case SSLCERT:
        result->osint->sslCert->clear();
        result->osint->sslCert->setHorizontalHeaderLabels({"Asn", "Name"});
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

void Osint::on_comboBoxOption_currentIndexChanged(int index){
    switch(index){
    case SUBDOMAINIP:
        ui->tableViewResults->setModel(result->osint->subdomainIpProxy);
        ui->labelResultsCount->setNum(result->osint->subdomainIpProxy->rowCount());
        ui->comboBoxFilter->clear();
        ui->comboBoxFilter->addItems({"Subdomain", "Ip"});
        ui->comboBoxFilter->show();
        break;
    case SUBDOMAIN:
        ui->tableViewResults->setModel(result->osint->subdomainProxy);
        ui->labelResultsCount->setNum(result->osint->subdomainProxy->rowCount());
        ui->comboBoxFilter->hide();
        break;
    case IP:
        ui->tableViewResults->setModel(result->osint->ipProxy);
        ui->labelResultsCount->setNum(result->osint->ipProxy->rowCount());
        ui->comboBoxFilter->hide();
        break;
    case EMAIL:
        ui->tableViewResults->setModel(result->osint->emailProxy);
        ui->labelResultsCount->setNum(result->osint->emailProxy->rowCount());
        ui->comboBoxFilter->hide();
        break;
    case URL:
        ui->tableViewResults->setModel(result->osint->urlProxy);
        ui->labelResultsCount->setNum(result->osint->urlProxy->rowCount());
        ui->comboBoxFilter->hide();
        break;
    case ASN:
        ui->tableViewResults->setModel(result->osint->asnProxy);
        ui->labelResultsCount->setNum(result->osint->asnProxy->rowCount());
        ui->comboBoxFilter->clear();
        ui->comboBoxFilter->addItems({"ASN", "Name"});
        ui->comboBoxFilter->show();
        break;
    case SSLCERT:
        ui->tableViewResults->setModel(result->osint->sslCertProxy);
        ui->labelResultsCount->setNum(result->osint->sslCertProxy->rowCount());
        ui->comboBoxFilter->clear();
        ui->comboBoxFilter->hide();
        break;
    }
}

void Osint::on_lineEditFilter_textChanged(const QString &filterKeyword){
    switch(ui->comboBoxOption->currentIndex()){
    case SUBDOMAINIP:
        result->osint->subdomainIpProxy->setFilterKeyColumn(ui->comboBoxFilter->currentIndex());
        result->osint->subdomainIpProxy->setFilterRegExp(filterKeyword);
        ui->tableViewResults->setModel(result->osint->subdomainIpProxy);
        ui->labelResultsCount->setNum(result->osint->subdomainIpProxy->rowCount());
        break;
    case SUBDOMAIN:
        result->osint->subdomainProxy->setFilterRegExp(filterKeyword);
        ui->tableViewResults->setModel(result->osint->subdomainProxy);
        ui->labelResultsCount->setNum(result->osint->subdomainProxy->rowCount());
        break;
    case IP:
        result->osint->ipProxy->setFilterRegExp(filterKeyword);
        ui->tableViewResults->setModel(result->osint->ipProxy);
        ui->labelResultsCount->setNum(result->osint->ipProxy->rowCount());
        break;
    case EMAIL:
        result->osint->emailProxy->setFilterRegExp(filterKeyword);
        ui->tableViewResults->setModel(result->osint->emailProxy);
        ui->labelResultsCount->setNum(result->osint->emailProxy->rowCount());
        break;
    case URL:
        result->osint->urlProxy->setFilterRegExp(filterKeyword);
        ui->tableViewResults->setModel(result->osint->urlProxy);
        ui->labelResultsCount->setNum(result->osint->urlProxy->rowCount());
        break;
    case ASN:
        result->osint->asnProxy->setFilterKeyColumn(ui->comboBoxFilter->currentIndex());
        result->osint->asnProxy->setFilterRegExp(filterKeyword);
        ui->tableViewResults->setModel(result->osint->asnProxy);
        ui->labelResultsCount->setNum(result->osint->asnProxy->rowCount());
        break;
    case SSLCERT:
        result->osint->sslCertProxy->setFilterRegExp(filterKeyword);
        ui->tableViewResults->setModel(result->osint->sslCertProxy);
        ui->labelResultsCount->setNum(result->osint->sslCertProxy->rowCount());
        break;
    }
}
