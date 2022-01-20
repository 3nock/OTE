#include "ProjectModel.h"


ProjectModel::ProjectModel():
    projectExplorerModel(new QStandardItemModel),
    active(new QStandardItem("Active")),
    passive(new QStandardItem("Passive")),
    enums(new QStandardItem("Enum")),
    custom(new QStandardItem("Custom")),

    /* active Results Explorer */
    activeSubdomainIp(new QStandardItem("Subdomain Ip")),
    activeSubdomain(new QStandardItem("Subdomain")),
    activeTld(new QStandardItem("TLD")),
    activeWildcard(new QStandardItem("Wildcard")),
    // dns
    activeDNS(new QStandardItem("DNS")),
    activeA(new QStandardItem("A")),
    activeAAAA(new QStandardItem("AAAA")),
    activeNS(new QStandardItem("NS")),
    activeMX(new QStandardItem("MX")),
    activeTXT(new QStandardItem("TXT")),
    activeCNAME(new QStandardItem("CNAME")),
    activeSRV(new QStandardItem("SRV")),
    // ssl
    activeSSL(new QStandardItem("SSL")),
    activeSSL_sha1(new QStandardItem("SHA-1")),
    activeSSL_sha256(new QStandardItem("SHA-256")),

    /* passive Results explorer */
    passiveSubdomainIp(new QStandardItem("Subdomain Ip")),
    passiveSubdomain(new QStandardItem("Subdomain")),
    passiveA(new QStandardItem("A")),
    passiveAAAA(new QStandardItem("AAAA")),
    passiveNS(new QStandardItem("NS")),
    passiveMX(new QStandardItem("MX")),
    passiveTXT(new QStandardItem("TXT")),
    passiveCNAME(new QStandardItem("CNAME")),
    passiveCidr(new QStandardItem("CIDR")),
    passiveEmail(new QStandardItem("EMAIL")),
    passiveUrl(new QStandardItem("URL")),
    passiveAsn(new QStandardItem("ASN")),
    passiveSSL(new QStandardItem("SSL")),

    /* enum Results explorer */
    enumIp(new QStandardItem("IP")),
    enumDomain(new QStandardItem("Domain")),
    enumASN(new QStandardItem("ASN")),
    enumCIDR(new QStandardItem("CIDR")),
    enumNS(new QStandardItem("NS")),
    enumMX(new QStandardItem("MX")),
    enumSSL(new QStandardItem("SSL")),
    enumEmail(new QStandardItem("Email")),
    enumURL(new QStandardItem("URL")),

    /* active Results Model */
    activeSubdomainIp_model(new QStandardItemModel),
    activeSubdomain_model(new QStandardItemModel),
    activeTld_model(new QStandardItemModel),
    activeWildcard_model(new QStandardItemModel),
    // dns
    activeDNS_model(new QStandardItemModel),
    activeA_model(new QStandardItemModel),
    activeAAAA_model(new QStandardItemModel),
    activeNS_model(new QStandardItemModel),
    activeMX_model(new QStandardItemModel),
    activeTXT_model(new QStandardItemModel),
    activeCNAME_model(new QStandardItemModel),
    activeSRV_model(new QStandardItemModel),
    // ssl
    activeSSL_model(new QStandardItemModel),
    activeSSL_sha1_model(new QStandardItemModel),
    activeSSL_sha256_model(new QStandardItemModel),

    /* passive results model */
    passiveSubdomainIp_model(new QStandardItemModel),
    passiveSubdomain_model(new QStandardItemModel),
    passiveA_model(new QStandardItemModel),
    passiveAAAA_model(new QStandardItemModel),
    passiveCidr_model(new QStandardItemModel),
    passiveNS_model(new QStandardItemModel),
    passiveMX_model(new QStandardItemModel),
    passiveTXT_model(new QStandardItemModel),
    passiveCNAME_model(new QStandardItemModel),
    passiveEmail_model(new QStandardItemModel),
    passiveUrl_model(new QStandardItemModel),
    passiveAsn_model(new QStandardItemModel),
    passiveSSL_model(new QStandardItemModel),

    /* enum Results model */
    enumIp_model(new QStandardItemModel),
    enumDomain_model(new QStandardItemModel),
    enumASN_model(new QStandardItemModel),
    enumCIDR_model(new QStandardItemModel),
    enumNS_model(new QStandardItemModel),
    enumMX_model(new QStandardItemModel),
    enumSSL_model(new QStandardItemModel),
    enumEmail_model(new QStandardItemModel),
    enumURL_model(new QStandardItemModel)
{
    this->setHeaderLabels();
    explorerRootItem = projectExplorerModel->invisibleRootItem();

    QFont font("Segoe UI", 9, QFont::Bold);
    active->setFont(font);
    active->setForeground(Qt::white);
    active->setIcon(QIcon(":/img/res/icons/folder.png"));
    passive->setFont(font);
    passive->setForeground(Qt::white);
    passive->setIcon(QIcon(":/img/res/icons/folder.png"));
    enums->setFont(font);
    enums->setForeground(Qt::white);
    enums->setIcon(QIcon(":/img/res/icons/folder.png"));
    custom->setFont(font);
    custom->setForeground(Qt::white);
    custom->setIcon(QIcon(":/img/res/icons/folder.png"));

    ///
    /// setting items icons
    ///
    activeSubdomainIp->setIcon(QIcon(":/img/res/icons/folder2.png"));
    activeSubdomain->setIcon(QIcon(":/img/res/icons/folder2.png"));
    activeTld->setIcon(QIcon(":/img/res/icons/folder2.png"));
    activeWildcard->setIcon(QIcon(":/img/res/icons/folder2.png"));
    activeDNS->setIcon(QIcon(":/img/res/icons/folder2.png"));
    activeA->setIcon(QIcon(":/img/res/icons/folder2.png"));
    activeAAAA->setIcon(QIcon(":/img/res/icons/folder2.png"));
    activeNS->setIcon(QIcon(":/img/res/icons/folder2.png"));
    activeMX->setIcon(QIcon(":/img/res/icons/folder2.png"));
    activeTXT->setIcon(QIcon(":/img/res/icons/folder2.png"));
    activeCNAME->setIcon(QIcon(":/img/res/icons/folder2.png"));
    activeSRV->setIcon(QIcon(":/img/res/icons/folder2.png"));
    activeSSL->setIcon(QIcon(":/img/res/icons/folder2.png"));
    activeSSL_sha1->setIcon(QIcon(":/img/res/icons/folder2.png"));
    activeSSL_sha256->setIcon(QIcon(":/img/res/icons/folder2.png"));
    passiveSubdomainIp->setIcon(QIcon(":/img/res/icons/folder2.png"));
    passiveSubdomain->setIcon(QIcon(":/img/res/icons/folder2.png"));
    passiveA->setIcon(QIcon(":/img/res/icons/folder2.png"));
    passiveAAAA->setIcon(QIcon(":/img/res/icons/folder2.png"));
    passiveCidr->setIcon(QIcon(":/img/res/icons/folder2.png"));
    passiveNS->setIcon(QIcon(":/img/res/icons/folder2.png"));
    passiveMX->setIcon(QIcon(":/img/res/icons/folder2.png"));
    passiveTXT->setIcon(QIcon(":/img/res/icons/folder2.png"));
    passiveCNAME->setIcon(QIcon(":/img/res/icons/folder2.png"));
    passiveEmail->setIcon(QIcon(":/img/res/icons/folder2.png"));
    passiveUrl->setIcon(QIcon(":/img/res/icons/folder2.png"));
    passiveAsn->setIcon(QIcon(":/img/res/icons/folder2.png"));
    passiveSSL->setIcon(QIcon(":/img/res/icons/folder2.png"));
    enumIp->setIcon(QIcon(":/img/res/icons/folder2.png"));
    enumDomain->setIcon(QIcon(":/img/res/icons/folder2.png"));
    enumASN->setIcon(QIcon(":/img/res/icons/folder2.png"));
    enumCIDR->setIcon(QIcon(":/img/res/icons/folder2.png"));
    enumNS->setIcon(QIcon(":/img/res/icons/folder2.png"));
    enumMX->setIcon(QIcon(":/img/res/icons/folder2.png"));
    enumSSL->setIcon(QIcon(":/img/res/icons/folder2.png"));
    enumEmail->setIcon(QIcon(":/img/res/icons/folder2.png"));
    enumURL->setIcon(QIcon(":/img/res/icons/folder2.png"));

    ///
    /// setting foreground
    ///
    activeSubdomainIp->setForeground(Qt::white);
    activeSubdomain->setForeground(Qt::white);
    activeTld->setForeground(Qt::white);
    activeWildcard->setForeground(Qt::white);
    activeDNS->setForeground(Qt::white);
    activeA->setForeground(Qt::white);
    activeAAAA->setForeground(Qt::white);
    activeNS->setForeground(Qt::white);
    activeMX->setForeground(Qt::white);
    activeTXT->setForeground(Qt::white);
    activeCNAME->setForeground(Qt::white);
    activeSRV->setForeground(Qt::white);
    activeSSL->setForeground(Qt::white);
    activeSSL_sha1->setForeground(Qt::white);
    activeSSL_sha256->setForeground(Qt::white);
    passiveSubdomain->setForeground(Qt::white);
    passiveSubdomainIp->setForeground(Qt::white);
    passiveA->setForeground(Qt::white);
    passiveAAAA->setForeground(Qt::white);
    passiveCidr->setForeground(Qt::white);
    passiveNS->setForeground(Qt::white);
    passiveMX->setForeground(Qt::white);
    passiveTXT->setForeground(Qt::white);
    passiveCNAME->setForeground(Qt::white);
    passiveEmail->setForeground(Qt::white);
    passiveUrl->setForeground(Qt::white);
    passiveAsn->setForeground(Qt::white);
    passiveSSL->setForeground(Qt::white);
    enumIp->setForeground(Qt::white);
    enumDomain->setForeground(Qt::white);
    enumASN->setForeground(Qt::white);
    enumCIDR->setForeground(Qt::white);
    enumNS->setForeground(Qt::white);
    enumMX->setForeground(Qt::white);
    enumSSL->setForeground(Qt::white);
    enumEmail->setForeground(Qt::white);
    enumURL->setForeground(Qt::white);

    ///
    /// appending items...
    ///

    activeDNS->appendRow(activeA);
    activeDNS->appendRow(activeAAAA);
    activeDNS->appendRow(activeNS);
    activeDNS->appendRow(activeMX);
    activeDNS->appendRow(activeTXT);
    activeDNS->appendRow(activeCNAME);
    activeDNS->appendRow(activeSRV);
    activeSSL->appendRow(activeSSL_sha1);
    activeSSL->appendRow(activeSSL_sha256);
    active->appendRow(activeSubdomainIp);
    active->appendRow(activeSubdomain);
    active->appendRow(activeTld);
    active->appendRow(activeWildcard);
    active->appendRow(activeDNS);
    active->appendRow(activeSSL);
    passive->appendRow(passiveSubdomainIp);
    passive->appendRow(passiveSubdomain);
    passive->appendRow(passiveA);
    passive->appendRow(passiveAAAA);
    passive->appendRow(passiveCidr);
    passive->appendRow(passiveNS);
    passive->appendRow(passiveMX);
    passive->appendRow(passiveTXT);
    passive->appendRow(passiveCNAME);
    passive->appendRow(passiveEmail);
    passive->appendRow(passiveUrl);
    passive->appendRow(passiveAsn);
    passive->appendRow(passiveSSL);
    enums->appendRow(enumIp);
    enums->appendRow(enumMX);
    enums->appendRow(enumDomain);
    enums->appendRow(enumASN);
    enums->appendRow(enumCIDR);
    enums->appendRow(enumNS);
    enums->appendRow(enumSSL);
    enums->appendRow(enumEmail);
    enums->appendRow(enumURL);

    ///
    /// append to rootItem....
    ///
    explorerRootItem->appendRow(active);
    explorerRootItem->appendRow(passive);
    explorerRootItem->appendRow(enums);
    explorerRootItem->appendRow(custom);
}
ProjectModel::~ProjectModel(){
    delete projectExplorerModel;
    delete active;
    delete passive;
    delete enums;
    delete custom;
    delete activeSubdomainIp;
    delete activeSubdomain;
    delete activeTld;
    delete activeWildcard;
    delete activeDNS;
    delete activeA;
    delete activeAAAA;
    delete activeNS;
    delete activeMX;
    delete activeTXT;
    delete activeCNAME;
    delete activeSRV;
    delete activeSSL;
    delete activeSSL_sha1;
    delete activeSSL_sha256;
    delete passiveSubdomainIp;
    delete passiveSubdomain;
    delete passiveA;
    delete passiveAAAA;
    delete passiveNS;
    delete passiveMX;
    delete passiveTXT;
    delete passiveCNAME;
    delete passiveCidr;
    delete passiveEmail;
    delete passiveUrl;
    delete passiveAsn;
    delete passiveSSL;
    delete enumIp;
    delete enumDomain;
    delete enumASN;
    delete enumCIDR;
    delete enumNS;
    delete enumMX;
    delete enumSSL;
    delete enumEmail;
    delete enumURL;
    delete activeSubdomainIp_model;
    delete activeSubdomain_model;
    delete activeTld_model;
    delete activeWildcard_model;
    delete activeDNS_model;
    delete activeA_model;
    delete activeAAAA_model;
    delete activeNS_model;
    delete activeMX_model;
    delete activeTXT_model;
    delete activeCNAME_model;
    delete activeSRV_model;
    delete activeSSL_model;
    delete activeSSL_sha1_model;
    delete activeSSL_sha256_model;
    delete passiveSubdomainIp_model;
    delete passiveSubdomain_model;
    delete passiveA_model;
    delete passiveAAAA_model;
    delete passiveCidr_model;
    delete passiveNS_model;
    delete passiveMX_model;
    delete passiveTXT_model;
    delete passiveCNAME_model;
    delete passiveEmail_model;
    delete passiveUrl_model;
    delete passiveAsn_model;
    delete passiveSSL_model;
    delete enumIp_model;
    delete enumDomain_model;
    delete enumASN_model;
    delete enumCIDR_model;
    delete enumNS_model;
    delete enumMX_model;
    delete enumSSL_model;
    delete enumEmail_model;
    delete enumURL_model;
}

