/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#include "RawTool.h"
#include "ui_RawTool.h"


void RawTool::on_moduleCertspotter_clicked(){
    ui->comboBoxOptions->clear();
    ModuleInfo::Certspotter meta;
    m_optionSet = meta.flags;
    ui->labelUrl->setText("<a href=\""+meta.url+"\" style=\"color: green;\">"+meta.name+"</a>");
    ui->labelApiDoc->setText("<a href=\""+meta.url_apiDoc+"\" style=\"color: green;\">"+meta.url_apiDoc+"</a>");
    ui->textEditEngineSummary->setText(meta.summary);
    ui->comboBoxOptions->addItems(meta.flags.keys());
}

void RawTool::on_moduleCertspotterFree_clicked(){
    ui->comboBoxOptions->clear();
    ModuleInfo::CertspotterFree meta;
    m_optionSet = meta.flags;
    ui->labelUrl->setText("<a href=\""+meta.url+"\" style=\"color: green;\">"+meta.name+"</a>");
    ui->labelApiDoc->setText("<a href=\""+meta.url_apiDoc+"\" style=\"color: green;\">"+meta.url_apiDoc+"</a>");
    ui->textEditEngineSummary->setText(meta.summary);
    ui->comboBoxOptions->addItems(meta.flags.keys());
}

void RawTool::on_moduleOtx_clicked(){
    ui->comboBoxOptions->clear();
    ModuleInfo::Otx meta;
    m_optionSet = meta.flags;
    ui->labelUrl->setText("<a href=\""+meta.url+"\" style=\"color: green;\">"+meta.name+"</a>");
    ui->labelApiDoc->setText("<a href=\""+meta.url_apiDoc+"\" style=\"color: green;\">"+meta.url_apiDoc+"</a>");
    ui->textEditEngineSummary->setText(meta.summary);
    ui->comboBoxOptions->addItems(meta.flags.keys());
}

void RawTool::on_moduleSublist3r_clicked(){
    ui->comboBoxOptions->clear();
    ModuleInfo::Sublist3r meta;
    m_optionSet = meta.flags;
    ui->labelUrl->setText("<a href=\""+meta.url+"\" style=\"color: green;\">"+meta.name+"</a>");
    ui->labelApiDoc->setText("<a href=\""+meta.url_apiDoc+"\" style=\"color: green;\">"+meta.url_apiDoc+"</a>");
    ui->textEditEngineSummary->setText(meta.summary);
    ui->comboBoxOptions->addItems(meta.flags.keys());
}

void RawTool::on_moduleThreatMiner_clicked(){
    ui->comboBoxOptions->clear();
    ModuleInfo::Threatminer meta;
    m_optionSet = meta.flags;
    ui->labelUrl->setText("<a href=\""+meta.url+"\" style=\"color: green;\">"+meta.name+"</a>");
    ui->labelApiDoc->setText("<a href=\""+meta.url_apiDoc+"\" style=\"color: green;\">"+meta.url_apiDoc+"</a>");
    ui->textEditEngineSummary->setText(meta.summary);
    ui->comboBoxOptions->addItems(meta.flags.keys());
}

void RawTool::on_moduleThreatCrowd_clicked(){
    ui->comboBoxOptions->clear();
    ModuleInfo::Threatcrowd meta;
    m_optionSet = meta.flags;
    ui->labelUrl->setText("<a href=\""+meta.url+"\" style=\"color: green;\">"+meta.name+"</a>");
    ui->labelApiDoc->setText("<a href=\""+meta.url_apiDoc+"\" style=\"color: green;\">"+meta.url_apiDoc+"</a>");
    ui->textEditEngineSummary->setText(meta.summary);
    ui->comboBoxOptions->addItems(meta.flags.keys());
}

void RawTool::on_moduleHackerTargetFree_clicked(){
    ui->comboBoxOptions->clear();
    ModuleInfo::HackerTargetFree meta;
    m_optionSet = meta.flags;
    ui->labelUrl->setText("<a href=\""+meta.url+"\" style=\"color: green;\">"+meta.name+"</a>");
    ui->labelApiDoc->setText("<a href=\""+meta.url_apiDoc+"\" style=\"color: green;\">"+meta.url_apiDoc+"</a>");
    ui->textEditEngineSummary->setText(meta.summary);
    ui->comboBoxOptions->addItems(meta.flags.keys());
}

void RawTool::on_moduleDnsBufferOverRun_clicked(){
    ui->comboBoxOptions->clear();
    ModuleInfo::Dnsbufferoverun meta;
    m_optionSet = meta.flags;
    ui->labelUrl->setText("<a href=\""+meta.url+"\" style=\"color: green;\">"+meta.name+"</a>");
    ui->labelApiDoc->setText("<a href=\""+meta.url_apiDoc+"\" style=\"color: green;\">"+meta.url_apiDoc+"</a>");
    ui->textEditEngineSummary->setText(meta.summary);
    ui->comboBoxOptions->addItems(meta.flags.keys());
}

void RawTool::on_moduleProjectDiscovery_clicked(){
    ui->comboBoxOptions->clear();
    ModuleInfo::Projectdiscovery meta;
    m_optionSet = meta.flags;
    ui->labelUrl->setText("<a href=\""+meta.url+"\" style=\"color: green;\">"+meta.name+"</a>");
    ui->labelApiDoc->setText("<a href=\""+meta.url_apiDoc+"\" style=\"color: green;\">"+meta.url_apiDoc+"</a>");
    ui->textEditEngineSummary->setText(meta.summary);
    ui->comboBoxOptions->addItems(meta.flags.keys());
}

void RawTool::on_moduleSpyse_clicked(){
    ui->comboBoxOptions->clear();
    ModuleInfo::Spyse meta;
    m_optionSet = meta.flags;
    ui->labelUrl->setText("<a href=\""+meta.url+"\" style=\"color: green;\">"+meta.name+"</a>");
    ui->labelApiDoc->setText("<a href=\""+meta.url_apiDoc+"\" style=\"color: green;\">"+meta.url_apiDoc+"</a>");
    ui->textEditEngineSummary->setText(meta.summary);
    ui->comboBoxOptions->addItems(meta.flags.keys());
}

