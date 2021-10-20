#include "Osint.h"
#include "ui_Osint.h"
//...
#include <QDateTime>
#include <QClipboard>
#include "src/utils/Config.h"
#include "src/dialogs/OsintConfigDialog.h"
//...
#include "src/modules/osint/site/PagesInventory.h"
#include "src/modules/osint/site/SiteDossier.h"
#include "src/modules/osint/scrape/Bing.h"
#include "src/modules/osint/scrape/Yahoo.h"
#include "src/modules/osint/scrape/Trello.h"
#include "src/modules/osint/scrape/Exalead.h"
#include "src/modules/osint/scrape/DuckDuckGo.h"
#include "src/modules/osint/scrape/DogPile.h"
#include "src/modules/osint/scrape/Baidu.h"
#include "src/modules/osint/scrape/Ask.h"
#include "src/modules/osint/api/ViewDns.h"
#include "src/modules/osint/api/IpApi.h"
#include "src/modules/osint/api/ZoomEye.h"
#include "src/modules/osint/api/ZETAlytics.h"
#include "src/modules/osint/api/WhoisXmlApi.h"
#include "src/modules/osint/api/ThreatBook.h"
#include "src/modules/osint/api/Shodan.h"
#include "src/modules/osint/api/SecurityTrails.h"
#include "src/modules/osint/api/Robtex.h"
#include "src/modules/osint/api/RiskIq.h"
#include "src/modules/osint/api/Mnemonic.h"
#include "src/modules/osint/api/IpInfo.h"
#include "src/modules/osint/api/HunterSearch.h"
#include "src/modules/osint/api/Github.h"
#include "src/modules/osint/api/C99.h"
#include "src/modules/osint/api/BinaryEdge.h"
#include "src/modules/osint/api/Bgpview.h"
#include "src/modules/osint/api/Anubis.h"
#include "src/modules/osint/api/Otx.h"
#include "src/modules/osint/api/Sublist3r.h"
#include "src/modules/osint/api/Threatminer.h"
#include "src/modules/osint/api/Threatcrowd.h"
#include "src/modules/osint/api/HackerTargetFree.h"
#include "src/modules/osint/api/HackerTargetPaid.h"
#include "src/modules/osint/api/Dnsbufferoverun.h"
#include "src/modules/osint/api/Projectdiscovery.h"
#include "src/modules/osint/api/Spyse.h"
#include "src/modules/osint/api/Omnisint.h"
#include "src/modules/osint/api/Qwant.h"
#include "src/modules/osint/api/VirusTotal.h"
#include "src/modules/osint/api/Urlscan.h"
#include "src/modules/osint/archive/Waybackmachine.h"
#include "src/modules/osint/archive/ArchiveToday.h"
#include "src/modules/osint/archive/CommonCrawl.h"
#include "src/modules/osint/archive/ArchiveIt.h"
#include "src/modules/osint/site/Dnsdumpster.h"
#include "src/modules/osint/site/Netcraft.h"
#include "src/modules/osint/site/Suip.h"
#include "src/modules/osint/site/Pkey.h"
#include "src/modules/osint/site/Rapiddns.h"
#include "src/modules/osint/cert/Crtsh.h"
#include "src/modules/osint/cert/GoogleCert.h"
#include "src/modules/osint/cert/Certspotter.h"
#include "src/modules/osint/cert/CensysFree.h"

#define SUBDOMAINIP 0
#define SUBDOMAIN 1
#define IP 2
#define EMAIL 3
#define URL 4
#define ASN 5

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
    ui->lineEditTarget->setPlaceholderText("eg. example.com");
    ui->lineEditFilter->setPlaceholderText("Filter Results...");
    //...
    result->osint->subdomainIp->setHorizontalHeaderLabels({"Subdomains", "IpAddresses"});
    result->osint->subdomain->setHorizontalHeaderLabels({"Subdomains"});
    result->osint->ip->setHorizontalHeaderLabels({"IpAddresses"});
    result->osint->email->setHorizontalHeaderLabels({"Emails"});
    result->osint->url->setHorizontalHeaderLabels({"Urls"});
    result->osint->asn->setHorizontalHeaderLabels({"Asn", "Name"});
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

