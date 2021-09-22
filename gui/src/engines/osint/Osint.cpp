#include "Osint.h"
#include "ui_Osint.h"
//...
#include "src/utils/Config.h"
#include "src/dialogs/OsintConfigDialog.h"
//...
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
#include "src/engines/osint/modules/site/Dnsdumpster.h"
#include "src/engines/osint/modules/site/Netcraft.h"
#include "src/engines/osint/modules/site/Suip.h"
#include "src/engines/osint/modules/site/Pkey.h"
#include "src/engines/osint/modules/site/Rapiddns.h"
#include "src/engines/osint/modules/cert/Crtsh.h"
#include "src/engines/osint/modules/cert/GoogleCert.h"
#include "src/engines/osint/modules/cert/Certspotter.h"

#define TRUE "1"
#define FALSE "0"

/*
 * https://dnschecker.org/all-tools.php
 * https://suip.biz/
 * https://viewdns.info/api/
 * https://rapiddns.io/tools
 * pkey.in
 * https://skrapp.io/api
 * https://hunter.io/bulk-finders
 *
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

/*
 * url = "https://api.securitytrails.com/v1/ips/list"
querystring = {"apikey":"your_api_key_here","page":"1"}
payload = "{\"query\":\"ptr_part = 'cloudflare.com'\"}"
response = requests.request("POST", url, data=payload, params=querystring)
print(response.text)

*/

Osint::Osint(QWidget *parent, ResultsModel *resultsModel, Status *status):
    BaseClass(ENGINE::OSINT, resultsModel, status, parent),
    ui(new Ui::Osint),
    m_scanArguments(new osint::ScanArguments),
    m_scanResults(new osint::ScanResults)
{
    ui->setupUi(this);
    ///
    /// init...
    ///
    ui->targets->init("Targets");
    initBaseClass(ui->targets);
    scanConfig->name = tr("ScanConfig-Osint");
    ///
    /// ...
    ///
    currentPath = QDir::currentPath();
    ui->lineEditTarget->setPlaceholderText("eg. example.com");
    ui->lineEditNewProfile->setPlaceholderText("Enter New Profile's Name...");
    //...
    resultsModel->osint->setHorizontalHeaderLabels({"Subdomain", "IpAddress"});
    ui->tableViewResults->setModel(resultsModel->osint);
    //...
    ui->buttonPause->setDisabled(true);
    ui->buttonStop->setDisabled(true);
    ///
    /// hide widgets...
    ///
    ui->progressBar->hide();
    ui->targets->hide();
    ui->frameProfiles->hide();
    ///
    /// equally seperate the widgets...
    ///
    ui->splitter->setSizes(QList<int>() << static_cast<int>((this->width() * 0.50))
                                        << static_cast<int>((this->width() * 0.50)));
    //...
    initProfiles();
}
Osint::~Osint(){
    delete m_scanArguments;
    delete m_scanResults;
    delete ui;
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
}
void Osint::on_lineEditTarget_returnPressed(){
    on_buttonStart_clicked();
}

void Osint::on_buttonStop_clicked(){
    emit stopScan();
    sendStatus("[*] Stopping...");
}