void RawTool::on_moduleGoogleCert_clicked(){
    ui->comboBoxOptions->clear();
    ModuleInfo::GoogleCert meta;
    m_optionSet = meta.flags;
    ui->labelUrl->setText("<a href=\""+meta.url+"\" style=\"color: green;\">"+meta.name+"</a>");
    ui->labelApiDoc->setText("<a href=\""+meta.url_apiDoc+"\" style=\"color: green;\">"+meta.url_apiDoc+"</a>");
    ui->textEditEngineSummary->setText(meta.summary);
    ui->comboBoxOptions->addItems(meta.flags.keys());
}

void RawTool::on_moduleOmnisint_clicked(){
    ui->comboBoxOptions->clear();
    ModuleInfo::Omnisint meta;
    m_optionSet = meta.flags;
    ui->labelUrl->setText("<a href=\""+meta.url+"\" style=\"color: green;\">"+meta.name+"</a>");
    ui->labelApiDoc->setText("<a href=\""+meta.url_apiDoc+"\" style=\"color: green;\">"+meta.url_apiDoc+"</a>");
    ui->textEditEngineSummary->setText(meta.summary);
    ui->comboBoxOptions->addItems(meta.flags.keys());
}

void RawTool::on_moduleRobtexPaid_clicked(){
    ui->comboBoxOptions->clear();
    ModuleInfo::Robtex meta;
    m_optionSet = meta.flags;
    ui->labelUrl->setText("<a href=\""+meta.url+"\" style=\"color: green;\">"+meta.name+"</a>");
    ui->labelApiDoc->setText("<a href=\""+meta.url_apiDoc+"\" style=\"color: green;\">"+meta.url_apiDoc+"</a>");
    ui->textEditEngineSummary->setText(meta.summary);
    ui->comboBoxOptions->addItems(meta.flags.keys());
}

void RawTool::on_moduleUrlScan_clicked(){
    ui->comboBoxOptions->clear();
    ModuleInfo::Urlscan meta;
    m_optionSet = meta.flags;
    ui->labelUrl->setText("<a href=\""+meta.url+"\" style=\"color: green;\">"+meta.name+"</a>");
    ui->labelApiDoc->setText("<a href=\""+meta.url_apiDoc+"\" style=\"color: green;\">"+meta.url_apiDoc+"</a>");
    ui->textEditEngineSummary->setText(meta.summary);
    ui->comboBoxOptions->addItems(meta.flags.keys());
}

void RawTool::on_moduleVirusTotal_clicked(){
    ui->comboBoxOptions->clear();
    ModuleInfo::VirusTotal meta;
    m_optionSet = meta.flags;
    ui->labelUrl->setText("<a href=\""+meta.url+"\" style=\"color: green;\">"+meta.name+"</a>");
    ui->labelApiDoc->setText("<a href=\""+meta.url_apiDoc+"\" style=\"color: green;\">"+meta.url_apiDoc+"</a>");
    ui->textEditEngineSummary->setText(meta.summary);
    ui->comboBoxOptions->addItems(meta.flags.keys());
}

void RawTool::on_moduleWaybackMachine_clicked(){
    ui->comboBoxOptions->clear();
    ModuleInfo::Waybackmachine meta;
    m_optionSet = meta.flags;
    ui->labelUrl->setText("<a href=\""+meta.url+"\" style=\"color: green;\">"+meta.name+"</a>");
    ui->labelApiDoc->setText("<a href=\""+meta.url_apiDoc+"\" style=\"color: green;\">"+meta.url_apiDoc+"</a>");
    ui->textEditEngineSummary->setText(meta.summary);
    ui->comboBoxOptions->addItems(meta.flags.keys());
}

void RawTool::on_moduleArchiveIt_clicked(){
    ui->comboBoxOptions->clear();
    ModuleInfo::ArchiveIt meta;
    m_optionSet = meta.flags;
    ui->labelUrl->setText("<a href=\""+meta.url+"\" style=\"color: green;\">"+meta.name+"</a>");
    ui->labelApiDoc->setText("<a href=\""+meta.url_apiDoc+"\" style=\"color: green;\">"+meta.url_apiDoc+"</a>");
    ui->textEditEngineSummary->setText(meta.summary);
    ui->comboBoxOptions->addItems(meta.flags.keys());
}

void RawTool::on_moduleBinaryEdge_clicked(){
    ui->comboBoxOptions->clear();
    ModuleInfo::BinaryEdge meta;
    m_optionSet = meta.flags;
    ui->labelUrl->setText("<a href=\""+meta.url+"\" style=\"color: green;\">"+meta.name+"</a>");
    ui->labelApiDoc->setText("<a href=\""+meta.url_apiDoc+"\" style=\"color: green;\">"+meta.url_apiDoc+"</a>");
    ui->textEditEngineSummary->setText(meta.summary);
    ui->comboBoxOptions->addItems(meta.flags.keys());
}

void RawTool::on_moduleC99_clicked(){
    ui->comboBoxOptions->clear();
    ModuleInfo::C99 meta;
    m_optionSet = meta.flags;
    ui->labelUrl->setText("<a href=\""+meta.url+"\" style=\"color: green;\">"+meta.name+"</a>");
    ui->labelApiDoc->setText("<a href=\""+meta.url_apiDoc+"\" style=\"color: green;\">"+meta.url_apiDoc+"</a>");
    ui->textEditEngineSummary->setText(meta.summary);
    ui->comboBoxOptions->addItems(meta.flags.keys());
}

void RawTool::on_moduleGithub_clicked(){
    ui->comboBoxOptions->clear();
    ModuleInfo::Github meta;
    m_optionSet = meta.flags;
    ui->labelUrl->setText("<a href=\""+meta.url+"\" style=\"color: green;\">"+meta.name+"</a>");
    ui->labelApiDoc->setText("<a href=\""+meta.url_apiDoc+"\" style=\"color: green;\">"+meta.url_apiDoc+"</a>");
    ui->textEditEngineSummary->setText(meta.summary);
    ui->comboBoxOptions->addItems(meta.flags.keys());
}

void RawTool::on_moduleHunter_clicked(){
    ui->comboBoxOptions->clear();
    ModuleInfo::Hunter meta;
    m_optionSet = meta.flags;
    ui->labelUrl->setText("<a href=\""+meta.url+"\" style=\"color: green;\">"+meta.name+"</a>");
    ui->labelApiDoc->setText("<a href=\""+meta.url_apiDoc+"\" style=\"color: green;\">"+meta.url_apiDoc+"</a>");
    ui->textEditEngineSummary->setText(meta.summary);
    ui->comboBoxOptions->addItems(meta.flags.keys());
}