void Osint::startScan(){
    OsintModules module;
    ui->modules->getChoosenModules(module);

    ScanArgs *scanArgs = new ScanArgs;
    scanArgs->target = ui->lineEditTarget->text();
    scanArgs->inputDomain = true;

    switch(ui->comboBoxOption->currentIndex()){
    case 0:
        scanArgs->outputSubdomainIp = true;
        break;
    case 1:
        scanArgs->outputSubdomain = true;
        break;
    case 2:
        scanArgs->outputIp = true;
        break;
    case 3:
        scanArgs->outputEmail = true;
        break;
    case 4:
        scanArgs->outputUrl = true;
        break;
    case 5:
        scanArgs->outputAsn = true;
    }

    if(module.anubis)
    {
        Anubis *anubis = new Anubis(scanArgs);
        QThread *cThread = new QThread(this);
        anubis->Enumerator(cThread);
        anubis->moveToThread(cThread);
        //...
        connect(anubis, &Anubis::subdomain, this, &Osint::onResultSubdomain);
        connect(anubis, &Anubis::errorLog, this, &Osint::onErrorLog);
        connect(anubis, &Anubis::infoLog, this, &Osint::onInfoLog);
        connect(cThread, &QThread::finished, this, &Osint::onScanThreadEnded);
        connect(cThread, &QThread::finished, anubis, &Anubis::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        status->osint->activeThreads++;
    }
    if(module.bgpview){
        Bgpview *bgpview = new Bgpview(scanArgs);
        QThread *cThread = new QThread(this);
        bgpview->Enumerator(cThread);
        bgpview->moveToThread(cThread);
        //...
        connect(bgpview, &Bgpview::subdomain, this, &Osint::onResultSubdomain);
        connect(bgpview, &Bgpview::ip, this, &Osint::onResultIp);
        connect(bgpview, &Bgpview::ipA, this, &Osint::onResultA);
        connect(bgpview, &Bgpview::ipAAAA, this, &Osint::onResultAAAA);
        connect(bgpview, &Bgpview::email, this, &Osint::onResultEmail);
        connect(bgpview, &Bgpview::asn, this, &Osint::onResultAsn);
        connect(bgpview, &Bgpview::errorLog, this, &Osint::onErrorLog);
        connect(bgpview, &Bgpview::infoLog, this, &Osint::onInfoLog);
        connect(cThread, &QThread::finished, this, &Osint::onScanThreadEnded);
        connect(cThread, &QThread::finished, bgpview, &Bgpview::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        status->osint->activeThreads++;
    }
    if(module.binaryEdge){
        BinaryEdge *binaryedge = new BinaryEdge(scanArgs);
        QThread *cThread = new QThread(this);
        binaryedge->Enumerator(cThread);
        binaryedge->moveToThread(cThread);
        //...
        connect(binaryedge, &BinaryEdge::subdomain, this, &Osint::onResultSubdomain);
        connect(binaryedge, &BinaryEdge::subdomainIp, this, &Osint::onResultSubdomainIp);
        connect(binaryedge, &BinaryEdge::ipA, this, &Osint::onResultA);
        connect(binaryedge, &BinaryEdge::ipAAAA, this, &Osint::onResultAAAA);
        connect(binaryedge, &BinaryEdge::NS, this, &Osint::onResultNS);
        connect(binaryedge, &BinaryEdge::MX, this, &Osint::onResultMX);
        connect(binaryedge, &BinaryEdge::errorLog, this, &Osint::onErrorLog);
        connect(binaryedge, &BinaryEdge::infoLog, this, &Osint::onInfoLog);
        connect(cThread, &QThread::finished, this, &Osint::onScanThreadEnded);
        connect(cThread, &QThread::finished, binaryedge, &BinaryEdge::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        status->osint->activeThreads++;
    }
    if(module.c99){
        C99 *c99 = new C99(scanArgs);
        QThread *cThread = new QThread(this);
        c99->Enumerator(cThread);
        c99->moveToThread(cThread);
        //...
        connect(c99, &C99::subdomain, this, &Osint::onResultSubdomain);
        connect(c99, &C99::subdomainIp, this, &Osint::onResultSubdomainIp);
        connect(c99, &C99::ip, this, &Osint::onResultIp);
        connect(c99, &C99::email, this, &Osint::onResultEmail);
        connect(c99, &C99::url, this, &Osint::onResultUrl);
        connect(c99, &C99::errorLog, this, &Osint::onErrorLog);
        connect(c99, &C99::infoLog, this, &Osint::onInfoLog);
        connect(cThread, &QThread::finished, this, &Osint::onScanThreadEnded);
        connect(cThread, &QThread::finished, c99, &C99::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        status->osint->activeThreads++;
    }
    if(module.dnsbufferoverrun)
    {
        Dnsbufferoverun *dnsbufferoverun = new Dnsbufferoverun(scanArgs);
        QThread *cThread = new QThread(this);
        dnsbufferoverun->Enumerator(cThread);
        dnsbufferoverun->moveToThread(cThread);
        //...
        connect(dnsbufferoverun, &Dnsbufferoverun::subdomainIp, this, &Osint::onResultSubdomainIp);
        connect(dnsbufferoverun, &Dnsbufferoverun::subdomain, this, &Osint::onResultSubdomain);
        connect(dnsbufferoverun, &Dnsbufferoverun::ip, this, &Osint::onResultIp);
        connect(dnsbufferoverun, &Dnsbufferoverun::errorLog, this, &Osint::onErrorLog);
        connect(dnsbufferoverun, &Dnsbufferoverun::infoLog, this, &Osint::onInfoLog);
        connect(cThread, &QThread::finished, this, &Osint::onScanThreadEnded);
        connect(cThread, &QThread::finished, dnsbufferoverun, &Dnsbufferoverun::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        status->osint->activeThreads++;
    }
    if(module.github){
        Github *github = new Github(scanArgs);
        QThread *cThread = new QThread(this);
        github->Enumerator(cThread);
        github->moveToThread(cThread);
        //...
        connect(github, &Github::subdomain, this, &Osint::onResultSubdomain);
        connect(github, &Github::subdomainIp, this, &Osint::onResultSubdomainIp);
        connect(github, &Github::ip, this, &Osint::onResultIp);
        connect(github, &Github::email, this, &Osint::onResultEmail);
        connect(github, &Github::url, this, &Osint::onResultUrl);
        connect(github, &Github::errorLog, this, &Osint::onErrorLog);
        connect(github, &Github::infoLog, this, &Osint::onInfoLog);
        connect(cThread, &QThread::finished, this, &Osint::onScanThreadEnded);
        connect(cThread, &QThread::finished, github, &Github::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        status->osint->activeThreads++;
    }
    if(module.hackertargetfree)
    {
        HackerTargetFree *hackertarget = new HackerTargetFree(scanArgs);
        QThread *cThread = new QThread(this);
        hackertarget->Enumerator(cThread);
        hackertarget->moveToThread(cThread);
        //...
        connect(hackertarget, &HackerTargetFree::subdomain, this, &Osint::onResultSubdomain);
        connect(hackertarget, &HackerTargetFree::subdomainIp, this, &Osint::onResultSubdomainIp);
        connect(hackertarget, &HackerTargetFree::ip, this, &Osint::onResultIp);
        connect(hackertarget, &HackerTargetFree::asn, this, &Osint::onResultAsn);
        connect(hackertarget, &HackerTargetFree::errorLog, this, &Osint::onErrorLog);
        connect(hackertarget, &HackerTargetFree::infoLog, this, &Osint::onInfoLog);
        connect(cThread, &QThread::finished, this, &Osint::onScanThreadEnded);
        connect(cThread, &QThread::finished, hackertarget, &HackerTargetFree::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        status->osint->activeThreads++;
    }
    if(module.certspotter)
    {
        Certspotter *certspotter = new Certspotter(scanArgs);
        QThread *cThread = new QThread(this);
        certspotter->Enumerator(cThread);
        certspotter->moveToThread(cThread);
        //...
        connect(certspotter, &Certspotter::subdomain, this, &Osint::onResultSubdomain);
        connect(certspotter, &Certspotter::subdomainIp, this, &Osint::onResultSubdomainIp);
        connect(certspotter, &Certspotter::ip, this, &Osint::onResultIp);
        connect(certspotter, &Certspotter::email, this, &Osint::onResultEmail);
        connect(certspotter, &Certspotter::url, this, &Osint::onResultUrl);
        connect(certspotter, &Certspotter::errorLog, this, &Osint::onErrorLog);
        connect(certspotter, &Certspotter::infoLog, this, &Osint::onInfoLog);
        connect(cThread, &QThread::finished, this, &Osint::onScanThreadEnded);
        connect(cThread, &QThread::finished, certspotter, &Certspotter::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        status->osint->activeThreads++;
    }
    if(module.otx)
    {
        Otx *otx = new Otx(scanArgs);
        QThread *cThread = new QThread(this);
        otx->Enumerator(cThread);
        otx->moveToThread(cThread);
        //...
        connect(otx, &Otx::subdomain, this, &Osint::onResultSubdomain);
        connect(otx, &Otx::subdomainIp, this, &Osint::onResultSubdomainIp);
        connect(otx, &Otx::ip, this, &Osint::onResultIp);
        connect(otx, &Otx::email, this, &Osint::onResultEmail);
        connect(otx, &Otx::url, this, &Osint::onResultUrl);
        connect(otx, &Otx::errorLog, this, &Osint::onErrorLog);
        connect(otx, &Otx::infoLog, this, &Osint::onInfoLog);
        connect(cThread, &QThread::finished, this, &Osint::onScanThreadEnded);
        connect(cThread, &QThread::finished, otx, &Otx::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        status->osint->activeThreads++;
    }
    if(module.sublist3r)
    {
        Sublist3r *sublist3r = new Sublist3r(scanArgs);
        QThread *cThread = new QThread(this);
        sublist3r->Enumerator(cThread);
        sublist3r->moveToThread(cThread);
        //...
        connect(sublist3r, &Sublist3r::subdomain, this, &Osint::onResultSubdomain);
        connect(sublist3r, &Sublist3r::subdomainIp, this, &Osint::onResultSubdomainIp);
        connect(sublist3r, &Sublist3r::ip, this, &Osint::onResultIp);
        connect(sublist3r, &Sublist3r::email, this, &Osint::onResultEmail);
        connect(sublist3r, &Sublist3r::url, this, &Osint::onResultUrl);
        connect(sublist3r, &Sublist3r::errorLog, this, &Osint::onErrorLog);
        connect(sublist3r, &Sublist3r::infoLog, this, &Osint::onInfoLog);
        connect(cThread, &QThread::finished, this, &Osint::onScanThreadEnded);
        connect(cThread, &QThread::finished, sublist3r, &Sublist3r::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        status->osint->activeThreads++;
    }
    if(module.threatminer)
    {
        Threatminer *threatminer = new Threatminer(scanArgs);
        QThread *cThread = new QThread(this);
        threatminer->Enumerator(cThread);
        threatminer->moveToThread(cThread);
        //...
        connect(threatminer, &Threatminer::subdomain, this, &Osint::onResultSubdomain);
        connect(threatminer, &Threatminer::subdomainIp, this, &Osint::onResultSubdomainIp);
        connect(threatminer, &Threatminer::ip, this, &Osint::onResultIp);
        connect(threatminer, &Threatminer::email, this, &Osint::onResultEmail);
        connect(threatminer, &Threatminer::url, this, &Osint::onResultUrl);
        connect(threatminer, &Threatminer::errorLog, this, &Osint::onErrorLog);
        connect(threatminer, &Threatminer::infoLog, this, &Osint::onInfoLog);
        connect(cThread, &QThread::finished, this, &Osint::onScanThreadEnded);
        connect(cThread, &QThread::finished, threatminer, &Threatminer::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        status->osint->activeThreads++;
    }
    if(module.threatcrowd)
    {
        Threatcrowd *threatcrowd = new Threatcrowd(scanArgs);
        QThread *cThread = new QThread(this);
        threatcrowd->Enumerator(cThread);
        threatcrowd->moveToThread(cThread);
        //...
        connect(threatcrowd, &Threatcrowd::subdomain, this, &Osint::onResultSubdomain);
        connect(threatcrowd, &Threatcrowd::subdomainIp, this, &Osint::onResultSubdomainIp);
        connect(threatcrowd, &Threatcrowd::ip, this, &Osint::onResultIp);
        connect(threatcrowd, &Threatcrowd::email, this, &Osint::onResultEmail);
        connect(threatcrowd, &Threatcrowd::url, this, &Osint::onResultUrl);
        connect(threatcrowd, &Threatcrowd::errorLog, this, &Osint::onErrorLog);
        connect(threatcrowd, &Threatcrowd::infoLog, this, &Osint::onInfoLog);
        connect(cThread, &QThread::finished, this, &Osint::onScanThreadEnded);
        connect(cThread, &QThread::finished, threatcrowd, &Threatcrowd::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        status->osint->activeThreads++;
    }
    if(module.projectdiscovery)
    {
        Projectdiscovery *projectdiscovery = new Projectdiscovery(scanArgs);
        QThread *cThread = new QThread(this);
        projectdiscovery->Enumerator(cThread);
        projectdiscovery->moveToThread(cThread);
        //...
        connect(projectdiscovery, &Projectdiscovery::subdomain, this, &Osint::onResultSubdomain);
        connect(projectdiscovery, &Projectdiscovery::subdomainIp, this, &Osint::onResultSubdomainIp);
        connect(projectdiscovery, &Projectdiscovery::ip, this, &Osint::onResultIp);
        connect(projectdiscovery, &Projectdiscovery::email, this, &Osint::onResultEmail);
        connect(projectdiscovery, &Projectdiscovery::url, this, &Osint::onResultUrl);
        connect(projectdiscovery, &Projectdiscovery::errorLog, this, &Osint::onErrorLog);
        connect(projectdiscovery, &Projectdiscovery::infoLog, this, &Osint::onInfoLog);
        connect(cThread, &QThread::finished, this, &Osint::onScanThreadEnded);
        connect(cThread, &QThread::finished, projectdiscovery, &Projectdiscovery::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        status->osint->activeThreads++;
    }
    if(module.spyse)
    {
        Spyse *spyse = new Spyse(scanArgs);
        QThread *cThread = new QThread(this);
        spyse->Enumerator(cThread);
        spyse->moveToThread(cThread);
        //...
        connect(spyse, &Spyse::subdomain, this, &Osint::onResultSubdomain);
        connect(spyse, &Spyse::subdomainIp, this, &Osint::onResultSubdomainIp);
        connect(spyse, &Spyse::ip, this, &Osint::onResultIp);
        connect(spyse, &Spyse::email, this, &Osint::onResultEmail);
        connect(spyse, &Spyse::url, this, &Osint::onResultUrl);
        connect(spyse, &Spyse::errorLog, this, &Osint::onErrorLog);
        connect(spyse, &Spyse::infoLog, this, &Osint::onInfoLog);
        connect(cThread, &QThread::finished, this, &Osint::onScanThreadEnded);
        connect(cThread, &QThread::finished, spyse, &Spyse::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        status->osint->activeThreads++;
    }
    if(module.crtsh)
    {
        Crtsh *crtsh = new Crtsh(scanArgs);
        QThread *cThread = new QThread(this);
        crtsh->Enumerator(cThread);
        crtsh->moveToThread(cThread);
        //...
        connect(crtsh, &Crtsh::subdomain, this, &Osint::onResultSubdomain);
        connect(crtsh, &Crtsh::subdomainIp, this, &Osint::onResultSubdomainIp);
        connect(crtsh, &Crtsh::ip, this, &Osint::onResultIp);
        connect(crtsh, &Crtsh::email, this, &Osint::onResultEmail);
        connect(crtsh, &Crtsh::url, this, &Osint::onResultUrl);
        connect(crtsh, &Crtsh::errorLog, this, &Osint::onErrorLog);
        connect(crtsh, &Crtsh::infoLog, this, &Osint::onInfoLog);
        connect(cThread, &QThread::finished, this, &Osint::onScanThreadEnded);
        connect(cThread, &QThread::finished, crtsh, &Crtsh::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        status->osint->activeThreads++;
    }
    if(module.dnsdumpster)
    {
        Dnsdumpster *dnsdumpster = new Dnsdumpster(scanArgs);
        QThread *cThread = new QThread(this);
        dnsdumpster->Enumerator(cThread);
        dnsdumpster->moveToThread(cThread);
        //...
        connect(dnsdumpster, &Dnsdumpster::subdomain, this, &Osint::onResultSubdomain);
        connect(dnsdumpster, &Dnsdumpster::subdomainIp, this, &Osint::onResultSubdomainIp);
        connect(dnsdumpster, &Dnsdumpster::ip, this, &Osint::onResultIp);
        connect(dnsdumpster, &Dnsdumpster::email, this, &Osint::onResultEmail);
        connect(dnsdumpster, &Dnsdumpster::url, this, &Osint::onResultUrl);
        connect(dnsdumpster, &Dnsdumpster::errorLog, this, &Osint::onErrorLog);
        connect(dnsdumpster, &Dnsdumpster::infoLog, this, &Osint::onInfoLog);
        connect(cThread, &QThread::finished, this, &Osint::onScanThreadEnded);
        connect(cThread, &QThread::finished, dnsdumpster, &Dnsdumpster::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        status->osint->activeThreads++;
    }
    if(module.netcraft)
    {
        Netcraft *netcraft = new Netcraft(scanArgs);
        QThread *cThread = new QThread(this);
        netcraft->Enumerator(cThread);
        netcraft->moveToThread(cThread);
        //...
        connect(netcraft, &Netcraft::subdomain, this, &Osint::onResultSubdomain);
        connect(netcraft, &Netcraft::subdomainIp, this, &Osint::onResultSubdomainIp);
        connect(netcraft, &Netcraft::ip, this, &Osint::onResultIp);
        connect(netcraft, &Netcraft::email, this, &Osint::onResultEmail);
        connect(netcraft, &Netcraft::url, this, &Osint::onResultUrl);
        connect(netcraft, &Netcraft::errorLog, this, &Osint::onErrorLog);
        connect(netcraft, &Netcraft::infoLog, this, &Osint::onInfoLog);
        connect(cThread, &QThread::finished, this, &Osint::onScanThreadEnded);
        connect(cThread, &QThread::finished, netcraft, &Netcraft::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        status->osint->activeThreads++;
    }
    if(module.suip)
    {
        Suip *suip = new Suip(scanArgs);
        QThread *cThread = new QThread(this);
        suip->Enumerator(cThread);
        suip->moveToThread(cThread);
        //...
        connect(suip, &Suip::subdomain, this, &Osint::onResultSubdomain);
        connect(suip, &Suip::subdomainIp, this, &Osint::onResultSubdomainIp);
        connect(suip, &Suip::ip, this, &Osint::onResultIp);
        connect(suip, &Suip::email, this, &Osint::onResultEmail);
        connect(suip, &Suip::url, this, &Osint::onResultUrl);
        connect(suip, &Suip::errorLog, this, &Osint::onErrorLog);
        connect(suip, &Suip::infoLog, this, &Osint::onInfoLog);
        connect(cThread, &QThread::finished, this, &Osint::onScanThreadEnded);
        connect(cThread, &QThread::finished, suip, &Suip::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        status->osint->activeThreads++;
    }
    if(module.pkey){
        Pkey *pkey = new Pkey(scanArgs);
        QThread *cThread = new QThread(this);
        pkey->Enumerator(cThread);
        pkey->moveToThread(cThread);
        //...
        connect(pkey, &Pkey::subdomain, this, &Osint::onResultSubdomain);
        connect(pkey, &Pkey::subdomainIp, this, &Osint::onResultSubdomainIp);
        connect(pkey, &Pkey::ip, this, &Osint::onResultIp);
        connect(pkey, &Pkey::email, this, &Osint::onResultEmail);
        connect(pkey, &Pkey::url, this, &Osint::onResultUrl);
        connect(pkey, &Pkey::errorLog, this, &Osint::onErrorLog);
        connect(pkey, &Pkey::infoLog, this, &Osint::onInfoLog);
        connect(cThread, &QThread::finished, this, &Osint::onScanThreadEnded);
        connect(cThread, &QThread::finished, pkey, &Pkey::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        status->osint->activeThreads++;
    }
    if(module.rapiddns){
        Rapiddns *rapiddns = new Rapiddns(scanArgs);
        QThread *cThread = new QThread(this);
        rapiddns->Enumerator(cThread);
        rapiddns->moveToThread(cThread);
        //...
        connect(rapiddns, &Rapiddns::subdomain, this, &Osint::onResultSubdomain);
        connect(rapiddns, &Rapiddns::subdomainIp, this, &Osint::onResultSubdomainIp);
        connect(rapiddns, &Rapiddns::ip, this, &Osint::onResultIp);
        connect(rapiddns, &Rapiddns::email, this, &Osint::onResultEmail);
        connect(rapiddns, &Rapiddns::url, this, &Osint::onResultUrl);
        connect(rapiddns, &Rapiddns::errorLog, this, &Osint::onErrorLog);
        connect(rapiddns, &Rapiddns::infoLog, this, &Osint::onInfoLog);
        connect(cThread, &QThread::finished, this, &Osint::onScanThreadEnded);
        connect(cThread, &QThread::finished, rapiddns, &Rapiddns::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        status->osint->activeThreads++;
    }
    if(module.googleCert){
        GoogleCert *googlecert = new GoogleCert(scanArgs);
        QThread *cThread = new QThread(this);
        googlecert->Enumerator(cThread);
        googlecert->moveToThread(cThread);
        //...
        connect(googlecert, &GoogleCert::subdomain, this, &Osint::onResultSubdomain);
        connect(googlecert, &GoogleCert::subdomainIp, this, &Osint::onResultSubdomainIp);
        connect(googlecert, &GoogleCert::ip, this, &Osint::onResultIp);
        connect(googlecert, &GoogleCert::email, this, &Osint::onResultEmail);
        connect(googlecert, &GoogleCert::url, this, &Osint::onResultUrl);
        connect(googlecert, &GoogleCert::errorLog, this, &Osint::onErrorLog);
        connect(googlecert, &GoogleCert::infoLog, this, &Osint::onInfoLog);
        connect(cThread, &QThread::finished, this, &Osint::onScanThreadEnded);
        connect(cThread, &QThread::finished, googlecert, &GoogleCert::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        status->osint->activeThreads++;
    }
    if(module.omnisint){
        Omnisint *omnisint = new Omnisint(scanArgs);
        QThread *cThread = new QThread(this);
        omnisint->Enumerator(cThread);
        omnisint->moveToThread(cThread);
        //...
        connect(omnisint, &Omnisint::subdomain, this, &Osint::onResultSubdomain);
        connect(omnisint, &Omnisint::subdomainIp, this, &Osint::onResultSubdomainIp);
        connect(omnisint, &Omnisint::ip, this, &Osint::onResultIp);
        connect(omnisint, &Omnisint::email, this, &Osint::onResultEmail);
        connect(omnisint, &Omnisint::url, this, &Osint::onResultUrl);
        connect(omnisint, &Omnisint::errorLog, this, &Osint::onErrorLog);
        connect(omnisint, &Omnisint::infoLog, this, &Osint::onInfoLog);
        connect(cThread, &QThread::finished, this, &Osint::onScanThreadEnded);
        connect(cThread, &QThread::finished, omnisint, &Omnisint::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        status->osint->activeThreads++;
    }
    if(module.qwant){
        Qwant *qwant = new Qwant(scanArgs);
        QThread *cThread = new QThread(this);
        qwant->Enumerator(cThread);
        qwant->moveToThread(cThread);
        //...
        connect(qwant, &Qwant::subdomain, this, &Osint::onResultSubdomain);
        connect(qwant, &Qwant::subdomainIp, this, &Osint::onResultSubdomainIp);
        connect(qwant, &Qwant::ip, this, &Osint::onResultIp);
        connect(qwant, &Qwant::email, this, &Osint::onResultEmail);
        connect(qwant, &Qwant::url, this, &Osint::onResultUrl);
        connect(qwant, &Qwant::errorLog, this, &Osint::onErrorLog);
        connect(qwant, &Qwant::infoLog, this, &Osint::onInfoLog);
        connect(cThread, &QThread::finished, this, &Osint::onScanThreadEnded);
        connect(cThread, &QThread::finished, qwant, &Qwant::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        status->osint->activeThreads++;
    }
    if(module.virustotalapi){
        VirusTotal *virustotal = new VirusTotal(scanArgs);
        QThread *cThread = new QThread(this);
        virustotal->Enumerator(cThread);
        virustotal->moveToThread(cThread);
        //...
        connect(virustotal, &VirusTotal::subdomain, this, &Osint::onResultSubdomain);
        connect(virustotal, &VirusTotal::subdomainIp, this, &Osint::onResultSubdomainIp);
        connect(virustotal, &VirusTotal::ip, this, &Osint::onResultIp);
        connect(virustotal, &VirusTotal::email, this, &Osint::onResultEmail);
        connect(virustotal, &VirusTotal::url, this, &Osint::onResultUrl);
        connect(virustotal, &VirusTotal::errorLog, this, &Osint::onErrorLog);
        connect(virustotal, &VirusTotal::infoLog, this, &Osint::onInfoLog);
        connect(cThread, &QThread::finished, this, &Osint::onScanThreadEnded);
        connect(cThread, &QThread::finished, virustotal, &VirusTotal::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        status->osint->activeThreads++;
    }
    if(module.urlscan){
        Urlscan *urlscan = new Urlscan(scanArgs);
        QThread *cThread = new QThread(this);
        urlscan->Enumerator(cThread);
        urlscan->moveToThread(cThread);
        //...
        connect(urlscan, &Urlscan::subdomain, this, &Osint::onResultSubdomain);
        connect(urlscan, &Urlscan::subdomainIp, this, &Osint::onResultSubdomainIp);
        connect(urlscan, &Urlscan::ip, this, &Osint::onResultIp);
        connect(urlscan, &Urlscan::email, this, &Osint::onResultEmail);
        connect(urlscan, &Urlscan::url, this, &Osint::onResultUrl);
        connect(urlscan, &Urlscan::errorLog, this, &Osint::onErrorLog);
        connect(urlscan, &Urlscan::infoLog, this, &Osint::onInfoLog);
        connect(cThread, &QThread::finished, this, &Osint::onScanThreadEnded);
        connect(cThread, &QThread::finished, urlscan, &Urlscan::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        status->osint->activeThreads++;
    }
    if(module.waybackmachine){
        Waybackmachine *waybackmachine = new Waybackmachine(scanArgs);
        QThread *cThread = new QThread(this);
        waybackmachine->Enumerator(cThread);
        waybackmachine->moveToThread(cThread);
        //...
        connect(waybackmachine, &Waybackmachine::subdomain, this, &Osint::onResultSubdomain);
        connect(waybackmachine, &Waybackmachine::subdomainIp, this, &Osint::onResultSubdomainIp);
        connect(waybackmachine, &Waybackmachine::ip, this, &Osint::onResultIp);
        connect(waybackmachine, &Waybackmachine::email, this, &Osint::onResultEmail);
        connect(waybackmachine, &Waybackmachine::url, this, &Osint::onResultUrl);
        connect(waybackmachine, &Waybackmachine::errorLog, this, &Osint::onErrorLog);
        connect(waybackmachine, &Waybackmachine::infoLog, this, &Osint::onInfoLog);
        connect(cThread, &QThread::finished, this, &Osint::onScanThreadEnded);
        connect(cThread, &QThread::finished, waybackmachine, &Waybackmachine::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        status->osint->activeThreads++;
    }
    if(module.archivetoday){
        ArchiveToday *archivetoday = new ArchiveToday(scanArgs);
        QThread *cThread = new QThread(this);
        archivetoday->Enumerator(cThread);
        archivetoday->moveToThread(cThread);
        //...
        connect(archivetoday, &ArchiveToday::subdomain, this, &Osint::onResultSubdomain);
        connect(archivetoday, &ArchiveToday::subdomainIp, this, &Osint::onResultSubdomainIp);
        connect(archivetoday, &ArchiveToday::ip, this, &Osint::onResultIp);
        connect(archivetoday, &ArchiveToday::email, this, &Osint::onResultEmail);
        connect(archivetoday, &ArchiveToday::url, this, &Osint::onResultUrl);
        connect(archivetoday, &ArchiveToday::errorLog, this, &Osint::onErrorLog);
        connect(archivetoday, &ArchiveToday::infoLog, this, &Osint::onInfoLog);
        connect(cThread, &QThread::finished, this, &Osint::onScanThreadEnded);
        connect(cThread, &QThread::finished, archivetoday, &ArchiveToday::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        status->osint->activeThreads++;
    }
    if(module.archiveit){
        ArchiveIt *archiveit = new ArchiveIt(scanArgs);
        QThread *cThread = new QThread(this);
        archiveit->Enumerator(cThread);
        archiveit->moveToThread(cThread);
        //...
        connect(archiveit, &ArchiveIt::subdomain, this, &Osint::onResultSubdomain);
        connect(archiveit, &ArchiveIt::subdomainIp, this, &Osint::onResultSubdomainIp);
        connect(archiveit, &ArchiveIt::ip, this, &Osint::onResultIp);
        connect(archiveit, &ArchiveIt::email, this, &Osint::onResultEmail);
        connect(archiveit, &ArchiveIt::url, this, &Osint::onResultUrl);
        connect(archiveit, &ArchiveIt::errorLog, this, &Osint::onErrorLog);
        connect(archiveit, &ArchiveIt::infoLog, this, &Osint::onInfoLog);
        connect(cThread, &QThread::finished, this, &Osint::onScanThreadEnded);
        connect(cThread, &QThread::finished, archiveit, &ArchiveIt::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        status->osint->activeThreads++;
    }
    if(module.censysFree){
        CensysFree *censysfree = new CensysFree(scanArgs);
        QThread *cThread = new QThread(this);
        censysfree->Enumerator(cThread);
        censysfree->moveToThread(cThread);
        //...
        connect(censysfree, &CensysFree::subdomain, this, &Osint::onResultSubdomain);
        connect(censysfree, &CensysFree::subdomainIp, this, &Osint::onResultSubdomainIp);
        connect(censysfree, &CensysFree::ip, this, &Osint::onResultIp);
        connect(censysfree, &CensysFree::email, this, &Osint::onResultEmail);
        connect(censysfree, &CensysFree::url, this, &Osint::onResultUrl);
        connect(censysfree, &CensysFree::errorLog, this, &Osint::onErrorLog);
        connect(censysfree, &CensysFree::infoLog, this, &Osint::onInfoLog);
        connect(cThread, &QThread::finished, this, &Osint::onScanThreadEnded);
        connect(cThread, &QThread::finished, censysfree, &CensysFree::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        status->osint->activeThreads++;
    }
    if(module.commoncrawl){
        CommonCrawl *commonCrawl = new CommonCrawl(scanArgs);
        QThread *cThread = new QThread(this);
        commonCrawl->Enumerator(cThread);
        commonCrawl->moveToThread(cThread);
        //...
        connect(commonCrawl, &CommonCrawl::subdomain, this, &Osint::onResultSubdomain);
        connect(commonCrawl, &CommonCrawl::subdomainIp, this, &Osint::onResultSubdomainIp);
        connect(commonCrawl, &CommonCrawl::ip, this, &Osint::onResultIp);
        connect(commonCrawl, &CommonCrawl::email, this, &Osint::onResultEmail);
        connect(commonCrawl, &CommonCrawl::url, this, &Osint::onResultUrl);
        connect(commonCrawl, &CommonCrawl::errorLog, this, &Osint::onErrorLog);
        connect(commonCrawl, &CommonCrawl::infoLog, this, &Osint::onInfoLog);
        connect(cThread, &QThread::finished, this, &Osint::onScanThreadEnded);
        connect(cThread, &QThread::finished, commonCrawl, &CommonCrawl::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        status->osint->activeThreads++;
    }
    if(module.github){
        Github *github = new Github(scanArgs);
        QThread *cThread = new QThread(this);
        github->Enumerator(cThread);
        github->moveToThread(cThread);
        //...
        connect(github, &Github::subdomain, this, &Osint::onResultSubdomain);
        connect(github, &Github::subdomainIp, this, &Osint::onResultSubdomainIp);
        connect(github, &Github::ip, this, &Osint::onResultIp);
        connect(github, &Github::email, this, &Osint::onResultEmail);
        connect(github, &Github::url, this, &Osint::onResultUrl);
        connect(github, &Github::errorLog, this, &Osint::onErrorLog);
        connect(github, &Github::infoLog, this, &Osint::onInfoLog);
        connect(cThread, &QThread::finished, this, &Osint::onScanThreadEnded);
        connect(cThread, &QThread::finished, github, &Github::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        status->osint->activeThreads++;
    }
    if(module.huntersearch){
        HunterSearch *huntersearch = new HunterSearch(scanArgs);
        QThread *cThread = new QThread(this);
        huntersearch->Enumerator(cThread);
        huntersearch->moveToThread(cThread);
        //...
        connect(huntersearch, &HunterSearch::subdomain, this, &Osint::onResultSubdomain);
        connect(huntersearch, &HunterSearch::subdomainIp, this, &Osint::onResultSubdomainIp);
        connect(huntersearch, &HunterSearch::ip, this, &Osint::onResultIp);
        connect(huntersearch, &HunterSearch::email, this, &Osint::onResultEmail);
        connect(huntersearch, &HunterSearch::url, this, &Osint::onResultUrl);
        connect(huntersearch, &HunterSearch::errorLog, this, &Osint::onErrorLog);
        connect(huntersearch, &HunterSearch::infoLog, this, &Osint::onInfoLog);
        connect(cThread, &QThread::finished, this, &Osint::onScanThreadEnded);
        connect(cThread, &QThread::finished, huntersearch, &HunterSearch::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        status->osint->activeThreads++;
    }
    if(module.ipinfo){
        IpInfo *ipinfo = new IpInfo(scanArgs);
        QThread *cThread = new QThread(this);
        ipinfo->Enumerator(cThread);
        ipinfo->moveToThread(cThread);
        //...
        connect(ipinfo, &IpInfo::subdomain, this, &Osint::onResultSubdomain);
        connect(ipinfo, &IpInfo::subdomainIp, this, &Osint::onResultSubdomainIp);
        connect(ipinfo, &IpInfo::ip, this, &Osint::onResultIp);
        connect(ipinfo, &IpInfo::email, this, &Osint::onResultEmail);
        connect(ipinfo, &IpInfo::url, this, &Osint::onResultUrl);
        connect(ipinfo, &IpInfo::errorLog, this, &Osint::onErrorLog);
        connect(ipinfo, &IpInfo::infoLog, this, &Osint::onInfoLog);
        connect(cThread, &QThread::finished, this, &Osint::onScanThreadEnded);
        connect(cThread, &QThread::finished, ipinfo, &IpInfo::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        status->osint->activeThreads++;
    }
    if(module.mnemonic){
        Mnemonic *mnemonic = new Mnemonic(scanArgs);
        QThread *cThread = new QThread(this);
        mnemonic->Enumerator(cThread);
        mnemonic->moveToThread(cThread);
        //...
        connect(mnemonic, &Mnemonic::subdomain, this, &Osint::onResultSubdomain);
        connect(mnemonic, &Mnemonic::subdomainIp, this, &Osint::onResultSubdomainIp);
        connect(mnemonic, &Mnemonic::ip, this, &Osint::onResultIp);
        connect(mnemonic, &Mnemonic::email, this, &Osint::onResultEmail);
        connect(mnemonic, &Mnemonic::url, this, &Osint::onResultUrl);
        connect(mnemonic, &Mnemonic::errorLog, this, &Osint::onErrorLog);
        connect(mnemonic, &Mnemonic::infoLog, this, &Osint::onInfoLog);
        connect(cThread, &QThread::finished, this, &Osint::onScanThreadEnded);
        connect(cThread, &QThread::finished, mnemonic, &Mnemonic::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        status->osint->activeThreads++;
    }
    if(module.riskiq){
        RiskIq *riskiq = new RiskIq(scanArgs);
        QThread *cThread = new QThread(this);
        riskiq->Enumerator(cThread);
        riskiq->moveToThread(cThread);
        //...
        connect(riskiq, &RiskIq::subdomain, this, &Osint::onResultSubdomain);
        connect(riskiq, &RiskIq::subdomainIp, this, &Osint::onResultSubdomainIp);
        connect(riskiq, &RiskIq::ip, this, &Osint::onResultIp);
        connect(riskiq, &RiskIq::email, this, &Osint::onResultEmail);
        connect(riskiq, &RiskIq::url, this, &Osint::onResultUrl);
        connect(riskiq, &RiskIq::errorLog, this, &Osint::onErrorLog);
        connect(riskiq, &RiskIq::infoLog, this, &Osint::onInfoLog);
        connect(cThread, &QThread::finished, this, &Osint::onScanThreadEnded);
        connect(cThread, &QThread::finished, riskiq, &RiskIq::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        status->osint->activeThreads++;
    }
    if(module.robtex){
        Robtex *robtex = new Robtex(scanArgs);
        QThread *cThread = new QThread(this);
        robtex->Enumerator(cThread);
        robtex->moveToThread(cThread);
        //...
        connect(robtex, &Robtex::subdomain, this, &Osint::onResultSubdomain);
        connect(robtex, &Robtex::subdomainIp, this, &Osint::onResultSubdomainIp);
        connect(robtex, &Robtex::ip, this, &Osint::onResultIp);
        connect(robtex, &Robtex::email, this, &Osint::onResultEmail);
        connect(robtex, &Robtex::url, this, &Osint::onResultUrl);
        connect(robtex, &Robtex::errorLog, this, &Osint::onErrorLog);
        connect(robtex, &Robtex::infoLog, this, &Osint::onInfoLog);
        connect(cThread, &QThread::finished, this, &Osint::onScanThreadEnded);
        connect(cThread, &QThread::finished, robtex, &Robtex::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        status->osint->activeThreads++;
    }
    if(module.securitytrails){
        SecurityTrails *securitytrails = new SecurityTrails(scanArgs);
        QThread *cThread = new QThread(this);
        securitytrails->Enumerator(cThread);
        securitytrails->moveToThread(cThread);
        //...
        connect(securitytrails, &SecurityTrails::subdomain, this, &Osint::onResultSubdomain);
        connect(securitytrails, &SecurityTrails::subdomainIp, this, &Osint::onResultSubdomainIp);
        connect(securitytrails, &SecurityTrails::ip, this, &Osint::onResultIp);
        connect(securitytrails, &SecurityTrails::email, this, &Osint::onResultEmail);
        connect(securitytrails, &SecurityTrails::url, this, &Osint::onResultUrl);
        connect(securitytrails, &SecurityTrails::errorLog, this, &Osint::onErrorLog);
        connect(securitytrails, &SecurityTrails::infoLog, this, &Osint::onInfoLog);
        connect(cThread, &QThread::finished, this, &Osint::onScanThreadEnded);
        connect(cThread, &QThread::finished, securitytrails, &SecurityTrails::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        status->osint->activeThreads++;
    }
    if(module.shodan){
        Shodan *shodan = new Shodan(scanArgs);
        QThread *cThread = new QThread(this);
        shodan->Enumerator(cThread);
        shodan->moveToThread(cThread);
        //...
        connect(shodan, &Shodan::subdomain, this, &Osint::onResultSubdomain);
        connect(shodan, &Shodan::subdomainIp, this, &Osint::onResultSubdomainIp);
        connect(shodan, &Shodan::ip, this, &Osint::onResultIp);
        connect(shodan, &Shodan::email, this, &Osint::onResultEmail);
        connect(shodan, &Shodan::url, this, &Osint::onResultUrl);
        connect(shodan, &Shodan::errorLog, this, &Osint::onErrorLog);
        connect(shodan, &Shodan::infoLog, this, &Osint::onInfoLog);
        connect(cThread, &QThread::finished, this, &Osint::onScanThreadEnded);
        connect(cThread, &QThread::finished, shodan, &Shodan::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        status->osint->activeThreads++;
    }
    if(module.threatbook){
        ThreatBook *threatbook = new ThreatBook(scanArgs);
        QThread *cThread = new QThread(this);
        threatbook->Enumerator(cThread);
        threatbook->moveToThread(cThread);
        //...
        connect(threatbook, &ThreatBook::subdomain, this, &Osint::onResultSubdomain);
        connect(threatbook, &ThreatBook::subdomainIp, this, &Osint::onResultSubdomainIp);
        connect(threatbook, &ThreatBook::ip, this, &Osint::onResultIp);
        connect(threatbook, &ThreatBook::email, this, &Osint::onResultEmail);
        connect(threatbook, &ThreatBook::url, this, &Osint::onResultUrl);
        connect(threatbook, &ThreatBook::errorLog, this, &Osint::onErrorLog);
        connect(threatbook, &ThreatBook::infoLog, this, &Osint::onInfoLog);
        connect(cThread, &QThread::finished, this, &Osint::onScanThreadEnded);
        connect(cThread, &QThread::finished, threatbook, &ThreatBook::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        status->osint->activeThreads++;
    }
    if(module.whoisxmlapi){
        WhoisXmlApi *whoisxmlapi = new WhoisXmlApi(scanArgs);
        QThread *cThread = new QThread(this);
        whoisxmlapi->Enumerator(cThread);
        whoisxmlapi->moveToThread(cThread);
        //...
        connect(whoisxmlapi, &WhoisXmlApi::subdomain, this, &Osint::onResultSubdomain);
        connect(whoisxmlapi, &WhoisXmlApi::subdomainIp, this, &Osint::onResultSubdomainIp);
        connect(whoisxmlapi, &WhoisXmlApi::ip, this, &Osint::onResultIp);
        connect(whoisxmlapi, &WhoisXmlApi::email, this, &Osint::onResultEmail);
        connect(whoisxmlapi, &WhoisXmlApi::url, this, &Osint::onResultUrl);
        connect(whoisxmlapi, &WhoisXmlApi::errorLog, this, &Osint::onErrorLog);
        connect(whoisxmlapi, &WhoisXmlApi::infoLog, this, &Osint::onInfoLog);
        connect(cThread, &QThread::finished, this, &Osint::onScanThreadEnded);
        connect(cThread, &QThread::finished, whoisxmlapi, &WhoisXmlApi::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        status->osint->activeThreads++;
    }
    if(module.zetalytics){
        ZETAlytics *zetalytics = new ZETAlytics(scanArgs);
        QThread *cThread = new QThread(this);
        zetalytics->Enumerator(cThread);
        zetalytics->moveToThread(cThread);
        //...
        connect(zetalytics, &ZETAlytics::subdomain, this, &Osint::onResultSubdomain);
        connect(zetalytics, &ZETAlytics::subdomainIp, this, &Osint::onResultSubdomainIp);
        connect(zetalytics, &ZETAlytics::ip, this, &Osint::onResultIp);
        connect(zetalytics, &ZETAlytics::email, this, &Osint::onResultEmail);
        connect(zetalytics, &ZETAlytics::url, this, &Osint::onResultUrl);
        connect(zetalytics, &ZETAlytics::errorLog, this, &Osint::onErrorLog);
        connect(zetalytics, &ZETAlytics::infoLog, this, &Osint::onInfoLog);
        connect(cThread, &QThread::finished, this, &Osint::onScanThreadEnded);
        connect(cThread, &QThread::finished, zetalytics, &ZETAlytics::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        status->osint->activeThreads++;
    }
    if(module.zoomeye){
        ZoomEye *zoomeye = new ZoomEye(scanArgs);
        QThread *cThread = new QThread(this);
        zoomeye->Enumerator(cThread);
        zoomeye->moveToThread(cThread);
        //...
        connect(zoomeye, &ZoomEye::subdomain, this, &Osint::onResultSubdomain);
        connect(zoomeye, &ZoomEye::subdomainIp, this, &Osint::onResultSubdomainIp);
        connect(zoomeye, &ZoomEye::ip, this, &Osint::onResultIp);
        connect(zoomeye, &ZoomEye::email, this, &Osint::onResultEmail);
        connect(zoomeye, &ZoomEye::url, this, &Osint::onResultUrl);
        connect(zoomeye, &ZoomEye::errorLog, this, &Osint::onErrorLog);
        connect(zoomeye, &ZoomEye::infoLog, this, &Osint::onInfoLog);
        connect(cThread, &QThread::finished, this, &Osint::onScanThreadEnded);
        connect(cThread, &QThread::finished, zoomeye, &ZoomEye::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        status->osint->activeThreads++;
    }
    if(module.ipapi){
        IpApi *ipapi = new IpApi(scanArgs);
        QThread *cThread = new QThread(this);
        ipapi->Enumerator(cThread);
        ipapi->moveToThread(cThread);
        //...
        connect(ipapi, &IpApi::subdomain, this, &Osint::onResultSubdomain);
        connect(ipapi, &IpApi::subdomainIp, this, &Osint::onResultSubdomainIp);
        connect(ipapi, &IpApi::ip, this, &Osint::onResultIp);
        connect(ipapi, &IpApi::email, this, &Osint::onResultEmail);
        connect(ipapi, &IpApi::url, this, &Osint::onResultUrl);
        connect(ipapi, &IpApi::errorLog, this, &Osint::onErrorLog);
        connect(ipapi, &IpApi::infoLog, this, &Osint::onInfoLog);
        connect(cThread, &QThread::finished, this, &Osint::onScanThreadEnded);
        connect(cThread, &QThread::finished, ipapi, &IpApi::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        status->osint->activeThreads++;
    }
    if(module.ask){
        Ask *ask = new Ask(scanArgs);
        QThread *cThread = new QThread(this);
        ask->Enumerator(cThread);
        ask->moveToThread(cThread);
        //...
        connect(ask, &Ask::subdomain, this, &Osint::onResultSubdomain);
        connect(ask, &Ask::subdomainIp, this, &Osint::onResultSubdomainIp);
        connect(ask, &Ask::ip, this, &Osint::onResultIp);
        connect(ask, &Ask::email, this, &Osint::onResultEmail);
        connect(ask, &Ask::url, this, &Osint::onResultUrl);
        connect(ask, &Ask::errorLog, this, &Osint::onErrorLog);
        connect(ask, &Ask::infoLog, this, &Osint::onInfoLog);
        connect(cThread, &QThread::finished, this, &Osint::onScanThreadEnded);
        connect(cThread, &QThread::finished, ask, &Ask::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        status->osint->activeThreads++;
    }
    if(module.baidu){
        Baidu *baidu = new Baidu(scanArgs);
        QThread *cThread = new QThread(this);
        baidu->Enumerator(cThread);
        baidu->moveToThread(cThread);
        //...
        connect(baidu, &Baidu::subdomain, this, &Osint::onResultSubdomain);
        connect(baidu, &Baidu::subdomainIp, this, &Osint::onResultSubdomainIp);
        connect(baidu, &Baidu::ip, this, &Osint::onResultIp);
        connect(baidu, &Baidu::email, this, &Osint::onResultEmail);
        connect(baidu, &Baidu::url, this, &Osint::onResultUrl);
        connect(baidu, &Baidu::errorLog, this, &Osint::onErrorLog);
        connect(baidu, &Baidu::infoLog, this, &Osint::onInfoLog);
        connect(cThread, &QThread::finished, this, &Osint::onScanThreadEnded);
        connect(cThread, &QThread::finished, baidu, &Baidu::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        status->osint->activeThreads++;
    }
    if(module.dogpile){
        DogPile *dogpile = new DogPile(scanArgs);
        QThread *cThread = new QThread(this);
        dogpile->Enumerator(cThread);
        dogpile->moveToThread(cThread);
        //...
        connect(dogpile, &DogPile::subdomain, this, &Osint::onResultSubdomain);
        connect(dogpile, &DogPile::subdomainIp, this, &Osint::onResultSubdomainIp);
        connect(dogpile, &DogPile::ip, this, &Osint::onResultIp);
        connect(dogpile, &DogPile::email, this, &Osint::onResultEmail);
        connect(dogpile, &DogPile::url, this, &Osint::onResultUrl);
        connect(dogpile, &DogPile::errorLog, this, &Osint::onErrorLog);
        connect(dogpile, &DogPile::infoLog, this, &Osint::onInfoLog);
        connect(cThread, &QThread::finished, this, &Osint::onScanThreadEnded);
        connect(cThread, &QThread::finished, dogpile, &DogPile::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        status->osint->activeThreads++;
    }
    if(module.duckduckgo){
        DuckDuckGo *duckduckgo = new DuckDuckGo(scanArgs);
        QThread *cThread = new QThread(this);
        duckduckgo->Enumerator(cThread);
        duckduckgo->moveToThread(cThread);
        //...
        connect(duckduckgo, &DuckDuckGo::subdomain, this, &Osint::onResultSubdomain);
        connect(duckduckgo, &DuckDuckGo::subdomainIp, this, &Osint::onResultSubdomainIp);
        connect(duckduckgo, &DuckDuckGo::ip, this, &Osint::onResultIp);
        connect(duckduckgo, &DuckDuckGo::email, this, &Osint::onResultEmail);
        connect(duckduckgo, &DuckDuckGo::url, this, &Osint::onResultUrl);
        connect(duckduckgo, &DuckDuckGo::errorLog, this, &Osint::onErrorLog);
        connect(duckduckgo, &DuckDuckGo::infoLog, this, &Osint::onInfoLog);
        connect(cThread, &QThread::finished, this, &Osint::onScanThreadEnded);
        connect(cThread, &QThread::finished, duckduckgo, &DuckDuckGo::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        status->osint->activeThreads++;
    }
    if(module.exalead){
        Exalead *exalead = new Exalead(scanArgs);
        QThread *cThread = new QThread(this);
        exalead->Enumerator(cThread);
        exalead->moveToThread(cThread);
        //...
        connect(exalead, &Exalead::subdomain, this, &Osint::onResultSubdomain);
        connect(exalead, &Exalead::subdomainIp, this, &Osint::onResultSubdomainIp);
        connect(exalead, &Exalead::ip, this, &Osint::onResultIp);
        connect(exalead, &Exalead::email, this, &Osint::onResultEmail);
        connect(exalead, &Exalead::url, this, &Osint::onResultUrl);
        connect(exalead, &Exalead::errorLog, this, &Osint::onErrorLog);
        connect(exalead, &Exalead::infoLog, this, &Osint::onInfoLog);
        connect(cThread, &QThread::finished, this, &Osint::onScanThreadEnded);
        connect(cThread, &QThread::finished, exalead, &Exalead::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        status->osint->activeThreads++;
    }
    if(module.trello){
        Trello *trello = new Trello(scanArgs);
        QThread *cThread = new QThread(this);
        trello->Enumerator(cThread);
        trello->moveToThread(cThread);
        //...
        connect(trello, &Trello::subdomain, this, &Osint::onResultSubdomain);
        connect(trello, &Trello::subdomainIp, this, &Osint::onResultSubdomainIp);
        connect(trello, &Trello::ip, this, &Osint::onResultIp);
        connect(trello, &Trello::email, this, &Osint::onResultEmail);
        connect(trello, &Trello::url, this, &Osint::onResultUrl);
        connect(trello, &Trello::errorLog, this, &Osint::onErrorLog);
        connect(trello, &Trello::infoLog, this, &Osint::onInfoLog);
        connect(cThread, &QThread::finished, this, &Osint::onScanThreadEnded);
        connect(cThread, &QThread::finished, trello, &Trello::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        status->osint->activeThreads++;
    }
    if(module.yahoo){
        Yahoo *yahoo = new Yahoo(scanArgs);
        QThread *cThread = new QThread(this);
        yahoo->Enumerator(cThread);
        yahoo->moveToThread(cThread);
        //...
        connect(yahoo, &Yahoo::subdomain, this, &Osint::onResultSubdomain);
        connect(yahoo, &Yahoo::subdomainIp, this, &Osint::onResultSubdomainIp);
        connect(yahoo, &Yahoo::ip, this, &Osint::onResultIp);
        connect(yahoo, &Yahoo::email, this, &Osint::onResultEmail);
        connect(yahoo, &Yahoo::url, this, &Osint::onResultUrl);
        connect(yahoo, &Yahoo::errorLog, this, &Osint::onErrorLog);
        connect(yahoo, &Yahoo::infoLog, this, &Osint::onInfoLog);
        connect(cThread, &QThread::finished, this, &Osint::onScanThreadEnded);
        connect(cThread, &QThread::finished, yahoo, &Yahoo::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        status->osint->activeThreads++;
    }
    if(module.bing){
        Bing *bing = new Bing(scanArgs);
        QThread *cThread = new QThread(this);
        bing->Enumerator(cThread);
        bing->moveToThread(cThread);
        //...
        connect(bing, &Bing::subdomain, this, &Osint::onResultSubdomain);
        connect(bing, &Bing::subdomainIp, this, &Osint::onResultSubdomainIp);
        connect(bing, &Bing::ip, this, &Osint::onResultIp);
        connect(bing, &Bing::email, this, &Osint::onResultEmail);
        connect(bing, &Bing::url, this, &Osint::onResultUrl);
        connect(bing, &Bing::errorLog, this, &Osint::onErrorLog);
        connect(bing, &Bing::infoLog, this, &Osint::onInfoLog);
        connect(cThread, &QThread::finished, this, &Osint::onScanThreadEnded);
        connect(cThread, &QThread::finished, bing, &Bing::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        status->osint->activeThreads++;
    }
    if(module.sitedossier){
        SiteDossier *sitedossier = new SiteDossier(scanArgs);
        QThread *cThread = new QThread(this);
        sitedossier->Enumerator(cThread);
        sitedossier->moveToThread(cThread);
        //...
        connect(sitedossier, &SiteDossier::subdomain, this, &Osint::onResultSubdomain);
        connect(sitedossier, &SiteDossier::subdomainIp, this, &Osint::onResultSubdomainIp);
        connect(sitedossier, &SiteDossier::ip, this, &Osint::onResultIp);
        connect(sitedossier, &SiteDossier::email, this, &Osint::onResultEmail);
        connect(sitedossier, &SiteDossier::url, this, &Osint::onResultUrl);
        connect(sitedossier, &SiteDossier::errorLog, this, &Osint::onErrorLog);
        connect(sitedossier, &SiteDossier::infoLog, this, &Osint::onInfoLog);
        connect(cThread, &QThread::finished, this, &Osint::onScanThreadEnded);
        connect(cThread, &QThread::finished, sitedossier, &SiteDossier::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        status->osint->activeThreads++;
    }
    if(module.pagesinventory){
        PagesInventory *pagesinventory = new PagesInventory(scanArgs);
        QThread *cThread = new QThread(this);
        pagesinventory->Enumerator(cThread);
        pagesinventory->moveToThread(cThread);
        //...
        connect(pagesinventory, &PagesInventory::subdomain, this, &Osint::onResultSubdomain);
        connect(pagesinventory, &PagesInventory::subdomainIp, this, &Osint::onResultSubdomainIp);
        connect(pagesinventory, &PagesInventory::ip, this, &Osint::onResultIp);
        connect(pagesinventory, &PagesInventory::email, this, &Osint::onResultEmail);
        connect(pagesinventory, &PagesInventory::url, this, &Osint::onResultUrl);
        connect(pagesinventory, &PagesInventory::errorLog, this, &Osint::onErrorLog);
        connect(pagesinventory, &PagesInventory::infoLog, this, &Osint::onInfoLog);
        connect(cThread, &QThread::finished, this, &Osint::onScanThreadEnded);
        connect(cThread, &QThread::finished, pagesinventory, &PagesInventory::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        status->osint->activeThreads++;
    }
    if(module.viewDns){
        ViewDns *viewdns = new ViewDns(scanArgs);
        QThread *cThread = new QThread(this);
        viewdns->Enumerator(cThread);
        viewdns->moveToThread(cThread);
        //...
        connect(viewdns, &ViewDns::subdomain, this, &Osint::onResultSubdomain);
        connect(viewdns, &ViewDns::subdomainIp, this, &Osint::onResultSubdomainIp);
        connect(viewdns, &ViewDns::ip, this, &Osint::onResultIp);
        connect(viewdns, &ViewDns::email, this, &Osint::onResultEmail);
        connect(viewdns, &ViewDns::url, this, &Osint::onResultUrl);
        connect(viewdns, &ViewDns::errorLog, this, &Osint::onErrorLog);
        connect(viewdns, &ViewDns::infoLog, this, &Osint::onInfoLog);
        connect(cThread, &QThread::finished, this, &Osint::onScanThreadEnded);
        connect(cThread, &QThread::finished, viewdns, &ViewDns::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        status->osint->activeThreads++;
    }
    ///
    /// after starting subdomainIp choosen enumerations...
    ///
    if(status->osint->activeThreads)
    {
        ui->buttonStart->setDisabled(true);
        ui->buttonStop->setEnabled(true);
    }
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

void Osint::connectActions(){
    connect(&actionClearResults, &QAction::triggered, this, [=](){this->onClearResults();});
    ///
    /// SAVE...
    ///
    connect(&actionSaveSubdomains, &QAction::triggered, this, [=](){this->onSaveResults(CHOICE::subdomain, PROXYMODEL_TYPE::subdomainIpProxy);});
    connect(&actionSaveIpAddresses, &QAction::triggered, this, [=](){this->onSaveResults(CHOICE::ip, PROXYMODEL_TYPE::subdomainIpProxy);});
    connect(&actionSaveAll, &QAction::triggered, this, [=](){this->onSaveResults(CHOICE::subdomainIp, PROXYMODEL_TYPE::subdomainIpProxy);});
    connect(&actionSaveEmails, &QAction::triggered, this, [=](){this->onSaveResults(CHOICE::email, PROXYMODEL_TYPE::emailProxy);});
    connect(&actionSaveUrls, &QAction::triggered, this, [=](){this->onSaveResults(CHOICE::url, PROXYMODEL_TYPE::urlProxy);});
    connect(&actionSaveAsns, &QAction::triggered, this, [=](){this->onSaveResults(CHOICE::asn, PROXYMODEL_TYPE::asnProxy);});
    //...
    connect(&actionSaveSubdomains_subdomain, &QAction::triggered, this, [=](){this->onSaveResults(CHOICE::subdomain, PROXYMODEL_TYPE::subdomainProxy);});
    connect(&actionSaveIpAddresses_ip, &QAction::triggered, this, [=](){this->onSaveResults(CHOICE::ip, PROXYMODEL_TYPE::ipProxy);});

    ///
    /// COPY...
    ///
    connect(&actionCopySubdomains, &QAction::triggered, this, [=](){this->onCopyResults(CHOICE::subdomain, PROXYMODEL_TYPE::subdomainIpProxy);});
    connect(&actionCopyIpAddresses, &QAction::triggered, this, [=](){this->onCopyResults(CHOICE::ip, PROXYMODEL_TYPE::subdomainIpProxy);});
    connect(&actionCopyAll, &QAction::triggered, this, [=](){this->onCopyResults(CHOICE::subdomainIp, PROXYMODEL_TYPE::subdomainIpProxy);});
    connect(&actionCopyEmails, &QAction::triggered, this, [=](){this->onCopyResults(CHOICE::email, PROXYMODEL_TYPE::emailProxy);});
    connect(&actionCopyUrls, &QAction::triggered, this, [=](){this->onCopyResults(CHOICE::url, PROXYMODEL_TYPE::urlProxy);});
    connect(&actionCopyAsns, &QAction::triggered, this, [=](){this->onCopyResults(CHOICE::asn, PROXYMODEL_TYPE::asnProxy);});
    //...
    connect(&actionCopySubdomains_subdomain, &QAction::triggered, this, [=](){this->onCopyResults(CHOICE::subdomain, PROXYMODEL_TYPE::subdomainProxy);});
    connect(&actionCopyIpAddresses_ip, &QAction::triggered, this, [=](){this->onCopyResults(CHOICE::ip, PROXYMODEL_TYPE::ipProxy);});
    ///
    /// SUBDOMAINS AND IPS...
    ///
    connect(&actionSendToIp, &QAction::triggered, this, [=](){emit sendIpAddressesToIp(ENGINE::OSINT, CHOICE::ip, PROXYMODEL_TYPE::subdomainIpProxy); emit changeTabToIp();});
    connect(&actionSendToOsint, &QAction::triggered, this, [=](){emit sendSubdomainsToOsint(ENGINE::OSINT, CHOICE::subdomain, PROXYMODEL_TYPE::subdomainIpProxy); emit changeTabToOsint();});
    connect(&actionSendToBrute, &QAction::triggered, this, [=](){emit sendSubdomainsToBrute(ENGINE::OSINT, CHOICE::subdomain, PROXYMODEL_TYPE::subdomainIpProxy); emit changeTabToBrute();});
    connect(&actionSendToActive, &QAction::triggered, this, [=](){emit sendSubdomainsToActive(ENGINE::OSINT, CHOICE::subdomain, PROXYMODEL_TYPE::subdomainIpProxy); emit changeTabToActive();});
    connect(&actionSendToRecords, &QAction::triggered, this, [=](){emit sendSubdomainsToRecord(ENGINE::OSINT, CHOICE::subdomain, PROXYMODEL_TYPE::subdomainIpProxy); emit changeTabToRecords();});
    //...
    connect(&actionSendToIp_ip, &QAction::triggered, this, [=](){emit sendIpAddressesToIp(ENGINE::OSINT, CHOICE::ip, PROXYMODEL_TYPE::ipProxy); emit changeTabToIp();});
    connect(&actionSendToOsint_subdomain, &QAction::triggered, this, [=](){emit sendSubdomainsToOsint(ENGINE::OSINT, CHOICE::subdomain, PROXYMODEL_TYPE::subdomainProxy); emit changeTabToOsint();});
    connect(&actionSendToBrute_subdomain, &QAction::triggered, this, [=](){emit sendSubdomainsToBrute(ENGINE::OSINT, CHOICE::subdomain, PROXYMODEL_TYPE::subdomainProxy); emit changeTabToBrute();});
    connect(&actionSendToActive_subdomain, &QAction::triggered, this, [=](){emit sendSubdomainsToActive(ENGINE::OSINT, CHOICE::subdomain, PROXYMODEL_TYPE::subdomainProxy); emit changeTabToActive();});
    connect(&actionSendToRecords_subdomain, &QAction::triggered, this, [=](){emit sendSubdomainsToRecord(ENGINE::OSINT, CHOICE::subdomain, PROXYMODEL_TYPE::subdomainProxy); emit changeTabToRecords();});
    /**** For Right-Click ContextMenu ****/
    connect(&actionSave, &QAction::triggered, this, [=](){this->onSaveResults(selectionModel);});
    connect(&actionCopy, &QAction::triggered, this, [=](){this->onCopyResults(selectionModel);});
    //...
    connect(&actionOpenInBrowser, &QAction::triggered, this, [=](){this->openInBrowser(selectionModel);});
    //...
    connect(&actionSendToOsint_c, &QAction::triggered, this, [=](){emit sendSubdomainsToOsint(selectionModel); emit changeTabToOsint();});
    connect(&actionSendToIp_c, &QAction::triggered, this, [=](){emit sendIpAddressesToIp(selectionModel); emit changeTabToIp();});
    connect(&actionSendToBrute_c, &QAction::triggered, this, [=](){emit sendSubdomainsToBrute(selectionModel); emit changeTabToBrute();});
    connect(&actionSendToActive_c, &QAction::triggered, this, [=](){emit sendSubdomainsToActive(selectionModel); emit changeTabToActive();});
    connect(&actionSendToRecords_c, &QAction::triggered, this, [=](){emit sendSubdomainsToRecord(selectionModel); emit changeTabToRecords();});
}

void Osint::on_buttonAction_clicked(){
    int option = ui->comboBoxOption->currentIndex();

    switch(option){
    case SUBDOMAINIP:
        if(result->osint->subdomainIp->rowCount() < 1)
            return;
        break;
    case SUBDOMAIN:
        if(result->osint->subdomain->rowCount() < 1)
            return;
        break;
    case IP:
        if(result->osint->ip->rowCount() < 1)
            return;
        break;
    case EMAIL:
        if(result->osint->email->rowCount() < 1)
            return;
        break;
    case URL:
        if(result->osint->url->rowCount() < 1)
            return;
        break;
    case ASN:
        if(result->osint->asn->rowCount() < 1)
            return;
        break;
    }

    QPoint pos = ui->buttonAction->mapToGlobal(QPoint(0,0));
    pos = QPoint(pos.x()+65, pos.y());

    QMenu *Menu = new QMenu(this);
    QMenu *saveMenu = new QMenu(this);
    QMenu *copyMenu = new QMenu(this);
    Menu->setAttribute(Qt::WA_DeleteOnClose, true);

    saveMenu->setTitle("Save");
    copyMenu->setTitle("Copy");

    ///
    /// ADDING ACTIONS TO THE CONTEXT MENU...
    ///
    if(option == SUBDOMAINIP){
        saveMenu->addAction(&actionSaveSubdomains);
        copyMenu->addAction(&actionCopySubdomains);
        saveMenu->addAction(&actionSaveIpAddresses);
        copyMenu->addAction(&actionCopyIpAddresses);
        saveMenu->addAction(&actionSaveAll);
        copyMenu->addAction(&actionCopyAll);
    }
    if(option == SUBDOMAIN){
        saveMenu->addAction(&actionSaveSubdomains_subdomain);
        copyMenu->addAction(&actionCopySubdomains_subdomain);
    }
    if(option == IP){
        saveMenu->addAction(&actionSaveIpAddresses_ip);
        copyMenu->addAction(&actionCopyIpAddresses_ip);
    }
    if(option == EMAIL){
        saveMenu->addAction(&actionSaveEmails);
        copyMenu->addAction(&actionCopyEmails);
    }
    if(option == URL){
        saveMenu->addAction(&actionSaveUrls);
        copyMenu->addAction(&actionCopyUrls);
    }
    if(option == ASN){
        saveMenu->addAction(&actionSaveAsns);
        copyMenu->addAction(&actionCopyAsns);
    }
    Menu->addAction(&actionClearResults);
    Menu->addSeparator();
    Menu->addMenu(copyMenu);
    Menu->addMenu(saveMenu);
    Menu->addSeparator();
    if(option == IP)
        Menu->addAction(&actionSendToIp_ip);
    if(option == SUBDOMAINIP){
        Menu->addAction(&actionSendToIp);
        Menu->addAction(&actionSendToOsint);
        Menu->addAction(&actionSendToBrute);
        Menu->addAction(&actionSendToActive);
        Menu->addAction(&actionSendToRecords);
    }
    if(option == SUBDOMAIN){
        Menu->addAction(&actionSendToOsint_subdomain);
        Menu->addAction(&actionSendToBrute_subdomain);
        Menu->addAction(&actionSendToActive_subdomain);
        Menu->addAction(&actionSendToRecords_subdomain);
    }
    ///
    /// showing the context menu...
    ///
    Menu->exec(pos);
}

void Osint::on_tableViewResults_customContextMenuRequested(const QPoint &pos){
    Q_UNUSED(pos);
    int option = ui->comboBoxOption->currentIndex();

    if(!ui->tableViewResults->selectionModel()->isSelected(ui->tableViewResults->currentIndex())){
        return;
    }
    selectionModel = ui->tableViewResults->selectionModel();

    QMenu *Menu = new QMenu(this);
    Menu->setAttribute(Qt::WA_DeleteOnClose, true);

    Menu->addAction(&actionCopy);
    Menu->addAction(&actionSave);
    Menu->addSeparator();
    if(option != EMAIL){
        Menu->addAction(&actionOpenInBrowser);
        Menu->addSeparator();
    }
    if(option == SUBDOMAINIP || option == IP)
        Menu->addAction(&actionSendToIp_c);
    if(option == SUBDOMAINIP || option == SUBDOMAIN){
        Menu->addAction(&actionSendToOsint_c);
        Menu->addAction(&actionSendToBrute_c);
        Menu->addAction(&actionSendToActive_c);
        Menu->addAction(&actionSendToRecords_c);
    }
    ///
    /// showing the menu...
    ///
    Menu->exec(QCursor::pos());
}

void Osint::on_checkBoxMultipleTargets_clicked(bool checked){
    if(checked)
        ui->targets->show();
    else
        ui->targets->hide();
}

void Osint::onSaveResults(CHOICE choice, PROXYMODEL_TYPE proxyType){
    ///
    /// checks...
    ///
    QString filename = QFileDialog::getSaveFileName(this, "Save To File", "./");
    if(filename.isEmpty()){
        return;
    }
    QFile file(filename);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    if(!file.isOpen()){
        return;
    }

    QString item;
    ui->plainTextEditLogs->setPlainText("hi there...");
    switch(proxyType){
    case PROXYMODEL_TYPE::subdomainIpProxy:
    {
        switch(choice){
        case CHOICE::subdomain:
            for(int i = 0; i != result->osint->subdomainIpProxy->rowCount(); ++i)
            {
                item = result->osint->subdomainIpProxy->data(result->osint->subdomainIpProxy->index(i, 0)).toString().append(NEWLINE);
                file.write(item.toUtf8());
            }
            break;
        case CHOICE::ip:
        {
            QSet<QString> itemSet;
            for(int i = 0; i != result->osint->subdomainIpProxy->rowCount(); ++i)
            {
                item = result->osint->subdomainIpProxy->data(result->osint->subdomainIpProxy->index(i, 1)).toString().append(NEWLINE);
                if(!itemSet.contains(item)){
                    itemSet.insert(item);
                    file.write(item.toUtf8());
                }
            }
            break;
        }
        case CHOICE::subdomainIp:
            for(int i = 0; i != result->osint->subdomainIpProxy->rowCount(); ++i)
            {
                item = result->osint->subdomainIpProxy->data(result->osint->subdomainIpProxy->index(i, 0)).toString()+":"+result->osint->subdomainIpProxy->data(result->osint->subdomainIpProxy->index(i, 1)).toString().append(NEWLINE);
                file.write(item.toUtf8());
            }
            break;
        default:
            break;
        }
        break;
    }

    case PROXYMODEL_TYPE::subdomainProxy:
        ui->plainTextEditLogs->setPlainText("hello there...");
        for(int i = 0; i != result->osint->subdomainProxy->rowCount(); ++i)
        {
            item = result->osint->subdomainProxy->data(result->osint->subdomainProxy->index(i, 0)).toString().append(NEWLINE);
            file.write(item.toUtf8());
        }
        break;

    case PROXYMODEL_TYPE::ipProxy:
        for(int i = 0; i != result->osint->ipProxy->rowCount(); ++i)
        {
            item = result->osint->ipProxy->data(result->osint->ipProxy->index(i, 0)).toString().append(NEWLINE);
            file.write(item.toUtf8());
        }
        break;

    case PROXYMODEL_TYPE::emailProxy:
        for(int i = 0; i != result->osint->emailProxy->rowCount(); ++i)
        {
            item = result->osint->emailProxy->data(result->osint->emailProxy->index(i, 0)).toString().append(NEWLINE);
            file.write(item.toUtf8());
        }
        break;

    case PROXYMODEL_TYPE::urlProxy:
        for(int i = 0; i != result->osint->urlProxy->rowCount(); ++i)
        {
            item = result->osint->urlProxy->data(result->osint->urlProxy->index(i, 0)).toString().append(NEWLINE);
            file.write(item.toUtf8());
        }
        break;

    case PROXYMODEL_TYPE::asnProxy:
        for(int i = 0; i != result->osint->asnProxy->rowCount(); ++i)
        {
            item = result->osint->asnProxy->data(result->osint->asnProxy->index(i, 0)).toString().append(NEWLINE);
            file.write(item.toUtf8());
        }
        break;

    default:
        break;
    }
    file.close();
}

void Osint::onSaveResults(QItemSelectionModel *selectionModel){
    QString filename = QFileDialog::getSaveFileName(this, "Save To File", "./");
    if(filename.isEmpty()){
        return;
    }
    QString data;
    QString item;
    QFile file(filename);

    if(file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        foreach(const QModelIndex &index, selectionModel->selectedIndexes()){
            item = index.data().toString();
            data.append(item.append(NEWLINE));
        }
        file.write(data.toUtf8());
        file.close();
    }
}

void Osint::onCopyResults(CHOICE choice, PROXYMODEL_TYPE proxyType){
    QClipboard *clipboard = QGuiApplication::clipboard();
    QString clipboardData;
    QString item;

    switch(proxyType){
    case PROXYMODEL_TYPE::subdomainIpProxy:
    {
        switch(choice){
        case CHOICE::subdomain:
            for(int i = 0; i != result->osint->subdomainIpProxy->rowCount(); ++i)
            {
                item = result->osint->subdomainIpProxy->data(result->osint->subdomainIpProxy->index(i, 0)).toString().append(NEWLINE);
                clipboardData.append(item);
            }
            break;
        case CHOICE::ip:
        {
            QSet<QString> itemSet;
            for(int i = 0; i != result->osint->subdomainIpProxy->rowCount(); ++i)
            {
                item = result->osint->subdomainIpProxy->data(result->osint->subdomainIpProxy->index(i, 1)).toString().append(NEWLINE);
                if(!itemSet.contains(item)){
                    itemSet.insert(item);
                    clipboardData.append(item);
                }
            }
            break;
        }
        case CHOICE::subdomainIp:
            for(int i = 0; i != result->osint->subdomainIpProxy->rowCount(); ++i)
            {
                item = result->osint->subdomainIpProxy->data(result->osint->subdomainIpProxy->index(i, 0)).toString()+":"+result->osint->subdomainIpProxy->data(result->osint->subdomainIpProxy->index(i, 1)).toString().append(NEWLINE);
                clipboardData.append(item);
            }
            break;
        default:
            break;
        }
        break;
    }

    case PROXYMODEL_TYPE::subdomainProxy:
        for(int i = 0; i != result->osint->subdomainProxy->rowCount(); ++i)
        {
            item = result->osint->subdomainProxy->data(result->osint->subdomainProxy->index(i, 0)).toString().append(NEWLINE);
            clipboardData.append(item);
        }
        break;

    case PROXYMODEL_TYPE::ipProxy:
        for(int i = 0; i != result->osint->ipProxy->rowCount(); ++i)
        {
            item = result->osint->ipProxy->data(result->osint->ipProxy->index(i, 0)).toString().append(NEWLINE);
            clipboardData.append(item);
        }
        break;

    case PROXYMODEL_TYPE::emailProxy:
        for(int i = 0; i != result->osint->emailProxy->rowCount(); ++i)
        {
            item = result->osint->emailProxy->data(result->osint->emailProxy->index(i, 0)).toString().append(NEWLINE);
            clipboardData.append(item);
        }
        break;

    case PROXYMODEL_TYPE::urlProxy:
        for(int i = 0; i != result->osint->urlProxy->rowCount(); ++i)
        {
            item = result->osint->urlProxy->data(result->osint->urlProxy->index(i, 0)).toString().append(NEWLINE);
            clipboardData.append(item);
        }
        break;

    case PROXYMODEL_TYPE::asnProxy:
        for(int i = 0; i != result->osint->asnProxy->rowCount(); ++i)
        {
            item = result->osint->asnProxy->data(result->osint->asnProxy->index(i, 0)).toString().append(NEWLINE);
            clipboardData.append(item);
        }
        break;

    default:
        break;
    }
    clipboard->setText(clipboardData);
}

void Osint::onCopyResults(QItemSelectionModel *selectionModel){
    QClipboard *clipboard = QGuiApplication::clipboard();
    QString data, item;

    foreach(const QModelIndex &index, selectionModel->selectedIndexes())
    {
        item = index.data().toString();
        data.append(item.append(NEWLINE));
    }
    clipboard->setText(data);
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
    }
}
