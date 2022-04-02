#include "ProjectModel.h"

#include "src/dialogs/ProjectConfigDialog.h"
#include <QCryptographicHash>
#include <QTranslator>

/*
 * TODO:
 *      QJsonDocument size limit is 128mb. find a workaround or an alternative for serialization.
 */
ProjectModel::ProjectModel(QObject *parent): QObject (parent),
    explorer(new ExplorerModel),
    /* active Results Model */
    activeHost(new QStandardItemModel),
    activeWildcard(new QStandardItemModel),
    activeDNS(new QStandardItemModel),
    activeA(new QStandardItemModel),
    activeAAAA(new QStandardItemModel),
    activeNS(new QStandardItemModel),
    activeMX(new QStandardItemModel),
    activeTXT(new QStandardItemModel),
    activeCNAME(new QStandardItemModel),
    activeSRV(new QStandardItemModel),
    activeSSL(new QStandardItemModel),
    activeSSL_sha1(new QStandardItemModel),
    activeSSL_sha256(new QStandardItemModel),
    activeSSL_altNames(new QStandardItemModel),
    activeURL(new QStandardItemModel),

    /* passive results model */
    passiveSubdomainIp(new QStandardItemModel),
    passiveSubdomain(new QStandardItemModel),
    passiveA(new QStandardItemModel),
    passiveAAAA(new QStandardItemModel),
    passiveCIDR(new QStandardItemModel),
    passiveNS(new QStandardItemModel),
    passiveMX(new QStandardItemModel),
    passiveTXT(new QStandardItemModel),
    passiveCNAME(new QStandardItemModel),
    passiveEmail(new QStandardItemModel),
    passiveURL(new QStandardItemModel),
    passiveASN(new QStandardItemModel),
    passiveSSL(new QStandardItemModel),

    /* enum Results model */
    enumIP(new QStandardItemModel),
    enumASN(new QStandardItemModel),
    enumCIDR(new QStandardItemModel),
    enumNS(new QStandardItemModel),
    enumMX(new QStandardItemModel),
    enumSSL(new QStandardItemModel),
    enumEmail(new QStandardItemModel),

    /* raw */
    raw(new QStandardItemModel)
{
    this->setHeaderLabels();
}
ProjectModel::~ProjectModel(){
    delete raw;
    delete enumEmail;
    delete enumSSL;
    delete enumMX;
    delete enumNS;
    delete enumCIDR;
    delete enumASN;
    delete enumIP;
    delete passiveSSL;
    delete passiveASN;
    delete passiveURL;
    delete passiveEmail;
    delete passiveCNAME;
    delete passiveTXT;
    delete passiveMX;
    delete passiveNS;
    delete passiveCIDR;
    delete passiveAAAA;
    delete passiveA;
    delete passiveSubdomain;
    delete passiveSubdomainIp;
    delete activeURL;
    delete activeSSL_altNames;
    delete activeSSL_sha256;
    delete activeSSL_sha1;
    delete activeSSL;
    delete activeSRV;
    delete activeCNAME;
    delete activeTXT;
    delete activeMX;
    delete activeNS;
    delete activeAAAA;
    delete activeA;
    delete activeDNS;
    delete activeWildcard;
    delete activeHost;
    delete explorer;
}

