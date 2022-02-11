#include "ProjectModel.h"

#include "src/dialogs/ProjectConfigDialog.h"
#include <QCryptographicHash>

/*
 * TODO:
 *      QJsonDocument size limit is 128mb. find a workaround or an alternative for serialization.
 */
ProjectModel::ProjectModel():
    model_explorer(new QStandardItemModel),
    project_explorer(new QStandardItem),
    active_explorer(new QStandardItem("Active")),
    passive_explorer(new QStandardItem("Passive")),
    enums_explorer(new QStandardItem("Enum")),
    custom_explorer(new QStandardItem("Custom")),

    /* active Results Explorer */
    activeSubdomainIp_explorer(new QStandardItem("Subdomain Ip")),
    activeSubdomain_explorer(new QStandardItem("Subdomain")),
    activeTld_explorer(new QStandardItem("TLD")),
    activeWildcard_explorer(new QStandardItem("Wildcard")),
    activeDNS_explorer(new QStandardItem("DNS")),
    activeA_explorer(new QStandardItem("A")),
    activeAAAA_explorer(new QStandardItem("AAAA")),
    activeNS_explorer(new QStandardItem("NS")),
    activeMX_explorer(new QStandardItem("MX")),
    activeTXT_explorer(new QStandardItem("TXT")),
    activeCNAME_explorer(new QStandardItem("CNAME")),
    activeSRV_explorer(new QStandardItem("SRV")),
    activeSSL_explorer(new QStandardItem("SSL")),
    activeSSL_sha1_explorer(new QStandardItem("SHA-1")),
    activeSSL_sha256_explorer(new QStandardItem("SHA-256")),
    activeURL_explorer(new QStandardItem("URL")),

    /* passive Results explorer */
    passiveSubdomainIp_explorer(new QStandardItem("Subdomain Ip")),
    passiveSubdomain_explorer(new QStandardItem("Subdomain")),
    passiveA_explorer(new QStandardItem("A")),
    passiveAAAA_explorer(new QStandardItem("AAAA")),
    passiveNS_explorer(new QStandardItem("NS")),
    passiveMX_explorer(new QStandardItem("MX")),
    passiveTXT_explorer(new QStandardItem("TXT")),
    passiveCNAME_explorer(new QStandardItem("CNAME")),
    passiveCidr_explorer(new QStandardItem("CIDR")),
    passiveEmail_explorer(new QStandardItem("EMAIL")),
    passiveUrl_explorer(new QStandardItem("URL")),
    passiveAsn_explorer(new QStandardItem("ASN")),
    passiveSSL_explorer(new QStandardItem("SSL")),

    /* enum Results explorer */
    enumIp_explorer(new QStandardItem("IP")),
    enumASN_explorer(new QStandardItem("ASN")),
    enumCIDR_explorer(new QStandardItem("CIDR")),
    enumNS_explorer(new QStandardItem("NS")),
    enumMX_explorer(new QStandardItem("MX")),
    enumSSL_explorer(new QStandardItem("SSL")),
    enumEmail_explorer(new QStandardItem("Email")),

    /* active Results Model */
    activeSubdomainIp_model(new QStandardItemModel),
    activeSubdomain_model(new QStandardItemModel),
    activeTld_model(new QStandardItemModel),
    activeWildcard_model(new QStandardItemModel),
    activeDNS_model(new QStandardItemModel),
    activeA_model(new QStandardItemModel),
    activeAAAA_model(new QStandardItemModel),
    activeNS_model(new QStandardItemModel),
    activeMX_model(new QStandardItemModel),
    activeTXT_model(new QStandardItemModel),
    activeCNAME_model(new QStandardItemModel),
    activeSRV_model(new QStandardItemModel),
    activeSSL_model(new QStandardItemModel),
    activeSSL_sha1_model(new QStandardItemModel),
    activeSSL_sha256_model(new QStandardItemModel),
    activeURL_model(new QStandardItemModel),

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
    enumASN_model(new QStandardItemModel),
    enumCIDR_model(new QStandardItemModel),
    enumNS_model(new QStandardItemModel),
    enumMX_model(new QStandardItemModel),
    enumSSL_model(new QStandardItemModel),
    enumEmail_model(new QStandardItemModel)
{
    this->setHeaderLabels();
    this->getRootItems();

    ///
    /// setting icons
    ///
    QFont font("Segoe UI", 9, QFont::Bold);
    project_explorer->setFont(font);
    project_explorer->setForeground(Qt::white);
    project_explorer->setIcon(QIcon(":/img/res/icons/setting.png"));
    active_explorer->setFont(font);
    active_explorer->setForeground(Qt::white);
    active_explorer->setIcon(QIcon(":/img/res/icons/folder.png"));
    passive_explorer->setFont(font);
    passive_explorer->setForeground(Qt::white);
    passive_explorer->setIcon(QIcon(":/img/res/icons/folder.png"));
    enums_explorer->setFont(font);
    enums_explorer->setForeground(Qt::white);
    enums_explorer->setIcon(QIcon(":/img/res/icons/folder.png"));
    custom_explorer->setFont(font);
    custom_explorer->setForeground(Qt::white);
    custom_explorer->setIcon(QIcon(":/img/res/icons/folder.png"));

    activeSubdomainIp_explorer->setIcon(QIcon(":/img/res/icons/folder2.png"));
    activeSubdomain_explorer->setIcon(QIcon(":/img/res/icons/folder2.png"));
    activeTld_explorer->setIcon(QIcon(":/img/res/icons/folder2.png"));
    activeWildcard_explorer->setIcon(QIcon(":/img/res/icons/folder2.png"));
    activeDNS_explorer->setIcon(QIcon(":/img/res/icons/folder2.png"));
    activeA_explorer->setIcon(QIcon(":/img/res/icons/folder2.png"));
    activeAAAA_explorer->setIcon(QIcon(":/img/res/icons/folder2.png"));
    activeNS_explorer->setIcon(QIcon(":/img/res/icons/folder2.png"));
    activeMX_explorer->setIcon(QIcon(":/img/res/icons/folder2.png"));
    activeTXT_explorer->setIcon(QIcon(":/img/res/icons/folder2.png"));
    activeCNAME_explorer->setIcon(QIcon(":/img/res/icons/folder2.png"));
    activeSRV_explorer->setIcon(QIcon(":/img/res/icons/folder2.png"));
    activeSSL_explorer->setIcon(QIcon(":/img/res/icons/folder2.png"));
    activeSSL_sha1_explorer->setIcon(QIcon(":/img/res/icons/folder2.png"));
    activeSSL_sha256_explorer->setIcon(QIcon(":/img/res/icons/folder2.png"));
    activeURL_explorer->setIcon(QIcon(":/img/res/icons/folder2.png"));
    passiveSubdomainIp_explorer->setIcon(QIcon(":/img/res/icons/folder2.png"));
    passiveSubdomain_explorer->setIcon(QIcon(":/img/res/icons/folder2.png"));
    passiveA_explorer->setIcon(QIcon(":/img/res/icons/folder2.png"));
    passiveAAAA_explorer->setIcon(QIcon(":/img/res/icons/folder2.png"));
    passiveCidr_explorer->setIcon(QIcon(":/img/res/icons/folder2.png"));
    passiveNS_explorer->setIcon(QIcon(":/img/res/icons/folder2.png"));
    passiveMX_explorer->setIcon(QIcon(":/img/res/icons/folder2.png"));
    passiveTXT_explorer->setIcon(QIcon(":/img/res/icons/folder2.png"));
    passiveCNAME_explorer->setIcon(QIcon(":/img/res/icons/folder2.png"));
    passiveEmail_explorer->setIcon(QIcon(":/img/res/icons/folder2.png"));
    passiveUrl_explorer->setIcon(QIcon(":/img/res/icons/folder2.png"));
    passiveAsn_explorer->setIcon(QIcon(":/img/res/icons/folder2.png"));
    passiveSSL_explorer->setIcon(QIcon(":/img/res/icons/folder2.png"));
    enumIp_explorer->setIcon(QIcon(":/img/res/icons/folder2.png"));
    enumASN_explorer->setIcon(QIcon(":/img/res/icons/folder2.png"));
    enumCIDR_explorer->setIcon(QIcon(":/img/res/icons/folder2.png"));
    enumNS_explorer->setIcon(QIcon(":/img/res/icons/folder2.png"));
    enumMX_explorer->setIcon(QIcon(":/img/res/icons/folder2.png"));
    enumSSL_explorer->setIcon(QIcon(":/img/res/icons/folder2.png"));
    enumEmail_explorer->setIcon(QIcon(":/img/res/icons/folder2.png"));

    ///
    /// setting foreground
    ///
    activeSubdomainIp_explorer->setForeground(Qt::white);
    activeSubdomain_explorer->setForeground(Qt::white);
    activeTld_explorer->setForeground(Qt::white);
    activeWildcard_explorer->setForeground(Qt::white);
    activeDNS_explorer->setForeground(Qt::white);
    activeA_explorer->setForeground(Qt::white);
    activeAAAA_explorer->setForeground(Qt::white);
    activeNS_explorer->setForeground(Qt::white);
    activeMX_explorer->setForeground(Qt::white);
    activeTXT_explorer->setForeground(Qt::white);
    activeCNAME_explorer->setForeground(Qt::white);
    activeSRV_explorer->setForeground(Qt::white);
    activeSSL_explorer->setForeground(Qt::white);
    activeSSL_sha1_explorer->setForeground(Qt::white);
    activeSSL_sha256_explorer->setForeground(Qt::white);
    activeURL_explorer->setForeground(Qt::white);
    passiveSubdomain_explorer->setForeground(Qt::white);
    passiveSubdomainIp_explorer->setForeground(Qt::white);
    passiveA_explorer->setForeground(Qt::white);
    passiveAAAA_explorer->setForeground(Qt::white);
    passiveCidr_explorer->setForeground(Qt::white);
    passiveNS_explorer->setForeground(Qt::white);
    passiveMX_explorer->setForeground(Qt::white);
    passiveTXT_explorer->setForeground(Qt::white);
    passiveCNAME_explorer->setForeground(Qt::white);
    passiveEmail_explorer->setForeground(Qt::white);
    passiveUrl_explorer->setForeground(Qt::white);
    passiveAsn_explorer->setForeground(Qt::white);
    passiveSSL_explorer->setForeground(Qt::white);
    enumIp_explorer->setForeground(Qt::white);
    enumASN_explorer->setForeground(Qt::white);
    enumCIDR_explorer->setForeground(Qt::white);
    enumNS_explorer->setForeground(Qt::white);
    enumMX_explorer->setForeground(Qt::white);
    enumSSL_explorer->setForeground(Qt::white);
    enumEmail_explorer->setForeground(Qt::white);

    ///
    /// appending items...
    ///
    activeDNS_explorer->appendRow(activeA_explorer);
    activeDNS_explorer->appendRow(activeAAAA_explorer);
    activeDNS_explorer->appendRow(activeNS_explorer);
    activeDNS_explorer->appendRow(activeMX_explorer);
    activeDNS_explorer->appendRow(activeTXT_explorer);
    activeDNS_explorer->appendRow(activeCNAME_explorer);
    activeDNS_explorer->appendRow(activeSRV_explorer);
    activeSSL_explorer->appendRow(activeSSL_sha1_explorer);
    activeSSL_explorer->appendRow(activeSSL_sha256_explorer);
    active_explorer->appendRow(activeSubdomainIp_explorer);
    active_explorer->appendRow(activeSubdomain_explorer);
    active_explorer->appendRow(activeTld_explorer);
    active_explorer->appendRow(activeWildcard_explorer);
    active_explorer->appendRow(activeDNS_explorer);
    active_explorer->appendRow(activeSSL_explorer);
    active_explorer->appendRow(activeURL_explorer);
    passive_explorer->appendRow(passiveSubdomainIp_explorer);
    passive_explorer->appendRow(passiveSubdomain_explorer);
    passive_explorer->appendRow(passiveA_explorer);
    passive_explorer->appendRow(passiveAAAA_explorer);
    passive_explorer->appendRow(passiveCidr_explorer);
    passive_explorer->appendRow(passiveNS_explorer);
    passive_explorer->appendRow(passiveMX_explorer);
    passive_explorer->appendRow(passiveTXT_explorer);
    passive_explorer->appendRow(passiveCNAME_explorer);
    passive_explorer->appendRow(passiveEmail_explorer);
    passive_explorer->appendRow(passiveUrl_explorer);
    passive_explorer->appendRow(passiveAsn_explorer);
    passive_explorer->appendRow(passiveSSL_explorer);
    enums_explorer->appendRow(enumIp_explorer);
    enums_explorer->appendRow(enumMX_explorer);
    enums_explorer->appendRow(enumASN_explorer);
    enums_explorer->appendRow(enumCIDR_explorer);
    enums_explorer->appendRow(enumNS_explorer);
    enums_explorer->appendRow(enumSSL_explorer);
    enums_explorer->appendRow(enumEmail_explorer);

    ///
    /// append to project explorer....
    ///
    model_explorer->invisibleRootItem()->appendRow(project_explorer);
    project_explorer->appendRow(active_explorer);
    project_explorer->appendRow(passive_explorer);
    project_explorer->appendRow(enums_explorer);
    project_explorer->appendRow(custom_explorer);
}
ProjectModel::~ProjectModel(){
    delete model_explorer;
    delete active_explorer;
    delete passive_explorer;
    delete enums_explorer;
    delete custom_explorer;
    delete activeSubdomainIp_explorer;
    delete activeSubdomain_explorer;
    delete activeTld_explorer;
    delete activeWildcard_explorer;
    delete activeDNS_explorer;
    delete activeA_explorer;
    delete activeAAAA_explorer;
    delete activeNS_explorer;
    delete activeMX_explorer;
    delete activeTXT_explorer;
    delete activeCNAME_explorer;
    delete activeSRV_explorer;
    delete activeSSL_explorer;
    delete activeSSL_sha1_explorer;
    delete activeSSL_sha256_explorer;
    delete activeURL_explorer;
    delete passiveSubdomainIp_explorer;
    delete passiveSubdomain_explorer;
    delete passiveA_explorer;
    delete passiveAAAA_explorer;
    delete passiveNS_explorer;
    delete passiveMX_explorer;
    delete passiveTXT_explorer;
    delete passiveCNAME_explorer;
    delete passiveCidr_explorer;
    delete passiveEmail_explorer;
    delete passiveUrl_explorer;
    delete passiveAsn_explorer;
    delete passiveSSL_explorer;
    delete enumIp_explorer;
    delete enumASN_explorer;
    delete enumCIDR_explorer;
    delete enumNS_explorer;
    delete enumMX_explorer;
    delete enumSSL_explorer;
    delete enumEmail_explorer;
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
    delete activeURL_model;
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
    delete enumASN_model;
    delete enumCIDR_model;
    delete enumNS_model;
    delete enumMX_model;
    delete enumSSL_model;
    delete enumEmail_model;
}