void ProjectModel::addActiveSubdomainIp(const QString &subdomain, const QString &ip){
    activeSubdomainIp_model->appendRow({new QStandardItem(subdomain), new QStandardItem(ip)});
}

void ProjectModel::addActiveSubdomain(const QString &subdomain){
    activeSubdomain_model->appendRow(new QStandardItem(subdomain));

}

void ProjectModel::addActiveTLD(const QString &subdomain, const QString &ip){
    activeTld_model->appendRow({new QStandardItem(subdomain), new QStandardItem(ip)});
}

void ProjectModel::addActiveWildcard(const QString &subdomain, const QString &ip){
    activeWildcard_model->appendRow({new QStandardItem(subdomain), new QStandardItem(ip)});
}

void ProjectModel::addActiveSSL(const SSL_struct &ssl){
    /* nothing yet */
}

void ProjectModel::addActiveSSL_sha1(const QString &sha1){
    activeSSL_sha1_model->appendRow(new QStandardItem(sha1));
}

void ProjectModel::addActiveSSL_sha256(const QString &sha256){
    activeSSL_sha256_model->appendRow(new QStandardItem(sha256));
}

void ProjectModel::addActiveDNS(const dns::ScanResult &dns){
    /* nothing yet */
}

void ProjectModel::addPassiveSubdomainIp(const QString &subdomain, const QString &ip){
    passiveSubdomainIp_model->appendRow({new QStandardItem(subdomain), new QStandardItem(ip)});
}

