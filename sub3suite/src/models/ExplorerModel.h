#ifndef EXPLORERMODEL_H
#define EXPLORERMODEL_H

#include <QStandardItemModel>


enum ExplorerType {
    activeHost,
    activeWildcard,
    activeDNS,
    activeDNS_A,
    activeDNS_AAAA,
    activeDNS_NS,
    activeDNS_MX,
    activeDNS_TXT,
    activeDNS_CNAME,
    activeDNS_SRV,
    activeSSL,
    activeSSL_sha1,
    activeSSL_sha256,
    activeSSL_altNames,
    activeURL,
    passive_subdomainIp,
    passive_subdomain,
    passive_A,
    passive_AAAA,
    passive_CIDR,
    passive_NS,
    passive_MX,
    passive_TXT,
    passive_CNAME,
    passive_Email,
    passive_URL,
    passive_ASN,
    passive_SSL,
    enum_IP,
    enum_CIDR,
    enum_ASN,
    enum_NS,
    enum_MX,
    enum_SSL,
    enum_Email,
    enum_URL,
    custom,
    raw
};

class ExplorerModel: public QStandardItemModel {
    public:
        ExplorerModel();
        ~ExplorerModel();

    public:
        QStandardItem *project;

        QStandardItem *active;
        QStandardItem *passive;
        QStandardItem *enums;
        QStandardItem *custom;

        /* active Results Explorer */
        QStandardItem *activeHost;
        QStandardItem *activeWildcard;
        QStandardItem *activeDNS;
        QStandardItem *activeA;
        QStandardItem *activeAAAA;
        QStandardItem *activeNS;
        QStandardItem *activeMX;
        QStandardItem *activeTXT;
        QStandardItem *activeCNAME;
        QStandardItem *activeSRV;
        QStandardItem *activeSSL;
        QStandardItem *activeSSL_sha1;
        QStandardItem *activeSSL_sha256;
        QStandardItem *activeSSL_altNames;
        QStandardItem *activeURL;

        /* passive Results explorer */
        QStandardItem *passiveSubdomainIp;
        QStandardItem *passiveSubdomain;
        QStandardItem *passiveA;
        QStandardItem *passiveAAAA;
        QStandardItem *passiveNS;
        QStandardItem *passiveMX;
        QStandardItem *passiveTXT;
        QStandardItem *passiveCNAME;
        QStandardItem *passiveCidr;
        QStandardItem *passiveEmail;
        QStandardItem *passiveUrl;
        QStandardItem *passiveAsn;
        QStandardItem *passiveSSL;

        /* enum Results explorer */
        QStandardItem *enumIp;
        QStandardItem *enumASN;
        QStandardItem *enumCIDR;
        QStandardItem *enumNS;
        QStandardItem *enumMX;
        QStandardItem *enumSSL;
        QStandardItem *enumEmail;

        QStandardItem *raw;
};

#endif // EXPLORERMODEL_H
