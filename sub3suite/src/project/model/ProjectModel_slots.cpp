#include "ProjectModel.h"

#include "src/items/SSLItem.h"


///
/// active slots...
///
void ProjectModel::addActiveHost(const s3s_struct::HOST &host){
    if(set_Host.contains(host.host))
    {
        s3s_item::HOST *item = set_Host.value(host.host);
        if(!host.ipv4.isEmpty())
            item->setValue_ipv4(host.ipv4);
        if(!host.ipv6.isEmpty())
            item->setValue_ipv6(host.ipv6);
        if(!host.ports.isEmpty())
            item->setValue_ports(host);
        return;
    }

    s3s_item::HOST *item = new s3s_item::HOST;
    item->setValues(host);
    activeHost->appendRow({item, item->ipv4, item->ipv6, item->ports});
    set_Host.insert(host.host, item);
}

void ProjectModel::addActiveWildcard(const s3s_struct::Wildcard &wildcard){
    s3s_item::Wildcard *item = new s3s_item::Wildcard;
    item->setValues(wildcard);
    activeWildcard->appendRow({item, item->ipv4, item->ipv6});
}

void ProjectModel::addActiveSSL(const QString &target, const QSslCertificate &ssl){
    s3s_item::SSL *item = new s3s_item::SSL;
    item->setValues(target, ssl);
    activeSSL->appendRow(item);
}

void ProjectModel::addActiveSSL(const QByteArray &cert){

}

void ProjectModel::addActiveSSL_hash(const QString &hash){
    if(hash.length() == 40)
        activeSSL_sha1->appendRow(new QStandardItem(hash));

    if(hash.length() == 64)
        activeSSL_sha256->appendRow(new QStandardItem(hash));
}

void ProjectModel::addActiveSSL_sha1(const QString &sha1){
    activeSSL_sha1->appendRow(new QStandardItem(sha1));
}

void ProjectModel::addActiveSSL_sha256(const QString &sha256){
    activeSSL_sha256->appendRow(new QStandardItem(sha256));
}

void ProjectModel::addActiveSSL_altNames(const QString &alternative_names){
    activeSSL_altNames->appendRow(new QStandardItem(alternative_names));
}

void ProjectModel::addActiveDNS(const s3s_struct::DNS &dns){
    s3s_item::DNS *item = new s3s_item::DNS;
    item->setValues(dns);
    activeDNS->appendRow(item);
}

void ProjectModel::addActiveURL(const s3s_struct::URL &url){
    s3s_item::URL *item = new s3s_item::URL;
    item->setValues(url);
    activeURL->appendRow({item, item->status_code, item->banner, item->content_type});
}

///
/// passive slots...
///
void ProjectModel::addPassiveSubdomainIp(const QString &subdomain, const QString &ip){
    passiveSubdomainIp->appendRow({new QStandardItem(subdomain), new QStandardItem(ip)});
}

void ProjectModel::addPassiveSubdomain(const QString &subdomain){
    passiveSubdomain->appendRow(new QStandardItem(subdomain));
}

void ProjectModel::addPassiveIp(const QString &ip){
    if(ip.contains(":"))
        passiveAAAA->appendRow(new QStandardItem(ip));
    else
        passiveA->appendRow(new QStandardItem(ip));
}

void ProjectModel::addPassiveA(const QString &ipv4){
    passiveA->appendRow(new QStandardItem(ipv4));
}

void ProjectModel::addPassiveAAAA(const QString &ipv6){
    passiveAAAA->appendRow(new QStandardItem(ipv6));
}

void ProjectModel::addPassiveCidr(const QString &cidr){
    passiveCidr->appendRow(new QStandardItem(cidr));
}

void ProjectModel::addPassiveNS(const QString &ns){
    passiveNS->appendRow(new QStandardItem(ns));
}

void ProjectModel::addPassiveMX(const QString &mx){
    passiveMX->appendRow(new QStandardItem(mx));
}

void ProjectModel::addPassiveTXT(const QString &txt){
    passiveTXT->appendRow(new QStandardItem(txt));
}

void ProjectModel::addPassiveCNAME(const QString &cname){
    passiveCNAME->appendRow(new QStandardItem(cname));
}

void ProjectModel::addPassiveEMail(const QString &email){
    passiveEmail->appendRow(new QStandardItem(email));
}

void ProjectModel::addPassiveUrl(const QString &url){
    passiveUrl->appendRow(new QStandardItem(url));
}

void ProjectModel::addPassiveAsn(const QString &asn, const QString &name){
    passiveAsn->appendRow({new QStandardItem(asn), new QStandardItem(name)});
}

void ProjectModel::addPassiveSSL(const QString &ssl){
    passiveSSL->appendRow(new QStandardItem(ssl));
}

///
/// enum slots...
///
void ProjectModel::addEnumASN(const s3s_struct::ASN &asn){
    s3s_item::ASN *item = new s3s_item::ASN;
    item->setValues(asn);
    enumASN->appendRow(item);
}

void ProjectModel::addEnumCIDR(const s3s_struct::CIDR &cidr){
    s3s_item::CIDR *item = new s3s_item::CIDR;
    item->setValues(cidr);
    enumCIDR->appendRow(item);
}

void ProjectModel::addEnumIP(const s3s_struct::IP &ip){
    s3s_item::IP *item = new s3s_item::IP;
    item->setValues(ip);
    enumIp->appendRow(item);
}

void ProjectModel::addEnumNS(const s3s_struct::NS &ns){
    s3s_item::NS *item = new s3s_item::NS;
    item->setValues(ns);
    enumNS->appendRow(item);
}

void ProjectModel::addEnumMX(const s3s_struct::MX &mx){
    s3s_item::MX *item = new s3s_item::MX;
    item->setValues(mx);
    enumMX->appendRow(item);
}

void ProjectModel::addEnumSSL(const QString &target, const QSslCertificate &ssl){
    s3s_item::SSL *item = new s3s_item::SSL;
    item->setValues(target, ssl);
    enumSSL->appendRow(item);
}

void ProjectModel::addEnumEmail(const s3s_struct::Email &email){
    s3s_item::Email *item = new s3s_item::Email;
    item->setValues(email);
    enumEmail->appendRow(item);
}

void ProjectModel::addRaw(const s3s_struct::RAW &raw_results){
    s3s_item::RAW *item = new s3s_item::RAW;
    item->setValues(raw_results);
    raw->appendRow(item);
}
