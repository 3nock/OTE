#ifndef RESULTSMODELS_H
#define RESULTSMODELS_H

#include <QStandardItemModel>
#include <QSortFilterProxyModel>


namespace Model {

class Osint{
    public:
        Osint()
            : subdomainIp(new QStandardItemModel),
              subdomain(new QStandardItemModel),
              ip(new QStandardItemModel),
              email(new QStandardItemModel),
              url(new QStandardItemModel),
              asn(new QStandardItemModel),
              sslCert(new QStandardItemModel),
              cidr(new QStandardItemModel),
              subdomainIpProxy(new QSortFilterProxyModel),
              subdomainProxy(new QSortFilterProxyModel),
              ipProxy(new QSortFilterProxyModel),
              emailProxy(new QSortFilterProxyModel),
              urlProxy(new QSortFilterProxyModel),
              asnProxy(new QSortFilterProxyModel),
              sslCertProxy(new QSortFilterProxyModel),
              cidrProxy(new QSortFilterProxyModel)
        {
            subdomainIpProxy->setSourceModel(subdomainIp);
            subdomainIpProxy->setFilterCaseSensitivity(Qt::CaseInsensitive);
            subdomainIpProxy->setRecursiveFilteringEnabled(true);
            subdomainIpProxy->setFilterKeyColumn(0);
            subdomainProxy->setSourceModel(subdomain);
            subdomainProxy->setFilterCaseSensitivity(Qt::CaseInsensitive);
            subdomainProxy->setRecursiveFilteringEnabled(true);
            subdomainProxy->setFilterKeyColumn(0);
            ipProxy->setSourceModel(ip);
            ipProxy->setFilterCaseSensitivity(Qt::CaseInsensitive);
            ipProxy->setRecursiveFilteringEnabled(true);
            ipProxy->setFilterKeyColumn(0);
            emailProxy->setSourceModel(email);
            emailProxy->setFilterCaseSensitivity(Qt::CaseInsensitive);
            emailProxy->setRecursiveFilteringEnabled(true);
            emailProxy->setFilterKeyColumn(0);
            urlProxy->setSourceModel(url);
            urlProxy->setFilterCaseSensitivity(Qt::CaseInsensitive);
            urlProxy->setRecursiveFilteringEnabled(true);
            urlProxy->setFilterKeyColumn(0);
            asnProxy->setSourceModel(asn);
            asnProxy->setFilterCaseSensitivity(Qt::CaseInsensitive);
            asnProxy->setRecursiveFilteringEnabled(true);
            asnProxy->setFilterKeyColumn(0);
            sslCertProxy->setSourceModel(sslCert);
            sslCertProxy->setFilterCaseSensitivity(Qt::CaseInsensitive);
            sslCertProxy->setRecursiveFilteringEnabled(true);
            sslCertProxy->setFilterKeyColumn(0);
            cidrProxy->setSourceModel(cidr);
            cidrProxy->setFilterCaseSensitivity(Qt::CaseInsensitive);
            cidrProxy->setRecursiveFilteringEnabled(true);
            cidrProxy->setFilterKeyColumn(0);
        }
        ~Osint(){
            delete subdomainIp;
            delete subdomain;
            delete ip;
            delete email;
            delete url;
            delete asn;
            delete sslCert;
            delete cidr;
            delete subdomainIpProxy;
            delete subdomainProxy;
            delete ipProxy;
            delete emailProxy;
            delete urlProxy;
            delete asnProxy;
            delete sslCertProxy;
            delete cidrProxy;
        }
        QStandardItemModel *subdomainIp;
        QStandardItemModel *subdomain;
        QStandardItemModel *ip;
        QStandardItemModel *email;
        QStandardItemModel *url;
        QStandardItemModel *asn;
        QStandardItemModel *sslCert;
        QStandardItemModel *cidr;
        QSortFilterProxyModel *subdomainIpProxy;
        QSortFilterProxyModel *subdomainProxy;
        QSortFilterProxyModel *ipProxy;
        QSortFilterProxyModel *emailProxy;
        QSortFilterProxyModel *urlProxy;
        QSortFilterProxyModel *asnProxy;
        QSortFilterProxyModel *sslCertProxy;
        QSortFilterProxyModel *cidrProxy;
};

class Ip{
    public:
        Ip()
            : subdomainIp(new QStandardItemModel),
              subdomainIpProxy(new QSortFilterProxyModel)
        {
            subdomainIpProxy->setSourceModel(subdomainIp);
            subdomainIpProxy->setFilterCaseSensitivity(Qt::CaseInsensitive);
            subdomainIpProxy->setRecursiveFilteringEnabled(true);
            subdomainIpProxy->setFilterKeyColumn(0);
        }
        ~Ip(){
            delete subdomainIp;
            delete subdomainIpProxy;
        }
        QStandardItemModel *subdomainIp;
        QSortFilterProxyModel *subdomainIpProxy;
};

class Records{
    public:
        Records()
            : dns(new QStandardItemModel),
              srv(new QStandardItemModel),
              dnsProxy(new QSortFilterProxyModel),
              srvProxy(new QSortFilterProxyModel)
        {
            dnsProxy->setSourceModel(dns);
            dnsProxy->setFilterCaseSensitivity(Qt::CaseInsensitive);
            dnsProxy->setRecursiveFilteringEnabled(true);
            dnsProxy->setFilterKeyColumn(0);
            srvProxy->setSourceModel(srv);
            srvProxy->setFilterCaseSensitivity(Qt::CaseInsensitive);
            srvProxy->setRecursiveFilteringEnabled(true);
            dnsProxy->setFilterKeyColumn(0);
        }
        ~Records(){
            delete dns;
            delete srv;
            delete dnsProxy;
            delete srvProxy;
        }
        QStandardItemModel *dns;
        QStandardItemModel *srv;
        QSortFilterProxyModel *dnsProxy;
        QSortFilterProxyModel *srvProxy;
};

class Brute{
    public:
        Brute()
            : subdomainIp(new QStandardItemModel),
              tld(new QStandardItemModel),
              subdomainIpProxy(new QSortFilterProxyModel),
              tldProxy(new QSortFilterProxyModel)
        {
            subdomainIpProxy->setSourceModel(subdomainIp);
            subdomainIpProxy->setFilterCaseSensitivity(Qt::CaseInsensitive);
            subdomainIpProxy->setRecursiveFilteringEnabled(true);
            subdomainIpProxy->setFilterKeyColumn(0);
            tldProxy->setSourceModel(tld);
            tldProxy->setFilterCaseSensitivity(Qt::CaseInsensitive);
            tldProxy->setRecursiveFilteringEnabled(true);
            tldProxy->setFilterKeyColumn(0);
        }
        ~Brute(){
            delete subdomainIp;
            delete tld;
            delete subdomainIpProxy;
            delete tldProxy;
        }
        QStandardItemModel *subdomainIp;
        QStandardItemModel *tld;
        QSortFilterProxyModel *subdomainIpProxy;
        QSortFilterProxyModel *tldProxy;
};

class Active{
    public:
        Active()
            : subdomainIp(new QStandardItemModel),
              subdomainIpProxy(new QSortFilterProxyModel)
        {
            subdomainIpProxy->setSourceModel(subdomainIp);
            subdomainIpProxy->setFilterCaseSensitivity(Qt::CaseInsensitive);
            subdomainIpProxy->setRecursiveFilteringEnabled(true);
            subdomainIpProxy->setFilterKeyColumn(0);
        }
        ~Active(){
            delete subdomainIp;
            delete subdomainIpProxy;
        }
        QStandardItemModel *subdomainIp;
        QSortFilterProxyModel *subdomainIpProxy;
};

class Cert{
    public:
        Cert()
            : subdomain(new QStandardItemModel),
              sslCert(new QStandardItemModel),
              certInfo(new QStandardItemModel),
              subdomainProxy(new QSortFilterProxyModel),
              sslCertProxy(new QSortFilterProxyModel),
              certInfoProxy(new QSortFilterProxyModel)
        {
            subdomainProxy->setSourceModel(subdomain);
            subdomainProxy->setFilterCaseSensitivity(Qt::CaseInsensitive);
            subdomainProxy->setRecursiveFilteringEnabled(true);
            subdomainProxy->setFilterKeyColumn(0);
            sslCertProxy->setSourceModel(sslCert);
            sslCertProxy->setFilterCaseSensitivity(Qt::CaseInsensitive);
            sslCertProxy->setRecursiveFilteringEnabled(true);
            sslCertProxy->setFilterKeyColumn(0);
            certInfoProxy->setSourceModel(certInfo);
            certInfoProxy->setFilterCaseSensitivity(Qt::CaseInsensitive);
            certInfoProxy->setRecursiveFilteringEnabled(true);
            certInfoProxy->setFilterKeyColumn(0);
        }
        ~Cert(){
            delete subdomain;
            delete sslCert;
            delete certInfo;
            delete subdomainProxy;
            delete sslCertProxy;
            delete certInfoProxy;
        }
        QStandardItemModel *subdomain;
        QStandardItemModel *sslCert;
        QStandardItemModel *certInfo;
        QSortFilterProxyModel *subdomainProxy;
        QSortFilterProxyModel *sslCertProxy;
        QSortFilterProxyModel *certInfoProxy;
};
}

class ResultsModel{

    public:
        ResultsModel()
            : ip(new Model::Ip),
              osint(new Model::Osint),
              brute(new Model::Brute),
              active(new Model::Active),
              records(new Model::Records),
              cert(new Model::Cert)
        {
        }
        ~ResultsModel(){
            delete ip;
            delete osint;
            delete brute;
            delete active;
            delete records;
            delete cert;
        }
        Model::Ip *ip;
        Model::Osint *osint;
        Model::Brute *brute;
        Model::Active *active;
        Model::Records *records;
        Model::Cert *cert;
};

#endif // RESULTSMODELS_H