void ProjectModel::setHeaderLabels(){
    activeSubdomainIp_model->setHorizontalHeaderLabels({" Subdomains", " IpAddress"});
    activeSubdomain_model->setHorizontalHeaderLabels({" Subdomains"});
    activeTld_model->setHorizontalHeaderLabels({" TLD", " IpAddress"});
    activeWildcard_model->setHorizontalHeaderLabels({" Wildcard", " IpV4", " IpV6"});
    activeDNS_model->setHorizontalHeaderLabels({" DNS Records", "", ""});
    activeA_model->setHorizontalHeaderLabels({" A DNS Records"});
    activeAAAA_model->setHorizontalHeaderLabels({" AAAA DNS Records"});
    activeNS_model->setHorizontalHeaderLabels({" NS DNS Records"});
    activeMX_model->setHorizontalHeaderLabels({" MX DNS Records"});
    activeTXT_model->setHorizontalHeaderLabels({" TXT DNS Records"});
    activeCNAME_model->setHorizontalHeaderLabels({" CNAME DNS Records"});
    activeSRV_model->setHorizontalHeaderLabels({" Name", " Target", " Port"});
    activeSSL_model->setHorizontalHeaderLabels({" SSL Certificates", " Values"});
    activeSSL_sha1_model->setHorizontalHeaderLabels({" SSL Certificates SHA-1 Fingerprints"});
    activeSSL_sha256_model->setHorizontalHeaderLabels({" SSL Certificates SHA-256 Fingerprints"});
    activeURL_model->setHorizontalHeaderLabels({" URL", " Status", " Server", " Content Type"});

    /* passive results model */
    passiveSubdomainIp_model->setHorizontalHeaderLabels({" Subdomains", " IpAddress"});
    passiveSubdomain_model->setHorizontalHeaderLabels({" Subdomains"});
    passiveA_model->setHorizontalHeaderLabels({" A Records"});
    passiveAAAA_model->setHorizontalHeaderLabels({" AAAA Records"});
    passiveCidr_model->setHorizontalHeaderLabels({" IP/CIDR"});
    passiveNS_model->setHorizontalHeaderLabels({" NS Records"});
    passiveMX_model->setHorizontalHeaderLabels({" MX Records"});
    passiveTXT_model->setHorizontalHeaderLabels({" TXT Records"});
    passiveCNAME_model->setHorizontalHeaderLabels({" CNAME Records"});
    passiveEmail_model->setHorizontalHeaderLabels({" Emails"});
    passiveUrl_model->setHorizontalHeaderLabels({" URLs"});
    passiveAsn_model->setHorizontalHeaderLabels({" ASN", " Name"});
    passiveSSL_model->setHorizontalHeaderLabels({" SSL Certificates ID"});

    /* enum Results model */
    enumIp_model->setHorizontalHeaderLabels({" IP", " Values"});
    enumASN_model->setHorizontalHeaderLabels({" ASN", " Values"});
    enumCIDR_model->setHorizontalHeaderLabels({" CIDR", " Values"});
    enumNS_model->setHorizontalHeaderLabels({" NS", " Values"});
    enumMX_model->setHorizontalHeaderLabels({" MX", " Values"});
    enumSSL_model->setHorizontalHeaderLabels({" SSL Certificate", " Values"});
    enumEmail_model->setHorizontalHeaderLabels({" Email", " Values"});
}

