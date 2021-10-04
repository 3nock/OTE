#include "Osint.h"
#include "ui_Osint.h"
//...
#include <QDateTime>
#include <QClipboard>
#include "src/utils/Config.h"
#include "src/dialogs/OsintConfigDialog.h"
//...
#include "src/engines/osint/modules/site/PagesInventory.h"
#include "src/engines/osint/modules/site/SiteDossier.h"
#include "src/engines/osint/modules/scrape/Bing.h"
#include "src/engines/osint/modules/scrape/Yahoo.h"
#include "src/engines/osint/modules/scrape/Trello.h"
#include "src/engines/osint/modules/scrape/Exalead.h"
#include "src/engines/osint/modules/scrape/DuckDuckGo.h"
#include "src/engines/osint/modules/scrape/DogPile.h"
#include "src/engines/osint/modules/scrape/Baidu.h"
#include "src/engines/osint/modules/scrape/Ask.h"
#include "src/engines/osint/modules/api/ViewDns.h"
#include "src/engines/osint/modules/api/IpApi.h"
#include "src/engines/osint/modules/api/ZoomEye.h"
#include "src/engines/osint/modules/api/ZETAlytics.h"
#include "src/engines/osint/modules/api/WhoisXmlApi.h"
#include "src/engines/osint/modules/api/ThreatBook.h"
#include "src/engines/osint/modules/api/Shodan.h"
#include "src/engines/osint/modules/api/SecurityTrails.h"
#include "src/engines/osint/modules/api/Robtex.h"
#include "src/engines/osint/modules/api/RiskIq.h"
#include "src/engines/osint/modules/api/Mnemonic.h"
#include "src/engines/osint/modules/api/IpInfo.h"
#include "src/engines/osint/modules/api/HunterSearch.h"
#include "src/engines/osint/modules/api/Github.h"
#include "src/engines/osint/modules/api/CommonCrawl.h"
#include "src/engines/osint/modules/api/C99.h"
#include "src/engines/osint/modules/api/BinaryEdge.h"
#include "src/engines/osint/modules/api/Bgpview.h"
#include "src/engines/osint/modules/api/Anubis.h"
#include "src/engines/osint/modules/api/Otx.h"
#include "src/engines/osint/modules/api/Sublist3r.h"
#include "src/engines/osint/modules/api/Threatminer.h"
#include "src/engines/osint/modules/api/Threatcrowd.h"
#include "src/engines/osint/modules/api/Hackertarget.h"
#include "src/engines/osint/modules/api/Dnsbufferoverun.h"
#include "src/engines/osint/modules/api/Projectdiscovery.h"
#include "src/engines/osint/modules/api/Spyse.h"
#include "src/engines/osint/modules/api/Omnisint.h"
#include "src/engines/osint/modules/api/Qwant.h"
#include "src/engines/osint/modules/api/VirusTotal.h"
#include "src/engines/osint/modules/api/Urlscan.h"
#include "src/engines/osint/modules/archive/Waybackmachine.h"
#include "src/engines/osint/modules/archive/ArchiveToday.h"
#include "src/engines/osint/modules/archive/ArchiveIt.h"
#include "src/engines/osint/modules/site/Dnsdumpster.h"
#include "src/engines/osint/modules/site/Netcraft.h"
#include "src/engines/osint/modules/site/Suip.h"
#include "src/engines/osint/modules/site/Pkey.h"
#include "src/engines/osint/modules/site/Rapiddns.h"
#include "src/engines/osint/modules/cert/Crtsh.h"
#include "src/engines/osint/modules/cert/GoogleCert.h"
#include "src/engines/osint/modules/cert/Certspotter.h"
#include "src/engines/osint/modules/cert/CensysFree.h"

#define TRUE "1"
#define FALSE "0"

/* log on every scan each module its results count
 *
 * http://ipv4info.com/account/register/
 * https://riddler.io/auth/register
 * https://dnschecker.org/all-tools.php
 * https://suip.biz/
 * https://viewdns.info/api/
 * https://rapiddns.io/tools
 * pkey.in
 * https://skrapp.io/api
 * https://hunter.io/bulk-finders
 *
 * https://tools.epieos.com/email.php ->email-lookup
 * rename AbstractOsintModule to AbstractOsintModule
 *
 * create a json to QStandardItem Tree...
 *
 * you can crawl the enumerated urls all eg from archives
 * and return only the status codes of each url and show a graph
 * of status codes eg like screaming frog crawler analysis
 * eg. how many 200,301,404,501
 * create its own seperate analysis tool...
 *
 * add option for certificates and on comboBox and their own
 * modelView which show all cert n subdomain info
 * graph of certificates
 *
 * create a dialog(program properties) that shows all api-calls url so
 * user can be able to fix them without compiling..
 *
 * emit a QStringList/QSet instead of individual list
 * automatically group the subdmains with stars into one group, from cert scans
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
 * all updated at the same time...
 *
 * osint config should contain every osint module with its options of what you
 * want to enumerate
 *
 * active services eg HTTP should be boolean on the structure...
 *
 */

Osint::Osint(QWidget *parent, ResultsModel *resultsModel, Status *status):
    AbstractEngine(parent, resultsModel, status),
    ui(new Ui::Osint),
    m_scanArguments(new osint::ScanArguments),
    m_scanResults(new osint::ScanResults),
    m_model(resultsModel->osint),
    m_proxyModel(resultsModel->proxy->osint)
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
    currentPath = QDir::currentPath();
    ui->lineEditTarget->setPlaceholderText("eg. example.com");
    ui->lineEditFilter->setPlaceholderText("Enter filter...");
    //...
    m_model->setHorizontalHeaderLabels({"Subdomain", "IpAddress"});
    ui->tableViewResults->setModel(m_proxyModel);
    //...
    //ui->buttonPause->setDisabled(true);
    ui->buttonStop->setDisabled(true);
    ///
    /// hide widgets...
    ///
    ui->buttonAction->hide();
    ui->progressBar->hide();
    ui->targets->hide();
    //...
    ui->lineEditFilter->hide();
    ui->buttonFilter->hide();
    ui->comboBoxFilter->hide();
    ///
    /// equally seperate the widgets...
    ///
    ui->splitter->setSizes(QList<int>() << static_cast<int>((this->width() * 0.50))
                                        << static_cast<int>((this->width() * 0.50)));
    ///
    /// ...
    ///
    this->connectActions();
    m_proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    m_proxyModel->setRecursiveFilteringEnabled(true);
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