void RawTool::on_moduleIpInfo_clicked(){
    ui->comboBoxOptions->clear();
    ModuleInfo::IpInfo meta;
    m_optionSet = meta.flags;
    ui->labelUrl->setText("<a href=\""+meta.url+"\" style=\"color: green;\">"+meta.name+"</a>");
    ui->labelApiDoc->setText("<a href=\""+meta.url_apiDoc+"\" style=\"color: green;\">"+meta.url_apiDoc+"</a>");
    ui->textEditEngineSummary->setText(meta.summary);
    ui->comboBoxOptions->addItems(meta.flags.keys());
}

void RawTool::on_moduleMnemonicFree_clicked(){
    ui->comboBoxOptions->clear();
    ModuleInfo::MnemonicFree meta;
    m_optionSet = meta.flags;
    ui->labelUrl->setText("<a href=\""+meta.url+"\" style=\"color: green;\">"+meta.name+"</a>");
    ui->labelApiDoc->setText("<a href=\""+meta.url_apiDoc+"\" style=\"color: green;\">"+meta.url_apiDoc+"</a>");
    ui->textEditEngineSummary->setText(meta.summary);
    ui->comboBoxOptions->addItems(meta.flags.keys());
}

void RawTool::on_moduleRiskIq_clicked(){
    ui->comboBoxOptions->clear();
    ModuleInfo::RiskIq meta;
    m_optionSet = meta.flags;
    ui->labelUrl->setText("<a href=\""+meta.url+"\" style=\"color: green;\">"+meta.name+"</a>");
    ui->labelApiDoc->setText("<a href=\""+meta.url_apiDoc+"\" style=\"color: green;\">"+meta.url_apiDoc+"</a>");
    ui->textEditEngineSummary->setText(meta.summary);
    ui->comboBoxOptions->addItems(meta.flags.keys());
}

void RawTool::on_moduleRobtex_clicked(){
    ui->comboBoxOptions->clear();
    ModuleInfo::Robtex meta;
    m_optionSet = meta.flags;
    ui->labelUrl->setText("<a href=\""+meta.url+"\" style=\"color: green;\">"+meta.name+"</a>");
    ui->labelApiDoc->setText("<a href=\""+meta.url_apiDoc+"\" style=\"color: green;\">"+meta.url_apiDoc+"</a>");
    ui->textEditEngineSummary->setText(meta.summary);
    ui->comboBoxOptions->addItems(meta.flags.keys());
}

void RawTool::on_moduleSecurityTrails_clicked(){
    ui->comboBoxOptions->clear();
    ModuleInfo::SecurityTrails meta;
    m_optionSet = meta.flags;
    ui->labelUrl->setText("<a href=\""+meta.url+"\" style=\"color: green;\">"+meta.name+"</a>");
    ui->labelApiDoc->setText("<a href=\""+meta.url_apiDoc+"\" style=\"color: green;\">"+meta.url_apiDoc+"</a>");
    ui->textEditEngineSummary->setText(meta.summary);
    ui->comboBoxOptions->addItems(meta.flags.keys());
}

void RawTool::on_moduleShodan_clicked(){
    ui->comboBoxOptions->clear();
    ModuleInfo::Shodan meta;
    m_optionSet = meta.flags;
    ui->labelUrl->setText("<a href=\""+meta.url+"\" style=\"color: green;\">"+meta.name+"</a>");
    ui->labelApiDoc->setText("<a href=\""+meta.url_apiDoc+"\" style=\"color: green;\">"+meta.url_apiDoc+"</a>");
    ui->textEditEngineSummary->setText(meta.summary);
    ui->comboBoxOptions->addItems(meta.flags.keys());
}

void RawTool::on_moduleThreatBook_clicked(){
    ui->comboBoxOptions->clear();
    ModuleInfo::ThreatBook meta;
    m_optionSet = meta.flags;
    ui->labelUrl->setText("<a href=\""+meta.url+"\" style=\"color: green;\">"+meta.name+"</a>");
    ui->labelApiDoc->setText("<a href=\""+meta.url_apiDoc+"\" style=\"color: green;\">"+meta.url_apiDoc+"</a>");
    ui->textEditEngineSummary->setText(meta.summary);
    ui->comboBoxOptions->addItems(meta.flags.keys());
}

void RawTool::on_moduleWhoisXmlApi_clicked(){
    ui->comboBoxOptions->clear();
    ModuleInfo::WhoisXmlApi meta;
    m_optionSet = meta.flags;
    ui->labelUrl->setText("<a href=\""+meta.url+"\" style=\"color: green;\">"+meta.name+"</a>");
    ui->labelApiDoc->setText("<a href=\""+meta.url_apiDoc+"\" style=\"color: green;\">"+meta.url_apiDoc+"</a>");
    ui->textEditEngineSummary->setText(meta.summary);
    ui->comboBoxOptions->addItems(meta.flags.keys());
}

void RawTool::on_moduleZETALytics_clicked(){
    ui->comboBoxOptions->clear();
    ModuleInfo::ZETAlytics meta;
    m_optionSet = meta.flags;
    ui->labelUrl->setText("<a href=\""+meta.url+"\" style=\"color: green;\">"+meta.name+"</a>");
    ui->labelApiDoc->setText("<a href=\""+meta.url_apiDoc+"\" style=\"color: green;\">"+meta.url_apiDoc+"</a>");
    ui->textEditEngineSummary->setText(meta.summary);
    ui->comboBoxOptions->addItems(meta.flags.keys());
}

void RawTool::on_moduleZoomEye_clicked(){
    ui->comboBoxOptions->clear();
    ModuleInfo::ZoomEye meta;
    m_optionSet = meta.flags;
    ui->labelUrl->setText("<a href=\""+meta.url+"\" style=\"color: green;\">"+meta.name+"</a>");
    ui->labelApiDoc->setText("<a href=\""+meta.url_apiDoc+"\" style=\"color: green;\">"+meta.url_apiDoc+"</a>");
    ui->textEditEngineSummary->setText(meta.summary);
    ui->comboBoxOptions->addItems(meta.flags.keys());
}

void RawTool::on_moduleIpApi_clicked(){
    ui->comboBoxOptions->clear();
    ModuleInfo::IpApi meta;
    m_optionSet = meta.flags;
    ui->labelUrl->setText("<a href=\""+meta.url+"\" style=\"color: green;\">"+meta.name+"</a>");
    ui->labelApiDoc->setText("<a href=\""+meta.url_apiDoc+"\" style=\"color: green;\">"+meta.url_apiDoc+"</a>");
    ui->textEditEngineSummary->setText(meta.summary);
    ui->comboBoxOptions->addItems(meta.flags.keys());
}