void ProjectModel::addPassiveSubdomain(const QString &subdomain){
    passiveSubdomain_model->appendRow(new QStandardItem(subdomain));
}

void ProjectModel::addPassiveIp(const QString &ip){
    if(ip.contains(":"))
        passiveAAAA_model->appendRow(new QStandardItem(ip));
    else
        passiveA_model->appendRow(new QStandardItem(ip));
}

void ProjectModel::addPassiveA(const QString &ipv4){
    passiveA_model->appendRow(new QStandardItem(ipv4));
}

void ProjectModel::addPassiveAAAA(const QString &ipv6){
    passiveAAAA_model->appendRow(new QStandardItem(ipv6));
}

void ProjectModel::addPassiveCidr(const QString &cidr){
    passiveCidr_model->appendRow(new QStandardItem(cidr));
}

void ProjectModel::addPassiveNS(const QString &ns){
    passiveNS_model->appendRow(new QStandardItem(ns));
}

void ProjectModel::addPassiveMX(const QString &mx){
    passiveMX_model->appendRow(new QStandardItem(mx));
}

void ProjectModel::addPassiveTXT(const QString &txt){
    passiveTXT_model->appendRow(new QStandardItem(txt));
}

void ProjectModel::addPassiveCNAME(const QString &cname){
    passiveCNAME_model->appendRow(new QStandardItem(cname));
}