void ProjectModel::getRootItems(){
    /* active results model */
    m_activeSubdomainIp_rootItem = activeSubdomainIp_model->invisibleRootItem();
    m_activeSubdomain_rootItem = activeSubdomain_model->invisibleRootItem();
    m_activeTld_rootItem = activeTld_model->invisibleRootItem();
    m_activeWildcard_rootItem = activeWildcard_model->invisibleRootItem();
    m_activeDNS_rootItem = activeDNS_model->invisibleRootItem();
    m_activeA_rootItem = activeA_model->invisibleRootItem();
    m_activeAAAA_rootItem = activeAAAA_model->invisibleRootItem();
    m_activeNS_rootItem = activeNS_model->invisibleRootItem();
    m_activeMX_rootItem = activeMX_model->invisibleRootItem();
    m_activeTXT_rootItem = activeTXT_model->invisibleRootItem();
    m_activeCNAME_rootItem = activeCNAME_model->invisibleRootItem();
    m_activeSRV_rootItem = activeSRV_model->invisibleRootItem();
    m_activeSSL_rootItem = activeSSL_model->invisibleRootItem();
    m_activeSSL_sha1_rootItem = activeSSL_sha1_model->invisibleRootItem();
    m_activeSSL_sha256_rootItem = activeSSL_sha256_model->invisibleRootItem();
    m_activeURL_rootItem = activeURL_model->invisibleRootItem();

    /* passive results model */
    m_passiveSubdomainIp_rootItem = passiveSubdomainIp_model->invisibleRootItem();
    m_passiveSubdomain_rootItem = passiveSubdomain_model->invisibleRootItem();
    m_passiveA_rootItem = passiveA_model->invisibleRootItem();
    m_passiveAAAA_rootItem = passiveAAAA_model->invisibleRootItem();
    m_passiveCidr_rootItem = passiveCidr_model->invisibleRootItem();
    m_passiveNS_rootItem = passiveNS_model->invisibleRootItem();
    m_passiveMX_rootItem = passiveMX_model->invisibleRootItem();
    m_passiveTXT_rootItem = passiveTXT_model->invisibleRootItem();
    m_passiveCNAME_rootItem = passiveCNAME_model->invisibleRootItem();
    m_passiveEmail_rootItem = passiveEmail_model->invisibleRootItem();
    m_passiveUrl_rootItem = passiveUrl_model->invisibleRootItem();
    m_passiveAsn_rootItem = passiveAsn_model->invisibleRootItem();
    m_passiveSSL_rootItem = passiveSSL_model->invisibleRootItem();

    /* enum Results model */
    m_enumIp_rootItem = enumIp_model->invisibleRootItem();
    m_enumASN_rootItem = enumASN_model->invisibleRootItem();
    m_enumCIDR_rootItem = enumCIDR_model->invisibleRootItem();
    m_enumNS_rootItem = enumNS_model->invisibleRootItem();
    m_enumMX_rootItem = enumMX_model->invisibleRootItem();
    m_enumSSL_rootItem = enumSSL_model->invisibleRootItem();
    m_enumEmail_rootItem = enumEmail_model->invisibleRootItem();
}