void RawTool::on_moduleViewDns_clicked(){
    ui->comboBoxOptions->clear();
    ModuleInfo::ViewDns meta;
    m_optionSet = meta.flags;
    ui->labelUrl->setText("<a href=\""+meta.url+"\" style=\"color: green;\">"+meta.name+"</a>");
    ui->labelApiDoc->setText("<a href=\""+meta.url_apiDoc+"\" style=\"color: green;\">"+meta.url_apiDoc+"</a>");
    ui->textEditEngineSummary->setText(meta.summary);
    ui->comboBoxOptions->addItems(meta.flags.keys());
}

void RawTool::on_moduleWebResolver_clicked(){
    ui->comboBoxOptions->clear();
    ModuleInfo::WebResolver meta;
    m_optionSet = meta.flags;
    ui->labelUrl->setText("<a href=\""+meta.url+"\" style=\"color: green;\">"+meta.name+"</a>");
    ui->labelApiDoc->setText("<a href=\""+meta.url_apiDoc+"\" style=\"color: green;\">"+meta.url_apiDoc+"</a>");
    ui->textEditEngineSummary->setText(meta.summary);
    ui->comboBoxOptions->addItems(meta.flags.keys());
}

void RawTool::on_moduleCircl_clicked(){
    ui->comboBoxOptions->clear();
    ModuleInfo::Circl meta;
    m_optionSet = meta.flags;
    ui->labelUrl->setText("<a href=\""+meta.url+"\" style=\"color: green;\">"+meta.name+"</a>");
    ui->labelApiDoc->setText("<a href=\""+meta.url_apiDoc+"\" style=\"color: green;\">"+meta.url_apiDoc+"</a>");
    ui->textEditEngineSummary->setText(meta.summary);
    ui->comboBoxOptions->addItems(meta.flags.keys());
}

void RawTool::on_moduleHackerTarget_clicked(){
    ui->comboBoxOptions->clear();
    ModuleInfo::HackerTarget meta;
    m_optionSet = meta.flags;
    ui->labelUrl->setText("<a href=\""+meta.url+"\" style=\"color: green;\">"+meta.name+"</a>");
    ui->labelApiDoc->setText("<a href=\""+meta.url_apiDoc+"\" style=\"color: green;\">"+meta.url_apiDoc+"</a>");
    ui->textEditEngineSummary->setText(meta.summary);
    ui->comboBoxOptions->addItems(meta.flags.keys());
}

void RawTool::on_moduleMnemonic_clicked(){
    ui->comboBoxOptions->clear();
    ModuleInfo::Mnemonic meta;
    m_optionSet = meta.flags;
    ui->labelUrl->setText("<a href=\""+meta.url+"\" style=\"color: green;\">"+meta.name+"</a>");
    ui->labelApiDoc->setText("<a href=\""+meta.url_apiDoc+"\" style=\"color: green;\">"+meta.url_apiDoc+"</a>");
    ui->textEditEngineSummary->setText(meta.summary);
    ui->comboBoxOptions->addItems(meta.flags.keys());
}

void RawTool::on_moduleArin_clicked(){
    ui->comboBoxOptions->clear();
    ModuleInfo::Arin meta;
    m_optionSet = meta.flags;
    ui->labelUrl->setText("<a href=\""+meta.url+"\" style=\"color: green;\">"+meta.name+"</a>");
    ui->labelApiDoc->setText("<a href=\""+meta.url_apiDoc+"\" style=\"color: green;\">"+meta.url_apiDoc+"</a>");
    ui->textEditEngineSummary->setText(meta.summary);
    ui->comboBoxOptions->addItems(meta.flags.keys());
}

void RawTool::on_moduleBuiltWith_clicked(){
    ui->comboBoxOptions->clear();
    ModuleInfo::BuiltWith meta;
    m_optionSet = meta.flags;
    ui->labelUrl->setText("<a href=\""+meta.url+"\" style=\"color: green;\">"+meta.name+"</a>");
    ui->labelApiDoc->setText("<a href=\""+meta.url_apiDoc+"\" style=\"color: green;\">"+meta.url_apiDoc+"</a>");
    ui->textEditEngineSummary->setText(meta.summary);
    ui->comboBoxOptions->addItems(meta.flags.keys());
}

void RawTool::on_moduleDnslytics_clicked(){
    ui->comboBoxOptions->clear();
    ModuleInfo::Dnslytics meta;
    m_optionSet = meta.flags;
    ui->labelUrl->setText("<a href=\""+meta.url+"\" style=\"color: green;\">"+meta.name+"</a>");
    ui->labelApiDoc->setText("<a href=\""+meta.url_apiDoc+"\" style=\"color: green;\">"+meta.url_apiDoc+"</a>");
    ui->textEditEngineSummary->setText(meta.summary);
    ui->comboBoxOptions->addItems(meta.flags.keys());
}

void RawTool::on_moduleDomainTools_clicked(){
    ui->comboBoxOptions->clear();
    ModuleInfo::DomainTools meta;
    m_optionSet = meta.flags;
    ui->labelUrl->setText("<a href=\""+meta.url+"\" style=\"color: green;\">"+meta.name+"</a>");
    ui->labelApiDoc->setText("<a href=\""+meta.url_apiDoc+"\" style=\"color: green;\">"+meta.url_apiDoc+"</a>");
    ui->textEditEngineSummary->setText(meta.summary);
    ui->comboBoxOptions->addItems(meta.flags.keys());
}

void RawTool::on_moduleMaltiverseFree_clicked(){
    ui->comboBoxOptions->clear();
    ModuleInfo::Maltiverse meta;
    m_optionSet = meta.flags;
    ui->labelUrl->setText("<a href=\""+meta.url+"\" style=\"color: green;\">"+meta.name+"</a>");
    ui->labelApiDoc->setText("<a href=\""+meta.url_apiDoc+"\" style=\"color: green;\">"+meta.url_apiDoc+"</a>");
    ui->textEditEngineSummary->setText(meta.summary);
    ui->comboBoxOptions->addItems(meta.flags.keys());
}