void Osint::startScan(){
    if(ui->checkBox_engine_certspotter->isChecked())
    {
        Certspotter *certspotter = new Certspotter(ui->lineEditTarget->text());
        QThread *cThread = new QThread(this);
        certspotter->Enumerator(cThread);
        certspotter->moveToThread(cThread);
        //...
        connect(certspotter, &Certspotter::scanResults, this, &Osint::scanResults);
        connect(cThread, &QThread::finished, this, &Osint::onEnumerationComplete);
        connect(cThread, &QThread::finished, certspotter, &Certspotter::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        activeThreads++;
    }
    if(ui->checkBox_engine_otx->isChecked())
    {
        Otx *otx = new Otx(ui->lineEditTarget->text());
        QThread *cThread = new QThread(this);
        otx->Enumerator(cThread);
        otx->moveToThread(cThread);
        //...
        connect(otx, &Otx::scanResults, this, &Osint::scanResults);
        connect(cThread, &QThread::finished, this, &Osint::onEnumerationComplete);
        connect(cThread, &QThread::finished, otx, &Otx::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        activeThreads++;
    }
    if(ui->checkBox_engine_sublist3r->isChecked())
    {
        Sublist3r *sublist3r = new Sublist3r(ui->lineEditTarget->text());
        QThread *cThread = new QThread(this);
        sublist3r->Enumerator(cThread);
        sublist3r->moveToThread(cThread);
        //...
        connect(sublist3r, &Sublist3r::scanResults, this, &Osint::scanResults);
        connect(cThread, &QThread::finished, this, &Osint::onEnumerationComplete);
        connect(cThread, &QThread::finished, sublist3r, &Sublist3r::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        activeThreads++;
    }
    if(ui->checkBox_engine_threatminer->isChecked())
    {
        Threatminer *threatminer = new Threatminer(ui->lineEditTarget->text());
        QThread *cThread = new QThread(this);
        threatminer->Enumerator(cThread);
        threatminer->moveToThread(cThread);
        //...
        connect(threatminer, &Threatminer::scanResults, this, &Osint::scanResults);
        connect(cThread, &QThread::finished, this, &Osint::onEnumerationComplete);
        connect(cThread, &QThread::finished, threatminer, &Threatminer::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        activeThreads++;
    }
    if(ui->checkBox_engine_threatcrowd->isChecked())
    {
        Threatcrowd *threatcrowd = new Threatcrowd(ui->lineEditTarget->text());
        QThread *cThread = new QThread(this);
        threatcrowd->Enumerator(cThread);
        threatcrowd->moveToThread(cThread);
        //...
        connect(threatcrowd, &Threatcrowd::scanResults, this, &Osint::scanResults);
        connect(cThread, &QThread::finished, this, &Osint::onEnumerationComplete);
        connect(cThread, &QThread::finished, threatcrowd, &Threatcrowd::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        activeThreads++;
    }
    if(ui->checkBox_engine_hackertarget->isChecked())
    {
        Hackertarget *hackertarget = new Hackertarget(ui->lineEditTarget->text());
        QThread *cThread = new QThread(this);
        hackertarget->Enumerator(cThread);
        hackertarget->moveToThread(cThread);
        //...
        connect(hackertarget, &Hackertarget::scanResults, this, &Osint::scanResults);
        connect(cThread, &QThread::finished, this, &Osint::onEnumerationComplete);
        connect(cThread, &QThread::finished, hackertarget, &Hackertarget::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        activeThreads++;
    }
    if(ui->checkBox_engine_dnsbufferoverrun->isChecked())
    {
        Dnsbufferoverun *dnsbufferoverun = new Dnsbufferoverun(ui->lineEditTarget->text());
        QThread *cThread = new QThread(this);
        dnsbufferoverun->Enumerator(cThread);
        dnsbufferoverun->moveToThread(cThread);
        //...
        connect(dnsbufferoverun, &Dnsbufferoverun::scanResults, this, &Osint::scanResults);
        connect(cThread, &QThread::finished, this, &Osint::onEnumerationComplete);
        connect(cThread, &QThread::finished, dnsbufferoverun, &Dnsbufferoverun::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        activeThreads++;
    }
    if(ui->checkBox_engine_anubis->isChecked())
    {
        Anubis *anubis = new Anubis(ui->lineEditTarget->text());
        QThread *cThread = new QThread(this);
        anubis->Enumerator(cThread);
        anubis->moveToThread(cThread);
        //...
        connect(anubis, &Anubis::scanResults, this, &Osint::scanResults);
        connect(cThread, &QThread::finished, this, &Osint::onEnumerationComplete);
        connect(cThread, &QThread::finished, anubis, &Anubis::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        activeThreads++;
    }
    if(ui->checkBox_engine_projectdiscovery->isChecked())
    {
        Projectdiscovery *projectdiscovery = new Projectdiscovery(ui->lineEditTarget->text());
        QThread *cThread = new QThread(this);
        projectdiscovery->Enumerator(cThread);
        projectdiscovery->moveToThread(cThread);
        //...
        connect(projectdiscovery, &Projectdiscovery::scanResults, this, &Osint::scanResults);
        connect(cThread, &QThread::finished, this, &Osint::onEnumerationComplete);
        connect(cThread, &QThread::finished, projectdiscovery, &Projectdiscovery::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        activeThreads++;
    }
    if(ui->checkBox_engine_spyse->isChecked())
    {
        Spyse *spyse = new Spyse(ui->lineEditTarget->text());
        QThread *cThread = new QThread(this);
        spyse->Enumerator(cThread);
        spyse->moveToThread(cThread);
        //...
        connect(spyse, &Spyse::scanResults, this, &Osint::scanResults);
        connect(cThread, &QThread::finished, this, &Osint::onEnumerationComplete);
        connect(cThread, &QThread::finished, spyse, &Spyse::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        activeThreads++;
    }
    if(ui->checkBox_engine_crtsh->isChecked())
    {
        Crtsh *crtsh = new Crtsh(ui->lineEditTarget->text());
        QThread *cThread = new QThread(this);
        crtsh->Enumerator(cThread);
        crtsh->moveToThread(cThread);
        //...
        connect(crtsh, &Crtsh::scanResults, this, &Osint::scanResults);
        connect(cThread, &QThread::finished, this, &Osint::onEnumerationComplete);
        connect(cThread, &QThread::finished, crtsh, &Crtsh::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        activeThreads++;
    }
    if(ui->checkBox_engine_dnsdumpster->isChecked())
    {
        Dnsdumpster *dnsdumpster = new Dnsdumpster(ui->lineEditTarget->text());
        QThread *cThread = new QThread(this);
        dnsdumpster->Enumerator(cThread);
        dnsdumpster->moveToThread(cThread);
        //...
        connect(dnsdumpster, &Dnsdumpster::scanResults, this, &Osint::scanResults);
        connect(cThread, &QThread::finished, this, &Osint::onEnumerationComplete);
        connect(cThread, &QThread::finished, dnsdumpster, &Dnsdumpster::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        activeThreads++;
    }
    if(ui->checkBox_engine_netcraft->isChecked())
    {
        Netcraft *netcraft = new Netcraft(ui->lineEditTarget->text());
        QThread *cThread = new QThread(this);
        netcraft->Enumerator(cThread);
        netcraft->moveToThread(cThread);
        //...
        connect(netcraft, &Netcraft::scanResults, this, &Osint::scanResults);
        connect(cThread, &QThread::finished, this, &Osint::onEnumerationComplete);
        connect(cThread, &QThread::finished, netcraft, &Netcraft::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        activeThreads++;
    }
    if(ui->checkBox_engine_suip->isChecked())
    {
        Suip *suip = new Suip(ui->lineEditTarget->text());
        QThread *cThread = new QThread(this);
        suip->Enumerator(cThread);
        suip->moveToThread(cThread);
        //...
        connect(suip, &Suip::scanResults, this, &Osint::scanResults);
        connect(cThread, &QThread::finished, this, &Osint::onEnumerationComplete);
        connect(cThread, &QThread::finished, suip, &Suip::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        activeThreads++;
    }
    if(ui->checkBox_engine_pkey->isChecked()){
        Pkey *pkey = new Pkey(ui->lineEditTarget->text());
        QThread *cThread = new QThread(this);
        pkey->Enumerator(cThread);
        pkey->moveToThread(cThread);
        //...
        connect(pkey, &Pkey::scanResults, this, &Osint::scanResults);
        connect(cThread, &QThread::finished, this, &Osint::onEnumerationComplete);
        connect(cThread, &QThread::finished, pkey, &Pkey::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        activeThreads++;
    }
    if(ui->checkBox_engine_rapiddns->isChecked()){
        Rapiddns *rapiddns = new Rapiddns(ui->lineEditTarget->text());
        QThread *cThread = new QThread(this);
        rapiddns->Enumerator(cThread);
        rapiddns->moveToThread(cThread);
        //...
        connect(rapiddns, &Rapiddns::scanResults, this, &Osint::scanResults);
        connect(cThread, &QThread::finished, this, &Osint::onEnumerationComplete);
        connect(cThread, &QThread::finished, rapiddns, &Rapiddns::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        activeThreads++;
    }
    if(ui->checkBox_engine_googleCert->isChecked()){
        GoogleCert *googlecert = new GoogleCert(ui->lineEditTarget->text());
        QThread *cThread = new QThread(this);
        googlecert->Enumerator(cThread);
        googlecert->moveToThread(cThread);
        //...
        connect(googlecert, &GoogleCert::scanResults, this, &Osint::scanResults);
        connect(cThread, &QThread::finished, this, &Osint::onEnumerationComplete);
        connect(cThread, &QThread::finished, googlecert, &GoogleCert::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        activeThreads++;
    }
    if(ui->checkBox_engine_omnisint->isChecked()){
        Omnisint *omnisint = new Omnisint(ui->lineEditTarget->text());
        QThread *cThread = new QThread(this);
        omnisint->Enumerator(cThread);
        omnisint->moveToThread(cThread);
        //...
        connect(omnisint, &Omnisint::scanResults, this, &Osint::scanResults);
        connect(cThread, &QThread::finished, this, &Osint::onEnumerationComplete);
        connect(cThread, &QThread::finished, omnisint, &Omnisint::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        activeThreads++;
    }
    if(ui->checkBox_engine_qwant->isChecked()){
        Qwant *qwant = new Qwant(ui->lineEditTarget->text());
        QThread *cThread = new QThread(this);
        qwant->Enumerator(cThread);
        qwant->moveToThread(cThread);
        //...
        connect(qwant, &Qwant::scanResults, this, &Osint::scanResults);
        connect(cThread, &QThread::finished, this, &Osint::onEnumerationComplete);
        connect(cThread, &QThread::finished, qwant, &Qwant::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        activeThreads++;
    }
    if(ui->checkBox_engine_virustotalapi->isChecked()){
        VirusTotal *virustotal = new VirusTotal(ui->lineEditTarget->text());
        QThread *cThread = new QThread(this);
        virustotal->Enumerator(cThread);
        virustotal->moveToThread(cThread);
        //...
        connect(virustotal, &VirusTotal::scanResults, this, &Osint::scanResults);
        connect(cThread, &QThread::finished, this, &Osint::onEnumerationComplete);
        connect(cThread, &QThread::finished, virustotal, &VirusTotal::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        activeThreads++;
    }
    if(ui->checkBox_engine_urlscan->isChecked()){
        Urlscan *urlscan = new Urlscan(ui->lineEditTarget->text());
        QThread *cThread = new QThread(this);
        urlscan->Enumerator(cThread);
        urlscan->moveToThread(cThread);
        //...
        connect(urlscan, &Urlscan::scanResults, this, &Osint::scanResults);
        connect(cThread, &QThread::finished, this, &Osint::onEnumerationComplete);
        connect(cThread, &QThread::finished, urlscan, &Urlscan::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        activeThreads++;
    }
    if(ui->checkBox_engine_waybackmachine->isChecked()){
        Waybackmachine *waybackmachine = new Waybackmachine(ui->lineEditTarget->text());
        QThread *cThread = new QThread(this);
        waybackmachine->Enumerator(cThread);
        waybackmachine->moveToThread(cThread);
        //...
        connect(waybackmachine, &Waybackmachine::scanResults, this, &Osint::scanResults);
        connect(cThread, &QThread::finished, this, &Osint::onEnumerationComplete);
        connect(cThread, &QThread::finished, waybackmachine, &Waybackmachine::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        activeThreads++;
    }
    if(ui->checkBox_engine_archiveToday->isChecked()){
        ArchiveToday *archivetoday = new ArchiveToday(ui->lineEditTarget->text());
        QThread *cThread = new QThread(this);
        archivetoday->Enumerator(cThread);
        archivetoday->moveToThread(cThread);
        //...
        connect(archivetoday, &ArchiveToday::scanResults, this, &Osint::scanResults);
        connect(cThread, &QThread::finished, this, &Osint::onEnumerationComplete);
        connect(cThread, &QThread::finished, archivetoday, &ArchiveToday::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        activeThreads++;
    }
    ///
    /// after starting all choosen enumerations...
    ///
    if(activeThreads)
    {
        ui->buttonStart->setDisabled(true);
        ui->buttonStop->setEnabled(true);
    }
}

void Osint::scanResults(QString subdomain){
    //resultsModel->osint->appendRow(new QStandardItem(subdomain));
    int prevSize = m_results.count();
    m_results.insert(subdomain);
    if(m_results.count() > prevSize)
        resultsModel->osint->appendRow(new QStandardItem(subdomain));
}


void Osint::onEnumerationComplete(){
    ///
    /// check if no active thread...
    ///
    activeThreads--;
    if(activeThreads)
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
    logs("[END] Enumeration Complete!\n");
}

void Osint::on_buttonClearResults_clicked(){
    // clear subdomains...
    if(ui->tabWidgetResults->currentIndex() == 0){
        resultsModel->osint->clear();
        ui->labelResultsCount->clear();
        m_results.clear();
        m_subdomainsCount = 0;
        //...
        resultsModel->osint->setHorizontalHeaderLabels({"Subdomain Name", "IpAddress"});
    }
    // clear logs...
    else{
        ui->listWidgetLogs->clear();
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


void Osint::initProfiles(){
    int size = Config::generalConfig().beginReadArray("Osint-Profiles");
    for(int i = 0; i < size; i++){
        Config::generalConfig().setArrayIndex(i);
        ui->comboBoxProfiles->addItem(Config::generalConfig().value("value").toString());
    }
    Config::generalConfig().endArray();
}

void Osint::on_checkBoxUseProfiles_clicked(bool checked){
    if(checked){
        ui->frameProfiles->show();
    }else{
        ui->frameProfiles->hide();
    }
}

void Osint::on_buttonLoadProfile_clicked(){
    ///
    /// opening the file containing the profiles...
    ///
    QSettings settings(currentPath+"/profiles.ini", QSettings::IniFormat);
    settings.beginGroup(ui->comboBoxProfiles->currentText());
    ///
    /// reading from file...
    ///
    if(settings.value(OSINT_THREATMINER).toString() == TRUE){
        ui->checkBox_engine_threatminer->setChecked(true);
    }else{
        ui->checkBox_engine_threatminer->setChecked(false);
    }
    if(settings.value(OSINT_SHODAN).toString() == TRUE){
        ui->checkBox_engine_shodan->setChecked(true);
    }else{
        ui->checkBox_engine_shodan->setChecked(false);
    }
    if(settings.value(OSINT_BING).toString() == TRUE){
        ui->checkBox_engine_bing->setChecked(true);
    }else{
        ui->checkBox_engine_bing->setChecked(false);
    }
    if(settings.value(OSINT_GITHUB).toString() == TRUE){
        ui->checkBox_engine_github->setChecked(true);
    }else{
        ui->checkBox_engine_github->setChecked(false);
    }
    if(settings.value(OSINT_CENSYS).toString() == TRUE){
        ui->checkBox_engine_censys->setChecked(true);
    }else{
        ui->checkBox_engine_censys->setChecked(false);
    }
    if(settings.value(OSINT_SECURITYTRAILS).toString() == TRUE){
        ui->checkBox_engine_securitytrails->setChecked(true);
    }else{
        ui->checkBox_engine_securitytrails->setChecked(false);
    }
    if(settings.value(OSINT_CLOUDFLARE).toString() == TRUE){
        ui->checkBox_engine_cloudflare->setChecked(true);
    }else{
        ui->checkBox_engine_cloudflare->setChecked(false);
    }
    if(settings.value(OSINT_INTELX).toString() == TRUE){
        ui->checkBox_engine_intelx->setChecked(true);
    }else{
        ui->checkBox_engine_intelx->setChecked(false);
    }
    if(settings.value(OSINT_GOOGLE).toString() == TRUE){
        ui->checkBox_engine_google->setChecked(true);
    }else{
        ui->checkBox_engine_google->setChecked(false);
    }
    if(settings.value(OSINT_CERTSPOTTER).toString() == TRUE){
        ui->checkBox_engine_certspotter->setChecked(true);
    }else{
        ui->checkBox_engine_certspotter->setChecked(false);
    }
    if(settings.value(OSINT_CRTSH).toString() == TRUE){
        ui->checkBox_engine_crtsh->setChecked(true);
    }else{
        ui->checkBox_engine_crtsh->setChecked(false);
    }
    if(settings.value(OSINT_DOGPILE).toString() == TRUE){
        ui->checkBox_engine_dogpile->setChecked(true);
    }else{
        ui->checkBox_engine_dogpile->setChecked(false);
    }
    if(settings.value(OSINT_DUCKDUCKGO).toString() == TRUE){
        ui->checkBox_engine_duckduckgo->setChecked(true);
    }else{
        ui->checkBox_engine_duckduckgo->setChecked(false);
    }
    if(settings.value(OSINT_EXALEAD).toString() == TRUE){
        ui->checkBox_engine_exalead->setChecked(true);
    }else{
        ui->checkBox_engine_exalead->setChecked(false);
    }
    if(settings.value(OSINT_HUNTERSEARCH).toString() == TRUE){
        ui->checkBox_engine_huntersearch->setChecked(true);
    }else{
        ui->checkBox_engine_huntersearch->setChecked(false);
    }
    if(settings.value(OSINT_NETCRAFT).toString() == TRUE){
        ui->checkBox_engine_netcraft->setChecked(true);
    }else{
        ui->checkBox_engine_netcraft->setChecked(false);
    }
    if(settings.value(OSINT_OTX).toString() == TRUE){
        ui->checkBox_engine_otx->setChecked(true);
    }else{
        ui->checkBox_engine_otx->setChecked(false);
    }
    if(settings.value(OSINT_SUIP).toString() == TRUE){
        ui->checkBox_engine_suip->setChecked(true);
    }else{
        ui->checkBox_engine_suip->setChecked(false);
    }
    if(settings.value(OSINT_TRELLO).toString() == TRUE){
        ui->checkBox_engine_trello->setChecked(true);
    }else{
        ui->checkBox_engine_trello->setChecked(false);
    }
    if(settings.value(OSINT_THREATCROWD).toString() == TRUE){
        ui->checkBox_engine_threatcrowd->setChecked(true);
    }else{
        ui->checkBox_engine_threatcrowd->setChecked(false);
    }
    if(settings.value(OSINT_DNSBUFFEROVERRUN).toString() == TRUE){
        ui->checkBox_engine_dnsbufferoverrun->setChecked(true);
    }else{
        ui->checkBox_engine_dnsbufferoverrun->setChecked(false);
    }
    if(settings.value(OSINT_HACKERTARGET).toString() == TRUE){
        ui->checkBox_engine_hackertarget->setChecked(true);
    }else{
        ui->checkBox_engine_hackertarget->setChecked(false);
    }
    if(settings.value(OSINT_PKEY).toString() == TRUE){
        ui->checkBox_engine_pkey->setChecked(true);
    }else{
        ui->checkBox_engine_pkey->setChecked(false);
    }
    if(settings.value(OSINT_WAYBACKMACHINE).toString() == TRUE){
        ui->checkBox_engine_waybackmachine->setChecked(true);
    }else{
        ui->checkBox_engine_waybackmachine->setChecked(false);
    }
    if(settings.value(OSINT_ASK).toString() == TRUE){
        ui->checkBox_engine_ask->setChecked(true);
    }else{
        ui->checkBox_engine_ask->setChecked(false);
    }
    if(settings.value(OSINT_BAIDU).toString() == TRUE){
        ui->checkBox_engine_baidu->setChecked(true);
    }else{
        ui->checkBox_engine_baidu->setChecked(false);
    }
    if(settings.value(OSINT_DNSDUMPSTER).toString() == TRUE){
        ui->checkBox_engine_dnsdumpster->setChecked(true);
    }else{
        ui->checkBox_engine_dnsdumpster->setChecked(false);
    }
    if(settings.value(OSINT_PASSIVEDNS).toString() == TRUE){
        ui->checkBox_engine_passivedns->setChecked(true);
    }else{
        ui->checkBox_engine_passivedns->setChecked(false);
    }
    if(settings.value(OSINT_YAHOO).toString() == TRUE){
        ui->checkBox_engine_yahoo->setChecked(true);
    }else{
        ui->checkBox_engine_yahoo->setChecked(false);
    }
    if(settings.value(OSINT_VIRUSTOTALAPI).toString() == TRUE){
        ui->checkBox_engine_virustotalapi->setChecked(true);
    }else{
        ui->checkBox_engine_virustotalapi->setChecked(false);
    }
    if(settings.value(OSINT_OMNISINT).toString() == TRUE){
        ui->checkBox_engine_omnisint->setChecked(true);
    }else{
        ui->checkBox_engine_omnisint->setChecked(false);
    }
    if(settings.value(OSINT_QWANT).toString() == TRUE){
        ui->checkBox_engine_qwant->setChecked(true);
    }
    else{
        ui->checkBox_engine_qwant->setChecked(false);
    }
    if(settings.value(OSINT_URLSCAN).toString() == TRUE){
        ui->checkBox_engine_urlscan->setChecked(true);
    }else{
        ui->checkBox_engine_urlscan->setChecked(false);
    }
    if(settings.value(OSINT_RAPIDDNS).toString() == TRUE){
        ui->checkBox_engine_rapiddns->setChecked(true);
    }
    else{
        ui->checkBox_engine_rapiddns->setChecked(false);
    }
    if(settings.value(OSINT_PROJECTDISCOVERY).toString() == TRUE){
        ui->checkBox_engine_projectdiscovery->setChecked(true);
    }
    else{
        ui->checkBox_engine_projectdiscovery->setChecked(false);
    }
    if(settings.value(OSINT_PENTESTTOOLS).toString() == TRUE){
        ui->checkBox_engine_pentesttools->setChecked(true);
    }else{
        ui->checkBox_engine_pentesttools->setChecked(false);
    }
    if(settings.value(OSINT_SPYSE).toString() == TRUE){
        ui->checkBox_engine_spyse->setChecked(true);
    }else{
        ui->checkBox_engine_spyse->setChecked(false);
    }
    settings.endGroup();
}

void Osint::on_buttonDeleteProfile_clicked(){
    ///
    /// remove the name of the profile from main config file...
    ///
    Config::generalConfig().beginWriteArray("Osint-Profiles");
    Config::generalConfig().remove(ui->comboBoxProfiles->currentText());
    Config::generalConfig().endArray();
    ///
    /// remove the entire group(profile) from profiles...
    ///
    QSettings settings(currentPath+"/profiles.ini", QSettings::IniFormat);
    settings.remove(ui->comboBoxProfiles->currentText());
    ///
    /// remove name from comboBox...
    ///
    ui->comboBoxProfiles->removeItem(ui->comboBoxProfiles->currentIndex());
}

void Osint::on_buttonNewProfile_clicked(){
    ///
    /// checks...
    ///
    if(ui->lineEditNewProfile->text().isEmpty())
        return;
    ///
    /// appending profile name on comboBox and config file...
    ///
    QString profileName = ui->lineEditNewProfile->text();
    ui->comboBoxProfiles->addItem(profileName);
    ui->lineEditNewProfile->clear();
    //...
    int size = Config::generalConfig().beginReadArray("Osint-Profiles");
    Config::generalConfig().endArray();
    Config::generalConfig().beginWriteArray("Osint-Profiles");
    Config::generalConfig().setArrayIndex(size);
    Config::generalConfig().setValue("value", profileName);
    Config::generalConfig().endArray();
    ///
    /// saving to profiles...
    ///
    QSettings settings(currentPath+"/profiles.ini", QSettings::IniFormat);
    settings.beginGroup(profileName);
    //...
    if(ui->checkBox_engine_censys->isChecked()){
        settings.setValue(OSINT_CENSYS, TRUE);
    }else{
        settings.setValue(OSINT_CENSYS, FALSE);
    }
    if(ui->checkBox_engine_threatminer->isChecked()){
        settings.setValue(OSINT_THREATMINER, TRUE);
    }else{
        settings.setValue(OSINT_THREATMINER, FALSE);
    }
    if(ui->checkBox_engine_shodan->isChecked()){
        settings.setValue(OSINT_SHODAN, TRUE);
    }else{
        settings.setValue(OSINT_SHODAN, FALSE);
    }
    if(ui->checkBox_engine_github->isChecked()){
        settings.setValue(OSINT_GITHUB, TRUE);
    }else{
        settings.setValue(OSINT_GITHUB, FALSE);
    }
    if(ui->checkBox_engine_certspotter->isChecked()){
        settings.setValue(OSINT_CERTSPOTTER, TRUE);
    }else{
        settings.setValue(OSINT_CERTSPOTTER, FALSE);
    }
    if(ui->checkBox_engine_dogpile->isChecked()){
        settings.setValue(OSINT_DOGPILE, TRUE);
    }else{
        settings.setValue(OSINT_DOGPILE, FALSE);
    }
    if(ui->checkBox_engine_duckduckgo->isChecked()){
        settings.setValue(OSINT_DUCKDUCKGO, TRUE);
    }else{
        settings.setValue(OSINT_DUCKDUCKGO, FALSE);
    }
    if(ui->checkBox_engine_exalead->isChecked()){
        settings.setValue(OSINT_EXALEAD, TRUE);
    }else{
        settings.setValue(OSINT_EXALEAD, FALSE);
    }
    if(ui->checkBox_engine_huntersearch->isChecked()){
        settings.setValue(OSINT_HUNTERSEARCH, TRUE);
    }else{
        settings.setValue(OSINT_HUNTERSEARCH, FALSE);
    }
    if(ui->checkBox_engine_intelx->isChecked()){
        settings.setValue(OSINT_INTELX, TRUE);
    }else{
        settings.setValue(OSINT_INTELX, FALSE);
    }
    if(ui->checkBox_engine_netcraft->isChecked()){
        settings.setValue(OSINT_NETCRAFT, TRUE);
    }else{
        settings.setValue(OSINT_NETCRAFT, FALSE);
    }
    if(ui->checkBox_engine_otx->isChecked()){
        settings.setValue(OSINT_OTX, TRUE);
    }else{
        settings.setValue(OSINT_OTX, FALSE);
    }
    if(ui->checkBox_engine_securitytrails->isChecked()){
        settings.setValue(OSINT_SECURITYTRAILS, TRUE);
    }else{
        settings.setValue(OSINT_SECURITYTRAILS, FALSE);
    }
    if(ui->checkBox_engine_suip->isChecked()){
        settings.setValue(OSINT_SUIP, TRUE);
    }else{
        settings.setValue(OSINT_SUIP, FALSE);
    }
    if(ui->checkBox_engine_trello->isChecked()){
        settings.setValue(OSINT_TRELLO, TRUE);
    }else{
        settings.setValue(OSINT_TRELLO, FALSE);
    }
    if(ui->checkBox_engine_cloudflare->isChecked()){
        settings.setValue(OSINT_CLOUDFLARE, TRUE);
    }else{
        settings.setValue(OSINT_CLOUDFLARE, FALSE);
    }
    if(ui->checkBox_engine_threatcrowd->isChecked()){
        settings.setValue(OSINT_THREATCROWD, TRUE);
    }else{
        settings.setValue(OSINT_THREATCROWD, FALSE);
    }
    if(ui->checkBox_engine_dnsbufferoverrun->isChecked()){
        settings.setValue(OSINT_DNSBUFFEROVERRUN, TRUE);
    }else{
        settings.setValue(OSINT_DNSBUFFEROVERRUN, FALSE);
    }
    if(ui->checkBox_engine_hackertarget->isChecked()){
        settings.setValue(OSINT_HACKERTARGET, TRUE);
    }else{
        settings.setValue(OSINT_HACKERTARGET, FALSE);
    }
    if(ui->checkBox_engine_pkey->isChecked()){
        settings.setValue(OSINT_PKEY, TRUE);
    }else{
        settings.setValue(OSINT_PKEY, FALSE);
    }
    if(ui->checkBox_engine_waybackmachine->isChecked()){
        settings.setValue(OSINT_WAYBACKMACHINE, TRUE);
    }else{
        settings.setValue(OSINT_WAYBACKMACHINE, FALSE);
    }
    if(ui->checkBox_engine_ask->isChecked()){
        settings.setValue(OSINT_ASK, TRUE);
    }else{
        settings.setValue(OSINT_ASK, FALSE);
    }
    if(ui->checkBox_engine_baidu->isChecked()){
        settings.setValue(OSINT_BAIDU, TRUE);
    }else{
        settings.setValue(OSINT_BAIDU, FALSE);
    }
    if(ui->checkBox_engine_bing->isChecked()){
        settings.setValue(OSINT_BING, TRUE);
    }else{
        settings.setValue(OSINT_BING, FALSE);
    }
    if(ui->checkBox_engine_crtsh->isChecked()){
        settings.setValue(OSINT_CRTSH, TRUE);
    }else{
        settings.setValue(OSINT_CRTSH, FALSE);
    }
    if(ui->checkBox_engine_dnsdumpster->isChecked()){
        settings.setValue(OSINT_DNSDUMPSTER, TRUE);
    }else{
        settings.setValue(OSINT_DNSDUMPSTER, FALSE);
    }
    if(ui->checkBox_engine_google->isChecked()){
        settings.setValue(OSINT_GOOGLE, TRUE);
    }else{
        settings.setValue(OSINT_GOOGLE, FALSE);
    }
    if(ui->checkBox_engine_passivedns->isChecked()){
        settings.setValue(OSINT_PASSIVEDNS, TRUE);
    }else{
        settings.setValue(OSINT_PASSIVEDNS, FALSE);
    }
    if(ui->checkBox_engine_yahoo->isChecked()){
        settings.setValue(OSINT_YAHOO, TRUE);
    }else{
        settings.setValue(OSINT_YAHOO, FALSE);
    }
    if(ui->checkBox_engine_virustotalapi->isChecked()){
        settings.setValue(OSINT_VIRUSTOTALAPI, TRUE);
    }else{
        settings.setValue(OSINT_VIRUSTOTALAPI, FALSE);
    }
    if(ui->checkBox_engine_omnisint->isChecked()){
        settings.setValue(OSINT_OMNISINT, TRUE);
    }else{
        settings.setValue(OSINT_OMNISINT, FALSE);
    }
    if(ui->checkBox_engine_qwant->isChecked()){
        settings.setValue(OSINT_QWANT, TRUE);
    }else{
        settings.setValue(OSINT_QWANT, FALSE);
    }
    if(ui->checkBox_engine_rapiddns->isChecked()){
        settings.setValue(OSINT_RAPIDDNS, TRUE);
    }else{
        settings.setValue(OSINT_RAPIDDNS, FALSE);
    }
    if(ui->checkBox_engine_urlscan->isChecked()){
        settings.setValue(OSINT_URLSCAN, TRUE);
    }else{
        settings.setValue(OSINT_URLSCAN, FALSE);
    }
    if(ui->checkBox_engine_pentesttools->isChecked()){
        settings.setValue(OSINT_PENTESTTOOLS, TRUE);
    }else{
        settings.setValue(OSINT_PENTESTTOOLS, FALSE);
    }
    if(ui->checkBox_engine_projectdiscovery->isChecked()){
        settings.setValue(OSINT_PROJECTDISCOVERY, TRUE);
    }else{
        settings.setValue(OSINT_PROJECTDISCOVERY, FALSE);
    }
    if(ui->checkBox_engine_spyse->isChecked()){
        settings.setValue(OSINT_SPYSE, TRUE);
    }else{
        settings.setValue(OSINT_SPYSE, FALSE);
    }
    settings.endGroup();
}

void Osint::on_buttonAction_clicked(){
    ///
    /// check if there are results available else dont show the context menu...
    ///
    if(resultsModel->osint->rowCount() < 1){
        return;
    }
    ///
    /// getting the position of the action button to place the context menu and
    /// showing the context menu right by the side of the action button...
    ///
    QPoint pos = ui->buttonAction->mapToGlobal(QPoint(0,0));
    a_Menu->exec(QPoint(pos.x()+76, pos.y()));
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
    c_Menu->exec(QCursor::pos());
}

void Osint::on_checkBoxMultipleTargets_clicked(bool checked){
    if(checked)
        ui->targets->show();
    else
        ui->targets->hide();
}
