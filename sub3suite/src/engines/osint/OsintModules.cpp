#include "Osint.h"
#include "ui_Osint.h"

#include "src/modules/osint/OsintModulesHeaders.h"


void Osint::initModules(){
    int INPUT_TYPE = ui->comboBoxInput->currentIndex();
    int OUTPUT_TYPE = ui->comboBoxOutput->currentIndex();

    ModuleInfo::Anubis anubis;
    if(anubis.input_output.contains(INPUT_TYPE) && anubis.input_output[INPUT_TYPE].contains(OUTPUT_TYPE))
        ui->moduleAnubis->show();
    else
        ui->moduleAnubis->hide();

    ModuleInfo::Bgpview bgpview;
    if(bgpview.input_output.contains(INPUT_TYPE) && bgpview.input_output[INPUT_TYPE].contains(OUTPUT_TYPE))
        ui->moduleBgpview->show();
    else
        ui->moduleBgpview->hide();

    ModuleInfo::BinaryEdge binaryedge;
    if(binaryedge.input_output.contains(INPUT_TYPE) && binaryedge.input_output[INPUT_TYPE].contains(OUTPUT_TYPE))
        ui->moduleBinaryEdge->show();
    else
        ui->moduleBinaryEdge->hide();

    ModuleInfo::C99 c99;
    if(c99.input_output.contains(INPUT_TYPE) && c99.input_output[INPUT_TYPE].contains(OUTPUT_TYPE))
        ui->moduleC99->show();
    else
        ui->moduleC99->hide();

    ModuleInfo::Circl circl;
    if(circl.input_output.contains(INPUT_TYPE) && circl.input_output[INPUT_TYPE].contains(OUTPUT_TYPE))
        ui->moduleCircl->show();
    else
        ui->moduleCircl->hide();

    ModuleInfo::Dnsbufferoverun dnsbufferoverun;
    if(dnsbufferoverun.input_output.contains(INPUT_TYPE) && dnsbufferoverun.input_output[INPUT_TYPE].contains(OUTPUT_TYPE))
        ui->moduleDnsbufferoverrun->show();
    else
        ui->moduleDnsbufferoverrun->hide();

    ModuleInfo::Github github;
    if(github.input_output.contains(INPUT_TYPE) && github.input_output[INPUT_TYPE].contains(OUTPUT_TYPE))
        ui->moduleGithub->show();
    else
        ui->moduleGithub->hide();

    ModuleInfo::HackerTargetFree hackertargetfree;
    if(hackertargetfree.input_output.contains(INPUT_TYPE) && hackertargetfree.input_output[INPUT_TYPE].contains(OUTPUT_TYPE))
        ui->moduleHackerTargetFree->show();
    else
        ui->moduleHackerTargetFree->hide();

    ModuleInfo::HackerTargetPaid hackertarget;
    if(hackertarget.input_output.contains(INPUT_TYPE) && hackertarget.input_output[INPUT_TYPE].contains(OUTPUT_TYPE))
        ui->moduleHackerTargetPaid->show();
    else
        ui->moduleHackerTargetPaid->hide();

    ModuleInfo::MnemonicFree mnemonicfree;
    if(mnemonicfree.input_output.contains(INPUT_TYPE) && mnemonicfree.input_output[INPUT_TYPE].contains(OUTPUT_TYPE))
        ui->moduleMnemonicFree->show();
    else
        ui->moduleMnemonicFree->hide();

    ModuleInfo::MnemonicPaid mnemonic;
    if(mnemonic.input_output.contains(INPUT_TYPE) && mnemonic.input_output[INPUT_TYPE].contains(OUTPUT_TYPE))
        ui->moduleMnemonicPaid->show();
    else
        ui->moduleMnemonicPaid->hide();

    ModuleInfo::Omnisint omnisint;
    if(omnisint.input_output.contains(INPUT_TYPE) && omnisint.input_output[INPUT_TYPE].contains(OUTPUT_TYPE))
        ui->moduleOmnisint->show();
    else
        ui->moduleOmnisint->hide();

    ModuleInfo::OtxFree otxfree;
    if(otxfree.input_output.contains(INPUT_TYPE) && otxfree.input_output[INPUT_TYPE].contains(OUTPUT_TYPE))
        ui->moduleOtxFree->show();
    else
        ui->moduleOtxFree->hide();

    ModuleInfo::OtxPaid otxpaid;
    if(otxpaid.input_output.contains(INPUT_TYPE) && otxpaid.input_output[INPUT_TYPE].contains(OUTPUT_TYPE))
        ui->moduleOtxPaid->show();
    else
        ui->moduleOtxPaid->hide();

    ModuleInfo::Projectdiscovery projectdiscovery;
    if(projectdiscovery.input_output.contains(INPUT_TYPE) && projectdiscovery.input_output[INPUT_TYPE].contains(OUTPUT_TYPE))
        ui->moduleProjectdiscovery->show();
    else
        ui->moduleProjectdiscovery->hide();

    ModuleInfo::RiskIq riskiq;
    if(riskiq.input_output.contains(INPUT_TYPE) && riskiq.input_output[INPUT_TYPE].contains(OUTPUT_TYPE))
        ui->moduleRiskIq->show();
    else
        ui->moduleRiskIq->hide();

    ModuleInfo::RobtexFree robtexfree;
    if(robtexfree.input_output.contains(INPUT_TYPE) && robtexfree.input_output[INPUT_TYPE].contains(OUTPUT_TYPE))
        ui->moduleRobtexFree->show();
    else
        ui->moduleRobtexFree->hide();

    ModuleInfo::RobtexPaid robtex;
    if(robtex.input_output.contains(INPUT_TYPE) && robtex.input_output[INPUT_TYPE].contains(OUTPUT_TYPE))
        ui->moduleRobtexPaid->show();
    else
        ui->moduleRobtexPaid->hide();

    ModuleInfo::SecurityTrails securitytrails;
    if(securitytrails.input_output.contains(INPUT_TYPE) && securitytrails.input_output[INPUT_TYPE].contains(OUTPUT_TYPE))
        ui->moduleSecuritytrails->show();
    else
        ui->moduleSecuritytrails->hide();

    ModuleInfo::Shodan shodan;
    if(shodan.input_output.contains(INPUT_TYPE) && shodan.input_output[INPUT_TYPE].contains(OUTPUT_TYPE))
        ui->moduleShodan->show();
    else
        ui->moduleShodan->hide();

    ModuleInfo::Spyse spyse;
    if(spyse.input_output.contains(INPUT_TYPE) && spyse.input_output[INPUT_TYPE].contains(OUTPUT_TYPE))
        ui->moduleSpyse->show();
    else
        ui->moduleSpyse->hide();

    ModuleInfo::Sublist3r sublist3r;
    if(sublist3r.input_output.contains(INPUT_TYPE) && sublist3r.input_output[INPUT_TYPE].contains(OUTPUT_TYPE))
        ui->moduleSublist3r->show();
    else
        ui->moduleSublist3r->hide();

    ModuleInfo::ThreatBook threatbook;
    if(threatbook.input_output.contains(INPUT_TYPE) && threatbook.input_output[INPUT_TYPE].contains(OUTPUT_TYPE))
        ui->moduleThreatBook->show();
    else
        ui->moduleThreatBook->hide();

    ModuleInfo::Threatcrowd threatcrowd;
    if(threatcrowd.input_output.contains(INPUT_TYPE) && threatcrowd.input_output[INPUT_TYPE].contains(OUTPUT_TYPE))
        ui->moduleThreatcrowd->show();
    else
        ui->moduleThreatcrowd->hide();

    ModuleInfo::Threatminer threatminer;
    if(threatminer.input_output.contains(INPUT_TYPE) && threatminer.input_output[INPUT_TYPE].contains(OUTPUT_TYPE))
        ui->moduleThreatminer->show();
    else
        ui->moduleThreatminer->hide();

    ModuleInfo::Urlscan urlscan;
    if(urlscan.input_output.contains(INPUT_TYPE) && urlscan.input_output[INPUT_TYPE].contains(OUTPUT_TYPE))
        ui->moduleUrlscan->show();
    else
        ui->moduleUrlscan->hide();

    ModuleInfo::ViewDns viewdns;
    if(viewdns.input_output.contains(INPUT_TYPE) && viewdns.input_output[INPUT_TYPE].contains(OUTPUT_TYPE))
        ui->moduleViewDns->show();
    else
        ui->moduleViewDns->hide();

    ModuleInfo::VirusTotal virustotal;
    if(virustotal.input_output.contains(INPUT_TYPE) && virustotal.input_output[INPUT_TYPE].contains(OUTPUT_TYPE))
        ui->moduleVirusTotal->show();
    else
        ui->moduleVirusTotal->hide();

    ModuleInfo::WebResolver webresolver;
    if(webresolver.input_output.contains(INPUT_TYPE) && webresolver.input_output[INPUT_TYPE].contains(OUTPUT_TYPE))
        ui->moduleWebResolver->show();
    else
        ui->moduleWebResolver->hide();

    ModuleInfo::WhoisXmlApi whoisxmlapi;
    if(whoisxmlapi.input_output.contains(INPUT_TYPE) && whoisxmlapi.input_output[INPUT_TYPE].contains(OUTPUT_TYPE))
        ui->moduleWhoisXmlApi->show();
    else
        ui->moduleWhoisXmlApi->hide();

    ModuleInfo::ZETAlytics zetalytics;
    if(zetalytics.input_output.contains(INPUT_TYPE) && zetalytics.input_output[INPUT_TYPE].contains(OUTPUT_TYPE))
        ui->moduleZetalytics->show();
    else
        ui->moduleZetalytics->hide();

    ModuleInfo::ZoomEye zoomeye;
    if(zoomeye.input_output.contains(INPUT_TYPE) && zoomeye.input_output[INPUT_TYPE].contains(OUTPUT_TYPE))
        ui->moduleZoomeye->show();
    else
        ui->moduleZoomeye->hide();

    ModuleInfo::IpInfo ipinfo;
    if(ipinfo.input_output.contains(INPUT_TYPE) && ipinfo.input_output[INPUT_TYPE].contains(OUTPUT_TYPE))
        ui->moduleIpInfo->show();
    else
        ui->moduleIpInfo->hide();

    ModuleInfo::Dnslytics dnslytics;
    if(dnslytics.input_output.contains(INPUT_TYPE) && dnslytics.input_output[INPUT_TYPE].contains(OUTPUT_TYPE))
        ui->moduleDnslytics->show();
    else
        ui->moduleDnslytics->hide();

    ModuleInfo::DomainTools domaintools;
    if(domaintools.input_output.contains(INPUT_TYPE) && domaintools.input_output[INPUT_TYPE].contains(OUTPUT_TYPE))
        ui->moduleDomainTools->show();
    else
        ui->moduleDomainTools->hide();

    ModuleInfo::Maltiverse maltiverse;
    if(maltiverse.input_output.contains(INPUT_TYPE) && maltiverse.input_output[INPUT_TYPE].contains(OUTPUT_TYPE))
        ui->moduleMaltiverseFree->show();
    else
        ui->moduleMaltiverseFree->hide();

    ModuleInfo::N45HT n45ht;
    if(n45ht.input_output.contains(INPUT_TYPE) && n45ht.input_output[INPUT_TYPE].contains(OUTPUT_TYPE))
        ui->moduleN45HT->show();
    else
        ui->moduleN45HT->hide();

    ModuleInfo::Onyphe onyphe;
    if(onyphe.input_output.contains(INPUT_TYPE) && onyphe.input_output[INPUT_TYPE].contains(OUTPUT_TYPE))
        ui->moduleOnyphe->show();
    else
        ui->moduleOnyphe->hide();

    ModuleInfo::Ripe ripe;
    if(ripe.input_output.contains(INPUT_TYPE) && ripe.input_output[INPUT_TYPE].contains(OUTPUT_TYPE))
        ui->moduleRipe->show();
    else
        ui->moduleRipe->hide();

    ModuleInfo::FullHunt fullhunt;
    if(fullhunt.input_output.contains(INPUT_TYPE) && fullhunt.input_output[INPUT_TYPE].contains(OUTPUT_TYPE))
        ui->moduleFullHunt->show();
    else
        ui->moduleFullHunt->hide();

    ModuleInfo::NetworksDB networksdb;
    if(networksdb.input_output.contains(INPUT_TYPE) && networksdb.input_output[INPUT_TYPE].contains(OUTPUT_TYPE))
        ui->moduleNetworksDB->show();
    else
        ui->moduleNetworksDB->hide();

    ModuleInfo::SpyOnWeb spyonweb;
    if(spyonweb.input_output.contains(INPUT_TYPE) && spyonweb.input_output[INPUT_TYPE].contains(OUTPUT_TYPE))
        ui->moduleSpyOnWeb->show();
    else
        ui->moduleSpyOnWeb->hide();

    ModuleInfo::ArchiveIt archiveit;
    if(archiveit.input_output.contains(INPUT_TYPE) && archiveit.input_output[INPUT_TYPE].contains(OUTPUT_TYPE))
        ui->moduleArchiveit->show();
    else
        ui->moduleArchiveit->hide();

    ModuleInfo::ArchiveToday archivetoday;
    if(archivetoday.input_output.contains(INPUT_TYPE) && archivetoday.input_output[INPUT_TYPE].contains(OUTPUT_TYPE))
        ui->moduleArchiveToday->show();
    else
        ui->moduleArchiveToday->hide();

    ModuleInfo::Arquivo arquivo;
    if(arquivo.input_output.contains(INPUT_TYPE) && arquivo.input_output[INPUT_TYPE].contains(OUTPUT_TYPE))
        ui->moduleArquivo->show();
    else
        ui->moduleArquivo->hide();

    ModuleInfo::CommonCrawl commoncrawl;
    if(commoncrawl.input_output.contains(INPUT_TYPE) && commoncrawl.input_output[INPUT_TYPE].contains(OUTPUT_TYPE))
        ui->moduleCommonCrawl->show();
    else
        ui->moduleCommonCrawl->hide();

    ModuleInfo::UKWebArchive ukwebarchive;
    if(ukwebarchive.input_output.contains(INPUT_TYPE) && ukwebarchive.input_output[INPUT_TYPE].contains(OUTPUT_TYPE))
        ui->moduleUKWebArchive->show();
    else
        ui->moduleUKWebArchive->hide();

    ModuleInfo::Waybackmachine waybackmachine;
    if(waybackmachine.input_output.contains(INPUT_TYPE) && waybackmachine.input_output[INPUT_TYPE].contains(OUTPUT_TYPE))
        ui->moduleWaybackmachine->show();
    else
        ui->moduleWaybackmachine->hide();

    ModuleInfo::CensysFree censysfree;
    if(censysfree.input_output.contains(INPUT_TYPE) && censysfree.input_output[INPUT_TYPE].contains(OUTPUT_TYPE))
        ui->moduleCensysFree->show();
    else
        ui->moduleCensysFree->hide();

    ModuleInfo::Censys censys;
    if(censys.input_output.contains(INPUT_TYPE) && censys.input_output[INPUT_TYPE].contains(OUTPUT_TYPE))
        ui->moduleCensys->show();
    else
        ui->moduleCensys->hide();

    ModuleInfo::CertspotterFree certspotterfree;
    if(certspotterfree.input_output.contains(INPUT_TYPE) && certspotterfree.input_output[INPUT_TYPE].contains(OUTPUT_TYPE))
        ui->moduleCertspotterFree->show();
    else
        ui->moduleCertspotterFree->hide();

    ModuleInfo::Certspotter certspotter;
    if(certspotter.input_output.contains(INPUT_TYPE) && certspotter.input_output[INPUT_TYPE].contains(OUTPUT_TYPE))
        ui->moduleCertspotter->show();
    else
        ui->moduleCertspotter->hide();

    ModuleInfo::Crtsh crtsh;
    if(crtsh.input_output.contains(INPUT_TYPE) && crtsh.input_output[INPUT_TYPE].contains(OUTPUT_TYPE))
        ui->moduleCrtsh->show();
    else
        ui->moduleCrtsh->hide();

    ModuleInfo::GoogleCert googlecert;
    if(googlecert.input_output.contains(INPUT_TYPE) && googlecert.input_output[INPUT_TYPE].contains(OUTPUT_TYPE))
        ui->moduleGoogleCert->show();
    else
        ui->moduleGoogleCert->hide();

    ModuleInfo::Hunter hunter;
    if(hunter.input_output.contains(INPUT_TYPE) && hunter.input_output[INPUT_TYPE].contains(OUTPUT_TYPE))
        ui->moduleHunter->show();
    else
        ui->moduleHunter->hide();

    ModuleInfo::EmailCrawlr emailcrawlr;
    if(emailcrawlr.input_output.contains(INPUT_TYPE) && emailcrawlr.input_output[INPUT_TYPE].contains(OUTPUT_TYPE))
        ui->moduleEmailCrawlr->show();
    else
        ui->moduleEmailCrawlr->hide();

    ModuleInfo::Dnsdumpster dnsdumpster;
    if(dnsdumpster.input_output.contains(INPUT_TYPE) && dnsdumpster.input_output[INPUT_TYPE].contains(OUTPUT_TYPE))
        ui->moduleDnsdumpster->show();
    else
        ui->moduleDnsdumpster->hide();

    ModuleInfo::Netcraft netcraft;
    if(netcraft.input_output.contains(INPUT_TYPE) && netcraft.input_output[INPUT_TYPE].contains(OUTPUT_TYPE))
        ui->moduleNetcraft->show();
    else
        ui->moduleNetcraft->hide();

    ModuleInfo::PagesInventory pagesinventory;
    if(pagesinventory.input_output.contains(INPUT_TYPE) && pagesinventory.input_output[INPUT_TYPE].contains(OUTPUT_TYPE))
        ui->modulePagesInventory->show();
    else
        ui->modulePagesInventory->hide();

    ModuleInfo::Pkey pkey;
    if(pkey.input_output.contains(INPUT_TYPE) && pkey.input_output[INPUT_TYPE].contains(OUTPUT_TYPE))
        ui->modulePkey->show();
    else
        ui->modulePkey->hide();

    ModuleInfo::Rapiddns rapiddns;
    if(rapiddns.input_output.contains(INPUT_TYPE) && rapiddns.input_output[INPUT_TYPE].contains(OUTPUT_TYPE))
        ui->moduleRapiddns->show();
    else
        ui->moduleRapiddns->hide();

    ModuleInfo::SiteDossier sitedossier;
    if(sitedossier.input_output.contains(INPUT_TYPE) && sitedossier.input_output[INPUT_TYPE].contains(OUTPUT_TYPE))
        ui->moduleSitedossier->show();
    else
        ui->moduleSitedossier->hide();

    ModuleInfo::Ask ask;
    if(ask.input_output.contains(INPUT_TYPE) && ask.input_output[INPUT_TYPE].contains(OUTPUT_TYPE))
        ui->moduleAsk->show();
    else
        ui->moduleAsk->hide();

    ModuleInfo::Baidu baidu;
    if(baidu.input_output.contains(INPUT_TYPE) && baidu.input_output[INPUT_TYPE].contains(OUTPUT_TYPE))
        ui->moduleBaidu->show();
    else
        ui->moduleBaidu->hide();

    ModuleInfo::DogPile dogpile;
    if(dogpile.input_output.contains(INPUT_TYPE) && dogpile.input_output[INPUT_TYPE].contains(OUTPUT_TYPE))
        ui->moduleDogpile->show();
    else
        ui->moduleDogpile->hide();

    ModuleInfo::DuckDuckGo duckduckgo;
    if(duckduckgo.input_output.contains(INPUT_TYPE) && duckduckgo.input_output[INPUT_TYPE].contains(OUTPUT_TYPE))
        ui->moduleDuckduckgo->show();
    else
        ui->moduleDuckduckgo->hide();

    ModuleInfo::Exalead exalead;
    if(exalead.input_output.contains(INPUT_TYPE) && exalead.input_output[INPUT_TYPE].contains(OUTPUT_TYPE))
        ui->moduleExalead->show();
    else
        ui->moduleExalead->hide();

    ModuleInfo::Yahoo yahoo;
    if(yahoo.input_output.contains(INPUT_TYPE) && yahoo.input_output[INPUT_TYPE].contains(OUTPUT_TYPE))
        ui->moduleYahoo->show();
    else
        ui->moduleYahoo->hide();

    ModuleInfo::Bing bing;
    if(bing.input_output.contains(INPUT_TYPE) && bing.input_output[INPUT_TYPE].contains(OUTPUT_TYPE))
        ui->moduleBing->show();
    else
        ui->moduleBing->hide();
}

void Osint::uncheckAllModules(){
    /* uncheck all modules */
}