void RawTool::on_moduleN45HT_clicked(){
    ui->comboBoxOptions->clear();
    ModuleInfo::N45HT meta;
    m_optionSet = meta.flags;
    ui->labelUrl->setText("<a href=\""+meta.url+"\" style=\"color: green;\">"+meta.name+"</a>");
    ui->labelApiDoc->setText("<a href=\""+meta.url_apiDoc+"\" style=\"color: green;\">"+meta.url_apiDoc+"</a>");
    ui->textEditEngineSummary->setText(meta.summary);
    ui->comboBoxOptions->addItems(meta.flags.keys());
}

void RawTool::on_moduleOnyphe_clicked(){
    ui->comboBoxOptions->clear();
    ModuleInfo::Onyphe meta;
    m_optionSet = meta.flags;
    ui->labelUrl->setText("<a href=\""+meta.url+"\" style=\"color: green;\">"+meta.name+"</a>");
    ui->labelApiDoc->setText("<a href=\""+meta.url_apiDoc+"\" style=\"color: green;\">"+meta.url_apiDoc+"</a>");
    ui->textEditEngineSummary->setText(meta.summary);
    ui->comboBoxOptions->addItems(meta.flags.keys());
}

void RawTool::on_moduleRipe_clicked(){
    ui->comboBoxOptions->clear();
    ModuleInfo::Ripe meta;
    m_optionSet = meta.flags;
    ui->labelUrl->setText("<a href=\""+meta.url+"\" style=\"color: green;\">"+meta.name+"</a>");
    ui->labelApiDoc->setText("<a href=\""+meta.url_apiDoc+"\" style=\"color: green;\">"+meta.url_apiDoc+"</a>");
    ui->textEditEngineSummary->setText(meta.summary);
    ui->comboBoxOptions->addItems(meta.flags.keys());
}

void RawTool::on_moduleAfrinic_clicked(){
    ui->comboBoxOptions->clear();
    ModuleInfo::Afrinic meta;
    m_optionSet = meta.flags;
    ui->labelUrl->setText("<a href=\""+meta.url+"\" style=\"color: green;\">"+meta.name+"</a>");
    ui->labelApiDoc->setText("<a href=\""+meta.url_apiDoc+"\" style=\"color: green;\">"+meta.url_apiDoc+"</a>");
    ui->textEditEngineSummary->setText(meta.summary);
    ui->comboBoxOptions->addItems(meta.flags.keys());
}

void RawTool::on_moduleApnic_clicked(){
    ui->comboBoxOptions->clear();
    ModuleInfo::Apnic meta;
    m_optionSet = meta.flags;
    ui->labelUrl->setText("<a href=\""+meta.url+"\" style=\"color: green;\">"+meta.name+"</a>");
    ui->labelApiDoc->setText("<a href=\""+meta.url_apiDoc+"\" style=\"color: green;\">"+meta.url_apiDoc+"</a>");
    ui->textEditEngineSummary->setText(meta.summary);
    ui->comboBoxOptions->addItems(meta.flags.keys());
}

void RawTool::on_moduleFullHunt_clicked(){
    ui->comboBoxOptions->clear();
    ModuleInfo::FullHunt meta;
    m_optionSet = meta.flags;
    ui->labelUrl->setText("<a href=\""+meta.url+"\" style=\"color: green;\">"+meta.name+"</a>");
    ui->labelApiDoc->setText("<a href=\""+meta.url_apiDoc+"\" style=\"color: green;\">"+meta.url_apiDoc+"</a>");
    ui->textEditEngineSummary->setText(meta.summary);
    ui->comboBoxOptions->addItems(meta.flags.keys());
}

void RawTool::on_moduleSpamHaus_clicked(){
    ui->comboBoxOptions->clear();
    ModuleInfo::SpamHaus meta;
    m_optionSet = meta.flags;
    ui->labelUrl->setText("<a href=\""+meta.url+"\" style=\"color: green;\">"+meta.name+"</a>");
    ui->labelApiDoc->setText("<a href=\""+meta.url_apiDoc+"\" style=\"color: green;\">"+meta.url_apiDoc+"</a>");
    ui->textEditEngineSummary->setText(meta.summary);
    ui->comboBoxOptions->addItems(meta.flags.keys());
}

void RawTool::on_moduleDebounce_clicked(){
    ui->comboBoxOptions->clear();
    ModuleInfo::Debounce meta;
    m_optionSet = meta.flags;
    ui->labelUrl->setText("<a href=\""+meta.url+"\" style=\"color: green;\">"+meta.name+"</a>");
    ui->labelApiDoc->setText("<a href=\""+meta.url_apiDoc+"\" style=\"color: green;\">"+meta.url_apiDoc+"</a>");
    ui->textEditEngineSummary->setText(meta.summary);
    ui->comboBoxOptions->addItems(meta.flags.keys());
}

void RawTool::on_moduleHaveIBeenPawned_clicked(){
    ui->comboBoxOptions->clear();
    ModuleInfo::HaveIBeenPawned meta;
    m_optionSet = meta.flags;
    ui->labelUrl->setText("<a href=\""+meta.url+"\" style=\"color: green;\">"+meta.name+"</a>");
    ui->labelApiDoc->setText("<a href=\""+meta.url_apiDoc+"\" style=\"color: green;\">"+meta.url_apiDoc+"</a>");
    ui->textEditEngineSummary->setText(meta.summary);
    ui->comboBoxOptions->addItems(meta.flags.keys());
}

void RawTool::on_moduleHost_clicked(){
    ui->comboBoxOptions->clear();
    ModuleInfo::Host meta;
    m_optionSet = meta.flags;
    ui->labelUrl->setText("<a href=\""+meta.url+"\" style=\"color: green;\">"+meta.name+"</a>");
    ui->labelApiDoc->setText("<a href=\""+meta.url_apiDoc+"\" style=\"color: green;\">"+meta.url_apiDoc+"</a>");
    ui->textEditEngineSummary->setText(meta.summary);
    ui->comboBoxOptions->addItems(meta.flags.keys());
}

void RawTool::on_moduleJsonWhois_clicked(){
    ui->comboBoxOptions->clear();
    ModuleInfo::JsonWhois meta;
    m_optionSet = meta.flags;
    ui->labelUrl->setText("<a href=\""+meta.url+"\" style=\"color: green;\">"+meta.name+"</a>");
    ui->labelApiDoc->setText("<a href=\""+meta.url_apiDoc+"\" style=\"color: green;\">"+meta.url_apiDoc+"</a>");
    ui->textEditEngineSummary->setText(meta.summary);
    ui->comboBoxOptions->addItems(meta.flags.keys());
}

