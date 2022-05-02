/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :implementation of the Slots to receive results from the scanning osint module threads
*/

#include "OsintTool.h"
#include "ui_OsintTool.h"


void OsintTool::onScanLog(ScanLog log){
    if(log.error){
        ui->plainTextEditLogs->appendHtml("[Module]        : <font color=\"red\">"+log.moduleName+"</font>");
        ui->plainTextEditLogs->appendHtml("[Target]        : <font color=\"red\">"+log.target+"</font>");
        ui->plainTextEditLogs->appendHtml("[Status Code]   : <font color=\"red\">"+QString::number(log.statusCode)+"</font>");
        ui->plainTextEditLogs->appendHtml("[Results Count] : <font color=\"red\">"+QString::number(log.resultsCount)+"</font>");
        if(!log.message.isEmpty())
            ui->plainTextEditLogs->appendHtml("[Error message] : <font color=\"red\">"+log.message+"</font>");
    }
    else{
        ui->plainTextEditLogs->appendHtml("[Module]        : <font color=\"green\">"+log.moduleName+"</font>");
        ui->plainTextEditLogs->appendHtml("[Target]        : <font color=\"green\">"+log.target+"</font>");
        ui->plainTextEditLogs->appendHtml("[Status Code]   : <font color=\"green\">"+QString::number(log.statusCode)+"</font>");
        ui->plainTextEditLogs->appendHtml("[Results Count] : <font color=\"green\">"+QString::number(log.resultsCount)+"</font>");
        if(!log.message.isEmpty())
            ui->plainTextEditLogs->appendHtml("[Error message] : <font color=\"green\">"+log.message+"</font>");
    }

    qDebug() << "Logged out: " << log.moduleName;

    // add a new line...
    ui->plainTextEditLogs->appendPlainText("");
}

void OsintTool::onResultSubdomainIp(QString subdomain, QString ip){
    if(set_subdomainIP.contains(subdomain))
        return;

    m_model_subdomainIp->appendRow({new QStandardItem(subdomain), new QStandardItem(ip)});
    set_subdomainIP.insert(subdomain);
    ui->labelResultsCount->setNum(proxyModel->rowCount());

    if(m_scanConfig->autosaveToProject)
        project->addPassiveSubdomainIp(subdomain, ip);
}

void OsintTool::onResultSubdomain(QString subdomain){
    if(set_subdomain.contains(subdomain))
        return;

    m_model_subdomain->appendRow(new QStandardItem(subdomain));
    set_subdomain.insert(subdomain);
    ui->labelResultsCount->setNum(proxyModel->rowCount());

    if(m_scanConfig->autosaveToProject)
        project->addPassiveSubdomain(subdomain);
}

void OsintTool::onResultIP(QString ip){
    if(set_ip.contains(ip))
        return;

    m_model_ip->appendRow(new QStandardItem(ip));
    set_ip.insert(ip);
    ui->labelResultsCount->setNum(proxyModel->rowCount());

    if(m_scanConfig->autosaveToProject)
        project->addPassiveIp(ip);
}

void OsintTool::onResultEmail(QString email){
    if(set_email.contains(email))
        return;

    m_model_email->appendRow(new QStandardItem(email));
    set_email.insert(email);
    ui->labelResultsCount->setNum(proxyModel->rowCount());

    if(m_scanConfig->autosaveToProject)
        project->addPassiveEMail(email);
}

void OsintTool::onResultURL(QString url){
    if(set_url.contains(url))
        return;

    m_model_url->appendRow(new QStandardItem(url));
    set_url.insert(url);
    ui->labelResultsCount->setNum(proxyModel->rowCount());

    if(m_scanConfig->autosaveToProject)
        project->addPassiveUrl(url);
}

void OsintTool::onResultASN(QString asn, QString name){
    if(asn.startsWith("AS", Qt::CaseInsensitive))
        asn = asn.remove(0, 2);

    if(set_asn.contains(asn))
        return;

    m_model_asn->appendRow({new QStandardItem(asn), new QStandardItem(name)});
    set_asn.insert(asn);
    ui->labelResultsCount->setNum(proxyModel->rowCount());

    if(m_scanConfig->autosaveToProject)
        project->addPassiveAsn(asn, name);
}

void OsintTool::onResultCIDR(QString cidr){
    if(set_cidr.contains(cidr))
        return;

    m_model_cidr->appendRow(new QStandardItem(cidr));
    set_cidr.insert(cidr);
    ui->labelResultsCount->setNum(proxyModel->rowCount());

    if(m_scanConfig->autosaveToProject)
        project->addPassiveCidr(cidr);
}

void OsintTool::onResultA(QString A){
    if(set_ip.contains(A))
        return;

    m_model_ip->appendRow(new QStandardItem(A));
    set_ip.insert(A);
    ui->labelResultsCount->setNum(proxyModel->rowCount());

    if(m_scanConfig->autosaveToProject)
        project->addPassiveA(A);
}

void OsintTool::onResultAAAA(QString AAAA){
    if(set_ip.contains(AAAA))
        return;

    m_model_ip->appendRow(new QStandardItem(AAAA));
    set_ip.insert(AAAA);
    ui->labelResultsCount->setNum(proxyModel->rowCount());

    if(m_scanConfig->autosaveToProject)
        project->addPassiveAAAA(AAAA);
}

void OsintTool::onResultCNAME(QString CNAME){
    if(set_subdomain.contains(CNAME))
        return;

    m_model_subdomain->appendRow(new QStandardItem(CNAME));
    set_subdomain.insert(CNAME);
    ui->labelResultsCount->setNum(proxyModel->rowCount());

    if(m_scanConfig->autosaveToProject)
        project->addPassiveCNAME(CNAME);
}

void OsintTool::onResultMX(QString MX){
    if(set_subdomain.contains(MX))
        return;

    m_model_subdomain->appendRow(new QStandardItem(MX));
    set_subdomain.insert(MX);
    ui->labelResultsCount->setNum(proxyModel->rowCount());

    if(m_scanConfig->autosaveToProject)
        project->addPassiveMX(MX);
}

void OsintTool::onResultNS(QString NS){
    if(set_subdomain.contains(NS))
        return;

    m_model_subdomain->appendRow(new QStandardItem(NS));
    set_subdomain.insert(NS);
    ui->labelResultsCount->setNum(proxyModel->rowCount());

    if(m_scanConfig->autosaveToProject)
        project->addPassiveNS(NS);
}

void OsintTool::onResultTXT(QString TXT){
    if(!TXT.isEmpty() && m_scanConfig->autosaveToProject)
        project->addPassiveTXT(TXT);
}

void OsintTool::onResultSSL(QString sha1_or_sha256){
    if(set_ssl.contains(sha1_or_sha256))
        return;

    m_model_ssl->appendRow(new QStandardItem(sha1_or_sha256));
    set_ssl.insert(sha1_or_sha256);
    ui->labelResultsCount->setNum(proxyModel->rowCount());

    if(m_scanConfig->autosaveToProject)
        project->addPassiveSSL(sha1_or_sha256);
}
