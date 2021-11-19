#include "Osint.h"
#include "ui_Osint.h"
//...
#include "src/modules/osint/OsintModulesHeaders.h"


/*********************************************************************************
                                Start Scan
**********************************************************************************/

void Osint::startScan(){
    ScanArgs *scanArgs = new ScanArgs;
    scanArgs->target = ui->lineEditTarget->text();

    /* getting input type as specified by user */
    switch(ui->comboBoxInput->currentIndex()){
    case INPUT_DOMAIN:
        scanArgs->inputDomain = true;
        break;
    case INPUT_IP:
        scanArgs->inputIp = true;
        break;
    case INPUT_EMAIL:
        scanArgs->inputEmail = true;
        break;
    case INPUT_URL:
        scanArgs->inputUrl = true;
        break;
    case INPUT_ASN:
        scanArgs->inputAsn = true;
        break;
    case INPUT_SSLCERT:
        scanArgs->inputSSLCert = true;
        break;
    }

    /* getting output type as specified by user */
    switch(ui->comboBoxOutput->currentIndex()){
    case OUTPUT_SUBDOMAINIP:
        scanArgs->outputSubdomainIp = true;
        break;
    case OUTPUT_SUBDOMAIN:
        scanArgs->outputSubdomain = true;
        break;
    case OUTPUT_IP:
        scanArgs->outputIp = true;
        break;
    case OUTPUT_EMAIL:
        scanArgs->outputEmail = true;
        break;
    case OUTPUT_URL:
        scanArgs->outputUrl = true;
        break;
    case OUTPUT_ASN:
        scanArgs->outputAsn = true;
        break;
    case OUTPUT_SSLCERT:
        scanArgs->outputSSLCert = true;
        break;
    }

    if(ui->moduleAnubis->isChecked())
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
    if(ui->moduleBgpview->isChecked()){
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
    if(ui->moduleBinaryEdge->isChecked()){
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
    if(ui->moduleC99->isChecked()){
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
    if(ui->moduleCircl->isChecked()){
        Circl *circl = new Circl(scanArgs);
        QThread *cThread = new QThread(this);
        circl->Enumerator(cThread);
        circl->moveToThread(cThread);
        //...
        connect(circl, &Circl::ipA, this, &Osint::onResultA);
        connect(circl, &Circl::ipAAAA, this, &Osint::onResultAAAA);
        connect(circl, &Circl::MX, this, &Osint::onResultMX);
        connect(circl, &Circl::NS, this, &Osint::onResultNS);
        connect(circl, &Circl::CNAME, this, &Osint::onResultCNAME);
        connect(circl, &Circl::asn, this, &Osint::onResultAsn);
        connect(circl, &Circl::sslCert, this, &Osint::onResultSSLCert);
        connect(circl, &Circl::errorLog, this, &Osint::onErrorLog);
        connect(circl, &Circl::infoLog, this, &Osint::onInfoLog);
        connect(cThread, &QThread::finished, this, &Osint::onScanThreadEnded);
        connect(cThread, &QThread::finished, circl, &Circl::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        status->osint->activeThreads++;
    }
    if(ui->moduleDnsbufferoverrun->isChecked())
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
    if(ui->moduleGithub->isChecked()){
        Github *github = new Github(scanArgs);
        QThread *cThread = new QThread(this);
        github->Enumerator(cThread);
        github->moveToThread(cThread);
        //...
        connect(github, &Github::subdomain, this, &Osint::onResultSubdomain);
        connect(github, &Github::errorLog, this, &Osint::onErrorLog);
        connect(github, &Github::infoLog, this, &Osint::onInfoLog);
        connect(cThread, &QThread::finished, this, &Osint::onScanThreadEnded);
        connect(cThread, &QThread::finished, github, &Github::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        status->osint->activeThreads++;
    }
    if(ui->moduleHackerTargetFree->isChecked())
    {
        HackerTargetFree *hackertarget = new HackerTargetFree(scanArgs);
        QThread *cThread = new QThread(this);
        hackertarget->Enumerator(cThread);
        hackertarget->moveToThread(cThread);
        //...
        connect(hackertarget, &HackerTargetFree::subdomain, this, &Osint::onResultSubdomain);
        connect(hackertarget, &HackerTargetFree::subdomainIp, this, &Osint::onResultSubdomainIp);
        connect(hackertarget, &HackerTargetFree::ipA, this, &Osint::onResultA);
        connect(hackertarget, &HackerTargetFree::ipAAAA, this, &Osint::onResultAAAA);
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
    if(ui->moduleHackerTargetPaid->isChecked())
    {
        HackerTargetPaid *hackertarget = new HackerTargetPaid(scanArgs);
        QThread *cThread = new QThread(this);
        hackertarget->Enumerator(cThread);
        hackertarget->moveToThread(cThread);
        //...
        connect(hackertarget, &HackerTargetPaid::subdomain, this, &Osint::onResultSubdomain);
        connect(hackertarget, &HackerTargetPaid::subdomainIp, this, &Osint::onResultSubdomainIp);
        connect(hackertarget, &HackerTargetPaid::ipA, this, &Osint::onResultA);
        connect(hackertarget, &HackerTargetPaid::ipAAAA, this, &Osint::onResultAAAA);
        connect(hackertarget, &HackerTargetPaid::asn, this, &Osint::onResultAsn);
        connect(hackertarget, &HackerTargetPaid::errorLog, this, &Osint::onErrorLog);
        connect(hackertarget, &HackerTargetPaid::infoLog, this, &Osint::onInfoLog);
        connect(cThread, &QThread::finished, this, &Osint::onScanThreadEnded);
        connect(cThread, &QThread::finished, hackertarget, &HackerTargetPaid::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        status->osint->activeThreads++;
    }
    if(ui->moduleHunter->isChecked()){
        Hunter *hunter = new Hunter(scanArgs);
        QThread *cThread = new QThread(this);
        hunter->Enumerator(cThread);
        hunter->moveToThread(cThread);
        //...
        connect(hunter, &Hunter::email, this, &Osint::onResultEmail);
        connect(hunter, &Hunter::errorLog, this, &Osint::onErrorLog);
        connect(hunter, &Hunter::infoLog, this, &Osint::onInfoLog);
        connect(cThread, &QThread::finished, this, &Osint::onScanThreadEnded);
        connect(cThread, &QThread::finished, hunter, &Hunter::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        status->osint->activeThreads++;
    }
    if(ui->moduleMnemonicFree->isChecked()){
        MnemonicFree *mnemonic = new MnemonicFree(scanArgs);
        QThread *cThread = new QThread(this);
        mnemonic->Enumerator(cThread);
        mnemonic->moveToThread(cThread);
        //...
        connect(mnemonic, &MnemonicFree::ipA, this, &Osint::onResultA);
        connect(mnemonic, &MnemonicFree::ipAAAA, this, &Osint::onResultAAAA);
        connect(mnemonic, &MnemonicFree::CNAME, this, &Osint::onResultCNAME);
        connect(mnemonic, &MnemonicFree::MX, this, &Osint::onResultMX);
        connect(mnemonic, &MnemonicFree::NS, this, &Osint::onResultNS);
        connect(mnemonic, &MnemonicFree::errorLog, this, &Osint::onErrorLog);
        connect(mnemonic, &MnemonicFree::infoLog, this, &Osint::onInfoLog);
        connect(cThread, &QThread::finished, this, &Osint::onScanThreadEnded);
        connect(cThread, &QThread::finished, mnemonic, &MnemonicFree::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        status->osint->activeThreads++;
    }
    if(ui->moduleMnemonicPaid->isChecked()){
        MnemonicPaid *mnemonic = new MnemonicPaid(scanArgs);
        QThread *cThread = new QThread(this);
        mnemonic->Enumerator(cThread);
        mnemonic->moveToThread(cThread);
        //...
        connect(mnemonic, &MnemonicPaid::ipA, this, &Osint::onResultA);
        connect(mnemonic, &MnemonicPaid::ipAAAA, this, &Osint::onResultAAAA);
        connect(mnemonic, &MnemonicPaid::CNAME, this, &Osint::onResultCNAME);
        connect(mnemonic, &MnemonicPaid::MX, this, &Osint::onResultMX);
        connect(mnemonic, &MnemonicPaid::NS, this, &Osint::onResultNS);
        connect(mnemonic, &MnemonicPaid::errorLog, this, &Osint::onErrorLog);
        connect(mnemonic, &MnemonicPaid::infoLog, this, &Osint::onInfoLog);
        connect(cThread, &QThread::finished, this, &Osint::onScanThreadEnded);
        connect(cThread, &QThread::finished, mnemonic, &MnemonicPaid::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        status->osint->activeThreads++;
    }
    if(ui->moduleOmnisint->isChecked()){
        Omnisint *omnisint = new Omnisint(scanArgs);
        QThread *cThread = new QThread(this);
        omnisint->Enumerator(cThread);
        omnisint->moveToThread(cThread);
        //...
        connect(omnisint, &Omnisint::subdomain, this, &Osint::onResultSubdomain);
        connect(omnisint, &Omnisint::errorLog, this, &Osint::onErrorLog);
        connect(omnisint, &Omnisint::infoLog, this, &Osint::onInfoLog);
        connect(cThread, &QThread::finished, this, &Osint::onScanThreadEnded);
        connect(cThread, &QThread::finished, omnisint, &Omnisint::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        status->osint->activeThreads++;
    }
    if(ui->moduleOtxFree->isChecked())
    {
        OtxFree *otx = new OtxFree(scanArgs);
        QThread *cThread = new QThread(this);
        otx->Enumerator(cThread);
        otx->moveToThread(cThread);
        //...
        connect(otx, &OtxFree::subdomain, this, &Osint::onResultSubdomain);
        connect(otx, &OtxFree::subdomainIp, this, &Osint::onResultSubdomainIp);
        connect(otx, &OtxFree::ipA, this, &Osint::onResultA);
        connect(otx, &OtxFree::ipAAAA, this, &Osint::onResultAAAA);
        connect(otx, &OtxFree::CNAME, this, &Osint::onResultCNAME);
        connect(otx, &OtxFree::NS, this, &Osint::onResultNS);
        connect(otx, &OtxFree::MX, this, &Osint::onResultMX);
        connect(otx, &OtxFree::asn, this, &Osint::onResultAsn);
        connect(otx, &OtxFree::errorLog, this, &Osint::onErrorLog);
        connect(otx, &OtxFree::infoLog, this, &Osint::onInfoLog);
        connect(cThread, &QThread::finished, this, &Osint::onScanThreadEnded);
        connect(cThread, &QThread::finished, otx, &OtxFree::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        status->osint->activeThreads++;
    }
    if(ui->moduleOtxPaid->isChecked())
    {
        OtxPaid *otx = new OtxPaid(scanArgs);
        QThread *cThread = new QThread(this);
        otx->Enumerator(cThread);
        otx->moveToThread(cThread);
        //...
        connect(otx, &OtxPaid::subdomain, this, &Osint::onResultSubdomain);
        connect(otx, &OtxPaid::subdomainIp, this, &Osint::onResultSubdomainIp);
        connect(otx, &OtxPaid::ipA, this, &Osint::onResultA);
        connect(otx, &OtxPaid::ipAAAA, this, &Osint::onResultAAAA);
        connect(otx, &OtxPaid::CNAME, this, &Osint::onResultCNAME);
        connect(otx, &OtxPaid::NS, this, &Osint::onResultNS);
        connect(otx, &OtxPaid::MX, this, &Osint::onResultMX);
        connect(otx, &OtxPaid::asn, this, &Osint::onResultAsn);
        connect(otx, &OtxPaid::errorLog, this, &Osint::onErrorLog);
        connect(otx, &OtxPaid::infoLog, this, &Osint::onInfoLog);
        connect(cThread, &QThread::finished, this, &Osint::onScanThreadEnded);
        connect(cThread, &QThread::finished, otx, &OtxPaid::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        status->osint->activeThreads++;
    }
    if(ui->moduleProjectdiscovery->isChecked())
    {
        Projectdiscovery *projectdiscovery = new Projectdiscovery(scanArgs);
        QThread *cThread = new QThread(this);
        projectdiscovery->Enumerator(cThread);
        projectdiscovery->moveToThread(cThread);
        //...
        connect(projectdiscovery, &Projectdiscovery::subdomain, this, &Osint::onResultSubdomain);
        connect(projectdiscovery, &Projectdiscovery::errorLog, this, &Osint::onErrorLog);
        connect(projectdiscovery, &Projectdiscovery::infoLog, this, &Osint::onInfoLog);
        connect(cThread, &QThread::finished, this, &Osint::onScanThreadEnded);
        connect(cThread, &QThread::finished, projectdiscovery, &Projectdiscovery::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        status->osint->activeThreads++;
    }
    if(ui->moduleRiskIq->isChecked()){
        RiskIq *riskiq = new RiskIq(scanArgs);
        QThread *cThread = new QThread(this);
        riskiq->Enumerator(cThread);
        riskiq->moveToThread(cThread);
        //...
        connect(riskiq, &RiskIq::subdomain, this, &Osint::onResultSubdomain);
        connect(riskiq, &RiskIq::subdomainIp, this, &Osint::onResultSubdomainIp);
        connect(riskiq, &RiskIq::ip, this, &Osint::onResultIp);
        connect(riskiq, &RiskIq::ipA, this, &Osint::onResultA);
        connect(riskiq, &RiskIq::ipAAAA, this, &Osint::onResultAAAA);
        connect(riskiq, &RiskIq::CNAME, this, &Osint::onResultCNAME);
        connect(riskiq, &RiskIq::NS, this, &Osint::onResultNS);
        connect(riskiq, &RiskIq::MX, this, &Osint::onResultMX);
        connect(riskiq, &RiskIq::sslCert, this, &Osint::onResultSSLCert);
        connect(riskiq, &RiskIq::errorLog, this, &Osint::onErrorLog);
        connect(riskiq, &RiskIq::infoLog, this, &Osint::onInfoLog);
        connect(cThread, &QThread::finished, this, &Osint::onScanThreadEnded);
        connect(cThread, &QThread::finished, riskiq, &RiskIq::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        status->osint->activeThreads++;
    }
    if(ui->moduleRobtexFree->isChecked()){
        RobtexFree *robtex = new RobtexFree(scanArgs);
        QThread *cThread = new QThread(this);
        robtex->Enumerator(cThread);
        robtex->moveToThread(cThread);
        //...
        connect(robtex, &RobtexFree::subdomain, this, &Osint::onResultSubdomain);
        connect(robtex, &RobtexFree::subdomainIp, this, &Osint::onResultSubdomainIp);
        connect(robtex, &RobtexFree::ip, this, &Osint::onResultIp);
        connect(robtex, &RobtexFree::ipA, this, &Osint::onResultA);
        connect(robtex, &RobtexFree::ipAAAA, this, &Osint::onResultAAAA);
        connect(robtex, &RobtexFree::NS, this, &Osint::onResultNS);
        connect(robtex, &RobtexFree::MX, this, &Osint::onResultMX);
        connect(robtex, &RobtexFree::CNAME, this, &Osint::onResultCNAME);
        connect(robtex, &RobtexFree::asn, this, &Osint::onResultAsn);
        connect(robtex, &RobtexFree::errorLog, this, &Osint::onErrorLog);
        connect(robtex, &RobtexFree::infoLog, this, &Osint::onInfoLog);
        connect(cThread, &QThread::finished, this, &Osint::onScanThreadEnded);
        connect(cThread, &QThread::finished, robtex, &RobtexFree::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        status->osint->activeThreads++;
    }
    if(ui->moduleRobtexPaid->isChecked()){
        RobtexPaid *robtex = new RobtexPaid(scanArgs);
        QThread *cThread = new QThread(this);
        robtex->Enumerator(cThread);
        robtex->moveToThread(cThread);
        //...
        connect(robtex, &RobtexPaid::subdomain, this, &Osint::onResultSubdomain);
        connect(robtex, &RobtexPaid::subdomainIp, this, &Osint::onResultSubdomainIp);
        connect(robtex, &RobtexPaid::ip, this, &Osint::onResultIp);
        connect(robtex, &RobtexPaid::ipA, this, &Osint::onResultA);
        connect(robtex, &RobtexPaid::ipAAAA, this, &Osint::onResultAAAA);
        connect(robtex, &RobtexPaid::NS, this, &Osint::onResultNS);
        connect(robtex, &RobtexPaid::MX, this, &Osint::onResultMX);
        connect(robtex, &RobtexPaid::CNAME, this, &Osint::onResultCNAME);
        connect(robtex, &RobtexPaid::asn, this, &Osint::onResultAsn);
        connect(robtex, &RobtexPaid::errorLog, this, &Osint::onErrorLog);
        connect(robtex, &RobtexPaid::infoLog, this, &Osint::onInfoLog);
        connect(cThread, &QThread::finished, this, &Osint::onScanThreadEnded);
        connect(cThread, &QThread::finished, robtex, &RobtexPaid::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        status->osint->activeThreads++;
    }
    if(ui->moduleSecuritytrails->isChecked()){
        SecurityTrails *securitytrails = new SecurityTrails(scanArgs);
        QThread *cThread = new QThread(this);
        securitytrails->Enumerator(cThread);
        securitytrails->moveToThread(cThread);
        //...
        connect(securitytrails, &SecurityTrails::subdomain, this, &Osint::onResultSubdomain);
        connect(securitytrails, &SecurityTrails::ip, this, &Osint::onResultIp);
        connect(securitytrails, &SecurityTrails::ipA, this, &Osint::onResultA);
        connect(securitytrails, &SecurityTrails::ipAAAA, this, &Osint::onResultAAAA);
        connect(securitytrails, &SecurityTrails::NS, this, &Osint::onResultNS);
        connect(securitytrails, &SecurityTrails::MX, this, &Osint::onResultMX);
        connect(securitytrails, &SecurityTrails::TXT, this, &Osint::onResultTXT);
        connect(securitytrails, &SecurityTrails::errorLog, this, &Osint::onErrorLog);
        connect(securitytrails, &SecurityTrails::infoLog, this, &Osint::onInfoLog);
        connect(cThread, &QThread::finished, this, &Osint::onScanThreadEnded);
        connect(cThread, &QThread::finished, securitytrails, &SecurityTrails::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        status->osint->activeThreads++;
    }
    if(ui->moduleShodan->isChecked()){
        Shodan *shodan = new Shodan(scanArgs);
        QThread *cThread = new QThread(this);
        shodan->Enumerator(cThread);
        shodan->moveToThread(cThread);
        //...
        connect(shodan, &Shodan::subdomain, this, &Osint::onResultSubdomain);
        connect(shodan, &Shodan::subdomainIp, this, &Osint::onResultSubdomainIp);
        connect(shodan, &Shodan::ip, this, &Osint::onResultIp);
        connect(shodan, &Shodan::ipA, this, &Osint::onResultA);
        connect(shodan, &Shodan::ipAAAA, this, &Osint::onResultAAAA);
        connect(shodan, &Shodan::NS, this, &Osint::onResultNS);
        connect(shodan, &Shodan::MX, this, &Osint::onResultMX);
        connect(shodan, &Shodan::CNAME, this, &Osint::onResultCNAME);
        connect(shodan, &Shodan::TXT, this, &Osint::onResultTXT);
        connect(shodan, &Shodan::errorLog, this, &Osint::onErrorLog);
        connect(shodan, &Shodan::infoLog, this, &Osint::onInfoLog);
        connect(cThread, &QThread::finished, this, &Osint::onScanThreadEnded);
        connect(cThread, &QThread::finished, shodan, &Shodan::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        status->osint->activeThreads++;
    }
    if(ui->moduleSpyse->isChecked())
    {
        Spyse *spyse = new Spyse(scanArgs);
        QThread *cThread = new QThread(this);
        spyse->Enumerator(cThread);
        spyse->moveToThread(cThread);
        //...
        connect(spyse, &Spyse::subdomain, this, &Osint::onResultSubdomain);
        connect(spyse, &Spyse::ip, this, &Osint::onResultIp);
        connect(spyse, &Spyse::ipA, this, &Osint::onResultA);
        connect(spyse, &Spyse::ipAAAA, this, &Osint::onResultAAAA);
        connect(spyse, &Spyse::email, this, &Osint::onResultEmail);
        connect(spyse, &Spyse::url, this, &Osint::onResultUrl);
        connect(spyse, &Spyse::NS, this, &Osint::onResultNS);
        connect(spyse, &Spyse::MX, this, &Osint::onResultMX);
        connect(spyse, &Spyse::TXT, this, &Osint::onResultTXT);
        connect(spyse, &Spyse::CNAME, this, &Osint::onResultCNAME);
        connect(spyse, &Spyse::sslCert, this, &Osint::onResultSSLCert);
        connect(spyse, &Spyse::errorLog, this, &Osint::onErrorLog);
        connect(spyse, &Spyse::infoLog, this, &Osint::onInfoLog);
        connect(cThread, &QThread::finished, this, &Osint::onScanThreadEnded);
        connect(cThread, &QThread::finished, spyse, &Spyse::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        status->osint->activeThreads++;
    }
    if(ui->moduleSublist3r->isChecked())
    {
        Sublist3r *sublist3r = new Sublist3r(scanArgs);
        QThread *cThread = new QThread(this);
        sublist3r->Enumerator(cThread);
        sublist3r->moveToThread(cThread);
        //...
        connect(sublist3r, &Sublist3r::subdomain, this, &Osint::onResultSubdomain);
        connect(sublist3r, &Sublist3r::errorLog, this, &Osint::onErrorLog);
        connect(sublist3r, &Sublist3r::infoLog, this, &Osint::onInfoLog);
        connect(cThread, &QThread::finished, this, &Osint::onScanThreadEnded);
        connect(cThread, &QThread::finished, sublist3r, &Sublist3r::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        status->osint->activeThreads++;
    }
    if(ui->moduleThreatBook->isChecked()){
        ThreatBook *threatbook = new ThreatBook(scanArgs);
        QThread *cThread = new QThread(this);
        threatbook->Enumerator(cThread);
        threatbook->moveToThread(cThread);
        //...
        connect(threatbook, &ThreatBook::subdomain, this, &Osint::onResultSubdomain);
        connect(threatbook, &ThreatBook::ip, this, &Osint::onResultIp);
        connect(threatbook, &ThreatBook::asn, this, &Osint::onResultAsn);
        connect(threatbook, &ThreatBook::errorLog, this, &Osint::onErrorLog);
        connect(threatbook, &ThreatBook::infoLog, this, &Osint::onInfoLog);
        connect(cThread, &QThread::finished, this, &Osint::onScanThreadEnded);
        connect(cThread, &QThread::finished, threatbook, &ThreatBook::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        status->osint->activeThreads++;
    }
    if(ui->moduleThreatcrowd->isChecked())
    {
        Threatcrowd *threatcrowd = new Threatcrowd(scanArgs);
        QThread *cThread = new QThread(this);
        threatcrowd->Enumerator(cThread);
        threatcrowd->moveToThread(cThread);
        //...
        connect(threatcrowd, &Threatcrowd::subdomain, this, &Osint::onResultSubdomain);
        connect(threatcrowd, &Threatcrowd::ip, this, &Osint::onResultIp);
        connect(threatcrowd, &Threatcrowd::email, this, &Osint::onResultEmail);
        connect(threatcrowd, &Threatcrowd::errorLog, this, &Osint::onErrorLog);
        connect(threatcrowd, &Threatcrowd::infoLog, this, &Osint::onInfoLog);
        connect(cThread, &QThread::finished, this, &Osint::onScanThreadEnded);
        connect(cThread, &QThread::finished, threatcrowd, &Threatcrowd::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        status->osint->activeThreads++;
    }
    if(ui->moduleThreatminer->isChecked())
    {
        Threatminer *threatminer = new Threatminer(scanArgs);
        QThread *cThread = new QThread(this);
        threatminer->Enumerator(cThread);
        threatminer->moveToThread(cThread);
        //...
        connect(threatminer, &Threatminer::subdomain, this, &Osint::onResultSubdomain);
        connect(threatminer, &Threatminer::sslCert, this, &Osint::onResultSSLCert);
        connect(threatminer, &Threatminer::ip, this, &Osint::onResultIp);
        connect(threatminer, &Threatminer::email, this, &Osint::onResultEmail);
        connect(threatminer, &Threatminer::url, this, &Osint::onResultUrl);
        connect(threatminer, &Threatminer::asn, this, &Osint::onResultAsn);
        connect(threatminer, &Threatminer::errorLog, this, &Osint::onErrorLog);
        connect(threatminer, &Threatminer::infoLog, this, &Osint::onInfoLog);
        connect(cThread, &QThread::finished, this, &Osint::onScanThreadEnded);
        connect(cThread, &QThread::finished, threatminer, &Threatminer::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        status->osint->activeThreads++;
    }
    if(ui->moduleViewDns->isChecked()){
        ViewDns *viewdns = new ViewDns(scanArgs);
        QThread *cThread = new QThread(this);
        viewdns->Enumerator(cThread);
        viewdns->moveToThread(cThread);
        //...
        connect(viewdns, &ViewDns::subdomain, this, &Osint::onResultSubdomain);
        connect(viewdns, &ViewDns::email, this, &Osint::onResultEmail);
        connect(viewdns, &ViewDns::NS, this, &Osint::onResultNS);
        connect(viewdns, &ViewDns::MX, this, &Osint::onResultMX);
        connect(viewdns, &ViewDns::TXT, this, &Osint::onResultTXT);
        connect(viewdns, &ViewDns::CNAME, this, &Osint::onResultCNAME);
        connect(viewdns, &ViewDns::ipA, this, &Osint::onResultA);
        connect(viewdns, &ViewDns::ipAAAA, this, &Osint::onResultAAAA);
        connect(viewdns, &ViewDns::ip, this, &Osint::onResultIp);
        connect(viewdns, &ViewDns::errorLog, this, &Osint::onErrorLog);
        connect(viewdns, &ViewDns::infoLog, this, &Osint::onInfoLog);
        connect(cThread, &QThread::finished, this, &Osint::onScanThreadEnded);
        connect(cThread, &QThread::finished, viewdns, &ViewDns::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        status->osint->activeThreads++;
    }
    if(ui->moduleVirusTotal->isChecked()){
        VirusTotal *virustotal = new VirusTotal(scanArgs);
        QThread *cThread = new QThread(this);
        virustotal->Enumerator(cThread);
        virustotal->moveToThread(cThread);
        //...
        connect(virustotal, &VirusTotal::subdomain, this, &Osint::onResultSubdomain);
        connect(virustotal, &VirusTotal::ip, this, &Osint::onResultIp);
        connect(virustotal, &VirusTotal::ipA, this, &Osint::onResultA);
        connect(virustotal, &VirusTotal::ipAAAA, this, &Osint::onResultAAAA);
        connect(virustotal, &VirusTotal::CNAME, this, &Osint::onResultCNAME);
        connect(virustotal, &VirusTotal::NS, this, &Osint::onResultNS);
        connect(virustotal, &VirusTotal::MX, this, &Osint::onResultMX);
        connect(virustotal, &VirusTotal::sslCert, this, &Osint::onResultSSLCert);
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
    if(ui->moduleWebResolver->isChecked()){
        WebResolver *webresolver = new WebResolver(scanArgs);
        QThread *cThread = new QThread(this);
        webresolver->Enumerator(cThread);
        webresolver->moveToThread(cThread);
        //...
        connect(webresolver, &WebResolver::subdomain, this, &Osint::onResultSubdomain);
        connect(webresolver, &WebResolver::ip, this, &Osint::onResultIp);
        connect(webresolver, &WebResolver::subdomainIp, this, &Osint::onResultSubdomainIp);
        connect(webresolver, &WebResolver::CNAME, this, &Osint::onResultCNAME);
        connect(webresolver, &WebResolver::NS, this, &Osint::onResultNS);
        connect(webresolver, &WebResolver::MX, this, &Osint::onResultMX);
        connect(webresolver, &WebResolver::TXT, this, &Osint::onResultTXT);
        connect(webresolver, &WebResolver::errorLog, this, &Osint::onErrorLog);
        connect(webresolver, &WebResolver::infoLog, this, &Osint::onInfoLog);
        connect(cThread, &QThread::finished, this, &Osint::onScanThreadEnded);
        connect(cThread, &QThread::finished, webresolver, &WebResolver::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        status->osint->activeThreads++;
    }
    if(ui->moduleWhoisXmlApi->isChecked()){
        WhoisXmlApi *whoisxmlapi = new WhoisXmlApi(scanArgs);
        QThread *cThread = new QThread(this);
        whoisxmlapi->Enumerator(cThread);
        whoisxmlapi->moveToThread(cThread);
        //...
        connect(whoisxmlapi, &WhoisXmlApi::subdomain, this, &Osint::onResultSubdomain);
        connect(whoisxmlapi, &WhoisXmlApi::ip, this, &Osint::onResultIp);
        connect(whoisxmlapi, &WhoisXmlApi::email, this, &Osint::onResultEmail);
        connect(whoisxmlapi, &WhoisXmlApi::asn, this, &Osint::onResultAsn);
        connect(whoisxmlapi, &WhoisXmlApi::NS, this, &Osint::onResultNS);
        connect(whoisxmlapi, &WhoisXmlApi::MX, this, &Osint::onResultMX);
        connect(whoisxmlapi, &WhoisXmlApi::CNAME, this, &Osint::onResultCNAME);
        connect(whoisxmlapi, &WhoisXmlApi::TXT, this, &Osint::onResultTXT);
        connect(whoisxmlapi, &WhoisXmlApi::errorLog, this, &Osint::onErrorLog);
        connect(whoisxmlapi, &WhoisXmlApi::infoLog, this, &Osint::onInfoLog);
        connect(cThread, &QThread::finished, this, &Osint::onScanThreadEnded);
        connect(cThread, &QThread::finished, whoisxmlapi, &WhoisXmlApi::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        status->osint->activeThreads++;
    }
    if(ui->moduleZetalytics->isChecked()){
        ZETAlytics *zetalytics = new ZETAlytics(scanArgs);
        QThread *cThread = new QThread(this);
        zetalytics->Enumerator(cThread);
        zetalytics->moveToThread(cThread);
        //...
        connect(zetalytics, &ZETAlytics::subdomain, this, &Osint::onResultSubdomain);
        connect(zetalytics, &ZETAlytics::errorLog, this, &Osint::onErrorLog);
        connect(zetalytics, &ZETAlytics::infoLog, this, &Osint::onInfoLog);
        connect(cThread, &QThread::finished, this, &Osint::onScanThreadEnded);
        connect(cThread, &QThread::finished, zetalytics, &ZETAlytics::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        status->osint->activeThreads++;
    }
    if(ui->moduleZoomeye->isChecked()){
        ZoomEye *zoomeye = new ZoomEye(scanArgs);
        QThread *cThread = new QThread(this);
        zoomeye->Enumerator(cThread);
        zoomeye->moveToThread(cThread);
        //...
        connect(zoomeye, &ZoomEye::subdomain, this, &Osint::onResultSubdomain);
        connect(zoomeye, &ZoomEye::subdomainIp, this, &Osint::onResultSubdomainIp);
        connect(zoomeye, &ZoomEye::ip, this, &Osint::onResultIp);
        connect(zoomeye, &ZoomEye::asn, this, &Osint::onResultAsn);
        connect(zoomeye, &ZoomEye::errorLog, this, &Osint::onErrorLog);
        connect(zoomeye, &ZoomEye::infoLog, this, &Osint::onInfoLog);
        connect(cThread, &QThread::finished, this, &Osint::onScanThreadEnded);
        connect(cThread, &QThread::finished, zoomeye, &ZoomEye::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        status->osint->activeThreads++;
    }
    if(ui->moduleIpInfo->isChecked()){
        IpInfo *ipinfo = new IpInfo(scanArgs);
        QThread *cThread = new QThread(this);
        ipinfo->Enumerator(cThread);
        ipinfo->moveToThread(cThread);
        //...
        connect(ipinfo, &IpInfo::subdomain, this, &Osint::onResultSubdomain);
        connect(ipinfo, &IpInfo::errorLog, this, &Osint::onErrorLog);
        connect(ipinfo, &IpInfo::infoLog, this, &Osint::onInfoLog);
        connect(cThread, &QThread::finished, this, &Osint::onScanThreadEnded);
        connect(cThread, &QThread::finished, ipinfo, &IpInfo::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        status->osint->activeThreads++;
    }
    if(ui->moduleDnslytics->isChecked()){
        Dnslytics *dnslytics = new Dnslytics(scanArgs);
        QThread *cThread = new QThread(this);
        dnslytics->Enumerator(cThread);
        dnslytics->moveToThread(cThread);
        //...
        connect(dnslytics, &Dnslytics::subdomain, this, &Osint::onResultSubdomain);
        connect(dnslytics, &Dnslytics::subdomainIp, this, &Osint::onResultSubdomainIp);
        connect(dnslytics, &Dnslytics::ip, this, &Osint::onResultIp);
        connect(dnslytics, &Dnslytics::ipA, this, &Osint::onResultA);
        connect(dnslytics, &Dnslytics::ipAAAA, this, &Osint::onResultAAAA);
        connect(dnslytics, &Dnslytics::NS, this, &Osint::onResultNS);
        connect(dnslytics, &Dnslytics::MX, this, &Osint::onResultMX);
        connect(dnslytics, &Dnslytics::errorLog, this, &Osint::onErrorLog);
        connect(dnslytics, &Dnslytics::infoLog, this, &Osint::onInfoLog);
        connect(cThread, &QThread::finished, this, &Osint::onScanThreadEnded);
        connect(cThread, &QThread::finished, dnslytics, &Dnslytics::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        status->osint->activeThreads++;
    }
    if(ui->moduleDomainTools->isChecked()){
        DomainTools *domaintools = new DomainTools(scanArgs);
        QThread *cThread = new QThread(this);
        domaintools->Enumerator(cThread);
        domaintools->moveToThread(cThread);
        //...
        connect(domaintools, &DomainTools::subdomain, this, &Osint::onResultSubdomain);
        connect(domaintools, &DomainTools::subdomainIp, this, &Osint::onResultSubdomainIp);
        connect(domaintools, &DomainTools::errorLog, this, &Osint::onErrorLog);
        connect(domaintools, &DomainTools::infoLog, this, &Osint::onInfoLog);
        connect(cThread, &QThread::finished, this, &Osint::onScanThreadEnded);
        connect(cThread, &QThread::finished, domaintools, &DomainTools::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        status->osint->activeThreads++;
    }
    if(ui->moduleMaltiverseFree->isChecked()){
        Maltiverse *maltiverse = new Maltiverse(scanArgs);
        QThread *cThread = new QThread(this);
        maltiverse->Enumerator(cThread);
        maltiverse->moveToThread(cThread);
        //...
        connect(maltiverse, &Maltiverse::ip, this, &Osint::onResultIp);
        connect(maltiverse, &Maltiverse::asn, this, &Osint::onResultAsn);
        connect(maltiverse, &Maltiverse::email, this, &Osint::onResultEmail);
        connect(maltiverse, &Maltiverse::errorLog, this, &Osint::onErrorLog);
        connect(maltiverse, &Maltiverse::infoLog, this, &Osint::onInfoLog);
        connect(cThread, &QThread::finished, this, &Osint::onScanThreadEnded);
        connect(cThread, &QThread::finished, maltiverse, &Maltiverse::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        status->osint->activeThreads++;
    }
    if(ui->moduleN45HT->isChecked()){
        N45HT *n45ht = new N45HT(scanArgs);
        QThread *cThread = new QThread(this);
        n45ht->Enumerator(cThread);
        n45ht->moveToThread(cThread);
        //...
        connect(n45ht, &N45HT::subdomain, this, &Osint::onResultSubdomain);
        connect(n45ht, &N45HT::errorLog, this, &Osint::onErrorLog);
        connect(n45ht, &N45HT::infoLog, this, &Osint::onInfoLog);
        connect(cThread, &QThread::finished, this, &Osint::onScanThreadEnded);
        connect(cThread, &QThread::finished, n45ht, &N45HT::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        status->osint->activeThreads++;
    }
    if(ui->moduleOnyphe->isChecked()){
        Onyphe *onyphe = new Onyphe(scanArgs);
        QThread *cThread = new QThread(this);
        onyphe->Enumerator(cThread);
        onyphe->moveToThread(cThread);
        //...
        connect(onyphe, &Onyphe::subdomain, this, &Osint::onResultSubdomain);
        connect(onyphe, &Onyphe::errorLog, this, &Osint::onErrorLog);
        connect(onyphe, &Onyphe::infoLog, this, &Osint::onInfoLog);
        connect(cThread, &QThread::finished, this, &Osint::onScanThreadEnded);
        connect(cThread, &QThread::finished, onyphe, &Onyphe::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        status->osint->activeThreads++;
    }
    if(ui->moduleRipe->isChecked()){
        Ripe *ripe = new Ripe(scanArgs);
        QThread *cThread = new QThread(this);
        ripe->Enumerator(cThread);
        ripe->moveToThread(cThread);
        /* others not yet */
        connect(ripe, &Ripe::asn, this, &Osint::onResultAsn);
        connect(ripe, &Ripe::errorLog, this, &Osint::onErrorLog);
        connect(ripe, &Ripe::infoLog, this, &Osint::onInfoLog);
        connect(cThread, &QThread::finished, this, &Osint::onScanThreadEnded);
        connect(cThread, &QThread::finished, ripe, &Ripe::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        status->osint->activeThreads++;
    }
    if(ui->moduleFullHunt->isChecked()){
        FullHunt *fullhunt = new FullHunt(scanArgs);
        QThread *cThread = new QThread(this);
        fullhunt->Enumerator(cThread);
        fullhunt->moveToThread(cThread);
        /* others not yet */
        connect(fullhunt, &FullHunt::subdomain, this, &Osint::onResultSubdomain);
        connect(fullhunt, &FullHunt::errorLog, this, &Osint::onErrorLog);
        connect(fullhunt, &FullHunt::infoLog, this, &Osint::onInfoLog);
        connect(cThread, &QThread::finished, this, &Osint::onScanThreadEnded);
        connect(cThread, &QThread::finished, fullhunt, &FullHunt::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        status->osint->activeThreads++;
    }
    if(ui->moduleNetworksDB->isChecked()){
        NetworksDB *networksdb = new NetworksDB(scanArgs);
        QThread *cThread = new QThread(this);
        networksdb->Enumerator(cThread);
        networksdb->moveToThread(cThread);
        //...
        connect(networksdb, &NetworksDB::subdomainIp, this, &Osint::onResultSubdomainIp);
        connect(networksdb, &NetworksDB::subdomain, this, &Osint::onResultSubdomain);
        connect(networksdb, &NetworksDB::ip, this, &Osint::onResultIp);
        connect(networksdb, &NetworksDB::prefix, this, &Osint::onResultPrefix);
        connect(networksdb, &NetworksDB::errorLog, this, &Osint::onErrorLog);
        connect(networksdb, &NetworksDB::infoLog, this, &Osint::onInfoLog);
        connect(cThread, &QThread::finished, this, &Osint::onScanThreadEnded);
        connect(cThread, &QThread::finished, networksdb, &NetworksDB::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        status->osint->activeThreads++;
    }
    if(ui->moduleSpyOnWeb->isChecked()){
        SpyOnWeb *spyonweb = new SpyOnWeb(scanArgs);
        QThread *cThread = new QThread(this);
        spyonweb->Enumerator(cThread);
        spyonweb->moveToThread(cThread);
        //...
        connect(spyonweb, &SpyOnWeb::subdomainIp, this, &Osint::onResultSubdomainIp);
        connect(spyonweb, &SpyOnWeb::subdomain, this, &Osint::onResultSubdomain);
        connect(spyonweb, &SpyOnWeb::ip, this, &Osint::onResultIp);
        connect(spyonweb, &SpyOnWeb::errorLog, this, &Osint::onErrorLog);
        connect(spyonweb, &SpyOnWeb::infoLog, this, &Osint::onInfoLog);
        connect(cThread, &QThread::finished, this, &Osint::onScanThreadEnded);
        connect(cThread, &QThread::finished, spyonweb, &SpyOnWeb::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        status->osint->activeThreads++;
    }

    /****************************************************************************
                                 ARCHIVES
    *****************************************************************************/

    if(ui->moduleArchiveit->isChecked()){
        ArchiveIt *archiveit = new ArchiveIt(scanArgs);
        QThread *cThread = new QThread(this);
        archiveit->Enumerator(cThread);
        archiveit->moveToThread(cThread);
        //...
        connect(archiveit, &ArchiveIt::url, this, &Osint::onResultUrl);
        connect(archiveit, &ArchiveIt::subdomain, this, &Osint::onResultSubdomain);
        connect(archiveit, &ArchiveIt::errorLog, this, &Osint::onErrorLog);
        connect(archiveit, &ArchiveIt::infoLog, this, &Osint::onInfoLog);
        connect(cThread, &QThread::finished, this, &Osint::onScanThreadEnded);
        connect(cThread, &QThread::finished, archiveit, &ArchiveIt::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        status->osint->activeThreads++;
    }
    if(ui->moduleArchiveToday->isChecked()){
        ArchiveToday *archivetoday = new ArchiveToday(scanArgs);
        QThread *cThread = new QThread(this);
        archivetoday->Enumerator(cThread);
        archivetoday->moveToThread(cThread);
        //...
        connect(archivetoday, &ArchiveToday::subdomain, this, &Osint::onResultSubdomain);
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
    if(ui->moduleArquivo->isChecked()){
        Arquivo *arquivo = new Arquivo(scanArgs);
        QThread *cThread = new QThread(this);
        arquivo->Enumerator(cThread);
        arquivo->moveToThread(cThread);
        //...
        connect(arquivo, &Arquivo::subdomain, this, &Osint::onResultSubdomain);
        connect(arquivo, &Arquivo::url, this, &Osint::onResultUrl);
        connect(arquivo, &Arquivo::errorLog, this, &Osint::onErrorLog);
        connect(arquivo, &Arquivo::infoLog, this, &Osint::onInfoLog);
        connect(cThread, &QThread::finished, this, &Osint::onScanThreadEnded);
        connect(cThread, &QThread::finished, arquivo, &Arquivo::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        status->osint->activeThreads++;
    }
    if(ui->moduleCommonCrawl->isChecked()){
        CommonCrawl *commonCrawl = new CommonCrawl(scanArgs);
        QThread *cThread = new QThread(this);
        commonCrawl->Enumerator(cThread);
        commonCrawl->moveToThread(cThread);
        //...
        connect(commonCrawl, &CommonCrawl::subdomain, this, &Osint::onResultSubdomain);
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
    if(ui->moduleUKWebArchive->isChecked()){
        UKWebArchive *ukwebarchive = new UKWebArchive(scanArgs);
        QThread *cThread = new QThread(this);
        ukwebarchive->Enumerator(cThread);
        ukwebarchive->moveToThread(cThread);
        //...
        connect(ukwebarchive, &UKWebArchive::subdomain, this, &Osint::onResultSubdomain);
        connect(ukwebarchive, &UKWebArchive::url, this, &Osint::onResultUrl);
        connect(ukwebarchive, &UKWebArchive::errorLog, this, &Osint::onErrorLog);
        connect(ukwebarchive, &UKWebArchive::infoLog, this, &Osint::onInfoLog);
        connect(cThread, &QThread::finished, this, &Osint::onScanThreadEnded);
        connect(cThread, &QThread::finished, ukwebarchive, &UKWebArchive::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        status->osint->activeThreads++;
    }
    if(ui->moduleWaybackmachine->isChecked()){
        Waybackmachine *waybackmachine = new Waybackmachine(scanArgs);
        QThread *cThread = new QThread(this);
        waybackmachine->Enumerator(cThread);
        waybackmachine->moveToThread(cThread);
        //...
        connect(waybackmachine, &Waybackmachine::subdomain, this, &Osint::onResultSubdomain);
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

    /****************************************************************************
                                 CERTS
    *****************************************************************************/

    if(ui->moduleCensysFree->isChecked()){
        CensysFree *censysfree = new CensysFree(scanArgs);
        QThread *cThread = new QThread(this);
        censysfree->Enumerator(cThread);
        censysfree->moveToThread(cThread);
        //...
        connect(censysfree, &CensysFree::subdomain, this, &Osint::onResultSubdomain);
        connect(censysfree, &CensysFree::sslCert, this, &Osint::onResultSSLCert);
        connect(censysfree, &CensysFree::errorLog, this, &Osint::onErrorLog);
        connect(censysfree, &CensysFree::infoLog, this, &Osint::onInfoLog);
        connect(cThread, &QThread::finished, this, &Osint::onScanThreadEnded);
        connect(cThread, &QThread::finished, censysfree, &CensysFree::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        status->osint->activeThreads++;
    }
    if(ui->moduleCertspotter->isChecked())
    {
        Certspotter *certspotter = new Certspotter(scanArgs);
        QThread *cThread = new QThread(this);
        certspotter->Enumerator(cThread);
        certspotter->moveToThread(cThread);
        //...
        connect(certspotter, &Certspotter::subdomain, this, &Osint::onResultSubdomain);
        connect(certspotter, &Certspotter::sslCert, this, &Osint::onResultSSLCert);
        connect(certspotter, &Certspotter::errorLog, this, &Osint::onErrorLog);
        connect(certspotter, &Certspotter::infoLog, this, &Osint::onInfoLog);
        connect(cThread, &QThread::finished, this, &Osint::onScanThreadEnded);
        connect(cThread, &QThread::finished, certspotter, &Certspotter::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        status->osint->activeThreads++;
    }
    if(ui->moduleCertspotterFree->isChecked())
    {
        CertspotterFree *certspotter = new CertspotterFree(scanArgs);
        QThread *cThread = new QThread(this);
        certspotter->Enumerator(cThread);
        certspotter->moveToThread(cThread);
        //...
        connect(certspotter, &CertspotterFree::subdomain, this, &Osint::onResultSubdomain);
        connect(certspotter, &CertspotterFree::sslCert, this, &Osint::onResultSSLCert);
        connect(certspotter, &CertspotterFree::errorLog, this, &Osint::onErrorLog);
        connect(certspotter, &CertspotterFree::infoLog, this, &Osint::onInfoLog);
        connect(cThread, &QThread::finished, this, &Osint::onScanThreadEnded);
        connect(cThread, &QThread::finished, certspotter, &CertspotterFree::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        status->osint->activeThreads++;
    }
    if(ui->moduleCrtsh->isChecked())
    {
        Crtsh *crtsh = new Crtsh(scanArgs);
        QThread *cThread = new QThread(this);
        crtsh->Enumerator(cThread);
        crtsh->moveToThread(cThread);
        //...
        connect(crtsh, &Crtsh::subdomain, this, &Osint::onResultSubdomain);
        connect(crtsh, &Crtsh::sslCert, this, &Osint::onResultSSLCert);
        connect(crtsh, &Crtsh::errorLog, this, &Osint::onErrorLog);
        connect(crtsh, &Crtsh::infoLog, this, &Osint::onInfoLog);
        connect(cThread, &QThread::finished, this, &Osint::onScanThreadEnded);
        connect(cThread, &QThread::finished, crtsh, &Crtsh::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        status->osint->activeThreads++;
    }
    if(ui->moduleDnsdumpster->isChecked())
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
    if(ui->moduleNetcraft->isChecked())
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
    if(ui->moduleSuip->isChecked())
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
    if(ui->modulePkey->isChecked()){
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
    if(ui->moduleRapiddns->isChecked()){
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
    if(ui->moduleGoogleCert->isChecked()){
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
    if(ui->moduleUrlscan->isChecked()){
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
    if(ui->moduleGithub->isChecked()){
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
    if(ui->moduleAsk->isChecked()){
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
    if(ui->moduleBaidu->isChecked()){
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
    if(ui->moduleDogpile->isChecked()){
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
    if(ui->moduleDuckduckgo->isChecked()){
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
    if(ui->moduleExalead->isChecked()){
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
    if(ui->moduleTrello->isChecked()){
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
    if(ui->moduleYahoo->isChecked()){
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
    if(ui->moduleBing->isChecked()){
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
    if(ui->moduleSitedossier->isChecked()){
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
    if(ui->modulePagesInventory->isChecked()){
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
    ///
    /// after starting subdomainIp choosen enumerations...
    ///
    if(status->osint->activeThreads)
    {
        ui->buttonStart->setDisabled(true);
        ui->buttonStop->setEnabled(true);
    }
    else{
        QMessageBox::warning(this, "Error!", "Please Choose Engine For Enumeration!");
    }
}