void ProjectModel::addPassiveEMail(const QString &email){
    passiveEmail_model->appendRow(new QStandardItem(email));
}

void ProjectModel::addPassiveUrl(const QString &url){
    passiveUrl_model->appendRow(new QStandardItem(url));
}

void ProjectModel::addPassiveAsn(const QString &asn, const QString &name){
    passiveAsn_model->appendRow({new QStandardItem(asn), new QStandardItem(name)});
}

void ProjectModel::addPassiveSSL(const QString &ssl){
    passiveSSL_model->appendRow(new QStandardItem(ssl));
}

void ProjectModel::addEnumIp(){

}

void ProjectModel::addEnumDomain(){

}

void ProjectModel::addEnumASN(){

}

void ProjectModel::addEnumCIDR(){

}

void ProjectModel::addEnumNS(){

}

void ProjectModel::addEnumMX(){

}

void ProjectModel::addEnumSSL(){

}

void ProjectModel::addEnumEmail(){

}

void ProjectModel::addEnumURL(){

}

void ProjectModel::setHeaderLabels(){
    /* active Results Model */
    activeSubdomainIp_model->setHorizontalHeaderLabels({"    Subdomains", "    IpAddress"});
    activeSubdomain_model->setHorizontalHeaderLabels({"    Subdomains"});
    activeTld_model->setHorizontalHeaderLabels({"    TLD", "    IpAddress"});
    activeWildcard_model->setHorizontalHeaderLabels({"    Wildcard", "    IpAddress"});
    // dns
    activeDNS_model->setHorizontalHeaderLabels({"    DNS Records"});
    activeA_model->setHorizontalHeaderLabels({"    A DNS Records"});
    activeAAAA_model->setHorizontalHeaderLabels({"    AAAA DNS Records"});
    activeNS_model->setHorizontalHeaderLabels({"    NS DNS Records"});
    activeMX_model->setHorizontalHeaderLabels({"    MX DNS Records"});
    activeTXT_model->setHorizontalHeaderLabels({"    TXT DNS Records"});
    activeCNAME_model->setHorizontalHeaderLabels({"    CNAME DNS Records"});
    activeSRV_model->setHorizontalHeaderLabels({"    Name", "    Target", "    Port"});
    // ssl
    activeSSL_model->setHorizontalHeaderLabels({"    SSL Certificates"});
    activeSSL_sha1_model->setHorizontalHeaderLabels({"    SSL Certificates SHA-1 Fingerprints"});
    activeSSL_sha256_model->setHorizontalHeaderLabels({"    SSL Certificates SHA-256 Fingerprints"});

    /* passive results model */
    passiveSubdomainIp_model->setHorizontalHeaderLabels({"    Subdomains", "    IpAddress"});
    passiveSubdomain_model->setHorizontalHeaderLabels({"    Subdomains"});
    passiveA_model->setHorizontalHeaderLabels({"    A Records"});
    passiveAAAA_model->setHorizontalHeaderLabels({"    AAAA Records"});
    passiveCidr_model->setHorizontalHeaderLabels({"    IP/CIDR"});
    passiveNS_model->setHorizontalHeaderLabels({"    NS Records"});
    passiveMX_model->setHorizontalHeaderLabels({"    MX Records"});
    passiveTXT_model->setHorizontalHeaderLabels({"    TXT Records"});
    passiveCNAME_model->setHorizontalHeaderLabels({"    CNAME Records"});
    passiveEmail_model->setHorizontalHeaderLabels({"    Emails"});
    passiveUrl_model->setHorizontalHeaderLabels({"    URLs"});
    passiveAsn_model->setHorizontalHeaderLabels({"    ASN", "    Name"});
    passiveSSL_model->setHorizontalHeaderLabels({"    SSL Certificates ID"});

    /* enum Results model */
    enumIp_model->setHorizontalHeaderLabels({"    IP Info"});
    enumDomain_model->setHorizontalHeaderLabels({"    Domain Info"});
    enumASN_model->setHorizontalHeaderLabels({"    ASN Info"});
    enumCIDR_model->setHorizontalHeaderLabels({"    CIDR Info"});
    enumNS_model->setHorizontalHeaderLabels({"    NS Info"});
    enumMX_model->setHorizontalHeaderLabels({"    MX Info"});
    enumSSL_model->setHorizontalHeaderLabels({"    SSL Certificate Info"});
    enumEmail_model->setHorizontalHeaderLabels({"    Email Info"});
    enumURL_model->setHorizontalHeaderLabels({"    URL Info"});
}