void RawTool::on_moduleLeakIX_clicked(){
    ui->comboBoxOptions->clear();
    ModuleInfo::LeakIX meta;
    m_optionSet = meta.flags;
    ui->labelUrl->setText("<a href=\""+meta.url+"\" style=\"color: green;\">"+meta.name+"</a>");
    ui->labelApiDoc->setText("<a href=\""+meta.url_apiDoc+"\" style=\"color: green;\">"+meta.url_apiDoc+"</a>");
    ui->textEditEngineSummary->setText(meta.summary);
    ui->comboBoxOptions->addItems(meta.flags.keys());
}

void RawTool::on_moduleNetworksDB_clicked(){
    ui->comboBoxOptions->clear();
    ModuleInfo::NetworksDB meta;
    m_optionSet = meta.flags;
    ui->labelUrl->setText("<a href=\""+meta.url+"\" style=\"color: green;\">"+meta.name+"</a>");
    ui->labelApiDoc->setText("<a href=\""+meta.url_apiDoc+"\" style=\"color: green;\">"+meta.url_apiDoc+"</a>");
    ui->textEditEngineSummary->setText(meta.summary);
    ui->comboBoxOptions->addItems(meta.flags.keys());
}

void RawTool::on_moduleNeutrinoApi_clicked(){
    ui->comboBoxOptions->clear();
    ModuleInfo::NeutrinoApi meta;
    m_optionSet = meta.flags;
    ui->labelUrl->setText("<a href=\""+meta.url+"\" style=\"color: green;\">"+meta.name+"</a>");
    ui->labelApiDoc->setText("<a href=\""+meta.url_apiDoc+"\" style=\"color: green;\">"+meta.url_apiDoc+"</a>");
    ui->textEditEngineSummary->setText(meta.summary);
    ui->comboBoxOptions->addItems(meta.flags.keys());
}

void RawTool::on_moduleSeon_clicked(){
    ui->comboBoxOptions->clear();
    ModuleInfo::Seon meta;
    m_optionSet = meta.flags;
    ui->labelUrl->setText("<a href=\""+meta.url+"\" style=\"color: green;\">"+meta.name+"</a>");
    ui->labelApiDoc->setText("<a href=\""+meta.url_apiDoc+"\" style=\"color: green;\">"+meta.url_apiDoc+"</a>");
    ui->textEditEngineSummary->setText(meta.summary);
    ui->comboBoxOptions->addItems(meta.flags.keys());
}

void RawTool::on_moduleSpyOnWeb_clicked(){
    ui->comboBoxOptions->clear();
    ModuleInfo::SpyOnWeb meta;
    m_optionSet = meta.flags;
    ui->labelUrl->setText("<a href=\""+meta.url+"\" style=\"color: green;\">"+meta.name+"</a>");
    ui->labelApiDoc->setText("<a href=\""+meta.url_apiDoc+"\" style=\"color: green;\">"+meta.url_apiDoc+"</a>");
    ui->textEditEngineSummary->setText(meta.summary);
    ui->comboBoxOptions->addItems(meta.flags.keys());
}

void RawTool::on_moduleWhatcms_clicked(){
    ui->comboBoxOptions->clear();
    ModuleInfo::Whatcms meta;
    m_optionSet = meta.flags;
    ui->labelUrl->setText("<a href=\""+meta.url+"\" style=\"color: green;\">"+meta.name+"</a>");
    ui->labelApiDoc->setText("<a href=\""+meta.url_apiDoc+"\" style=\"color: green;\">"+meta.url_apiDoc+"</a>");
    ui->textEditEngineSummary->setText(meta.summary);
    ui->comboBoxOptions->addItems(meta.flags.keys());
}

void RawTool::on_moduleWhoxy_clicked(){
    ui->comboBoxOptions->clear();
    ModuleInfo::Whoxy meta;
    m_optionSet = meta.flags;
    ui->labelUrl->setText("<a href=\""+meta.url+"\" style=\"color: green;\">"+meta.name+"</a>");
    ui->labelApiDoc->setText("<a href=\""+meta.url_apiDoc+"\" style=\"color: green;\">"+meta.url_apiDoc+"</a>");
    ui->textEditEngineSummary->setText(meta.summary);
    ui->comboBoxOptions->addItems(meta.flags.keys());
}

void RawTool::on_moduleArquivo_clicked(){
    ui->comboBoxOptions->clear();
    ModuleInfo::Arquivo meta;
    m_optionSet = meta.flags;
    ui->labelUrl->setText("<a href=\""+meta.url+"\" style=\"color: green;\">"+meta.name+"</a>");
    ui->labelApiDoc->setText("<a href=\""+meta.url_apiDoc+"\" style=\"color: green;\">"+meta.url_apiDoc+"</a>");
    ui->textEditEngineSummary->setText(meta.summary);
    ui->comboBoxOptions->addItems(meta.flags.keys());
}

void RawTool::on_moduleUKWebArchive_clicked(){
    ui->comboBoxOptions->clear();
    ModuleInfo::UKWebArchive meta;
    m_optionSet = meta.flags;
    ui->labelUrl->setText("<a href=\""+meta.url+"\" style=\"color: green;\">"+meta.name+"</a>");
    ui->labelApiDoc->setText("<a href=\""+meta.url_apiDoc+"\" style=\"color: green;\">"+meta.url_apiDoc+"</a>");
    ui->textEditEngineSummary->setText(meta.summary);
    ui->comboBoxOptions->addItems(meta.flags.keys());
}

void RawTool::on_moduleCensys_clicked(){
    ui->comboBoxOptions->clear();
    ModuleInfo::Censys meta;
    m_optionSet = meta.flags;
    ui->labelUrl->setText("<a href=\""+meta.url+"\" style=\"color: green;\">"+meta.name+"</a>");
    ui->labelApiDoc->setText("<a href=\""+meta.url_apiDoc+"\" style=\"color: green;\">"+meta.url_apiDoc+"</a>");
    ui->textEditEngineSummary->setText(meta.summary);
    ui->comboBoxOptions->addItems(meta.flags.keys());
}

void RawTool::on_moduleEmailCrawlr_clicked(){
    ui->comboBoxOptions->clear();
    ModuleInfo::EmailCrawlr meta;
    m_optionSet = meta.flags;
    ui->labelUrl->setText("<a href=\""+meta.url+"\" style=\"color: green;\">"+meta.name+"</a>");
    ui->labelApiDoc->setText("<a href=\""+meta.url_apiDoc+"\" style=\"color: green;\">"+meta.url_apiDoc+"</a>");
    ui->textEditEngineSummary->setText(meta.summary);
    ui->comboBoxOptions->addItems(meta.flags.keys());
}

