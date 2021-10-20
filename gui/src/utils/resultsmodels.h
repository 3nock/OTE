#ifndef RESULTSMODELS_H
#define RESULTSMODELS_H

#include <QStandardItemModel>
#include <QSortFilterProxyModel>


/*
 * The Results Models of subdomainIp Engines...
 */
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
              subdomainIpProxy(new QSortFilterProxyModel),
              subdomainProxy(new QSortFilterProxyModel),
              ipProxy(new QSortFilterProxyModel),
              emailProxy(new QSortFilterProxyModel),
              urlProxy(new QSortFilterProxyModel),
              asnProxy(new QSortFilterProxyModel)
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
        }
        ~Osint(){
            delete subdomainIp;
            delete subdomain;
            delete ip;
            delete email;
            delete url;
            delete asn;
            delete subdomainIpProxy;
            delete subdomainProxy;
            delete ipProxy;
            delete emailProxy;
            delete urlProxy;
            delete asnProxy;
        }
        ///
        /// the models...
        ///
        QStandardItemModel *subdomainIp;
        QStandardItemModel *subdomain;
        QStandardItemModel *ip;
        QStandardItemModel *email;
        QStandardItemModel *url;
        QStandardItemModel *asn;
        QSortFilterProxyModel *subdomainIpProxy;
        QSortFilterProxyModel *subdomainProxy;
        QSortFilterProxyModel *ipProxy;
        QSortFilterProxyModel *emailProxy;
        QSortFilterProxyModel *urlProxy;
        QSortFilterProxyModel *asnProxy;
};

class Ip{
    public:
        Ip()
            : subdomainIp(new QStandardItemModel),
              asn(new QStandardItemModel),
              subdomainIpProxy(new QSortFilterProxyModel),
              asnProxy(new QSortFilterProxyModel)
        {
            subdomainIpProxy->setSourceModel(subdomainIp);
            subdomainIpProxy->setFilterCaseSensitivity(Qt::CaseInsensitive);
            subdomainIpProxy->setRecursiveFilteringEnabled(true);
            subdomainIpProxy->setFilterKeyColumn(0);
            asnProxy->setSourceModel(asn);
            asnProxy->setFilterCaseSensitivity(Qt::CaseInsensitive);
            asnProxy->setRecursiveFilteringEnabled(true);
            asnProxy->setFilterKeyColumn(0);
        }
        ~Ip(){
            delete subdomainIp;
            delete asn;
            delete subdomainIpProxy;
            delete asnProxy;
        }
        ///
        /// the models...
        ///
        QStandardItemModel *subdomainIp;
        QStandardItemModel *asn;
        QSortFilterProxyModel *subdomainIpProxy;
        QSortFilterProxyModel *asnProxy;
};

class Records{
    public:
        Records()
            : dns(new QStandardItemModel),
              srv(new QStandardItemModel),
              //...
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
        ///
        /// the models...
        ///
        QStandardItemModel *dns;
        QStandardItemModel *srv;
        QSortFilterProxyModel *dnsProxy;
        QSortFilterProxyModel *srvProxy;
};

class Brute{
    public:
        Brute()
            : subdomainIp(new QStandardItemModel),
              subdomainIpProxy(new QSortFilterProxyModel)
        {
            subdomainIpProxy->setSourceModel(subdomainIp);
            subdomainIpProxy->setFilterCaseSensitivity(Qt::CaseInsensitive);
            subdomainIpProxy->setRecursiveFilteringEnabled(true);
            subdomainIpProxy->setFilterKeyColumn(0);
        }
        ~Brute(){
            delete subdomainIp;
            delete subdomainIpProxy;
        }
        ///
        /// the models...
        ///
        QStandardItemModel *subdomainIp;
        QSortFilterProxyModel *subdomainIpProxy;
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
        ///
        /// the models...
        ///
        QStandardItemModel *subdomainIp;
        QSortFilterProxyModel *subdomainIpProxy;
};
}

class ResultsModel{

    public:
        ResultsModel()
            : ip(new Model::Ip),
              osint(new Model::Osint),
              brute(new Model::Brute),
              active(new Model::Active),
              records(new Model::Records)
        {
        }
        ~ResultsModel(){
            delete ip;
            delete osint;
            delete brute;
            delete active;
            delete records;
        }
        ///
        /// the results Models...
        ///
        Model::Ip *ip;
        Model::Osint *osint;
        Model::Brute *brute;
        Model::Active *active;
        Model::Records *records;
};

enum PROXYMODEL_TYPE{
    subdomainIpProxy,
    subdomainProxy,
    ipProxy,
    emailProxy,
    urlProxy,
    asnProxy,
    dnsProxy,
    srvProxy
};

#endif // RESULTSMODELS_H
