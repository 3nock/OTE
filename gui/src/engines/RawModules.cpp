#include "Raw.h"
#include "ui_Raw.h"
//...
#include <QMessageBox>


/*******************************************************************************
                                  Start Scan
********************************************************************************/
void Raw::startScan(){
    m_scanArgs->rawOption = ui->comboBoxOptions->currentIndex();
    m_scanArgs->option = ui->comboBoxOptions->currentText();
    m_scanArgs->module = ui->comboBoxModule->currentText();
    m_scanArgs->target = ui->lineEditTarget->text();
    m_scanArgs->raw = true;

    QThread *cThread = new QThread(this);
    if(ui->moduleCertSpotter->isChecked())
    {
        Certspotter *certspotter = new Certspotter(m_scanArgs);
        certspotter->Enumerator(cThread);
        certspotter->moveToThread(cThread);
        //...
        connect(certspotter, &Certspotter::rawResults, this, &Raw::onResults);
        connect(certspotter, &Certspotter::errorLog, this, &Raw::onErrorLog);
        connect(certspotter, &Certspotter::infoLog, this, &Raw::onInfoLog);
        connect(cThread, &QThread::finished, this, &Raw::onEnumerationComplete);
        connect(cThread, &QThread::finished, certspotter, &Certspotter::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        return;
    }
    if(ui->moduleOtx->isChecked())
    {
        OtxFree *otx = new OtxFree(m_scanArgs);
        otx->Enumerator(cThread);
        otx->moveToThread(cThread);
        //...
        connect(otx, &OtxFree::rawResults, this, &Raw::onResults);
        connect(otx, &OtxFree::errorLog, this, &Raw::onErrorLog);
        connect(otx, &OtxFree::infoLog, this, &Raw::onInfoLog);
        connect(cThread, &QThread::finished, this, &Raw::onEnumerationComplete);
        connect(cThread, &QThread::finished, otx, &OtxFree::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        return;
    }
    if(ui->moduleSublist3r->isChecked())
    {
        Sublist3r *sublist3r = new Sublist3r(m_scanArgs);
        sublist3r->Enumerator(cThread);
        sublist3r->moveToThread(cThread);
        //...
        connect(sublist3r, &Sublist3r::rawResults, this, &Raw::onResults);
        connect(sublist3r, &Sublist3r::errorLog, this, &Raw::onErrorLog);
        connect(sublist3r, &Sublist3r::infoLog, this, &Raw::onInfoLog);
        connect(cThread, &QThread::finished, this, &Raw::onEnumerationComplete);
        connect(cThread, &QThread::finished, sublist3r, &Sublist3r::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        return;
    }
    if(ui->moduleThreatMiner->isChecked())
    {
        Threatminer *threatminer = new Threatminer(m_scanArgs);
        threatminer->Enumerator(cThread);
        threatminer->moveToThread(cThread);
        //...
        connect(threatminer, &Threatminer::rawResults, this, &Raw::onResults);
        connect(threatminer, &Threatminer::errorLog, this, &Raw::onErrorLog);
        connect(threatminer, &Threatminer::infoLog, this, &Raw::onInfoLog);
        connect(cThread, &QThread::finished, this, &Raw::onEnumerationComplete);
        connect(cThread, &QThread::finished, threatminer, &Threatminer::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        return;
    }
    if(ui->moduleThreatCrowd->isChecked())
    {
        Threatcrowd *threatcrowd = new Threatcrowd(m_scanArgs);
        threatcrowd->Enumerator(cThread);
        threatcrowd->moveToThread(cThread);
        //...
        connect(threatcrowd, &Threatcrowd::rawResults, this, &Raw::onResults);
        connect(threatcrowd, &Threatcrowd::errorLog, this, &Raw::onErrorLog);
        connect(threatcrowd, &Threatcrowd::infoLog, this, &Raw::onInfoLog);
        connect(cThread, &QThread::finished, this, &Raw::onEnumerationComplete);
        connect(cThread, &QThread::finished, threatcrowd, &Threatcrowd::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        return;
    }
    if(ui->moduleHackerTargetFree->isChecked())
    {
        HackerTargetFree *hackertarget = new HackerTargetFree(m_scanArgs);
        hackertarget->Enumerator(cThread);
        hackertarget->moveToThread(cThread);
        //...
        connect(hackertarget, &HackerTargetFree::rawResults, this, &Raw::onResultsTxt);
        connect(hackertarget, &HackerTargetFree::errorLog, this, &Raw::onErrorLog);
        connect(hackertarget, &HackerTargetFree::infoLog, this, &Raw::onInfoLog);
        connect(cThread, &QThread::finished, this, &Raw::onEnumerationComplete);
        connect(cThread, &QThread::finished, hackertarget, &HackerTargetFree::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        return;
    }
    if(ui->moduleDnsBufferOverRun->isChecked())
    {
        Dnsbufferoverun *dnsbufferoverun = new Dnsbufferoverun(m_scanArgs);
        dnsbufferoverun->Enumerator(cThread);
        dnsbufferoverun->moveToThread(cThread);
        //...
        connect(dnsbufferoverun, &Dnsbufferoverun::rawResults, this, &Raw::onResults);
        connect(dnsbufferoverun, &Dnsbufferoverun::errorLog, this, &Raw::onErrorLog);
        connect(dnsbufferoverun, &Dnsbufferoverun::infoLog, this, &Raw::onInfoLog);
        connect(cThread, &QThread::finished, this, &Raw::onEnumerationComplete);
        connect(cThread, &QThread::finished, dnsbufferoverun, &Dnsbufferoverun::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        return;
    }
    if(ui->moduleAnubis->isChecked())
    {
        Anubis *anubis = new Anubis(m_scanArgs);

        anubis->Enumerator(cThread);
        anubis->moveToThread(cThread);
        //...
        connect(anubis, &Anubis::rawResults, this, &Raw::onResults);
        connect(anubis, &Anubis::errorLog, this, &Raw::onErrorLog);
        connect(anubis, &Anubis::infoLog, this, &Raw::onInfoLog);
        connect(cThread, &QThread::finished, this, &Raw::onEnumerationComplete);
        connect(cThread, &QThread::finished, anubis, &Anubis::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        return;
    }
    if(ui->moduleProjectDiscovery->isChecked())
    {
        Projectdiscovery *projectdiscovery = new Projectdiscovery(m_scanArgs);
        projectdiscovery->Enumerator(cThread);
        projectdiscovery->moveToThread(cThread);
        //...
        connect(projectdiscovery, &Projectdiscovery::rawResults, this, &Raw::onResults);
        connect(projectdiscovery, &Projectdiscovery::errorLog, this, &Raw::onErrorLog);
        connect(projectdiscovery, &Projectdiscovery::infoLog, this, &Raw::onInfoLog);
        connect(cThread, &QThread::finished, this, &Raw::onEnumerationComplete);
        connect(cThread, &QThread::finished, projectdiscovery, &Projectdiscovery::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        return;
    }
    if(ui->moduleSpyse->isChecked())
    {
        Spyse *spyse = new Spyse(m_scanArgs);
        spyse->Enumerator(cThread);
        spyse->moveToThread(cThread);
        //...
        connect(spyse, &Spyse::rawResults, this, &Raw::onResults);
        connect(spyse, &Spyse::errorLog, this, &Raw::onErrorLog);
        connect(spyse, &Spyse::infoLog, this, &Raw::onInfoLog);
        connect(cThread, &QThread::finished, this, &Raw::onEnumerationComplete);
        connect(cThread, &QThread::finished, spyse, &Spyse::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        return;
    }
    if(ui->moduleGoogleCert->isChecked())
    {
        GoogleCert *googlecert = new GoogleCert(m_scanArgs);
        googlecert->Enumerator(cThread);
        googlecert->moveToThread(cThread);
        //...
        connect(googlecert, &GoogleCert::rawResults, this, &Raw::onResults);
        connect(googlecert, &GoogleCert::errorLog, this, &Raw::onErrorLog);
        connect(googlecert, &GoogleCert::infoLog, this, &Raw::onInfoLog);
        connect(cThread, &QThread::finished, this, &Raw::onEnumerationComplete);
        connect(cThread, &QThread::finished, googlecert, &GoogleCert::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        return;
    }
    if(ui->moduleOmnisint->isChecked())
    {
        Omnisint *omnisint = new Omnisint(m_scanArgs);
        omnisint->Enumerator(cThread);
        omnisint->moveToThread(cThread);
        //...
        connect(omnisint, &Omnisint::rawResults, this, &Raw::onResults);
        connect(omnisint, &Omnisint::errorLog, this, &Raw::onErrorLog);
        connect(omnisint, &Omnisint::infoLog, this, &Raw::onInfoLog);
        connect(cThread, &QThread::finished, this, &Raw::onEnumerationComplete);
        connect(cThread, &QThread::finished, omnisint, &Omnisint::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        return;
    }
    if(ui->moduleRobtexPaid->isChecked())
    {
        RobtexPaid *robtex = new RobtexPaid(m_scanArgs);
        robtex->Enumerator(cThread);
        robtex->moveToThread(cThread);
        //...
        connect(robtex, &RobtexPaid::rawResults, this, &Raw::onResults);
        connect(robtex, &RobtexPaid::errorLog, this, &Raw::onErrorLog);
        connect(robtex, &RobtexPaid::infoLog, this, &Raw::onInfoLog);
        connect(cThread, &QThread::finished, this, &Raw::onEnumerationComplete);
        connect(cThread, &QThread::finished, robtex, &RobtexPaid::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        return;
    }
    if(ui->moduleVirusTotal->isChecked())
    {
        VirusTotal *virustotal = new VirusTotal(m_scanArgs);
        virustotal->Enumerator(cThread);
        virustotal->moveToThread(cThread);
        //...
        connect(virustotal, &VirusTotal::rawResults, this, &Raw::onResults);
        connect(virustotal, &VirusTotal::errorLog, this, &Raw::onErrorLog);
        connect(virustotal, &VirusTotal::infoLog, this, &Raw::onInfoLog);
        connect(cThread, &QThread::finished, this, &Raw::onEnumerationComplete);
        connect(cThread, &QThread::finished, virustotal, &VirusTotal::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        return;
    }
    if(ui->moduleUrlScan->isChecked())
    {
        Urlscan *urlscan = new Urlscan(m_scanArgs);
        urlscan->Enumerator(cThread);
        urlscan->moveToThread(cThread);
        //...
        connect(urlscan, &Urlscan::rawResults, this, &Raw::onResults);
        connect(urlscan, &Urlscan::errorLog, this, &Raw::onErrorLog);
        connect(urlscan, &Urlscan::infoLog, this, &Raw::onInfoLog);
        connect(cThread, &QThread::finished, this, &Raw::onEnumerationComplete);
        connect(cThread, &QThread::finished, urlscan, &Urlscan::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        return;
    }
    if(ui->moduleWaybackMachine->isChecked())
    {
        Waybackmachine *waybackmachine = new Waybackmachine(m_scanArgs);
        waybackmachine->Enumerator(cThread);
        waybackmachine->moveToThread(cThread);
        //...
        connect(waybackmachine, &Waybackmachine::rawResults, this, &Raw::onResults);
        connect(waybackmachine, &Waybackmachine::errorLog, this, &Raw::onErrorLog);
        connect(waybackmachine, &Waybackmachine::infoLog, this, &Raw::onInfoLog);
        connect(cThread, &QThread::finished, this, &Raw::onEnumerationComplete);
        connect(cThread, &QThread::finished, waybackmachine, &Waybackmachine::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        return;
    }
    if(ui->moduleArchiveIt->isChecked())
    {
        ArchiveIt *archiveit = new ArchiveIt(m_scanArgs);
        archiveit->Enumerator(cThread);
        archiveit->moveToThread(cThread);
        //...
        connect(archiveit, &ArchiveIt::rawResults, this, &Raw::onResults);
        connect(archiveit, &ArchiveIt::errorLog, this, &Raw::onErrorLog);
        connect(archiveit, &ArchiveIt::infoLog, this, &Raw::onInfoLog);
        connect(cThread, &QThread::finished, this, &Raw::onEnumerationComplete);
        connect(cThread, &QThread::finished, archiveit, &ArchiveIt::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        return;
    }
    if(ui->moduleBgpview->isChecked())
    {
        Bgpview *bgpview = new Bgpview(m_scanArgs);
        bgpview->Enumerator(cThread);
        bgpview->moveToThread(cThread);
        //...
        connect(bgpview, &Bgpview::rawResults, this, &Raw::onResults);
        connect(bgpview, &Bgpview::errorLog, this, &Raw::onErrorLog);
        connect(bgpview, &Bgpview::infoLog, this, &Raw::onInfoLog);
        connect(cThread, &QThread::finished, this, &Raw::onEnumerationComplete);
        connect(cThread, &QThread::finished, bgpview, &Bgpview::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        return;
    }
    if(ui->moduleBinaryEdge->isChecked())
    {
        BinaryEdge *binaryedge = new BinaryEdge(m_scanArgs);
        binaryedge->Enumerator(cThread);
        binaryedge->moveToThread(cThread);
        //...
        connect(binaryedge, &BinaryEdge::rawResults, this, &Raw::onResults);
        connect(binaryedge, &BinaryEdge::errorLog, this, &Raw::onErrorLog);
        connect(binaryedge, &BinaryEdge::infoLog, this, &Raw::onInfoLog);
        connect(cThread, &QThread::finished, this, &Raw::onEnumerationComplete);
        connect(cThread, &QThread::finished, binaryedge, &BinaryEdge::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        return;
    }
    if(ui->moduleC99->isChecked())
    {
        C99 *c99 = new C99(m_scanArgs);
        c99->Enumerator(cThread);
        c99->moveToThread(cThread);
        //...
        connect(c99, &C99::rawResults, this, &Raw::onResults);
        connect(c99, &C99::errorLog, this, &Raw::onErrorLog);
        connect(c99, &C99::infoLog, this, &Raw::onInfoLog);
        connect(cThread, &QThread::finished, this, &Raw::onEnumerationComplete);
        connect(cThread, &QThread::finished, c99, &C99::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        return;
    }
    if(ui->moduleCommonCrawl->isChecked())
    {
        CommonCrawl *commonCrawl = new CommonCrawl(m_scanArgs);
        commonCrawl->Enumerator(cThread);
        commonCrawl->moveToThread(cThread);
        //...
        connect(commonCrawl, &CommonCrawl::rawResults, this, &Raw::onResults);
        connect(commonCrawl, &CommonCrawl::errorLog, this, &Raw::onErrorLog);
        connect(commonCrawl, &CommonCrawl::infoLog, this, &Raw::onInfoLog);
        connect(cThread, &QThread::finished, this, &Raw::onEnumerationComplete);
        connect(cThread, &QThread::finished, commonCrawl, &CommonCrawl::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        return;
    }
    if(ui->moduleGithub->isChecked())
    {
        Github *github = new Github(m_scanArgs);
        github->Enumerator(cThread);
        github->moveToThread(cThread);
        //...
        connect(github, &Github::rawResults, this, &Raw::onResults);
        connect(github, &Github::errorLog, this, &Raw::onErrorLog);
        connect(github, &Github::infoLog, this, &Raw::onInfoLog);
        connect(cThread, &QThread::finished, this, &Raw::onEnumerationComplete);
        connect(cThread, &QThread::finished, github, &Github::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        return;
    }
    if(ui->moduleHunterSearch->isChecked())
    {
        HunterSearch *huntersearch = new HunterSearch(m_scanArgs);
        huntersearch->Enumerator(cThread);
        huntersearch->moveToThread(cThread);
        //...
        connect(huntersearch, &HunterSearch::rawResults, this, &Raw::onResults);
        connect(huntersearch, &HunterSearch::errorLog, this, &Raw::onErrorLog);
        connect(huntersearch, &HunterSearch::infoLog, this, &Raw::onInfoLog);
        connect(cThread, &QThread::finished, this, &Raw::onEnumerationComplete);
        connect(cThread, &QThread::finished, huntersearch, &HunterSearch::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        return;
    }
    if(ui->moduleIpInfo->isChecked())
    {
        IpInfo *ipinfo = new IpInfo(m_scanArgs);
        ipinfo->Enumerator(cThread);
        ipinfo->moveToThread(cThread);
        //...
        connect(ipinfo, &IpInfo::rawResults, this, &Raw::onResults);
        connect(ipinfo, &IpInfo::errorLog, this, &Raw::onErrorLog);
        connect(ipinfo, &IpInfo::infoLog, this, &Raw::onInfoLog);
        connect(cThread, &QThread::finished, this, &Raw::onEnumerationComplete);
        connect(cThread, &QThread::finished, ipinfo, &IpInfo::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        return;
    }
    if(ui->moduleMnemonicFree->isChecked())
    {
        MnemonicFree *mnemonic = new MnemonicFree(m_scanArgs);
        mnemonic->Enumerator(cThread);
        mnemonic->moveToThread(cThread);
        //...
        connect(mnemonic, &MnemonicFree::rawResults, this, &Raw::onResults);
        connect(mnemonic, &MnemonicFree::errorLog, this, &Raw::onErrorLog);
        connect(mnemonic, &MnemonicFree::infoLog, this, &Raw::onInfoLog);
        connect(cThread, &QThread::finished, this, &Raw::onEnumerationComplete);
        connect(cThread, &QThread::finished, mnemonic, &MnemonicFree::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        return;
    }
    if(ui->moduleRiskIq->isChecked())
    {
        RiskIq *riskiq = new RiskIq(m_scanArgs);
        riskiq->Enumerator(cThread);
        riskiq->moveToThread(cThread);
        //...
        connect(riskiq, &RiskIq::rawResults, this, &Raw::onResults);
        connect(riskiq, &RiskIq::errorLog, this, &Raw::onErrorLog);
        connect(riskiq, &RiskIq::infoLog, this, &Raw::onInfoLog);
        connect(cThread, &QThread::finished, this, &Raw::onEnumerationComplete);
        connect(cThread, &QThread::finished, riskiq, &RiskIq::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        return;
    }
    if(ui->moduleRobtex->isChecked())
    {
        RobtexFree *robtex = new RobtexFree(m_scanArgs);
        robtex->Enumerator(cThread);
        robtex->moveToThread(cThread);
        //...
        connect(robtex, &RobtexFree::rawResults, this, &Raw::onResults);
        connect(robtex, &RobtexFree::errorLog, this, &Raw::onErrorLog);
        connect(robtex, &RobtexFree::infoLog, this, &Raw::onInfoLog);
        connect(cThread, &QThread::finished, this, &Raw::onEnumerationComplete);
        connect(cThread, &QThread::finished, robtex, &RobtexFree::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        return;
    }
    if(ui->moduleSecurityTrails->isChecked())
    {
        SecurityTrails *securitytrails = new SecurityTrails(m_scanArgs);
        securitytrails->Enumerator(cThread);
        securitytrails->moveToThread(cThread);
        //...
        connect(securitytrails, &SecurityTrails::rawResults, this, &Raw::onResults);
        connect(securitytrails, &SecurityTrails::errorLog, this, &Raw::onErrorLog);
        connect(securitytrails, &SecurityTrails::infoLog, this, &Raw::onInfoLog);
        connect(cThread, &QThread::finished, this, &Raw::onEnumerationComplete);
        connect(cThread, &QThread::finished, securitytrails, &SecurityTrails::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        return;
    }
    if(ui->moduleShodan->isChecked())
    {
        Shodan *shodan = new Shodan(m_scanArgs);
        shodan->Enumerator(cThread);
        shodan->moveToThread(cThread);
        //...
        connect(shodan, &Shodan::rawResults, this, &Raw::onResults);
        connect(shodan, &Shodan::errorLog, this, &Raw::onErrorLog);
        connect(shodan, &Shodan::infoLog, this, &Raw::onInfoLog);
        connect(cThread, &QThread::finished, this, &Raw::onEnumerationComplete);
        connect(cThread, &QThread::finished, shodan, &Shodan::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        return;
    }
    if(ui->moduleThreatBook->isChecked())
    {
        ThreatBook *threatbook = new ThreatBook(m_scanArgs);
        threatbook->Enumerator(cThread);
        threatbook->moveToThread(cThread);
        //...
        connect(threatbook, &ThreatBook::rawResults, this, &Raw::onResults);
        connect(threatbook, &ThreatBook::errorLog, this, &Raw::onErrorLog);
        connect(threatbook, &ThreatBook::infoLog, this, &Raw::onInfoLog);
        connect(cThread, &QThread::finished, this, &Raw::onEnumerationComplete);
        connect(cThread, &QThread::finished, threatbook, &ThreatBook::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        return;
    }
    if(ui->moduleWhoisXmlApi->isChecked())
    {
        WhoisXmlApi *whoisxmlapi = new WhoisXmlApi(m_scanArgs);
        whoisxmlapi->Enumerator(cThread);
        whoisxmlapi->moveToThread(cThread);
        //...
        connect(whoisxmlapi, &WhoisXmlApi::rawResults, this, &Raw::onResults);
        connect(whoisxmlapi, &WhoisXmlApi::errorLog, this, &Raw::onErrorLog);
        connect(whoisxmlapi, &WhoisXmlApi::infoLog, this, &Raw::onInfoLog);
        connect(cThread, &QThread::finished, this, &Raw::onEnumerationComplete);
        connect(cThread, &QThread::finished, whoisxmlapi, &WhoisXmlApi::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        return;
    }
    if(ui->moduleZETALytics->isChecked())
    {
        ZETAlytics *zetalytics = new ZETAlytics(m_scanArgs);
        zetalytics->Enumerator(cThread);
        zetalytics->moveToThread(cThread);
        //...
        connect(zetalytics, &ZETAlytics::rawResults, this, &Raw::onResults);
        connect(zetalytics, &ZETAlytics::errorLog, this, &Raw::onErrorLog);
        connect(zetalytics, &ZETAlytics::infoLog, this, &Raw::onInfoLog);
        connect(cThread, &QThread::finished, this, &Raw::onEnumerationComplete);
        connect(cThread, &QThread::finished, zetalytics, &ZETAlytics::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        return;
    }
    if(ui->moduleZoomEye->isChecked())
    {
        ZoomEye *zoomeye = new ZoomEye(m_scanArgs);
        zoomeye->Enumerator(cThread);
        zoomeye->moveToThread(cThread);
        //...
        connect(zoomeye, &ZoomEye::rawResults, this, &Raw::onResults);
        connect(zoomeye, &ZoomEye::errorLog, this, &Raw::onErrorLog);
        connect(zoomeye, &ZoomEye::infoLog, this, &Raw::onInfoLog);
        connect(cThread, &QThread::finished, this, &Raw::onEnumerationComplete);
        connect(cThread, &QThread::finished, zoomeye, &ZoomEye::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        return;
    }
    if(ui->moduleIpApi->isChecked())
    {
        IpApi *ipapi = new IpApi(m_scanArgs);
        ipapi->Enumerator(cThread);
        ipapi->moveToThread(cThread);
        //...
        connect(ipapi, &IpApi::rawResults, this, &Raw::onResults);
        connect(ipapi, &IpApi::errorLog, this, &Raw::onErrorLog);
        connect(ipapi, &IpApi::infoLog, this, &Raw::onInfoLog);
        connect(cThread, &QThread::finished, this, &Raw::onEnumerationComplete);
        connect(cThread, &QThread::finished, ipapi, &IpApi::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        return;
    }
    if(ui->moduleViewDns->isChecked())
    {
        ViewDns *viewdns = new ViewDns(m_scanArgs);
        viewdns->Enumerator(cThread);
        viewdns->moveToThread(cThread);
        //...
        connect(viewdns, &ViewDns::rawResults, this, &Raw::onResults);
        connect(viewdns, &ViewDns::errorLog, this, &Raw::onErrorLog);
        connect(viewdns, &ViewDns::infoLog, this, &Raw::onInfoLog);
        connect(cThread, &QThread::finished, this, &Raw::onEnumerationComplete);
        connect(cThread, &QThread::finished, viewdns, &ViewDns::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        return;
    }
    if(ui->moduleWebResolver->isChecked())
    {
        WebResolver *webresolver = new WebResolver(m_scanArgs);
        webresolver->Enumerator(cThread);
        webresolver->moveToThread(cThread);
        //...
        connect(webresolver, &WebResolver::rawResults, this, &Raw::onResults);
        connect(webresolver, &WebResolver::errorLog, this, &Raw::onErrorLog);
        connect(webresolver, &WebResolver::infoLog, this, &Raw::onInfoLog);
        connect(cThread, &QThread::finished, this, &Raw::onEnumerationComplete);
        connect(cThread, &QThread::finished, webresolver, &WebResolver::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        return;
    }
    if(ui->moduleCircl->isChecked())
    {
        Circl *circl = new Circl(m_scanArgs);
        circl->Enumerator(cThread);
        circl->moveToThread(cThread);
        //...
        connect(circl, &Circl::rawResults, this, &Raw::onResults);
        connect(circl, &Circl::errorLog, this, &Raw::onErrorLog);
        connect(circl, &Circl::infoLog, this, &Raw::onInfoLog);
        connect(cThread, &QThread::finished, this, &Raw::onEnumerationComplete);
        connect(cThread, &QThread::finished, circl, &Circl::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        return;
    }
    if(ui->moduleHackerTarget->isChecked())
    {
        HackerTargetPaid *hackertarget = new HackerTargetPaid(m_scanArgs);
        hackertarget->Enumerator(cThread);
        hackertarget->moveToThread(cThread);
        //...
        connect(hackertarget, &HackerTargetPaid::rawResults, this, &Raw::onResultsTxt);
        connect(hackertarget, &HackerTargetPaid::errorLog, this, &Raw::onErrorLog);
        connect(hackertarget, &HackerTargetPaid::infoLog, this, &Raw::onInfoLog);
        connect(cThread, &QThread::finished, this, &Raw::onEnumerationComplete);
        connect(cThread, &QThread::finished, hackertarget, &HackerTargetPaid::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        return;
    }
    if(ui->moduleMnemonic->isChecked())
    {
        MnemonicPaid *mnemonic = new MnemonicPaid(m_scanArgs);
        mnemonic->Enumerator(cThread);
        mnemonic->moveToThread(cThread);
        //...
        connect(mnemonic, &MnemonicPaid::rawResults, this, &Raw::onResults);
        connect(mnemonic, &MnemonicPaid::errorLog, this, &Raw::onErrorLog);
        connect(mnemonic, &MnemonicPaid::infoLog, this, &Raw::onInfoLog);
        connect(cThread, &QThread::finished, this, &Raw::onEnumerationComplete);
        connect(cThread, &QThread::finished, mnemonic, &MnemonicPaid::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        return;
    }
    if(ui->moduleOtx->isChecked())
    {
        OtxPaid *otx = new OtxPaid(m_scanArgs);
        otx->Enumerator(cThread);
        otx->moveToThread(cThread);
        //...
        connect(otx, &OtxPaid::rawResults, this, &Raw::onResults);
        connect(otx, &OtxPaid::errorLog, this, &Raw::onErrorLog);
        connect(otx, &OtxPaid::infoLog, this, &Raw::onInfoLog);
        connect(cThread, &QThread::finished, this, &Raw::onEnumerationComplete);
        connect(cThread, &QThread::finished, otx, &OtxPaid::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        return;
    }
    ///
    /// if control reaches here means no module was selected...
    ///
    ui->buttonStart->setEnabled(true);
    ui->buttonStop->setDisabled(true);
    QMessageBox::warning(this, "Error", "Please Chooose Osint Engine To Query...");
}


/*******************************************************************************
                                    Modules
********************************************************************************/

void Raw::on_moduleCertSpotter_clicked(){
    ui->comboBoxOptions->clear();
    ModuleInfo::Certspotter meta;
    m_optionSet = meta.flags;
    ui->labelUrl->setText("<a href=\""+meta.url+"\" style=\"color: green;\">"+meta.name+"</a>");
    ui->labelApiDoc->setText("<a href=\""+meta.url_apiDoc+"\" style=\"color: green;\">"+meta.url_apiDoc+"</a>");
    ui->textEditEngineSummary->setText(meta.summary);
    ui->comboBoxOptions->addItems(meta.flags.keys());
}

void Raw::on_moduleOtx_clicked(){
    ui->comboBoxOptions->clear();
    ModuleInfo::OtxPaid meta;
    m_optionSet = meta.flags;
    ui->labelUrl->setText("<a href=\""+meta.url+"\" style=\"color: green;\">"+meta.name+"</a>");
    ui->labelApiDoc->setText("<a href=\""+meta.url_apiDoc+"\" style=\"color: green;\">"+meta.url_apiDoc+"</a>");
    ui->textEditEngineSummary->setText(meta.summary);
    ui->comboBoxOptions->addItems(meta.flags.keys());
}

void Raw::on_moduleSublist3r_clicked(){
    ui->comboBoxOptions->clear();
    ModuleInfo::Sublist3r meta;
    m_optionSet = meta.flags;
    ui->labelUrl->setText("<a href=\""+meta.url+"\" style=\"color: green;\">"+meta.name+"</a>");
    ui->labelApiDoc->setText("<a href=\""+meta.url_apiDoc+"\" style=\"color: green;\">"+meta.url_apiDoc+"</a>");
    ui->textEditEngineSummary->setText(meta.summary);
    ui->comboBoxOptions->addItems(meta.flags.keys());
}

void Raw::on_moduleThreatMiner_clicked(){
    ui->comboBoxOptions->clear();
    ModuleInfo::Threatminer meta;
    m_optionSet = meta.flags;
    ui->labelUrl->setText("<a href=\""+meta.url+"\" style=\"color: green;\">"+meta.name+"</a>");
    ui->labelApiDoc->setText("<a href=\""+meta.url_apiDoc+"\" style=\"color: green;\">"+meta.url_apiDoc+"</a>");
    ui->textEditEngineSummary->setText(meta.summary);
    ui->comboBoxOptions->addItems(meta.flags.keys());
}

void Raw::on_moduleThreatCrowd_clicked(){
    ui->comboBoxOptions->clear();
    ModuleInfo::Threatcrowd meta;
    m_optionSet = meta.flags;
    ui->labelUrl->setText("<a href=\""+meta.url+"\" style=\"color: green;\">"+meta.name+"</a>");
    ui->labelApiDoc->setText("<a href=\""+meta.url_apiDoc+"\" style=\"color: green;\">"+meta.url_apiDoc+"</a>");
    ui->textEditEngineSummary->setText(meta.summary);
    ui->comboBoxOptions->addItems(meta.flags.keys());
}

void Raw::on_moduleHackerTargetFree_clicked(){
    ui->comboBoxOptions->clear();
    ModuleInfo::HackerTargetFree meta;
    m_optionSet = meta.flags;
    ui->labelUrl->setText("<a href=\""+meta.url+"\" style=\"color: green;\">"+meta.name+"</a>");
    ui->labelApiDoc->setText("<a href=\""+meta.url_apiDoc+"\" style=\"color: green;\">"+meta.url_apiDoc+"</a>");
    ui->textEditEngineSummary->setText(meta.summary);
    ui->comboBoxOptions->addItems(meta.flags.keys());
}

void Raw::on_moduleDnsBufferOverRun_clicked(){
    ui->comboBoxOptions->clear();
    ModuleInfo::Dnsbufferoverun meta;
    m_optionSet = meta.flags;
    ui->labelUrl->setText("<a href=\""+meta.url+"\" style=\"color: green;\">"+meta.name+"</a>");
    ui->labelApiDoc->setText("<a href=\""+meta.url_apiDoc+"\" style=\"color: green;\">"+meta.url_apiDoc+"</a>");
    ui->textEditEngineSummary->setText(meta.summary);
    ui->comboBoxOptions->addItems(meta.flags.keys());
}

void Raw::on_moduleAnubis_clicked(){
    ui->comboBoxOptions->clear();
    ModuleInfo::Anubis meta;
    m_optionSet = meta.flags;
    ui->labelUrl->setText("<a href=\""+meta.url+"\" style=\"color: green;\">"+meta.name+"</a>");
    ui->labelApiDoc->setText("<a href=\""+meta.url_apiDoc+"\" style=\"color: green;\">"+meta.url_apiDoc+"</a>");
    ui->textEditEngineSummary->setText(meta.summary);
    ui->comboBoxOptions->addItems(meta.flags.keys());
}

void Raw::on_moduleProjectDiscovery_clicked(){
    ui->comboBoxOptions->clear();
    ModuleInfo::Projectdiscovery meta;
    m_optionSet = meta.flags;
    ui->labelUrl->setText("<a href=\""+meta.url+"\" style=\"color: green;\">"+meta.name+"</a>");
    ui->labelApiDoc->setText("<a href=\""+meta.url_apiDoc+"\" style=\"color: green;\">"+meta.url_apiDoc+"</a>");
    ui->textEditEngineSummary->setText(meta.summary);
    ui->comboBoxOptions->addItems(meta.flags.keys());
}

void Raw::on_moduleSpyse_clicked(){
    ui->comboBoxOptions->clear();
    ModuleInfo::Spyse meta;
    m_optionSet = meta.flags;
    ui->labelUrl->setText("<a href=\""+meta.url+"\" style=\"color: green;\">"+meta.name+"</a>");
    ui->labelApiDoc->setText("<a href=\""+meta.url_apiDoc+"\" style=\"color: green;\">"+meta.url_apiDoc+"</a>");
    ui->textEditEngineSummary->setText(meta.summary);
    ui->comboBoxOptions->addItems(meta.flags.keys());
}

void Raw::on_moduleGoogleCert_clicked(){
    ui->comboBoxOptions->clear();
    ModuleInfo::GoogleCert meta;
    m_optionSet = meta.flags;
    ui->labelUrl->setText("<a href=\""+meta.url+"\" style=\"color: green;\">"+meta.name+"</a>");
    ui->labelApiDoc->setText("<a href=\""+meta.url_apiDoc+"\" style=\"color: green;\">"+meta.url_apiDoc+"</a>");
    ui->textEditEngineSummary->setText(meta.summary);
    ui->comboBoxOptions->addItems(meta.flags.keys());
}

void Raw::on_moduleOmnisint_clicked(){
    ui->comboBoxOptions->clear();
    ModuleInfo::Omnisint meta;
    m_optionSet = meta.flags;
    ui->labelUrl->setText("<a href=\""+meta.url+"\" style=\"color: green;\">"+meta.name+"</a>");
    ui->labelApiDoc->setText("<a href=\""+meta.url_apiDoc+"\" style=\"color: green;\">"+meta.url_apiDoc+"</a>");
    ui->textEditEngineSummary->setText(meta.summary);
    ui->comboBoxOptions->addItems(meta.flags.keys());
}

void Raw::on_moduleRobtexPaid_clicked(){
    ui->comboBoxOptions->clear();
    ModuleInfo::RobtexPaid meta;
    m_optionSet = meta.flags;
    ui->labelUrl->setText("<a href=\""+meta.url+"\" style=\"color: green;\">"+meta.name+"</a>");
    ui->labelApiDoc->setText("<a href=\""+meta.url_apiDoc+"\" style=\"color: green;\">"+meta.url_apiDoc+"</a>");
    ui->textEditEngineSummary->setText(meta.summary);
    ui->comboBoxOptions->addItems(meta.flags.keys());
}

void Raw::on_moduleUrlScan_clicked(){
    ui->comboBoxOptions->clear();
    ModuleInfo::Urlscan meta;
    m_optionSet = meta.flags;
    ui->labelUrl->setText("<a href=\""+meta.url+"\" style=\"color: green;\">"+meta.name+"</a>");
    ui->labelApiDoc->setText("<a href=\""+meta.url_apiDoc+"\" style=\"color: green;\">"+meta.url_apiDoc+"</a>");
    ui->textEditEngineSummary->setText(meta.summary);
    ui->comboBoxOptions->addItems(meta.flags.keys());
}

void Raw::on_moduleVirusTotal_clicked(){
    ui->comboBoxOptions->clear();
    ModuleInfo::VirusTotal meta;
    m_optionSet = meta.flags;
    ui->labelUrl->setText("<a href=\""+meta.url+"\" style=\"color: green;\">"+meta.name+"</a>");
    ui->labelApiDoc->setText("<a href=\""+meta.url_apiDoc+"\" style=\"color: green;\">"+meta.url_apiDoc+"</a>");
    ui->textEditEngineSummary->setText(meta.summary);
    ui->comboBoxOptions->addItems(meta.flags.keys());
}

void Raw::on_moduleWaybackMachine_clicked(){
    ui->comboBoxOptions->clear();
    ModuleInfo::Waybackmachine meta;
    m_optionSet = meta.flags;
    ui->labelUrl->setText("<a href=\""+meta.url+"\" style=\"color: green;\">"+meta.name+"</a>");
    ui->labelApiDoc->setText("<a href=\""+meta.url_apiDoc+"\" style=\"color: green;\">"+meta.url_apiDoc+"</a>");
    ui->textEditEngineSummary->setText(meta.summary);
    ui->comboBoxOptions->addItems(meta.flags.keys());
}

void Raw::on_moduleArchiveIt_clicked(){
    ui->comboBoxOptions->clear();
    ModuleInfo::ArchiveIt meta;
    m_optionSet = meta.flags;
    ui->labelUrl->setText("<a href=\""+meta.url+"\" style=\"color: green;\">"+meta.name+"</a>");
    ui->labelApiDoc->setText("<a href=\""+meta.url_apiDoc+"\" style=\"color: green;\">"+meta.url_apiDoc+"</a>");
    ui->textEditEngineSummary->setText(meta.summary);
    ui->comboBoxOptions->addItems(meta.flags.keys());
}

void Raw::on_moduleBgpview_clicked(){
    ui->comboBoxOptions->clear();
    ModuleInfo::Bgpview meta;
    m_optionSet = meta.flags;
    ui->labelUrl->setText("<a href=\""+meta.url+"\" style=\"color: green;\">"+meta.name+"</a>");
    ui->labelApiDoc->setText("<a href=\""+meta.url_apiDoc+"\" style=\"color: green;\">"+meta.url_apiDoc+"</a>");
    ui->textEditEngineSummary->setText(meta.summary);
    ui->comboBoxOptions->addItems(meta.flags.keys());
}

void Raw::on_moduleBinaryEdge_clicked(){
    ui->comboBoxOptions->clear();
    ModuleInfo::BinaryEdge meta;
    m_optionSet = meta.flags;
    ui->labelUrl->setText("<a href=\""+meta.url+"\" style=\"color: green;\">"+meta.name+"</a>");
    ui->labelApiDoc->setText("<a href=\""+meta.url_apiDoc+"\" style=\"color: green;\">"+meta.url_apiDoc+"</a>");
    ui->textEditEngineSummary->setText(meta.summary);
    ui->comboBoxOptions->addItems(meta.flags.keys());
}

void Raw::on_moduleC99_clicked(){
    ui->comboBoxOptions->clear();
    ModuleInfo::C99 meta;
    m_optionSet = meta.flags;
    ui->labelUrl->setText("<a href=\""+meta.url+"\" style=\"color: green;\">"+meta.name+"</a>");
    ui->labelApiDoc->setText("<a href=\""+meta.url_apiDoc+"\" style=\"color: green;\">"+meta.url_apiDoc+"</a>");
    ui->textEditEngineSummary->setText(meta.summary);
    ui->comboBoxOptions->addItems(meta.flags.keys());
}

void Raw::on_moduleCommonCrawl_clicked(){
    ui->comboBoxOptions->clear();
    ModuleInfo::CommonCrawl meta;
    m_optionSet = meta.flags;
    ui->labelUrl->setText("<a href=\""+meta.url+"\" style=\"color: green;\">"+meta.name+"</a>");
    ui->labelApiDoc->setText("<a href=\""+meta.url_apiDoc+"\" style=\"color: green;\">"+meta.url_apiDoc+"</a>");
    ui->textEditEngineSummary->setText(meta.summary);
    ui->comboBoxOptions->addItems(meta.flags.keys());
}

void Raw::on_moduleGithub_clicked(){
    ui->comboBoxOptions->clear();
    ModuleInfo::Github meta;
    m_optionSet = meta.flags;
    ui->labelUrl->setText("<a href=\""+meta.url+"\" style=\"color: green;\">"+meta.name+"</a>");
    ui->labelApiDoc->setText("<a href=\""+meta.url_apiDoc+"\" style=\"color: green;\">"+meta.url_apiDoc+"</a>");
    ui->textEditEngineSummary->setText(meta.summary);
    ui->comboBoxOptions->addItems(meta.flags.keys());
}

void Raw::on_moduleHunterSearch_clicked(){
    ui->comboBoxOptions->clear();
    ModuleInfo::HunterSearch meta;
    m_optionSet = meta.flags;
    ui->labelUrl->setText("<a href=\""+meta.url+"\" style=\"color: green;\">"+meta.name+"</a>");
    ui->labelApiDoc->setText("<a href=\""+meta.url_apiDoc+"\" style=\"color: green;\">"+meta.url_apiDoc+"</a>");
    ui->textEditEngineSummary->setText(meta.summary);
    ui->comboBoxOptions->addItems(meta.flags.keys());
}

void Raw::on_moduleIpInfo_clicked(){
    ui->comboBoxOptions->clear();
    ModuleInfo::IpInfo meta;
    m_optionSet = meta.flags;
    ui->labelUrl->setText("<a href=\""+meta.url+"\" style=\"color: green;\">"+meta.name+"</a>");
    ui->labelApiDoc->setText("<a href=\""+meta.url_apiDoc+"\" style=\"color: green;\">"+meta.url_apiDoc+"</a>");
    ui->textEditEngineSummary->setText(meta.summary);
    ui->comboBoxOptions->addItems(meta.flags.keys());
}

void Raw::on_moduleMnemonicFree_clicked(){
    ui->comboBoxOptions->clear();
    ModuleInfo::MnemonicFree meta;
    m_optionSet = meta.flags;
    ui->labelUrl->setText("<a href=\""+meta.url+"\" style=\"color: green;\">"+meta.name+"</a>");
    ui->labelApiDoc->setText("<a href=\""+meta.url_apiDoc+"\" style=\"color: green;\">"+meta.url_apiDoc+"</a>");
    ui->textEditEngineSummary->setText(meta.summary);
    ui->comboBoxOptions->addItems(meta.flags.keys());
}

void Raw::on_moduleRiskIq_clicked(){
    ui->comboBoxOptions->clear();
    ModuleInfo::RiskIq meta;
    m_optionSet = meta.flags;
    ui->labelUrl->setText("<a href=\""+meta.url+"\" style=\"color: green;\">"+meta.name+"</a>");
    ui->labelApiDoc->setText("<a href=\""+meta.url_apiDoc+"\" style=\"color: green;\">"+meta.url_apiDoc+"</a>");
    ui->textEditEngineSummary->setText(meta.summary);
    ui->comboBoxOptions->addItems(meta.flags.keys());
}

void Raw::on_moduleRobtex_clicked(){
    ui->comboBoxOptions->clear();
    ModuleInfo::RobtexPaid meta;
    m_optionSet = meta.flags;
    ui->labelUrl->setText("<a href=\""+meta.url+"\" style=\"color: green;\">"+meta.name+"</a>");
    ui->labelApiDoc->setText("<a href=\""+meta.url_apiDoc+"\" style=\"color: green;\">"+meta.url_apiDoc+"</a>");
    ui->textEditEngineSummary->setText(meta.summary);
    ui->comboBoxOptions->addItems(meta.flags.keys());
}

void Raw::on_moduleSecurityTrails_clicked(){
    ui->comboBoxOptions->clear();
    ModuleInfo::SecurityTrails meta;
    m_optionSet = meta.flags;
    ui->labelUrl->setText("<a href=\""+meta.url+"\" style=\"color: green;\">"+meta.name+"</a>");
    ui->labelApiDoc->setText("<a href=\""+meta.url_apiDoc+"\" style=\"color: green;\">"+meta.url_apiDoc+"</a>");
    ui->textEditEngineSummary->setText(meta.summary);
    ui->comboBoxOptions->addItems(meta.flags.keys());
}

void Raw::on_moduleShodan_clicked(){
    ui->comboBoxOptions->clear();
    ModuleInfo::Shodan meta;
    m_optionSet = meta.flags;
    ui->labelUrl->setText("<a href=\""+meta.url+"\" style=\"color: green;\">"+meta.name+"</a>");
    ui->labelApiDoc->setText("<a href=\""+meta.url_apiDoc+"\" style=\"color: green;\">"+meta.url_apiDoc+"</a>");
    ui->textEditEngineSummary->setText(meta.summary);
    ui->comboBoxOptions->addItems(meta.flags.keys());
}

void Raw::on_moduleThreatBook_clicked(){
    ui->comboBoxOptions->clear();
    ModuleInfo::ThreatBook meta;
    m_optionSet = meta.flags;
    ui->labelUrl->setText("<a href=\""+meta.url+"\" style=\"color: green;\">"+meta.name+"</a>");
    ui->labelApiDoc->setText("<a href=\""+meta.url_apiDoc+"\" style=\"color: green;\">"+meta.url_apiDoc+"</a>");
    ui->textEditEngineSummary->setText(meta.summary);
    ui->comboBoxOptions->addItems(meta.flags.keys());
}

void Raw::on_moduleWhoisXmlApi_clicked(){
    ui->comboBoxOptions->clear();
    ModuleInfo::WhoisXmlApi meta;
    m_optionSet = meta.flags;
    ui->labelUrl->setText("<a href=\""+meta.url+"\" style=\"color: green;\">"+meta.name+"</a>");
    ui->labelApiDoc->setText("<a href=\""+meta.url_apiDoc+"\" style=\"color: green;\">"+meta.url_apiDoc+"</a>");
    ui->textEditEngineSummary->setText(meta.summary);
    ui->comboBoxOptions->addItems(meta.flags.keys());
}

void Raw::on_moduleZETALytics_clicked(){
    ui->comboBoxOptions->clear();
    ModuleInfo::ZETAlytics meta;
    m_optionSet = meta.flags;
    ui->labelUrl->setText("<a href=\""+meta.url+"\" style=\"color: green;\">"+meta.name+"</a>");
    ui->labelApiDoc->setText("<a href=\""+meta.url_apiDoc+"\" style=\"color: green;\">"+meta.url_apiDoc+"</a>");
    ui->textEditEngineSummary->setText(meta.summary);
    ui->comboBoxOptions->addItems(meta.flags.keys());
}

void Raw::on_moduleZoomEye_clicked(){
    ui->comboBoxOptions->clear();
    ModuleInfo::ZoomEye meta;
    m_optionSet = meta.flags;
    ui->labelUrl->setText("<a href=\""+meta.url+"\" style=\"color: green;\">"+meta.name+"</a>");
    ui->labelApiDoc->setText("<a href=\""+meta.url_apiDoc+"\" style=\"color: green;\">"+meta.url_apiDoc+"</a>");
    ui->textEditEngineSummary->setText(meta.summary);
    ui->comboBoxOptions->addItems(meta.flags.keys());
}

void Raw::on_moduleIpApi_clicked(){
    ui->comboBoxOptions->clear();
    ModuleInfo::IpApi meta;
    m_optionSet = meta.flags;
    ui->labelUrl->setText("<a href=\""+meta.url+"\" style=\"color: green;\">"+meta.name+"</a>");
    ui->labelApiDoc->setText("<a href=\""+meta.url_apiDoc+"\" style=\"color: green;\">"+meta.url_apiDoc+"</a>");
    ui->textEditEngineSummary->setText(meta.summary);
    ui->comboBoxOptions->addItems(meta.flags.keys());
}

void Raw::on_moduleViewDns_clicked(){
    ui->comboBoxOptions->clear();
    ModuleInfo::ViewDns meta;
    m_optionSet = meta.flags;
    ui->labelUrl->setText("<a href=\""+meta.url+"\" style=\"color: green;\">"+meta.name+"</a>");
    ui->labelApiDoc->setText("<a href=\""+meta.url_apiDoc+"\" style=\"color: green;\">"+meta.url_apiDoc+"</a>");
    ui->textEditEngineSummary->setText(meta.summary);
    ui->comboBoxOptions->addItems(meta.flags.keys());
}

void Raw::on_moduleWebResolver_clicked(){
    ui->comboBoxOptions->clear();
    ModuleInfo::WebResolver meta;
    m_optionSet = meta.flags;
    ui->labelUrl->setText("<a href=\""+meta.url+"\" style=\"color: green;\">"+meta.name+"</a>");
    ui->labelApiDoc->setText("<a href=\""+meta.url_apiDoc+"\" style=\"color: green;\">"+meta.url_apiDoc+"</a>");
    ui->textEditEngineSummary->setText(meta.summary);
    ui->comboBoxOptions->addItems(meta.flags.keys());
}

void Raw::on_moduleCircl_clicked(){
    ui->comboBoxOptions->clear();
    ModuleInfo::Circl meta;
    m_optionSet = meta.flags;
    ui->labelUrl->setText("<a href=\""+meta.url+"\" style=\"color: green;\">"+meta.name+"</a>");
    ui->labelApiDoc->setText("<a href=\""+meta.url_apiDoc+"\" style=\"color: green;\">"+meta.url_apiDoc+"</a>");
    ui->textEditEngineSummary->setText(meta.summary);
    ui->comboBoxOptions->addItems(meta.flags.keys());
}

void Raw::on_moduleHackerTarget_clicked(){
    ui->comboBoxOptions->clear();
    ModuleInfo::HackerTargetPaid meta;
    m_optionSet = meta.flags;
    ui->labelUrl->setText("<a href=\""+meta.url+"\" style=\"color: green;\">"+meta.name+"</a>");
    ui->labelApiDoc->setText("<a href=\""+meta.url_apiDoc+"\" style=\"color: green;\">"+meta.url_apiDoc+"</a>");
    ui->textEditEngineSummary->setText(meta.summary);
    ui->comboBoxOptions->addItems(meta.flags.keys());
}

void Raw::on_moduleMnemonic_clicked(){
    ui->comboBoxOptions->clear();
    ModuleInfo::MnemonicPaid meta;
    m_optionSet = meta.flags;
    ui->labelUrl->setText("<a href=\""+meta.url+"\" style=\"color: green;\">"+meta.name+"</a>");
    ui->labelApiDoc->setText("<a href=\""+meta.url_apiDoc+"\" style=\"color: green;\">"+meta.url_apiDoc+"</a>");
    ui->textEditEngineSummary->setText(meta.summary);
    ui->comboBoxOptions->addItems(meta.flags.keys());
}