void ProjectModel::setHeaderLabels(){
    activeHost->setHorizontalHeaderLabels({QObject::tr(" Host"), QObject::tr(" IpV4"), QObject::tr(" IpV6"), QObject::tr(" Ports")});
    activeWildcard->setHorizontalHeaderLabels({QObject::tr(" Wildcard"), QObject::tr(" IpV4"), QObject::tr(" IpV6")});
    activeDNS->setHorizontalHeaderLabels({QObject::tr(" DNS"), QObject::tr(" Records"), QObject::tr(" SRV Port")});
    activeA->setHorizontalHeaderLabels({QObject::tr(" A DNS Records")});
    activeAAAA->setHorizontalHeaderLabels({QObject::tr(" AAAA DNS Records")});
    activeNS->setHorizontalHeaderLabels({QObject::tr(" NS DNS Records")});
    activeMX->setHorizontalHeaderLabels({QObject::tr(" MX DNS Records")});
    activeTXT->setHorizontalHeaderLabels({QObject::tr(" TXT DNS Records")});
    activeCNAME->setHorizontalHeaderLabels({QObject::tr(" CNAME DNS Records")});
    activeSRV->setHorizontalHeaderLabels({QObject::tr(" Name"), QObject::tr(" Target"), QObject::tr(" Port")});
    activeSSL->setHorizontalHeaderLabels({QObject::tr(" SSL Certificates"), QObject::tr(" Values")});
    activeSSL_sha1->setHorizontalHeaderLabels({QObject::tr(" SSL Certificates SHA-1 Fingerprints")});
    activeSSL_sha256->setHorizontalHeaderLabels({QObject::tr(" SSL Certificates SHA-256 Fingerprints")});
    activeSSL_altNames->setHorizontalHeaderLabels({QObject::tr(" Alternative Names")});
    activeURL->setHorizontalHeaderLabels({QObject::tr(" URL"), QObject::tr(" Status"), QObject::tr(" Server"), QObject::tr(" Content Type")});

    /* passive results model */
    passiveSubdomainIp->setHorizontalHeaderLabels({QObject::tr(" Subdomains"), QObject::tr(" IpAddress")});
    passiveSubdomain->setHorizontalHeaderLabels({QObject::tr(" Subdomains")});
    passiveA->setHorizontalHeaderLabels({QObject::tr(" A Records")});
    passiveAAAA->setHorizontalHeaderLabels({QObject::tr(" AAAA Records")});
    passiveCIDR->setHorizontalHeaderLabels({QObject::tr(" IP/CIDR")});
    passiveNS->setHorizontalHeaderLabels({QObject::tr(" NS Records")});
    passiveMX->setHorizontalHeaderLabels({QObject::tr(" MX Records")});
    passiveTXT->setHorizontalHeaderLabels({QObject::tr(" TXT Records")});
    passiveCNAME->setHorizontalHeaderLabels({QObject::tr(" CNAME Records")});
    passiveEmail->setHorizontalHeaderLabels({QObject::tr(" Emails")});
    passiveURL->setHorizontalHeaderLabels({QObject::tr(" URLs")});
    passiveASN->setHorizontalHeaderLabels({QObject::tr(" ASN"), QObject::tr(" Name")});
    passiveSSL->setHorizontalHeaderLabels({QObject::tr(" SSL Certificates ID")});

    /* enum Results model */
    enumIP->setHorizontalHeaderLabels({QObject::tr(" IP"), QObject::tr(" Values")});
    enumASN->setHorizontalHeaderLabels({QObject::tr(" ASN"), QObject::tr(" Values")});
    enumCIDR->setHorizontalHeaderLabels({QObject::tr(" CIDR"), QObject::tr(" Values")});
    enumNS->setHorizontalHeaderLabels({QObject::tr(" NS"), QObject::tr(" Values")});
    enumMX->setHorizontalHeaderLabels({QObject::tr(" MX"), QObject::tr(" Values")});
    enumSSL->setHorizontalHeaderLabels({QObject::tr(" SSL Certificate"), QObject::tr(" Values")});
    enumEmail->setHorizontalHeaderLabels({QObject::tr(" Email"), QObject::tr(" Values")});

    /* raw */
    raw->setHorizontalHeaderLabels({QObject::tr(" Properties"), QObject::tr(" Values")});
}

void ProjectModel::clearModels(){
    activeHost->clear();
    activeWildcard->clear();
    activeDNS->clear();
    activeA->clear();
    activeAAAA->clear();
    activeNS->clear();
    activeMX->clear();
    activeTXT->clear();
    activeCNAME->clear();
    activeSRV->clear();
    activeSSL->clear();
    activeSSL_sha1->clear();
    activeSSL_sha256->clear();
    activeSSL_altNames->clear();
    activeURL->clear();
    passiveSubdomainIp->clear();
    passiveSubdomain->clear();
    passiveA->clear();
    passiveAAAA->clear();
    passiveCIDR->clear();
    passiveNS->clear();
    passiveMX->clear();
    passiveTXT->clear();
    passiveCNAME->clear();
    passiveEmail->clear();
    passiveURL->clear();
    passiveASN->clear();
    passiveSSL->clear();
    enumIP->clear();
    enumASN->clear();
    enumCIDR->clear();
    enumNS->clear();
    enumMX->clear();
    enumSSL->clear();
    enumEmail->clear();
    raw->clear();

    map_activeHost.clear();
    map_activeWildcard.clear();
    map_activeDNS.clear();
    map_activeSSL.clear();
    map_activeURL.clear();
    map_enumIP.clear();
    map_enumASN.clear();
    map_enumCIDR.clear();
    map_enumNS.clear();
    map_enumMX.clear();
    map_enumSSL.clear();
    map_enumEmail.clear();

    this->setHeaderLabels();
}

int ProjectModel::getItemsCount(){
    return  activeHost->rowCount()+
            activeWildcard->rowCount()+
            activeDNS->rowCount()+
            activeA->rowCount()+
            activeAAAA->rowCount()+
            activeNS->rowCount()+
            activeMX->rowCount()+
            activeTXT->rowCount()+
            activeCNAME->rowCount()+
            activeSRV->rowCount()+
            activeSSL->rowCount()+
            activeSSL_sha1->rowCount()+
            activeSSL_sha256->rowCount()+
            activeSSL_altNames->rowCount()+
            activeURL->rowCount()+
            passiveSubdomainIp->rowCount()+
            passiveSubdomain->rowCount()+
            passiveA->rowCount()+
            passiveAAAA->rowCount()+
            passiveCIDR->rowCount()+
            passiveNS->rowCount()+
            passiveMX->rowCount()+
            passiveTXT->rowCount()+
            passiveCNAME->rowCount()+
            passiveEmail->rowCount()+
            passiveURL->rowCount()+
            passiveASN->rowCount()+
            passiveSSL->rowCount()+
            enumIP->rowCount()+
            enumASN->rowCount()+
            enumCIDR->rowCount()+
            enumNS->rowCount()+
            enumMX->rowCount()+
            enumSSL->rowCount()+
            enumEmail->rowCount()+
            raw->rowCount();
}
