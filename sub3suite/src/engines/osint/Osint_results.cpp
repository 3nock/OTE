/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#include "Osint.h"
#include "ui_Osint.h"


void Osint::onInfoLog(ScanLog log){
    ui->plainTextEditLogs->appendHtml("[Module]        : <font color=\"green\">"+log.moduleName+"</font>");
    ui->plainTextEditLogs->appendHtml("[Status Code]   : <font color=\"green\">"+QString::number(log.statusCode)+"</font>");
    ui->plainTextEditLogs->appendHtml("[Results Count] : <font color=\"green\">"+QString::number(log.resultsCount)+"</font>");
    if(!log.message.isEmpty())
        ui->plainTextEditLogs->appendHtml("[Error message] : <font color=\"green\">"+log.message+"</font>");

    // add a new line...
    ui->plainTextEditLogs->appendPlainText("");
}

void Osint::onErrorLog(ScanLog log){
    ui->plainTextEditLogs->appendHtml("[Module]        : <font color=\"red\">"+log.moduleName+"</font>");
    ui->plainTextEditLogs->appendHtml("[Status Code]   : <font color=\"red\">"+QString::number(log.statusCode)+"</font>");
    ui->plainTextEditLogs->appendHtml("[Results Count] : <font color=\"red\">"+QString::number(log.resultsCount)+"</font>");
    if(!log.message.isEmpty())
        ui->plainTextEditLogs->appendHtml("[Error message] : <font color=\"red\">"+log.message+"</font>");

    // add a new line...
    ui->plainTextEditLogs->appendPlainText("");
}

void Osint::onRateLimitLog(ScanLog log){
    ui->plainTextEditLogs->appendHtml("[Module]        : <font color=\"yellow\">"+log.moduleName+"</font>");
    ui->plainTextEditLogs->appendHtml("[Status Code]   : <font color=\"yellow\">"+QString::number(log.statusCode)+"</font>");
    ui->plainTextEditLogs->appendHtml("[Results Count] : <font color=\"yellow\">"+QString::number(log.resultsCount)+"</font>");
    if(!log.message.isEmpty())
        ui->plainTextEditLogs->appendHtml("[Error message] : <font color=\"yellow\">"+log.message+"</font>");

    // add a new line...
    ui->plainTextEditLogs->appendPlainText("");
}

void Osint::onResultSubdomainIp(QString subdomain, QString ip){
    int prevSize = m_subdomainIpSet.count();
    /* inserting subdomain+ip */
    m_subdomainIpSet.insert(subdomain+ip);
    if(m_subdomainIpSet.count() > prevSize && !subdomain.isEmpty()){
        m_resultModelSubdomainIp->appendRow({new QStandardItem(subdomain), new QStandardItem(ip)});
        project->addPassiveSubdomainIp(subdomain, ip);
        ui->labelResultsCount->setNum(m_resultProxyModel->rowCount());
    }
}

void Osint::onResultSubdomain(QString subdomain){
    int prevSize = m_subdomainSet.count();
    m_subdomainSet.insert(subdomain);
    if(m_subdomainSet.count() > prevSize && !subdomain.isEmpty()){
        m_resultModelSubdomain->appendRow(new QStandardItem(subdomain));
        project->addPassiveSubdomain(subdomain);
        ui->labelResultsCount->setNum(m_resultProxyModel->rowCount());
    }
}

void Osint::onResultMightContainWildcards(QString subdomain){
    int prevSize = m_subdomainSet.count();
    m_subdomainSet.insert(subdomain);
    if(m_subdomainSet.count() > prevSize && !subdomain.isEmpty())
    {
        m_resultModelSubdomain->appendRow(new QStandardItem(subdomain));
        ui->labelResultsCount->setNum(m_resultProxyModel->rowCount());

        /* save as wildcard if starts with *. */
        if(!subdomain.startsWith("*"))
            project->addPassiveSubdomain(subdomain);
    }
}

void Osint::onResultIp(QString ip){
    int prevSize = m_ipSet.count();
    m_ipSet.insert(ip);
    if(m_ipSet.count() > prevSize && !ip.isEmpty()){
        m_resultModelIp->appendRow(new QStandardItem(ip));
        project->addPassiveIp(ip);
        ui->labelResultsCount->setNum(m_resultProxyModel->rowCount());
    }
}