void RawTool::on_moduleEmailFormat_clicked(){
    ui->comboBoxOptions->clear();
    ModuleInfo::EmailFormat meta;
    m_optionSet = meta.flags;
    ui->labelUrl->setText("<a href=\""+meta.url+"\" style=\"color: green;\">"+meta.name+"</a>");
    ui->labelApiDoc->setText("<a href=\""+meta.url_apiDoc+"\" style=\"color: green;\">"+meta.url_apiDoc+"</a>");
    ui->textEditEngineSummary->setText(meta.summary);
    ui->comboBoxOptions->addItems(meta.flags.keys());
}

void RawTool::on_moduleEmailRep_clicked(){
    ui->comboBoxOptions->clear();
    ModuleInfo::EmailRep meta;
    m_optionSet = meta.flags;
    ui->labelUrl->setText("<a href=\""+meta.url+"\" style=\"color: green;\">"+meta.name+"</a>");
    ui->labelApiDoc->setText("<a href=\""+meta.url_apiDoc+"\" style=\"color: green;\">"+meta.url_apiDoc+"</a>");
    ui->textEditEngineSummary->setText(meta.summary);
    ui->comboBoxOptions->addItems(meta.flags.keys());
}

void RawTool::on_moduleSnov_clicked(){
    ui->comboBoxOptions->clear();
    ModuleInfo::Snov meta;
    m_optionSet = meta.flags;
    ui->labelUrl->setText("<a href=\""+meta.url+"\" style=\"color: green;\">"+meta.name+"</a>");
    ui->labelApiDoc->setText("<a href=\""+meta.url_apiDoc+"\" style=\"color: green;\">"+meta.url_apiDoc+"</a>");
    ui->textEditEngineSummary->setText(meta.summary);
    ui->comboBoxOptions->addItems(meta.flags.keys());
}

void RawTool::on_moduleTruMail_clicked(){
    ui->comboBoxOptions->clear();
    ModuleInfo::TruMail meta;
    m_optionSet = meta.flags;
    ui->labelUrl->setText("<a href=\""+meta.url+"\" style=\"color: green;\">"+meta.name+"</a>");
    ui->labelApiDoc->setText("<a href=\""+meta.url_apiDoc+"\" style=\"color: green;\">"+meta.url_apiDoc+"</a>");
    ui->textEditEngineSummary->setText(meta.summary);
    ui->comboBoxOptions->addItems(meta.flags.keys());
}

void RawTool::on_moduleIpData_clicked(){
    ui->comboBoxOptions->clear();
    ModuleInfo::IpData meta;
    m_optionSet = meta.flags;
    ui->labelUrl->setText("<a href=\""+meta.url+"\" style=\"color: green;\">"+meta.name+"</a>");
    ui->labelApiDoc->setText("<a href=\""+meta.url_apiDoc+"\" style=\"color: green;\">"+meta.url_apiDoc+"</a>");
    ui->textEditEngineSummary->setText(meta.summary);
    ui->comboBoxOptions->addItems(meta.flags.keys());
}

void RawTool::on_moduleIpGeoLocation_clicked(){
    ui->comboBoxOptions->clear();
    ModuleInfo::IpGeoLocation meta;
    m_optionSet = meta.flags;
    ui->labelUrl->setText("<a href=\""+meta.url+"\" style=\"color: green;\">"+meta.name+"</a>");
    ui->labelApiDoc->setText("<a href=\""+meta.url_apiDoc+"\" style=\"color: green;\">"+meta.url_apiDoc+"</a>");
    ui->textEditEngineSummary->setText(meta.summary);
    ui->comboBoxOptions->addItems(meta.flags.keys());
}

void RawTool::on_moduleIpRegistry_clicked(){
    ui->comboBoxOptions->clear();
    ModuleInfo::IpRegistry meta;
    m_optionSet = meta.flags;
    ui->labelUrl->setText("<a href=\""+meta.url+"\" style=\"color: green;\">"+meta.name+"</a>");
    ui->labelApiDoc->setText("<a href=\""+meta.url_apiDoc+"\" style=\"color: green;\">"+meta.url_apiDoc+"</a>");
    ui->textEditEngineSummary->setText(meta.summary);
    ui->comboBoxOptions->addItems(meta.flags.keys());
}

void RawTool::on_moduleIpStack_clicked(){
    ui->comboBoxOptions->clear();
    ModuleInfo::IpStack meta;
    m_optionSet = meta.flags;
    ui->labelUrl->setText("<a href=\""+meta.url+"\" style=\"color: green;\">"+meta.name+"</a>");
    ui->labelApiDoc->setText("<a href=\""+meta.url_apiDoc+"\" style=\"color: green;\">"+meta.url_apiDoc+"</a>");
    ui->textEditEngineSummary->setText(meta.summary);
    ui->comboBoxOptions->addItems(meta.flags.keys());
}

void RawTool::on_moduleAbuseIPDB_clicked(){
    ui->comboBoxOptions->clear();
    ModuleInfo::AbuseIPDB meta;
    m_optionSet = meta.flags;
    ui->labelUrl->setText("<a href=\""+meta.url+"\" style=\"color: green;\">"+meta.name+"</a>");
    ui->labelApiDoc->setText("<a href=\""+meta.url_apiDoc+"\" style=\"color: green;\">"+meta.url_apiDoc+"</a>");
    ui->textEditEngineSummary->setText(meta.summary);
    ui->comboBoxOptions->addItems(meta.flags.keys());
}

void RawTool::on_moduleBotScout_clicked(){
    ui->comboBoxOptions->clear();
    ModuleInfo::BotScout meta;
    m_optionSet = meta.flags;
    ui->labelUrl->setText("<a href=\""+meta.url+"\" style=\"color: green;\">"+meta.name+"</a>");
    ui->labelApiDoc->setText("<a href=\""+meta.url_apiDoc+"\" style=\"color: green;\">"+meta.url_apiDoc+"</a>");
    ui->textEditEngineSummary->setText(meta.summary);
    ui->comboBoxOptions->addItems(meta.flags.keys());
}