void Osint::on_buttonStart_clicked(){
    ///
    /// checks...
    ///
    if(ui->lineEditTarget->text().isEmpty()){
        QMessageBox::warning(this, "Error!", "Please Target Domain For Enumerations!");
        return;
    }
    ///
    /// get target and other scan structures...
    ///
    m_targetDomain = TargetNameFilter(ui->lineEditTarget->text(), ENGINE::OSINT);
    ///
    /// enumerate...
    ///
    startScan();
    ui->buttonAction->show();
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

    if(module.certspotter)
    {
        Certspotter *certspotter = new Certspotter(ui->lineEditTarget->text());
        QThread *cThread = new QThread(this);
        certspotter->Enumerator(cThread);
        certspotter->moveToThread(cThread);
        //...
        connect(certspotter, &Certspotter::scanResults, this, &Osint::scanResults);
        connect(certspotter, &Certspotter::errorLog, this, &Osint::onErrorLog);
        connect(certspotter, &Certspotter::infoLog, this, &Osint::onInfoLog);
        connect(cThread, &QThread::finished, this, &Osint::onEnumerationComplete);
        connect(cThread, &QThread::finished, certspotter, &Certspotter::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        status->osint->activeThreads++;
    }
    if(module.otx)
    {
        Otx *otx = new Otx(ui->lineEditTarget->text());
        QThread *cThread = new QThread(this);
        otx->Enumerator(cThread);
        otx->moveToThread(cThread);
        //...
        connect(otx, &Otx::scanResults, this, &Osint::scanResults);
        connect(otx, &Otx::errorLog, this, &Osint::onErrorLog);
        connect(otx, &Otx::infoLog, this, &Osint::onInfoLog);
        connect(cThread, &QThread::finished, this, &Osint::onEnumerationComplete);
        connect(cThread, &QThread::finished, otx, &Otx::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        status->osint->activeThreads++;
    }
    if(module.sublist3r)
    {
        Sublist3r *sublist3r = new Sublist3r(ui->lineEditTarget->text());
        QThread *cThread = new QThread(this);
        sublist3r->Enumerator(cThread);
        sublist3r->moveToThread(cThread);
        //...
        connect(sublist3r, &Sublist3r::scanResults, this, &Osint::scanResults);
        connect(sublist3r, &Sublist3r::errorLog, this, &Osint::onErrorLog);
        connect(sublist3r, &Sublist3r::infoLog, this, &Osint::onInfoLog);
        connect(cThread, &QThread::finished, this, &Osint::onEnumerationComplete);
        connect(cThread, &QThread::finished, sublist3r, &Sublist3r::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        status->osint->activeThreads++;
    }
    if(module.threatminer)
    {
        Threatminer *threatminer = new Threatminer(ui->lineEditTarget->text());
        QThread *cThread = new QThread(this);
        threatminer->Enumerator(cThread);
        threatminer->moveToThread(cThread);
        //...
        connect(threatminer, &Threatminer::scanResults, this, &Osint::scanResults);
        connect(threatminer, &Threatminer::errorLog, this, &Osint::onErrorLog);
        connect(threatminer, &Threatminer::infoLog, this, &Osint::onInfoLog);
        connect(cThread, &QThread::finished, this, &Osint::onEnumerationComplete);
        connect(cThread, &QThread::finished, threatminer, &Threatminer::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        status->osint->activeThreads++;
    }
    if(module.threatcrowd)
    {
        Threatcrowd *threatcrowd = new Threatcrowd(ui->lineEditTarget->text());
        QThread *cThread = new QThread(this);
        threatcrowd->Enumerator(cThread);
        threatcrowd->moveToThread(cThread);
        //...
        connect(threatcrowd, &Threatcrowd::scanResults, this, &Osint::scanResults);
        connect(threatcrowd, &Threatcrowd::errorLog, this, &Osint::onErrorLog);
        connect(threatcrowd, &Threatcrowd::infoLog, this, &Osint::onInfoLog);
        connect(cThread, &QThread::finished, this, &Osint::onEnumerationComplete);
        connect(cThread, &QThread::finished, threatcrowd, &Threatcrowd::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        status->osint->activeThreads++;
    }
    if(module.hackertarget)
    {
        Hackertarget *hackertarget = new Hackertarget(ui->lineEditTarget->text());
        QThread *cThread = new QThread(this);
        hackertarget->Enumerator(cThread);
        hackertarget->moveToThread(cThread);
        //...
        connect(hackertarget, &Hackertarget::scanResults, this, &Osint::scanResults);
        connect(hackertarget, &Hackertarget::errorLog, this, &Osint::onErrorLog);
        connect(hackertarget, &Hackertarget::infoLog, this, &Osint::onInfoLog);
        connect(cThread, &QThread::finished, this, &Osint::onEnumerationComplete);
        connect(cThread, &QThread::finished, hackertarget, &Hackertarget::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        status->osint->activeThreads++;
    }
    if(module.dnsbufferoverrun)
    {
        Dnsbufferoverun *dnsbufferoverun = new Dnsbufferoverun(ui->lineEditTarget->text());
        QThread *cThread = new QThread(this);
        dnsbufferoverun->Enumerator(cThread);
        dnsbufferoverun->moveToThread(cThread);
        //...
        connect(dnsbufferoverun, &Dnsbufferoverun::scanResults, this, &Osint::scanResults);
        connect(dnsbufferoverun, &Dnsbufferoverun::errorLog, this, &Osint::onErrorLog);
        connect(dnsbufferoverun, &Dnsbufferoverun::infoLog, this, &Osint::onInfoLog);
        connect(cThread, &QThread::finished, this, &Osint::onEnumerationComplete);
        connect(cThread, &QThread::finished, dnsbufferoverun, &Dnsbufferoverun::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        status->osint->activeThreads++;
    }
    if(module.anubis)
    {
        Anubis *anubis = new Anubis(ui->lineEditTarget->text());
        QThread *cThread = new QThread(this);
        anubis->Enumerator(cThread);
        anubis->moveToThread(cThread);
        //...
        connect(anubis, &Anubis::scanResults, this, &Osint::scanResults);
        connect(anubis, &Anubis::errorLog, this, &Osint::onErrorLog);
        connect(anubis, &Anubis::infoLog, this, &Osint::onInfoLog);
        connect(cThread, &QThread::finished, this, &Osint::onEnumerationComplete);
        connect(cThread, &QThread::finished, anubis, &Anubis::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        status->osint->activeThreads++;
    }
    if(module.projectdiscovery)
    {
        Projectdiscovery *projectdiscovery = new Projectdiscovery(ui->lineEditTarget->text());
        QThread *cThread = new QThread(this);
        projectdiscovery->Enumerator(cThread);
        projectdiscovery->moveToThread(cThread);
        //...
        connect(projectdiscovery, &Projectdiscovery::scanResults, this, &Osint::scanResults);
        connect(projectdiscovery, &Projectdiscovery::errorLog, this, &Osint::onErrorLog);
        connect(projectdiscovery, &Projectdiscovery::infoLog, this, &Osint::onInfoLog);
        connect(cThread, &QThread::finished, this, &Osint::onEnumerationComplete);
        connect(cThread, &QThread::finished, projectdiscovery, &Projectdiscovery::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        status->osint->activeThreads++;
    }
    if(module.spyse)
    {
        Spyse *spyse = new Spyse(ui->lineEditTarget->text());
        QThread *cThread = new QThread(this);
        spyse->Enumerator(cThread);
        spyse->moveToThread(cThread);
        //...
        connect(spyse, &Spyse::scanResults, this, &Osint::scanResults);
        connect(spyse, &Spyse::errorLog, this, &Osint::onErrorLog);
        connect(spyse, &Spyse::infoLog, this, &Osint::onInfoLog);
        connect(cThread, &QThread::finished, this, &Osint::onEnumerationComplete);
        connect(cThread, &QThread::finished, spyse, &Spyse::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        status->osint->activeThreads++;
    }
    if(module.crtsh)
    {
        Crtsh *crtsh = new Crtsh(ui->lineEditTarget->text());
        QThread *cThread = new QThread(this);
        crtsh->Enumerator(cThread);
        crtsh->moveToThread(cThread);
        //...
        connect(crtsh, &Crtsh::scanResults, this, &Osint::scanResults);
        connect(crtsh, &Crtsh::errorLog, this, &Osint::onErrorLog);
        connect(crtsh, &Crtsh::infoLog, this, &Osint::onInfoLog);
        connect(cThread, &QThread::finished, this, &Osint::onEnumerationComplete);
        connect(cThread, &QThread::finished, crtsh, &Crtsh::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        status->osint->activeThreads++;
    }
    if(module.dnsdumpster)
    {
        Dnsdumpster *dnsdumpster = new Dnsdumpster(ui->lineEditTarget->text());
        QThread *cThread = new QThread(this);
        dnsdumpster->Enumerator(cThread);
        dnsdumpster->moveToThread(cThread);
        //...
        connect(dnsdumpster, &Dnsdumpster::scanResults, this, &Osint::scanResults);
        connect(dnsdumpster, &Dnsdumpster::errorLog, this, &Osint::onErrorLog);
        connect(dnsdumpster, &Dnsdumpster::infoLog, this, &Osint::onInfoLog);
        connect(cThread, &QThread::finished, this, &Osint::onEnumerationComplete);
        connect(cThread, &QThread::finished, dnsdumpster, &Dnsdumpster::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        status->osint->activeThreads++;
    }
    if(module.netcraft)
    {
        Netcraft *netcraft = new Netcraft(ui->lineEditTarget->text());
        QThread *cThread = new QThread(this);
        netcraft->Enumerator(cThread);
        netcraft->moveToThread(cThread);
        //...
        connect(netcraft, &Netcraft::scanResults, this, &Osint::scanResults);
        connect(netcraft, &Netcraft::errorLog, this, &Osint::onErrorLog);
        connect(netcraft, &Netcraft::infoLog, this, &Osint::onInfoLog);
        connect(cThread, &QThread::finished, this, &Osint::onEnumerationComplete);
        connect(cThread, &QThread::finished, netcraft, &Netcraft::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        status->osint->activeThreads++;
    }
    if(module.suip)
    {
        Suip *suip = new Suip(ui->lineEditTarget->text());
        QThread *cThread = new QThread(this);
        suip->Enumerator(cThread);
        suip->moveToThread(cThread);
        //...
        connect(suip, &Suip::scanResults, this, &Osint::scanResults);
        connect(suip, &Suip::errorLog, this, &Osint::onErrorLog);
        connect(suip, &Suip::infoLog, this, &Osint::onInfoLog);
        connect(cThread, &QThread::finished, this, &Osint::onEnumerationComplete);
        connect(cThread, &QThread::finished, suip, &Suip::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        status->osint->activeThreads++;
    }
    if(module.pkey){
        Pkey *pkey = new Pkey(ui->lineEditTarget->text());
        QThread *cThread = new QThread(this);
        pkey->Enumerator(cThread);
        pkey->moveToThread(cThread);
        //...
        connect(pkey, &Pkey::scanResults, this, &Osint::scanResults);
        connect(pkey, &Pkey::errorLog, this, &Osint::onErrorLog);
        connect(pkey, &Pkey::infoLog, this, &Osint::onInfoLog);
        connect(cThread, &QThread::finished, this, &Osint::onEnumerationComplete);
        connect(cThread, &QThread::finished, pkey, &Pkey::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        status->osint->activeThreads++;
    }
    if(module.rapiddns){
        Rapiddns *rapiddns = new Rapiddns(ui->lineEditTarget->text());
        QThread *cThread = new QThread(this);
        rapiddns->Enumerator(cThread);
        rapiddns->moveToThread(cThread);
        //...
        connect(rapiddns, &Rapiddns::scanResults, this, &Osint::scanResults);
        connect(rapiddns, &Rapiddns::errorLog, this, &Osint::onErrorLog);
        connect(rapiddns, &Rapiddns::infoLog, this, &Osint::onInfoLog);
        connect(cThread, &QThread::finished, this, &Osint::onEnumerationComplete);
        connect(cThread, &QThread::finished, rapiddns, &Rapiddns::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        status->osint->activeThreads++;
    }
    if(module.googleCert){
        GoogleCert *googlecert = new GoogleCert(ui->lineEditTarget->text());
        QThread *cThread = new QThread(this);
        googlecert->Enumerator(cThread);
        googlecert->moveToThread(cThread);
        //...
        connect(googlecert, &GoogleCert::scanResults, this, &Osint::scanResults);
        connect(googlecert, &GoogleCert::errorLog, this, &Osint::onErrorLog);
        connect(googlecert, &GoogleCert::infoLog, this, &Osint::onInfoLog);
        connect(cThread, &QThread::finished, this, &Osint::onEnumerationComplete);
        connect(cThread, &QThread::finished, googlecert, &GoogleCert::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        status->osint->activeThreads++;
    }
    if(module.omnisint){
        Omnisint *omnisint = new Omnisint(ui->lineEditTarget->text());
        QThread *cThread = new QThread(this);
        omnisint->Enumerator(cThread);
        omnisint->moveToThread(cThread);
        //...
        connect(omnisint, &Omnisint::scanResults, this, &Osint::scanResults);
        connect(omnisint, &Omnisint::errorLog, this, &Osint::onErrorLog);
        connect(omnisint, &Omnisint::infoLog, this, &Osint::onInfoLog);
        connect(cThread, &QThread::finished, this, &Osint::onEnumerationComplete);
        connect(cThread, &QThread::finished, omnisint, &Omnisint::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        status->osint->activeThreads++;
    }
    if(module.qwant){
        Qwant *qwant = new Qwant(ui->lineEditTarget->text());
        QThread *cThread = new QThread(this);
        qwant->Enumerator(cThread);
        qwant->moveToThread(cThread);
        //...
        connect(qwant, &Qwant::scanResults, this, &Osint::scanResults);
        connect(qwant, &Qwant::errorLog, this, &Osint::onErrorLog);
        connect(qwant, &Qwant::infoLog, this, &Osint::onInfoLog);
        connect(cThread, &QThread::finished, this, &Osint::onEnumerationComplete);
        connect(cThread, &QThread::finished, qwant, &Qwant::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        status->osint->activeThreads++;
    }
    if(module.virustotalapi){
        VirusTotal *virustotal = new VirusTotal(ui->lineEditTarget->text());
        QThread *cThread = new QThread(this);
        virustotal->Enumerator(cThread);
        virustotal->moveToThread(cThread);
        //...
        connect(virustotal, &VirusTotal::scanResults, this, &Osint::scanResults);
        connect(virustotal, &VirusTotal::errorLog, this, &Osint::onErrorLog);
        connect(virustotal, &VirusTotal::infoLog, this, &Osint::onInfoLog);
        connect(cThread, &QThread::finished, this, &Osint::onEnumerationComplete);
        connect(cThread, &QThread::finished, virustotal, &VirusTotal::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        status->osint->activeThreads++;
    }
    if(module.urlscan){
        Urlscan *urlscan = new Urlscan(ui->lineEditTarget->text());
        QThread *cThread = new QThread(this);
        urlscan->Enumerator(cThread);
        urlscan->moveToThread(cThread);
        //...
        connect(urlscan, &Urlscan::scanResults, this, &Osint::scanResults);
        connect(urlscan, &Urlscan::errorLog, this, &Osint::onErrorLog);
        connect(urlscan, &Urlscan::infoLog, this, &Osint::onInfoLog);
        connect(cThread, &QThread::finished, this, &Osint::onEnumerationComplete);
        connect(cThread, &QThread::finished, urlscan, &Urlscan::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        status->osint->activeThreads++;
    }
    if(module.waybackmachine){
        Waybackmachine *waybackmachine = new Waybackmachine(ui->lineEditTarget->text());
        QThread *cThread = new QThread(this);
        waybackmachine->Enumerator(cThread);
        waybackmachine->moveToThread(cThread);
        //...
        connect(waybackmachine, &Waybackmachine::scanResults, this, &Osint::scanResults);
        connect(waybackmachine, &Waybackmachine::errorLog, this, &Osint::onErrorLog);
        connect(waybackmachine, &Waybackmachine::infoLog, this, &Osint::onInfoLog);
        connect(cThread, &QThread::finished, this, &Osint::onEnumerationComplete);
        connect(cThread, &QThread::finished, waybackmachine, &Waybackmachine::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        status->osint->activeThreads++;
    }
    if(module.archivetoday){
        ArchiveToday *archivetoday = new ArchiveToday(ui->lineEditTarget->text());
        QThread *cThread = new QThread(this);
        archivetoday->Enumerator(cThread);
        archivetoday->moveToThread(cThread);
        //...
        connect(archivetoday, &ArchiveToday::scanResults, this, &Osint::scanResults);
        connect(archivetoday, &ArchiveToday::errorLog, this, &Osint::onErrorLog);
        connect(archivetoday, &ArchiveToday::infoLog, this, &Osint::onInfoLog);
        connect(cThread, &QThread::finished, this, &Osint::onEnumerationComplete);
        connect(cThread, &QThread::finished, archivetoday, &ArchiveToday::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        status->osint->activeThreads++;
    }
    if(module.archiveit){
        ArchiveIt *archiveit = new ArchiveIt(ui->lineEditTarget->text());
        QThread *cThread = new QThread(this);
        archiveit->Enumerator(cThread);
        archiveit->moveToThread(cThread);
        //...
        connect(archiveit, &ArchiveIt::scanResults, this, &Osint::scanResults);
        connect(archiveit, &ArchiveIt::errorLog, this, &Osint::onErrorLog);
        connect(archiveit, &ArchiveIt::infoLog, this, &Osint::onInfoLog);
        connect(cThread, &QThread::finished, this, &Osint::onEnumerationComplete);
        connect(cThread, &QThread::finished, archiveit, &ArchiveIt::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        status->osint->activeThreads++;
    }
    if(module.censysFree){
        CensysFree *censysfree = new CensysFree(ui->lineEditTarget->text());
        QThread *cThread = new QThread(this);
        censysfree->Enumerator(cThread);
        censysfree->moveToThread(cThread);
        //...
        connect(censysfree, &CensysFree::scanResults, this, &Osint::scanResults);
        connect(censysfree, &CensysFree::errorLog, this, &Osint::onErrorLog);
        connect(censysfree, &CensysFree::infoLog, this, &Osint::onInfoLog);
        connect(cThread, &QThread::finished, this, &Osint::onEnumerationComplete);
        connect(cThread, &QThread::finished, censysfree, &CensysFree::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        status->osint->activeThreads++;
    }
    if(module.bgpview){
        Bgpview *bgpview = new Bgpview(ui->lineEditTarget->text());
        QThread *cThread = new QThread(this);
        bgpview->Enumerator(cThread);
        bgpview->moveToThread(cThread);
        //...
        connect(bgpview, &Bgpview::scanResults, this, &Osint::scanResults);
        connect(bgpview, &Bgpview::errorLog, this, &Osint::onErrorLog);
        connect(bgpview, &Bgpview::infoLog, this, &Osint::onInfoLog);
        connect(cThread, &QThread::finished, this, &Osint::onEnumerationComplete);
        connect(cThread, &QThread::finished, bgpview, &Bgpview::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        status->osint->activeThreads++;
    }
    if(module.binaryEdge){
        BinaryEdge *binaryedge = new BinaryEdge(ui->lineEditTarget->text());
        QThread *cThread = new QThread(this);
        binaryedge->Enumerator(cThread);
        binaryedge->moveToThread(cThread);
        //...
        connect(binaryedge, &BinaryEdge::scanResults, this, &Osint::scanResults);
        connect(binaryedge, &BinaryEdge::errorLog, this, &Osint::onErrorLog);
        connect(binaryedge, &BinaryEdge::infoLog, this, &Osint::onInfoLog);
        connect(cThread, &QThread::finished, this, &Osint::onEnumerationComplete);
        connect(cThread, &QThread::finished, binaryedge, &BinaryEdge::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        status->osint->activeThreads++;
    }
    if(module.c99){
        C99 *c99 = new C99(ui->lineEditTarget->text());
        QThread *cThread = new QThread(this);
        c99->Enumerator(cThread);
        c99->moveToThread(cThread);
        //...
        connect(c99, &C99::scanResults, this, &Osint::scanResults);
        connect(c99, &C99::errorLog, this, &Osint::onErrorLog);
        connect(c99, &C99::infoLog, this, &Osint::onInfoLog);
        connect(cThread, &QThread::finished, this, &Osint::onEnumerationComplete);
        connect(cThread, &QThread::finished, c99, &C99::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        status->osint->activeThreads++;
    }
    if(module.commoncrawl){
        CommonCrawl *commonCrawl = new CommonCrawl(ui->lineEditTarget->text());
        QThread *cThread = new QThread(this);
        commonCrawl->Enumerator(cThread);
        commonCrawl->moveToThread(cThread);
        //...
        connect(commonCrawl, &CommonCrawl::scanResults, this, &Osint::scanResults);
        connect(commonCrawl, &CommonCrawl::errorLog, this, &Osint::onErrorLog);
        connect(commonCrawl, &CommonCrawl::infoLog, this, &Osint::onInfoLog);
        connect(cThread, &QThread::finished, this, &Osint::onEnumerationComplete);
        connect(cThread, &QThread::finished, commonCrawl, &CommonCrawl::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        status->osint->activeThreads++;
    }
    if(module.github){
        Github *github = new Github(ui->lineEditTarget->text());
        QThread *cThread = new QThread(this);
        github->Enumerator(cThread);
        github->moveToThread(cThread);
        //...
        connect(github, &Github::scanResults, this, &Osint::scanResults);
        connect(github, &Github::errorLog, this, &Osint::onErrorLog);
        connect(github, &Github::infoLog, this, &Osint::onInfoLog);
        connect(cThread, &QThread::finished, this, &Osint::onEnumerationComplete);
        connect(cThread, &QThread::finished, github, &Github::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        status->osint->activeThreads++;
    }
    if(module.huntersearch){
        HunterSearch *huntersearch = new HunterSearch(ui->lineEditTarget->text());
        QThread *cThread = new QThread(this);
        huntersearch->Enumerator(cThread);
        huntersearch->moveToThread(cThread);
        //...
        connect(huntersearch, &HunterSearch::scanResults, this, &Osint::scanResults);
        connect(huntersearch, &HunterSearch::errorLog, this, &Osint::onErrorLog);
        connect(huntersearch, &HunterSearch::infoLog, this, &Osint::onInfoLog);
        connect(cThread, &QThread::finished, this, &Osint::onEnumerationComplete);
        connect(cThread, &QThread::finished, huntersearch, &HunterSearch::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        status->osint->activeThreads++;
    }
    if(module.ipinfo){
        IpInfo *ipinfo = new IpInfo(ui->lineEditTarget->text());
        QThread *cThread = new QThread(this);
        ipinfo->Enumerator(cThread);
        ipinfo->moveToThread(cThread);
        //...
        connect(ipinfo, &IpInfo::scanResults, this, &Osint::scanResults);
        connect(ipinfo, &IpInfo::errorLog, this, &Osint::onErrorLog);
        connect(ipinfo, &IpInfo::infoLog, this, &Osint::onInfoLog);
        connect(cThread, &QThread::finished, this, &Osint::onEnumerationComplete);
        connect(cThread, &QThread::finished, ipinfo, &IpInfo::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        status->osint->activeThreads++;
    }
    if(module.mnemonic){
        Mnemonic *mnemonic = new Mnemonic(ui->lineEditTarget->text());
        QThread *cThread = new QThread(this);
        mnemonic->Enumerator(cThread);
        mnemonic->moveToThread(cThread);
        //...
        connect(mnemonic, &Mnemonic::scanResults, this, &Osint::scanResults);
        connect(mnemonic, &Mnemonic::errorLog, this, &Osint::onErrorLog);
        connect(mnemonic, &Mnemonic::infoLog, this, &Osint::onInfoLog);
        connect(cThread, &QThread::finished, this, &Osint::onEnumerationComplete);
        connect(cThread, &QThread::finished, mnemonic, &Mnemonic::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        status->osint->activeThreads++;
    }
    if(module.riskiq){
        RiskIq *riskiq = new RiskIq(ui->lineEditTarget->text());
        QThread *cThread = new QThread(this);
        riskiq->Enumerator(cThread);
        riskiq->moveToThread(cThread);
        //...
        connect(riskiq, &RiskIq::scanResults, this, &Osint::scanResults);
        connect(riskiq, &RiskIq::errorLog, this, &Osint::onErrorLog);
        connect(riskiq, &RiskIq::infoLog, this, &Osint::onInfoLog);
        connect(cThread, &QThread::finished, this, &Osint::onEnumerationComplete);
        connect(cThread, &QThread::finished, riskiq, &RiskIq::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        status->osint->activeThreads++;
    }
    if(module.robtex){
        Robtex *robtex = new Robtex(ui->lineEditTarget->text());
        QThread *cThread = new QThread(this);
        robtex->Enumerator(cThread);
        robtex->moveToThread(cThread);
        //...
        connect(robtex, &Robtex::scanResults, this, &Osint::scanResults);
        connect(robtex, &Robtex::errorLog, this, &Osint::onErrorLog);
        connect(robtex, &Robtex::infoLog, this, &Osint::onInfoLog);
        connect(cThread, &QThread::finished, this, &Osint::onEnumerationComplete);
        connect(cThread, &QThread::finished, robtex, &Robtex::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        status->osint->activeThreads++;
    }
    if(module.securitytrails){
        SecurityTrails *securitytrails = new SecurityTrails(ui->lineEditTarget->text());
        QThread *cThread = new QThread(this);
        securitytrails->Enumerator(cThread);
        securitytrails->moveToThread(cThread);
        //...
        connect(securitytrails, &SecurityTrails::scanResults, this, &Osint::scanResults);
        connect(securitytrails, &SecurityTrails::errorLog, this, &Osint::onErrorLog);
        connect(securitytrails, &SecurityTrails::infoLog, this, &Osint::onInfoLog);
        connect(cThread, &QThread::finished, this, &Osint::onEnumerationComplete);
        connect(cThread, &QThread::finished, securitytrails, &SecurityTrails::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        status->osint->activeThreads++;
    }
    if(module.shodan){
        Shodan *shodan = new Shodan(ui->lineEditTarget->text());
        QThread *cThread = new QThread(this);
        shodan->Enumerator(cThread);
        shodan->moveToThread(cThread);
        //...
        connect(shodan, &Shodan::scanResults, this, &Osint::scanResults);
        connect(shodan, &Shodan::errorLog, this, &Osint::onErrorLog);
        connect(shodan, &Shodan::infoLog, this, &Osint::onInfoLog);
        connect(cThread, &QThread::finished, this, &Osint::onEnumerationComplete);
        connect(cThread, &QThread::finished, shodan, &Shodan::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        status->osint->activeThreads++;
    }
    if(module.threatbook){
        ThreatBook *threatbook = new ThreatBook(ui->lineEditTarget->text());
        QThread *cThread = new QThread(this);
        threatbook->Enumerator(cThread);
        threatbook->moveToThread(cThread);
        //...
        connect(threatbook, &ThreatBook::scanResults, this, &Osint::scanResults);
        connect(threatbook, &ThreatBook::errorLog, this, &Osint::onErrorLog);
        connect(threatbook, &ThreatBook::infoLog, this, &Osint::onInfoLog);
        connect(cThread, &QThread::finished, this, &Osint::onEnumerationComplete);
        connect(cThread, &QThread::finished, threatbook, &ThreatBook::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        status->osint->activeThreads++;
    }
    if(module.whoisxmlapi){
        WhoisXmlApi *whoisxmlapi = new WhoisXmlApi(ui->lineEditTarget->text());
        QThread *cThread = new QThread(this);
        whoisxmlapi->Enumerator(cThread);
        whoisxmlapi->moveToThread(cThread);
        //...
        connect(whoisxmlapi, &WhoisXmlApi::scanResults, this, &Osint::scanResults);
        connect(whoisxmlapi, &WhoisXmlApi::errorLog, this, &Osint::onErrorLog);
        connect(whoisxmlapi, &WhoisXmlApi::infoLog, this, &Osint::onInfoLog);
        connect(cThread, &QThread::finished, this, &Osint::onEnumerationComplete);
        connect(cThread, &QThread::finished, whoisxmlapi, &WhoisXmlApi::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        status->osint->activeThreads++;
    }
    if(module.zetalytics){
        ZETAlytics *zetalytics = new ZETAlytics(ui->lineEditTarget->text());
        QThread *cThread = new QThread(this);
        zetalytics->Enumerator(cThread);
        zetalytics->moveToThread(cThread);
        //...
        connect(zetalytics, &ZETAlytics::scanResults, this, &Osint::scanResults);
        connect(zetalytics, &ZETAlytics::errorLog, this, &Osint::onErrorLog);
        connect(zetalytics, &ZETAlytics::infoLog, this, &Osint::onInfoLog);
        connect(cThread, &QThread::finished, this, &Osint::onEnumerationComplete);
        connect(cThread, &QThread::finished, zetalytics, &ZETAlytics::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        status->osint->activeThreads++;
    }
    if(module.zoomeye){
        ZoomEye *zoomeye = new ZoomEye(ui->lineEditTarget->text());
        QThread *cThread = new QThread(this);
        zoomeye->Enumerator(cThread);
        zoomeye->moveToThread(cThread);
        //...
        connect(zoomeye, &ZoomEye::scanResults, this, &Osint::scanResults);
        connect(zoomeye, &ZoomEye::errorLog, this, &Osint::onErrorLog);
        connect(zoomeye, &ZoomEye::infoLog, this, &Osint::onInfoLog);
        connect(cThread, &QThread::finished, this, &Osint::onEnumerationComplete);
        connect(cThread, &QThread::finished, zoomeye, &ZoomEye::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        status->osint->activeThreads++;
    }
    if(module.ipapi){
        IpApi *ipapi = new IpApi(ui->lineEditTarget->text());
        QThread *cThread = new QThread(this);
        ipapi->Enumerator(cThread);
        ipapi->moveToThread(cThread);
        //...
        connect(ipapi, &IpApi::scanResults, this, &Osint::scanResults);
        connect(ipapi, &IpApi::errorLog, this, &Osint::onErrorLog);
        connect(ipapi, &IpApi::infoLog, this, &Osint::onInfoLog);
        connect(cThread, &QThread::finished, this, &Osint::onEnumerationComplete);
        connect(cThread, &QThread::finished, ipapi, &IpApi::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        status->osint->activeThreads++;
    }
    if(module.ask){
        Ask *ask = new Ask(ui->lineEditTarget->text());
        QThread *cThread = new QThread(this);
        ask->Enumerator(cThread);
        ask->moveToThread(cThread);
        //...
        connect(ask, &Ask::scanResults, this, &Osint::scanResults);
        connect(ask, &Ask::errorLog, this, &Osint::onErrorLog);
        connect(ask, &Ask::infoLog, this, &Osint::onInfoLog);
        connect(cThread, &QThread::finished, this, &Osint::onEnumerationComplete);
        connect(cThread, &QThread::finished, ask, &Ask::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        status->osint->activeThreads++;
    }
    if(module.baidu){
        Baidu *baidu = new Baidu(ui->lineEditTarget->text());
        QThread *cThread = new QThread(this);
        baidu->Enumerator(cThread);
        baidu->moveToThread(cThread);
        //...
        connect(baidu, &Baidu::scanResults, this, &Osint::scanResults);
        connect(baidu, &Baidu::errorLog, this, &Osint::onErrorLog);
        connect(baidu, &Baidu::infoLog, this, &Osint::onInfoLog);
        connect(cThread, &QThread::finished, this, &Osint::onEnumerationComplete);
        connect(cThread, &QThread::finished, baidu, &Baidu::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        status->osint->activeThreads++;
    }
    if(module.dogpile){
        DogPile *dogpile = new DogPile(ui->lineEditTarget->text());
        QThread *cThread = new QThread(this);
        dogpile->Enumerator(cThread);
        dogpile->moveToThread(cThread);
        //...
        connect(dogpile, &DogPile::scanResults, this, &Osint::scanResults);
        connect(dogpile, &DogPile::errorLog, this, &Osint::onErrorLog);
        connect(dogpile, &DogPile::infoLog, this, &Osint::onInfoLog);
        connect(cThread, &QThread::finished, this, &Osint::onEnumerationComplete);
        connect(cThread, &QThread::finished, dogpile, &DogPile::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        status->osint->activeThreads++;
    }
    if(module.duckduckgo){
        DuckDuckGo *duckduckgo = new DuckDuckGo(ui->lineEditTarget->text());
        QThread *cThread = new QThread(this);
        duckduckgo->Enumerator(cThread);
        duckduckgo->moveToThread(cThread);
        //...
        connect(duckduckgo, &DuckDuckGo::scanResults, this, &Osint::scanResults);
        connect(duckduckgo, &DuckDuckGo::errorLog, this, &Osint::onErrorLog);
        connect(duckduckgo, &DuckDuckGo::infoLog, this, &Osint::onInfoLog);
        connect(cThread, &QThread::finished, this, &Osint::onEnumerationComplete);
        connect(cThread, &QThread::finished, duckduckgo, &DuckDuckGo::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        status->osint->activeThreads++;
    }
    if(module.exalead){
        Exalead *exalead = new Exalead(ui->lineEditTarget->text());
        QThread *cThread = new QThread(this);
        exalead->Enumerator(cThread);
        exalead->moveToThread(cThread);
        //...
        connect(exalead, &Exalead::scanResults, this, &Osint::scanResults);
        connect(exalead, &Exalead::errorLog, this, &Osint::onErrorLog);
        connect(exalead, &Exalead::infoLog, this, &Osint::onInfoLog);
        connect(cThread, &QThread::finished, this, &Osint::onEnumerationComplete);
        connect(cThread, &QThread::finished, exalead, &Exalead::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        status->osint->activeThreads++;
    }
    if(module.trello){
        Trello *trello = new Trello(ui->lineEditTarget->text());
        QThread *cThread = new QThread(this);
        trello->Enumerator(cThread);
        trello->moveToThread(cThread);
        //...
        connect(trello, &Trello::scanResults, this, &Osint::scanResults);
        connect(trello, &Trello::errorLog, this, &Osint::onErrorLog);
        connect(trello, &Trello::infoLog, this, &Osint::onInfoLog);
        connect(cThread, &QThread::finished, this, &Osint::onEnumerationComplete);
        connect(cThread, &QThread::finished, trello, &Trello::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        status->osint->activeThreads++;
    }
    if(module.yahoo){
        Yahoo *yahoo = new Yahoo(ui->lineEditTarget->text());
        QThread *cThread = new QThread(this);
        yahoo->Enumerator(cThread);
        yahoo->moveToThread(cThread);
        //...
        connect(yahoo, &Yahoo::scanResults, this, &Osint::scanResults);
        connect(yahoo, &Yahoo::errorLog, this, &Osint::onErrorLog);
        connect(yahoo, &Yahoo::infoLog, this, &Osint::onInfoLog);
        connect(cThread, &QThread::finished, this, &Osint::onEnumerationComplete);
        connect(cThread, &QThread::finished, yahoo, &Yahoo::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        status->osint->activeThreads++;
    }
    if(module.bing){
        Bing *bing = new Bing(ui->lineEditTarget->text());
        QThread *cThread = new QThread(this);
        bing->Enumerator(cThread);
        bing->moveToThread(cThread);
        //...
        connect(bing, &Bing::scanResults, this, &Osint::scanResults);
        connect(bing, &Bing::errorLog, this, &Osint::onErrorLog);
        connect(bing, &Bing::infoLog, this, &Osint::onInfoLog);
        connect(cThread, &QThread::finished, this, &Osint::onEnumerationComplete);
        connect(cThread, &QThread::finished, bing, &Bing::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        status->osint->activeThreads++;
    }
    if(module.sitedossier){
        SiteDossier *sitedossier = new SiteDossier(ui->lineEditTarget->text());
        QThread *cThread = new QThread(this);
        sitedossier->Enumerator(cThread);
        sitedossier->moveToThread(cThread);
        //...
        connect(sitedossier, &SiteDossier::scanResults, this, &Osint::scanResults);
        connect(sitedossier, &SiteDossier::errorLog, this, &Osint::onErrorLog);
        connect(sitedossier, &SiteDossier::infoLog, this, &Osint::onInfoLog);
        connect(cThread, &QThread::finished, this, &Osint::onEnumerationComplete);
        connect(cThread, &QThread::finished, sitedossier, &SiteDossier::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        status->osint->activeThreads++;
    }
    if(module.pagesinventory){
        PagesInventory *pagesinventory = new PagesInventory(ui->lineEditTarget->text());
        QThread *cThread = new QThread(this);
        pagesinventory->Enumerator(cThread);
        pagesinventory->moveToThread(cThread);
        //...
        connect(pagesinventory, &PagesInventory::scanResults, this, &Osint::scanResults);
        connect(pagesinventory, &PagesInventory::errorLog, this, &Osint::onErrorLog);
        connect(pagesinventory, &PagesInventory::infoLog, this, &Osint::onInfoLog);
        connect(cThread, &QThread::finished, this, &Osint::onEnumerationComplete);
        connect(cThread, &QThread::finished, pagesinventory, &PagesInventory::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        status->osint->activeThreads++;
    }
    if(module.viewDns){
        ViewDns *viewdns = new ViewDns(ui->lineEditTarget->text());
        QThread *cThread = new QThread(this);
        viewdns->Enumerator(cThread);
        viewdns->moveToThread(cThread);
        //...
        connect(viewdns, &ViewDns::scanResults, this, &Osint::scanResults);
        connect(viewdns, &ViewDns::errorLog, this, &Osint::onErrorLog);
        connect(viewdns, &ViewDns::infoLog, this, &Osint::onInfoLog);
        connect(cThread, &QThread::finished, this, &Osint::onEnumerationComplete);
        connect(cThread, &QThread::finished, viewdns, &ViewDns::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        status->osint->activeThreads++;
    }
    ///
    /// after starting all choosen enumerations...
    ///
    if(status->osint->activeThreads)
    {
        ui->buttonStart->setDisabled(true);
        ui->buttonStop->setEnabled(true);
    }
}

void Osint::ResumeScan(){

}

void Osint::scanResults(QString subdomain){
    //m_model->appendRow(new QStandardItem(subdomain));
    int prevSize = m_results.count();
    m_results.insert(subdomain);
    if(m_results.count() > prevSize)
        m_model->appendRow(new QStandardItem(subdomain));
}


void Osint::onEnumerationComplete(){
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
    ///
    /// clear the results...
    ///
    m_model->clear();
    ui->labelResultsCount->clear();
    m_model->setHorizontalHeaderLabels({"Subdomain", "IpAddress"});
    ///
    /// clear the progressbar...
    ui->progressBar->clearMask();
    ui->progressBar->reset();
    ui->progressBar->hide();
    ///
    /// hide the action button...
    ///
    ui->buttonAction->hide();
    ///
    /// hide the filter...
    ///
    ui->buttonFilter->hide();
    ui->lineEditFilter->hide();
    ui->comboBoxFilter->hide();
}

void Osint::onShowFilter(bool show){
    if(show){
        ui->buttonFilter->show();
        ui->lineEditFilter->show();
        ui->comboBoxFilter->show();
    }
    else{
        ui->buttonFilter->hide();
        ui->lineEditFilter->hide();
        ui->comboBoxFilter->hide();
    }
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
    connect(&actionShowFilter, &QAction::triggered, this, [=](){this->onShowFilter(true);});
    connect(&actionHideFilter, &QAction::triggered, this, [=](){this->onShowFilter(false);});
    ///
    /// SAVE...
    ///
    connect(&actionSaveSubdomains, &QAction::triggered, this, [=](){this->onSaveResults(CHOICE::susbdomains);});
    connect(&actionSaveIpAddresses, &QAction::triggered, this, [=](){this->onSaveResults(CHOICE::ipaddress);});
    connect(&actionSaveAll, &QAction::triggered, this, [=](){this->onSaveResults(CHOICE::all);});
    ///
    /// COPY...
    ///
    connect(&actionCopySubdomains, &QAction::triggered, this, [=](){this->onCopyResults(CHOICE::susbdomains);});
    connect(&actionCopyIpAddresses, &QAction::triggered, this, [=](){this->onCopyResults(CHOICE::ipaddress);});
    connect(&actionCopyAll, &QAction::triggered, this, [=](){this->onCopyResults(CHOICE::all);});
    ///
    /// SUBDOMAINS AND IPS...
    ///
    connect(&actionSendToIp, &QAction::triggered, this, [=](){emit sendIpAddressesToIp(ENGINE::OSINT, CHOICE::ipaddress); emit changeTabToIp();});
    connect(&actionSendToOsint, &QAction::triggered, this, [=](){emit sendSubdomainsToOsint(ENGINE::OSINT, CHOICE::susbdomains); emit changeTabToOsint();});
    connect(&actionSendToBrute, &QAction::triggered, this, [=](){emit sendSubdomainsToBrute(ENGINE::OSINT, CHOICE::susbdomains); emit changeTabToBrute();});
    connect(&actionSendToActive, &QAction::triggered, this, [=](){emit sendSubdomainsToActive(ENGINE::OSINT, CHOICE::susbdomains); emit changeTabToActive();});
    connect(&actionSendToRecords, &QAction::triggered, this, [=](){emit sendSubdomainsToRecord(ENGINE::OSINT, CHOICE::susbdomains); emit changeTabToRecords();});

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
    ///
    /// check if there are results available else dont show the context menu...
    ///
    if(m_model->rowCount() < 1){
        return;
    }
    ///
    /// getting the position of the action button to place the context menu and
    /// showing the context menu right by the side of the action button...
    ///
    QPoint pos = ui->buttonAction->mapToGlobal(QPoint(0,0));
    pos = QPoint(pos.x()+65, pos.y());
    ///
    /// creating the context menu...
    ///
    QMenu *Menu = new QMenu(this);
    QMenu *saveMenu = new QMenu(this);
    QMenu *copyMenu = new QMenu(this);
    Menu->setAttribute(Qt::WA_DeleteOnClose, true);
    //...
    saveMenu->setTitle("Save");
    copyMenu->setTitle("Copy");
    ///
    /// ADDING ACTIONS TO THE CONTEXT MENU...
    ///
    saveMenu->addAction(&actionSaveSubdomains);
    saveMenu->addAction(&actionSaveIpAddresses);
    saveMenu->addAction(&actionSaveAll);
    //...
    copyMenu->addAction(&actionCopySubdomains);
    copyMenu->addAction(&actionCopyIpAddresses);
    copyMenu->addAction(&actionCopyAll);
    ///
    /// ....
    ///
    Menu->addAction(&actionClearResults);
    if(ui->lineEditFilter->isHidden() && ui->buttonFilter->isHidden()){
        Menu->addAction(&actionShowFilter);
    }else{
        Menu->addAction(&actionHideFilter);
    }
    Menu->addSeparator();
    //...
    Menu->addMenu(copyMenu);
    Menu->addMenu(saveMenu);
    //...
    Menu->addSeparator();
    Menu->addAction(&actionSendToIp);
    Menu->addAction(&actionSendToOsint);
    Menu->addAction(&actionSendToBrute);
    Menu->addAction(&actionSendToActive);
    Menu->addAction(&actionSendToRecords);
    ///
    /// showing the context menu...
    ///
    Menu->exec(pos);
}

void Osint::on_tableViewResults_customContextMenuRequested(const QPoint &pos){
    Q_UNUSED(pos);
    ///
    /// check if user right clicked on items else dont show the context menu...
    ///
    if(!ui->tableViewResults->selectionModel()->isSelected(ui->tableViewResults->currentIndex())){
        return;
    }
    selectionModel = ui->tableViewResults->selectionModel();
    ///
    /// creating the context menu...
    ///
    QMenu *Menu = new QMenu(this);
    Menu->setAttribute(Qt::WA_DeleteOnClose, true);
    ///
    /// ...
    ///
    Menu->addAction(&actionCopy);
    Menu->addAction(&actionSave);
    Menu->addSeparator();
    Menu->addAction(&actionOpenInBrowser);
    Menu->addSeparator();
    Menu->addAction(&actionSendToIp_c);
    Menu->addAction(&actionSendToOsint_c);
    Menu->addAction(&actionSendToBrute_c);
    Menu->addAction(&actionSendToActive_c);
    Menu->addAction(&actionSendToRecords_c);
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

void Osint::onSaveResults(CHOICE choice){
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
    ///
    /// variable declarations...
    ///
    QSet<QString> itemSet;
    QString item;
    ///
    /// choice of item to save...
    ///
    switch(choice){
    case CHOICE::susbdomains:
        for(int i = 0; i != m_proxyModel->rowCount(); ++i)
        {
            item = m_proxyModel->data(m_proxyModel->index(i, 0)).toString().append(NEWLINE);
            if(!itemSet.contains(item)){
                itemSet.insert(item);
                file.write(item.toUtf8());
            }
        }
        break;
    case CHOICE::ipaddress:
        for(int i = 0; i != m_proxyModel->rowCount(); ++i)
        {
            item = m_proxyModel->data(m_proxyModel->index(i, 1)).toString().append(NEWLINE);
            if(!itemSet.contains(item)){
                itemSet.insert(item);
                file.write(item.toUtf8());
            }
        }
        break;
    case CHOICE::all:
        for(int i = 0; i != m_proxyModel->rowCount(); ++i)
        {
            item = m_proxyModel->data(m_proxyModel->index(i, 0)).toString()+":"+m_proxyModel->data(m_proxyModel->index(i, 1)).toString().append(NEWLINE);
            if(!itemSet.contains(item)){
                itemSet.insert(item);
                file.write(item.toUtf8());
            }
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
    QSet<QString> itemSet;
    QString data;
    QString item;
    ///
    /// ...
    ///
    QFile file(filename);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    if(file.isOpen())
    {
        foreach(const QModelIndex &index, selectionModel->selectedIndexes()){
            item = index.data().toString();
            if(!itemSet.contains(item)){
                itemSet.insert(item);
                data.append(item.append(NEWLINE));
            }
        }
        file.write(data.toUtf8());
        file.close();
    }
}

void Osint::onCopyResults(CHOICE choice){
    ///
    /// variable declaration...
    ///
    QClipboard *clipboard = QGuiApplication::clipboard();
    QString clipboardData;
    QSet<QString> itemSet;
    QString item;
    ///
    /// type of item to save...
    ///
    switch(choice){
    case CHOICE::susbdomains:
        for(int i = 0; i != m_proxyModel->rowCount(); ++i)
        {
            item = m_proxyModel->data(m_proxyModel->index(i, 0)).toString().append(NEWLINE);
            if(!itemSet.contains(item)){
                itemSet.insert(item);
                clipboardData.append(item);
            }
        }
        break;
    case CHOICE::ipaddress:
        for(int i = 0; i != m_proxyModel->rowCount(); ++i)
        {
            item = m_proxyModel->data(m_proxyModel->index(i, 1)).toString().append(NEWLINE);
            if(!itemSet.contains(item)){
                itemSet.insert(item);
                clipboardData.append(item);
            }
        }
        break;
    case CHOICE::all:
        for(int i = 0; i != m_proxyModel->rowCount(); ++i)
        {
            item = m_proxyModel->data(m_proxyModel->index(i, 0)).toString()+"|"+m_proxyModel->data(m_proxyModel->index(i, 1)).toString().append(NEWLINE);
            if(!itemSet.contains(item)){
                itemSet.insert(item);
                clipboardData.append(item);
            }
        }
        break;
    default:
        break;
    }
    clipboard->setText(clipboardData);
}

void Osint::onCopyResults(QItemSelectionModel *selectionModel){
    QClipboard *clipboard = QGuiApplication::clipboard();
    QSet<QString> itemSet;
    QString data;
    QString item;
    ///
    /// ...
    ///
    foreach(const QModelIndex &index, selectionModel->selectedIndexes())
    {
        item = index.data().toString();
        if(!itemSet.contains(item)){
            itemSet.insert(item);
            data.append(item.append(NEWLINE));
        }
    }
    clipboard->setText(data);
}

void Osint::on_buttonFilter_clicked(){
    QString filterKeyword = ui->lineEditFilter->text();
    m_proxyModel->setFilterKeyColumn(ui->comboBoxFilter->currentIndex());
    m_proxyModel->setFilterRegExp(filterKeyword);
    ui->tableViewResults->setModel(m_proxyModel);
}