void Osint::onResultEmail(QString email){
    int prevSize = m_emailSet.count();
    m_emailSet.insert(email);
    if(m_emailSet.count() > prevSize && !email.isEmpty()){
        m_resultModelEmail->appendRow(new QStandardItem(email));
        project->addPassiveEMail(email);
        ui->labelResultsCount->setNum(m_resultProxyModel->rowCount());
    }
}

void Osint::onResultUrl(QString url){
    int prevSize = m_urlSet.count();
    m_urlSet.insert(url);
    if(m_urlSet.count() > prevSize && !url.isEmpty()){
        m_resultModelUrl->appendRow(new QStandardItem(url));
        project->addPassiveUrl(url);
        ui->labelResultsCount->setNum(m_resultProxyModel->rowCount());
    }
}

void Osint::onResultAsn(QString asn, QString name){
    int prevSize = m_asnSet.count();
    m_asnSet.insert(asn);
    if(m_asnSet.count() > prevSize && !asn.isEmpty()){
        m_resultModelAsn->appendRow({new QStandardItem(asn), new QStandardItem(name)});
        project->addPassiveAsn(asn, name);
        ui->labelResultsCount->setNum(m_resultProxyModel->rowCount());
    }
}

void Osint::onResultCidr(QString cidr){
    int prevSize = m_cidrSet.count();
    m_cidrSet.insert(cidr);
    if(m_cidrSet.count() > prevSize && !cidr.isEmpty()){
        m_resultModelCidr->appendRow(new QStandardItem(cidr));
        project->addPassiveCidr(cidr);
        ui->labelResultsCount->setNum(m_resultProxyModel->rowCount());
    }
}

void Osint::onResultA(QString A){
    int prevSize = m_ipSet.count();
    m_ipSet.insert(A);
    if(m_ipSet.count() > prevSize && !A.isEmpty()){
        m_resultModelIp->appendRow(new QStandardItem(A));
        project->addPassiveA(A);
        ui->labelResultsCount->setNum(m_resultProxyModel->rowCount());
    }
}

void Osint::onResultAAAA(QString AAAA){
    int prevSize = m_ipSet.count();
    m_ipSet.insert(AAAA);
    if(m_ipSet.count() > prevSize && !AAAA.isEmpty()){
        m_resultModelIp->appendRow(new QStandardItem(AAAA));
        project->addPassiveAAAA(AAAA);
        ui->labelResultsCount->setNum(m_resultProxyModel->rowCount());
    }
}

void Osint::onResultCNAME(QString CNAME){
    int prevSize = m_subdomainSet.count();
    m_subdomainSet.insert(CNAME);
    if(m_subdomainSet.count() > prevSize && !CNAME.isEmpty()){
        m_resultModelSubdomain->appendRow(new QStandardItem(CNAME));
        project->addPassiveCNAME(CNAME);
        ui->labelResultsCount->setNum(m_resultProxyModel->rowCount());
    }
}

void Osint::onResultMX(QString MX){
    int prevSize = m_subdomainSet.count();
    m_subdomainSet.insert(MX);
    if(m_subdomainSet.count() > prevSize && !MX.isEmpty()){
        m_resultModelSubdomain->appendRow(new QStandardItem(MX));
        project->addPassiveMX(MX);
        ui->labelResultsCount->setNum(m_resultProxyModel->rowCount());
    }
}

void Osint::onResultNS(QString NS){
    int prevSize = m_subdomainSet.count();
    m_subdomainSet.insert(NS);
    if(m_subdomainSet.count() > prevSize && !NS.isEmpty()){
        m_resultModelSubdomain->appendRow(new QStandardItem(NS));
        project->addPassiveNS(NS);
        ui->labelResultsCount->setNum(m_resultProxyModel->rowCount());
    }
}

void Osint::onResultTXT(QString TXT){
    if(!TXT.isEmpty())
        project->addPassiveTXT(TXT);
}

void Osint::onResultSSLCert(QString sha1_or_sha256){
    int prevSize = m_sslCertSet.count();
    m_sslCertSet.insert(sha1_or_sha256);
    if(m_sslCertSet.count() > prevSize && !sha1_or_sha256.isEmpty()){
        m_resultModelCert->appendRow(new QStandardItem(sha1_or_sha256));
        project->addPassiveSSL(sha1_or_sha256);
        ui->labelResultsCount->setNum(m_resultProxyModel->rowCount());
    }
}
