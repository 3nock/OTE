#include "ProjectModel.h"

#include "src/models/SSLModel.h"


///
/// active slots...
///
void ProjectModel::addActiveSubdomainIp(const QString &subdomain, const QString &ip){
    m_activeSubdomainIp_rootItem->appendRow({new QStandardItem(subdomain), new QStandardItem(ip)});
}

void ProjectModel::addActiveSubdomain(const QString &subdomain){
    m_activeSubdomain_rootItem->appendRow(new QStandardItem(subdomain));

}

void ProjectModel::addActiveTLD(const QString &subdomain, const QString &ip){
    m_activeTld_rootItem->appendRow({new QStandardItem(subdomain), new QStandardItem(ip)});
}

void ProjectModel::addActiveWildcard(const QString &subdomain, const QString &ip){
    m_activeWildcard_rootItem->appendRow({new QStandardItem(subdomain), new QStandardItem(ip)});
}

void ProjectModel::addActiveSSL(const QString &target, const QSslCertificate &ssl){
    s3s_item::SSL *item = new s3s_item::SSL;
    item->setValues(target, ssl);
    m_activeSSL_rootItem->appendRow(item);
}

void ProjectModel::addActiveSSL_sha1(const QString &sha1){
    m_activeSSL_sha1_rootItem->appendRow(new QStandardItem(sha1));
}

void ProjectModel::addActiveSSL_sha256(const QString &sha256){
    m_activeSSL_sha256_rootItem->appendRow(new QStandardItem(sha256));
}

void ProjectModel::addActiveDNS(const s3s_struct::DNS &dns){
    s3s_item::DNS *item = new s3s_item::DNS;
    item->setValues(dns);
    m_activeDNS_rootItem->appendRow(item);
}

void ProjectModel::addActiveURL(const s3s_struct::URL &url){
    s3s_item::URL *item = new s3s_item::URL;
    item->setValues(url);
    m_activeURL_rootItem->appendRow({item, item->status_code, item->banner, item->content_type});
}

///
/// passive slots...
///
void ProjectModel::addPassiveSubdomainIp(const QString &subdomain, const QString &ip){
    m_passiveSubdomainIp_rootItem->appendRow({new QStandardItem(subdomain), new QStandardItem(ip)});
}

void ProjectModel::addPassiveSubdomain(const QString &subdomain){
    m_passiveSubdomain_rootItem->appendRow(new QStandardItem(subdomain));
}

void ProjectModel::addPassiveIp(const QString &ip){
    if(ip.contains(":"))
        m_passiveAAAA_rootItem->appendRow(new QStandardItem(ip));
    else
        m_passiveA_rootItem->appendRow(new QStandardItem(ip));
}

void ProjectModel::addPassiveA(const QString &ipv4){
    m_passiveA_rootItem->appendRow(new QStandardItem(ipv4));
}

void ProjectModel::addPassiveAAAA(const QString &ipv6){
    m_passiveAAAA_rootItem->appendRow(new QStandardItem(ipv6));
}

void ProjectModel::addPassiveCidr(const QString &cidr){
    m_passiveCidr_rootItem->appendRow(new QStandardItem(cidr));
}

void ProjectModel::addPassiveNS(const QString &ns){
    m_passiveNS_rootItem->appendRow(new QStandardItem(ns));
}

void ProjectModel::addPassiveMX(const QString &mx){
    m_passiveMX_rootItem->appendRow(new QStandardItem(mx));
}

void ProjectModel::addPassiveTXT(const QString &txt){
    m_passiveTXT_rootItem->appendRow(new QStandardItem(txt));
}

void ProjectModel::addPassiveCNAME(const QString &cname){
    m_passiveCNAME_rootItem->appendRow(new QStandardItem(cname));
}

void ProjectModel::addPassiveEMail(const QString &email){
    m_passiveEmail_rootItem->appendRow(new QStandardItem(email));
}

void ProjectModel::addPassiveUrl(const QString &url){
    m_passiveUrl_rootItem->appendRow(new QStandardItem(url));
}

void ProjectModel::addPassiveAsn(const QString &asn, const QString &name){
    m_passiveAsn_rootItem->appendRow({new QStandardItem(asn), new QStandardItem(name)});
}

void ProjectModel::addPassiveSSL(const QString &ssl){
    m_passiveSSL_rootItem->appendRow(new QStandardItem(ssl));
}

///
/// enum slots...
///
void ProjectModel::addEnumASN(const s3s_struct::ASN &asn){
    s3s_item::ASN *item = new s3s_item::ASN;
    item->setValues(asn);
    m_enumASN_rootItem->appendRow(item);
}

void ProjectModel::addEnumCIDR(const s3s_struct::CIDR &cidr){
    s3s_item::CIDR *item = new s3s_item::CIDR;
    item->setValues(cidr);
    m_enumCIDR_rootItem->appendRow(item);
}

void ProjectModel::addEnumIP(const s3s_struct::IP &ip){
    s3s_item::IP *item = new s3s_item::IP;
    item->setValues(ip);
    m_enumIp_rootItem->appendRow(item);
}

void ProjectModel::addEnumNS(const s3s_struct::NS &ns){
    s3s_item::NS *item = new s3s_item::NS;
    item->setValues(ns);
    m_enumNS_rootItem->appendRow(item);
}

void ProjectModel::addEnumMX(const s3s_struct::MX &mx){
    s3s_item::MX *item = new s3s_item::MX;
    item->setValues(mx);
    m_enumMX_rootItem->appendRow(item);
}

void ProjectModel::addEnumSSL(const QString &target, const QSslCertificate &ssl){
    s3s_item::SSL *item = new s3s_item::SSL;
    item->setValues(target, ssl);
    m_enumSSL_rootItem->appendRow(item);
}

void ProjectModel::addEnumEmail(const s3s_struct::Email &email){
    s3s_item::Email *item = new s3s_item::Email;
    item->setValues(email);
    m_enumEmail_rootItem->appendRow(item);
}
