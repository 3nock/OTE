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
        result->osint->subdomainIp->appendRow({new QStandardItem(subdomain), new QStandardItem(ip)});
        project->addPassiveSubdomain({subdomain, ip});
        ui->labelResultsCount->setNum(result->osint->subdomainIpProxy->rowCount());
    }
}

void Osint::onResultSubdomain(QString subdomain){
    int prevSize = m_subdomainSet.count();
    m_subdomainSet.insert(subdomain);
    if(m_subdomainSet.count() > prevSize && !subdomain.isEmpty()){
        result->osint->subdomain->appendRow(new QStandardItem(subdomain));
        project->addPassiveSubdomain({subdomain});
        ui->labelResultsCount->setNum(result->osint->subdomainProxy->rowCount());
    }
}

void Osint::onResultMightContainWildcards(QString subdomain){
    int prevSize = m_subdomainSet.count();
    m_subdomainSet.insert(subdomain);
    if(m_subdomainSet.count() > prevSize && !subdomain.isEmpty())
    {
        result->osint->subdomain->appendRow(new QStandardItem(subdomain));
        ui->labelResultsCount->setNum(result->osint->subdomainProxy->rowCount());

        /* save as wildcard if starts with *. */
        if(subdomain.startsWith("*"))
            project->addPassiveWildcard({subdomain});
        else
            project->addPassiveSubdomain({subdomain});
    }
}

void Osint::onResultIp(QString ip){
    int prevSize = m_ipSet.count();
    m_ipSet.insert(ip);
    if(m_ipSet.count() > prevSize && !ip.isEmpty()){
        result->osint->ip->appendRow(new QStandardItem(ip));
        project->addPassiveIp({ip});
        ui->labelResultsCount->setNum(result->osint->ipProxy->rowCount());
    }
}

void Osint::onResultEmail(QString email){
    int prevSize = m_emailSet.count();
    m_emailSet.insert(email);
    if(m_emailSet.count() > prevSize && !email.isEmpty()){
        result->osint->email->appendRow(new QStandardItem(email));
        project->addPassiveEMail({email});
        ui->labelResultsCount->setNum(result->osint->emailProxy->rowCount());
    }
}

void Osint::onResultUrl(QString url){
    int prevSize = m_urlSet.count();
    m_urlSet.insert(url);
    if(m_urlSet.count() > prevSize && !url.isEmpty()){
        result->osint->url->appendRow(new QStandardItem(url));
        project->addPassiveUrl({url});
        ui->labelResultsCount->setNum(result->osint->urlProxy->rowCount());
    }
}

void Osint::onResultAsn(QString asnValue, QString asnName){
    int prevSize = m_asnSet.count();
    m_asnSet.insert(asnValue);
    if(m_asnSet.count() > prevSize && !asnValue.isEmpty()){
        result->osint->asn->appendRow({new QStandardItem(asnValue), new QStandardItem(asnName)});
        project->addPassiveAsn({asnValue, asnName});
        ui->labelResultsCount->setNum(result->osint->asnProxy->rowCount());
    }
}

void Osint::onResultCidr(QString cidr){
    int prevSize = m_cidrSet.count();
    m_cidrSet.insert(cidr);
    if(m_cidrSet.count() > prevSize && !cidr.isEmpty()){
        result->osint->cidr->appendRow(new QStandardItem(A));
        project->addPassiveCidr({cidr});
        ui->labelResultsCount->setNum(result->osint->cidrProxy->rowCount());
    }
}

void Osint::onResultA(QString A){
    int prevSize = m_ipSet.count();
    m_ipSet.insert(A);
    if(m_ipSet.count() > prevSize && !A.isEmpty()){
        result->osint->ip->appendRow(new QStandardItem(A));
        project->addPassiveA({A});
        ui->labelResultsCount->setNum(result->osint->ipProxy->rowCount());
    }
}

void Osint::onResultAAAA(QString AAAA){
    int prevSize = m_ipSet.count();
    m_ipSet.insert(AAAA);
    if(m_ipSet.count() > prevSize && !AAAA.isEmpty()){
        result->osint->ip->appendRow(new QStandardItem(AAAA));
        project->addPassiveAAAA({AAAA});
        ui->labelResultsCount->setNum(result->osint->ipProxy->rowCount());
    }
}

void Osint::onResultCNAME(QString CNAME){
    int prevSize = m_subdomainSet.count();
    m_subdomainSet.insert(CNAME);
    if(m_subdomainSet.count() > prevSize && !CNAME.isEmpty()){
        result->osint->subdomain->appendRow(new QStandardItem(CNAME));
        project->addPassiveCNAME({CNAME});
        ui->labelResultsCount->setNum(result->osint->subdomainProxy->rowCount());
    }
}

void Osint::onResultMX(QString MX){
    int prevSize = m_subdomainSet.count();
    m_subdomainSet.insert(MX);
    if(m_subdomainSet.count() > prevSize && !MX.isEmpty()){
        result->osint->subdomain->appendRow(new QStandardItem(MX));
        project->addPassiveMX({MX});
        ui->labelResultsCount->setNum(result->osint->subdomainProxy->rowCount());
    }
}

void Osint::onResultNS(QString NS){
    int prevSize = m_subdomainSet.count();
    m_subdomainSet.insert(NS);
    if(m_subdomainSet.count() > prevSize && !NS.isEmpty()){
        result->osint->subdomain->appendRow(new QStandardItem(NS));
        project->addPassiveNS({NS});
        ui->labelResultsCount->setNum(result->osint->subdomainProxy->rowCount());
    }
}

void Osint::onResultTXT(QString TXT){
    if(!TXT.isEmpty())
        project->addPassiveTXT({TXT});
}

void Osint::onResultSSLCert(QString sha1_or_sha256){
    int prevSize = m_sslCertSet.count();
    m_sslCertSet.insert(sha1_or_sha256);
    if(m_sslCertSet.count() > prevSize && !sha1_or_sha256.isEmpty()){
        result->osint->sslCert->appendRow(new QStandardItem(sha1_or_sha256));
        project->addPassiveSSLCert({sha1_or_sha256});
        ui->labelResultsCount->setNum(result->osint->sslCertProxy->rowCount());
    }
}