void ProjectModel::clearModels(){
    activeSubdomainIp_model->clear();
    activeSubdomain_model->clear();
    activeTld_model->clear();
    activeWildcard_model->clear();
    activeDNS_model->clear();
    activeA_model->clear();
    activeAAAA_model->clear();
    activeNS_model->clear();
    activeMX_model->clear();
    activeTXT_model->clear();
    activeCNAME_model->clear();
    activeSRV_model->clear();
    activeSSL_model->clear();
    activeSSL_sha1_model->clear();
    activeSSL_sha256_model->clear();
    activeURL_model->clear();
    passiveSubdomainIp_model->clear();
    passiveSubdomain_model->clear();
    passiveA_model->clear();
    passiveAAAA_model->clear();
    passiveCidr_model->clear();
    passiveNS_model->clear();
    passiveMX_model->clear();
    passiveTXT_model->clear();
    passiveCNAME_model->clear();
    passiveEmail_model->clear();
    passiveUrl_model->clear();
    passiveAsn_model->clear();
    passiveSSL_model->clear();
    enumIp_model->clear();
    enumASN_model->clear();
    enumCIDR_model->clear();
    enumNS_model->clear();
    enumMX_model->clear();
    enumSSL_model->clear();
    enumEmail_model->clear();

    this->setHeaderLabels();
    this->getRootItems();
}