void RawTool::on_moduleFraudGuard_clicked(){
    ui->comboBoxOptions->clear();
    ModuleInfo::FraudGuard meta;
    m_optionSet = meta.flags;
    ui->labelUrl->setText("<a href=\""+meta.url+"\" style=\"color: green;\">"+meta.name+"</a>");
    ui->labelApiDoc->setText("<a href=\""+meta.url_apiDoc+"\" style=\"color: green;\">"+meta.url_apiDoc+"</a>");
    ui->textEditEngineSummary->setText(meta.summary);
    ui->comboBoxOptions->addItems(meta.flags.keys());
}

void RawTool::on_moduleHybridAnalysis_clicked(){
    ui->comboBoxOptions->clear();
    ModuleInfo::HybridAnalysis meta;
    m_optionSet = meta.flags;
    ui->labelUrl->setText("<a href=\""+meta.url+"\" style=\"color: green;\">"+meta.name+"</a>");
    ui->labelApiDoc->setText("<a href=\""+meta.url_apiDoc+"\" style=\"color: green;\">"+meta.url_apiDoc+"</a>");
    ui->textEditEngineSummary->setText(meta.summary);
    ui->comboBoxOptions->addItems(meta.flags.keys());
}

void RawTool::on_moduleIpQualityScore_clicked(){
    ui->comboBoxOptions->clear();
    ModuleInfo::IpQualityScore meta;
    m_optionSet = meta.flags;
    ui->labelUrl->setText("<a href=\""+meta.url+"\" style=\"color: green;\">"+meta.name+"</a>");
    ui->labelApiDoc->setText("<a href=\""+meta.url_apiDoc+"\" style=\"color: green;\">"+meta.url_apiDoc+"</a>");
    ui->textEditEngineSummary->setText(meta.summary);
    ui->comboBoxOptions->addItems(meta.flags.keys());
}

void RawTool::on_moduleLeakLookup_clicked(){
    ui->comboBoxOptions->clear();
    ModuleInfo::LeakLookup meta;
    m_optionSet = meta.flags;
    ui->labelUrl->setText("<a href=\""+meta.url+"\" style=\"color: green;\">"+meta.name+"</a>");
    ui->labelApiDoc->setText("<a href=\""+meta.url_apiDoc+"\" style=\"color: green;\">"+meta.url_apiDoc+"</a>");
    ui->textEditEngineSummary->setText(meta.summary);
    ui->comboBoxOptions->addItems(meta.flags.keys());
}

void RawTool::on_moduleAnubis_clicked(){
    ui->comboBoxOptions->clear();
    ModuleInfo::LeakLookup meta;
    m_optionSet = meta.flags;
    ui->labelUrl->setText("<a href=\""+meta.url+"\" style=\"color: green;\">"+meta.name+"</a>");
    ui->labelApiDoc->setText("<a href=\""+meta.url_apiDoc+"\" style=\"color: green;\">"+meta.url_apiDoc+"</a>");
    ui->textEditEngineSummary->setText(meta.summary);
    ui->comboBoxOptions->addItems(meta.flags.keys());
}

void RawTool::on_moduleBgpview_clicked(){
    ui->comboBoxOptions->clear();
    ModuleInfo::Bgpview meta;
    m_optionSet = meta.flags;
    ui->labelUrl->setText("<a href=\""+meta.url+"\" style=\"color: green;\">"+meta.name+"</a>");
    ui->labelApiDoc->setText("<a href=\""+meta.url_apiDoc+"\" style=\"color: green;\">"+meta.url_apiDoc+"</a>");
    ui->textEditEngineSummary->setText(meta.summary);
    ui->comboBoxOptions->addItems(meta.flags.keys());
}

void RawTool::on_moduleWappalyzer_clicked(){
    ui->comboBoxOptions->clear();
    ModuleInfo::Wappalyzer meta;
    m_optionSet = meta.flags;
    ui->labelUrl->setText("<a href=\""+meta.url+"\" style=\"color: green;\">"+meta.name+"</a>");
    ui->labelApiDoc->setText("<a href=\""+meta.url_apiDoc+"\" style=\"color: green;\">"+meta.url_apiDoc+"</a>");
    ui->textEditEngineSummary->setText(meta.summary);
    ui->comboBoxOptions->addItems(meta.flags.keys());
}

void RawTool::on_modulePassiveTotal_clicked(){
    ui->comboBoxOptions->clear();
    ModuleInfo::PassiveTotal meta;
    m_optionSet = meta.flags;
    ui->labelUrl->setText("<a href=\""+meta.url+"\" style=\"color: green;\">"+meta.name+"</a>");
    ui->labelApiDoc->setText("<a href=\""+meta.url_apiDoc+"\" style=\"color: green;\">"+meta.url_apiDoc+"</a>");
    ui->textEditEngineSummary->setText(meta.summary);
    ui->comboBoxOptions->addItems(meta.flags.keys());
}

void RawTool::on_moduleIpfy_clicked(){
    ui->comboBoxOptions->clear();
    ModuleInfo::Ipfy meta;
    m_optionSet = meta.flags;
    ui->labelUrl->setText("<a href=\""+meta.url+"\" style=\"color: green;\">"+meta.name+"</a>");
    ui->labelApiDoc->setText("<a href=\""+meta.url_apiDoc+"\" style=\"color: green;\">"+meta.url_apiDoc+"</a>");
    ui->textEditEngineSummary->setText(meta.summary);
    ui->comboBoxOptions->addItems(meta.flags.keys());
}

void RawTool::on_moduleCirclPublic_clicked(){
    ui->comboBoxOptions->clear();
    ModuleInfo::CirclPublic meta;
    m_optionSet = meta.flags;
    ui->labelUrl->setText("<a href=\""+meta.url+"\" style=\"color: green;\">"+meta.name+"</a>");
    ui->labelApiDoc->setText("<a href=\""+meta.url_apiDoc+"\" style=\"color: green;\">"+meta.url_apiDoc+"</a>");
    ui->textEditEngineSummary->setText(meta.summary);
    ui->comboBoxOptions->addItems(meta.flags.keys());
}

void RawTool::on_moduleASRank_clicked(){
    ui->comboBoxOptions->clear();
    ModuleInfo::ASRank meta;
    m_optionSet = meta.flags;
    ui->labelUrl->setText("<a href=\""+meta.url+"\" style=\"color: green;\">"+meta.name+"</a>");
    ui->labelApiDoc->setText("<a href=\""+meta.url_apiDoc+"\" style=\"color: green;\">"+meta.url_apiDoc+"</a>");
    ui->textEditEngineSummary->setText(meta.summary);
    ui->comboBoxOptions->addItems(meta.flags.keys());
}
