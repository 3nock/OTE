#include "ProjectModel.h"

#include "src/models/SSLModel.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>


ProjectModel::ProjectModel(QString projectPath):
    projectFile(projectPath),
    model_explorer(new QStandardItemModel),
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
    enumDomain_explorer(new QStandardItem("Domain")),
    enumASN_explorer(new QStandardItem("ASN")),
    enumCIDR_explorer(new QStandardItem("CIDR")),
    enumNS_explorer(new QStandardItem("NS")),
    enumMX_explorer(new QStandardItem("MX")),
    enumSSL_explorer(new QStandardItem("SSL")),
    enumEmail_explorer(new QStandardItem("Email")),
    enumURL_explorer(new QStandardItem("URL")),

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
    ///
    /// models header labels...
    ///
    activeSubdomainIp_model->setHorizontalHeaderLabels({"    Subdomains", "    IpAddress"});
    activeSubdomain_model->setHorizontalHeaderLabels({"    Subdomains"});
    activeTld_model->setHorizontalHeaderLabels({"    TLD", "    IpAddress"});
    activeWildcard_model->setHorizontalHeaderLabels({"    Wildcard", "    IpAddress"});
    activeDNS_model->setHorizontalHeaderLabels({"    DNS Records"});
    activeA_model->setHorizontalHeaderLabels({"    A DNS Records"});
    activeAAAA_model->setHorizontalHeaderLabels({"    AAAA DNS Records"});
    activeNS_model->setHorizontalHeaderLabels({"    NS DNS Records"});
    activeMX_model->setHorizontalHeaderLabels({"    MX DNS Records"});
    activeTXT_model->setHorizontalHeaderLabels({"    TXT DNS Records"});
    activeCNAME_model->setHorizontalHeaderLabels({"    CNAME DNS Records"});
    activeSRV_model->setHorizontalHeaderLabels({"    Name", "    Target", "    Port"});
    activeSSL_model->setHorizontalHeaderLabels({"    SSL Certificates", "    Values"});
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

    ///
    /// getting rootItems
    ///
    rootItem_explorer = model_explorer->invisibleRootItem();

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
    m_enumDomain_rootItem = enumDomain_model->invisibleRootItem();
    m_enumASN_rootItem = enumASN_model->invisibleRootItem();
    m_enumCIDR_rootItem = enumCIDR_model->invisibleRootItem();
    m_enumNS_rootItem = enumNS_model->invisibleRootItem();
    m_enumMX_rootItem = enumMX_model->invisibleRootItem();
    m_enumSSL_rootItem = enumSSL_model->invisibleRootItem();
    m_enumEmail_rootItem = enumEmail_model->invisibleRootItem();
    m_enumURL_rootItem = enumURL_model->invisibleRootItem();

    ///
    /// setting icons
    ///
    QFont font("Segoe UI", 9, QFont::Bold);
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
    enumDomain_explorer->setIcon(QIcon(":/img/res/icons/folder2.png"));
    enumASN_explorer->setIcon(QIcon(":/img/res/icons/folder2.png"));
    enumCIDR_explorer->setIcon(QIcon(":/img/res/icons/folder2.png"));
    enumNS_explorer->setIcon(QIcon(":/img/res/icons/folder2.png"));
    enumMX_explorer->setIcon(QIcon(":/img/res/icons/folder2.png"));
    enumSSL_explorer->setIcon(QIcon(":/img/res/icons/folder2.png"));
    enumEmail_explorer->setIcon(QIcon(":/img/res/icons/folder2.png"));
    enumURL_explorer->setIcon(QIcon(":/img/res/icons/folder2.png"));

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
    enumDomain_explorer->setForeground(Qt::white);
    enumASN_explorer->setForeground(Qt::white);
    enumCIDR_explorer->setForeground(Qt::white);
    enumNS_explorer->setForeground(Qt::white);
    enumMX_explorer->setForeground(Qt::white);
    enumSSL_explorer->setForeground(Qt::white);
    enumEmail_explorer->setForeground(Qt::white);
    enumURL_explorer->setForeground(Qt::white);

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
    enums_explorer->appendRow(enumDomain_explorer);
    enums_explorer->appendRow(enumASN_explorer);
    enums_explorer->appendRow(enumCIDR_explorer);
    enums_explorer->appendRow(enumNS_explorer);
    enums_explorer->appendRow(enumSSL_explorer);
    enums_explorer->appendRow(enumEmail_explorer);
    enums_explorer->appendRow(enumURL_explorer);

    ///
    /// append to rootItem....
    ///
    rootItem_explorer->appendRow(active_explorer);
    rootItem_explorer->appendRow(passive_explorer);
    rootItem_explorer->appendRow(enums_explorer);
    rootItem_explorer->appendRow(custom_explorer);

    ///
    /// opening the project...
    ///
    this->openProject();
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
    delete enumDomain_explorer;
    delete enumASN_explorer;
    delete enumCIDR_explorer;
    delete enumNS_explorer;
    delete enumMX_explorer;
    delete enumSSL_explorer;
    delete enumEmail_explorer;
    delete enumURL_explorer;
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

void ProjectModel::addActiveSSL(const QString &target, const QSslCertificate &cert){
    CertModel *certModel = new CertModel;
    certModel->mainItem->setText(target);

    /* ... */
    certModel->info_verison->setText(cert.version());
    certModel->info_serialNumber->setText(cert.serialNumber());
    certModel->info_signatureAlgorithm->setText(""); // none yet

    /* fingerprint */
    certModel->fingerprint_md5->setText(cert.digest(QCryptographicHash::Md5).toHex());
    certModel->fingerprint_sha1->setText(cert.digest(QCryptographicHash::Sha1).toHex());
    certModel->fingerprint_sha256->setText(cert.digest(QCryptographicHash::Sha256).toHex());

    /* validity */
    certModel->validity_notBefore->setText(cert.effectiveDate().toString());
    certModel->validity_notAfter->setText(cert.expiryDate().toString());

    /* issuer Info */
    if(cert.issuerInfo(QSslCertificate::CommonName).length() > 0)
        certModel->issuer_commonName->setText(cert.issuerInfo(QSslCertificate::CommonName)[0]);
    if(cert.issuerInfo(QSslCertificate::Organization).length() > 0)
        certModel->issuer_organizationName->setText(cert.issuerInfo(QSslCertificate::Organization)[0]);
    if(cert.issuerInfo(QSslCertificate::CountryName).length() > 0)
        certModel->issuer_countryName->setText(cert.issuerInfo(QSslCertificate::CountryName)[0]);

    /* subject info */
    if(cert.subjectInfo(QSslCertificate::CommonName).length() > 0)
        certModel->subject_commonName->setText(cert.subjectInfo(QSslCertificate::CommonName)[0]);
    if(cert.subjectInfo(QSslCertificate::CountryName).length() > 0)
        certModel->subject_countryName->setText(cert.subjectInfo(QSslCertificate::CountryName)[0]);
    if(cert.subjectInfo(QSslCertificate::LocalityName).length() > 0)
        certModel->subject_localityName->setText(cert.subjectInfo(QSslCertificate::LocalityName)[0]);
    if(cert.subjectInfo(QSslCertificate::Organization).length() > 0)
        certModel->subject_organizationName->setText(cert.subjectInfo(QSslCertificate::Organization)[0]);
    if(cert.subjectInfo(QSslCertificate::StateOrProvinceName).length() > 0)
        certModel->subject_stateOrProvinceName->setText(cert.subjectInfo(QSslCertificate::StateOrProvinceName)[0]);
    if(cert.subjectInfo(QSslCertificate::EmailAddress).length() > 0)
        certModel->subject_email->setText(cert.subjectInfo(QSslCertificate::EmailAddress)[0]);

    int alternativeName = 0;
    foreach(const QString &value, cert.subjectAlternativeNames()){
        certModel->subjectAltNames->appendRow({new QStandardItem(QString::number(alternativeName)), new QStandardItem(value)});
        alternativeName++;
    }

    m_activeSSL_rootItem->appendRow(certModel->mainItem);
    /* to sha1 & sha256 model */
    m_activeSSL_sha1_rootItem->appendRow(new QStandardItem(QString(cert.digest(QCryptographicHash::Sha1).toHex())));
    m_activeSSL_sha256_rootItem->appendRow(new QStandardItem(QString(cert.digest(QCryptographicHash::Sha256).toHex())));
}

void ProjectModel::addActiveSSL_sha1(const QString &sha1){
    m_activeSSL_sha1_rootItem->appendRow(new QStandardItem(sha1));
}

void ProjectModel::addActiveSSL_sha256(const QString &sha256){
    m_activeSSL_sha256_rootItem->appendRow(new QStandardItem(sha256));
}

void ProjectModel::addActiveDNS(const dns::ScanResult &dns){
    QStandardItem *targetItem = new QStandardItem(dns.target);
    targetItem->setIcon(QIcon(":/img/res/icons/folder.png"));
    targetItem->setForeground(Qt::white);
    m_activeDNS_rootItem->appendRow(targetItem);

    if(!dns.A.isEmpty()){
        QStandardItem *recordItem = new QStandardItem("A");
        recordItem->setIcon(QIcon(":/img/res/icons/folder2.png"));
        recordItem->setForeground(Qt::white);

        for(const QString &item: dns.A){
            recordItem->appendRow(new QStandardItem(item));
            m_activeA_rootItem->appendRow(new QStandardItem(item));
        }
        targetItem->appendRow(recordItem);
    }

    if(!dns.AAAA.isEmpty()){
        QStandardItem *recordItem = new QStandardItem("AAAA");
        recordItem->setIcon(QIcon(":/img/res/icons/folder2.png"));
        recordItem->setForeground(Qt::white);

        for(QString item: dns.AAAA){
            recordItem->appendRow(new QStandardItem(item));
            m_activeAAAA_rootItem->appendRow(new QStandardItem(item));
        }
        targetItem->appendRow(recordItem);
    }

    if(!dns.NS.isEmpty()){
        QStandardItem *recordItem = new QStandardItem("NS");
        recordItem->setIcon(QIcon(":/img/res/icons/folder2.png"));
        recordItem->setForeground(Qt::white);

        for(QString item: dns.NS){
            recordItem->appendRow(new QStandardItem(item));
            m_activeNS_rootItem->appendRow(new QStandardItem(item));
        }
        targetItem->appendRow(recordItem);
    }

    if(!dns.MX.isEmpty()){
        QStandardItem *recordItem = new QStandardItem("MX");
        recordItem->setIcon(QIcon(":/img/res/icons/folder2.png"));
        recordItem->setForeground(Qt::white);

        for(QString item: dns.MX){
            recordItem->appendRow(new QStandardItem(item));
            m_activeMX_rootItem->appendRow(new QStandardItem(item));
        }
        targetItem->appendRow(recordItem);
    }

    if(!dns.TXT.isEmpty()){
        QStandardItem *recordItem = new QStandardItem("TXT");
        recordItem->setIcon(QIcon(":/img/res/icons/folder2.png"));
        recordItem->setForeground(Qt::white);

        for(QString item: dns.TXT){
            recordItem->appendRow(new QStandardItem(item));
            m_activeTXT_rootItem->appendRow(new QStandardItem(item));
        }
        targetItem->appendRow(recordItem);
    }

    if(!dns.CNAME.isEmpty()){
        QStandardItem *recordItem = new QStandardItem("CNAME");
        recordItem->setIcon(QIcon(":/img/res/icons/folder2.png"));
        recordItem->setForeground(Qt::white);

        for(QString item: dns.CNAME){
            recordItem->appendRow(new QStandardItem(item));
            m_activeCNAME_rootItem->appendRow(new QStandardItem(item));
        }
        targetItem->appendRow(recordItem);
    }
}

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

/*
 * opening and saving project....
 */
void ProjectModel::openProject(){
    qDebug() << "Opening Project: " << projectFile;

    QFile file(projectFile);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        qWarning() << "Failed To Open Project File, defaulting to Temporary Project";

        /* default to Temporary Project */
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(file.readAll());
    file.close();
    if(document.isNull() || document.isEmpty()){
        qWarning() << "Error parsing the project file";
        return;
    }

    QJsonObject mainObj = document.object();

    ///
    /// Project Information
    ///
    QJsonObject info = mainObj["general"].toObject();

    ///
    /// Project Data
    ///
    QJsonObject data = mainObj["data"].toObject();

    /* active subdomainIp */
    foreach(const QJsonValue &value, data["active_subdomainIP"].toArray())
        m_activeSubdomainIp_rootItem->appendRow({new QStandardItem(value.toArray()[0].toString()),
                                                 new QStandardItem(value.toArray()[1].toString())});

    /* active subdomain */
    foreach(const QJsonValue &value, data["active_subdomain"].toArray())
        m_activeSubdomain_rootItem->appendRow(new QStandardItem(value.toString()));

    /* active TLD */
    foreach(const QJsonValue &value, data["active_TLD"].toArray())
        m_activeTld_rootItem->appendRow({new QStandardItem(value.toArray()[0].toString()),
                                         new QStandardItem(value.toArray()[1].toString())});

    /* active Wildcards */
    foreach(const QJsonValue &value, data["active_wildcard"].toArray())
        m_activeWildcard_rootItem->appendRow({new QStandardItem(value.toArray()[0].toString()),
                                              new QStandardItem(value.toArray()[1].toString())});

    /* active dns */
    foreach(const QJsonValue &value, data["active_dns"].toArray()){
        QJsonObject obj = value.toObject();

        QStandardItem *dns_item = new QStandardItem(obj["domain"].toString());
        dns_item->setIcon(QIcon(":/img/res/icons/folder.png"));
        dns_item->setForeground(Qt::white);
        m_activeDNS_rootItem->appendRow(dns_item);

        QJsonArray A = obj["A"].toArray();
        QJsonArray AAAA = obj["AAAA"].toArray();
        QJsonArray NS = obj["NS"].toArray();
        QJsonArray MX = obj["MX"].toArray();
        QJsonArray TXT = obj["TXT"].toArray();
        QJsonArray CNAME = obj["CNAME"].toArray();
        QJsonArray SRV = obj["SRV"].toArray();

        if(!A.isEmpty()){
            QStandardItem *record = new QStandardItem("A");
            record->setIcon(QIcon(":/img/res/icons/folder2.png"));
            record->setForeground(Qt::white);
            foreach(const QJsonValue &value, A){
                record->appendRow(new QStandardItem(value.toString()));
                m_activeA_rootItem->appendRow(new QStandardItem(value.toString()));
            }
            dns_item->appendRow(record);
        }
        if(!AAAA.isEmpty()){
            QStandardItem *record = new QStandardItem("AAAA");
            record->setIcon(QIcon(":/img/res/icons/folder2.png"));
            record->setForeground(Qt::white);
            foreach(const QJsonValue &value, AAAA){
                record->appendRow(new QStandardItem(value.toString()));
                m_activeAAAA_rootItem->appendRow(new QStandardItem(value.toString()));
            }
            dns_item->appendRow(record);
        }
        if(!NS.isEmpty()){
            QStandardItem *record = new QStandardItem("NS");
            record->setIcon(QIcon(":/img/res/icons/folder2.png"));
            record->setForeground(Qt::white);
            foreach(const QJsonValue &value, NS){
                record->appendRow(new QStandardItem(value.toString()));
                m_activeNS_rootItem->appendRow(new QStandardItem(value.toString()));
            }
            dns_item->appendRow(record);
        }
        if(!MX.isEmpty()){
            QStandardItem *record = new QStandardItem("MX");
            record->setIcon(QIcon(":/img/res/icons/folder2.png"));
            record->setForeground(Qt::white);
            foreach(const QJsonValue &value, MX){
                record->appendRow(new QStandardItem(value.toString()));
                m_activeMX_rootItem->appendRow(new QStandardItem(value.toString()));
            }
            dns_item->appendRow(record);
        }
        if(!TXT.isEmpty()){
            QStandardItem *record = new QStandardItem("TXT");
            record->setIcon(QIcon(":/img/res/icons/folder2.png"));
            record->setForeground(Qt::white);
            foreach(const QJsonValue &value, TXT){
                record->appendRow(new QStandardItem(value.toString()));
                m_activeTXT_rootItem->appendRow(new QStandardItem(value.toString()));
            }
            dns_item->appendRow(record);
        }
        if(!CNAME.isEmpty()){
            QStandardItem *record = new QStandardItem("CNAME");
            record->setIcon(QIcon(":/img/res/icons/folder2.png"));
            record->setForeground(Qt::white);
            foreach(const QJsonValue &value, CNAME){
                record->appendRow(new QStandardItem(value.toString()));
                m_activeCNAME_rootItem->appendRow(new QStandardItem(value.toString()));
            }
            dns_item->appendRow(record);
        }
        if(!SRV.isEmpty()){
            QStandardItem *record = new QStandardItem("SRV");
            record->setIcon(QIcon(":/img/res/icons/folder2.png"));
            record->setForeground(Qt::white);
            foreach(const QJsonValue &value, SRV){
                record->appendRow(new QStandardItem(value.toString()));
                m_activeSRV_rootItem->appendRow({new QStandardItem(value.toArray().at(0).toString()),
                                                 new QStandardItem(value.toArray().at(1).toString()),
                                                 new QStandardItem(value.toArray().at(2).toString())});
            }
            dns_item->appendRow(record);
        }
        activeDNS_model->appendRow(dns_item);
    }

    /* active SSL sha1 */
    foreach(const QJsonValue &value, data["active_SSL_sha1"].toArray())
        m_activeSSL_sha1_rootItem->appendRow(new QStandardItem(value.toString()));

    /* active SSL sha256 */
    foreach(const QJsonValue &value, data["active_SSL_sha256"].toArray())
        m_activeSSL_sha256_rootItem->appendRow(new QStandardItem(value.toString()));

    /* active SSL */
    foreach(const QJsonValue &value, data["active_SSL"].toArray()){
        CertModel *certModel = new CertModel;
        certModel->mainItem->setText(value.toObject()["domain"].toString());

        /* info */
        certModel->info_verison->setText(value.toObject()["info_version"].toString());
        certModel->info_serialNumber->setText(value.toObject()["info_serialNumber"].toString());
        certModel->info_signatureAlgorithm->setText(value.toObject()["info_signatureAlgorithm"].toString()); // none yet

        /* fingerprint */
        certModel->fingerprint_md5->setText(value.toObject()["fingerprint_md5"].toString());
        certModel->fingerprint_sha1->setText(value.toObject()["fingerprint_sha1"].toString());
        certModel->fingerprint_sha256->setText(value.toObject()["fingerprint_sha256"].toString());

        /* validity */
        certModel->validity_notBefore->setText(value.toObject()["validity_notBefore"].toString());
        certModel->validity_notAfter->setText(value.toObject()["validity_notAfter"].toString());

        /* issuer */
        certModel->issuer_commonName->setText(value.toObject()["issuer_commonName"].toString());
        certModel->issuer_organizationName->setText(value.toObject()["issuer_organizationName"].toString());
        certModel->issuer_countryName->setText(value.toObject()["issuer_countryName"].toString());

        /* subject */
        certModel->subject_commonName->setText(value.toObject()["subject_commonName"].toString());
        certModel->subject_countryName->setText(value.toObject()["subject_countryName"].toString());
        certModel->subject_localityName->setText(value.toObject()["subject_localityName"].toString());
        certModel->subject_organizationName->setText(value.toObject()["subject_organizationName"].toString());
        certModel->subject_stateOrProvinceName->setText(value.toObject()["subject_state"].toString());
        certModel->subject_email->setText(value.toObject()["subject_email"].toString());

        /* key */
        certModel->key_type->setText(value.toObject()["key_type"].toString());
        certModel->key_algorithm->setText(value.toObject()["key_algorithm"].toString());

        int count = 0;
        foreach(const QJsonValue &alternativeName, value.toArray()){
            certModel->subjectAltNames->appendRow({new QStandardItem(QString::number(count)),
                                                   new QStandardItem(alternativeName.toString())});
            count++;
        }

        m_activeSSL_rootItem->appendRow(certModel->mainItem);
        /* to sha1 & sha256 model */
        m_activeSSL_sha1_rootItem->appendRow(new QStandardItem(value.toObject()["fingerprint_sha1"].toString()));
        m_activeSSL_sha256_rootItem->appendRow(new QStandardItem(value.toObject()["fingerprint_sha256"].toString()));
    }

    /* passive subdomainIP */
    foreach(const QJsonValue &value, data["passive_subdomainIP"].toArray())
        m_passiveSubdomainIp_rootItem->appendRow({new QStandardItem(value.toArray()[0].toString()),
                                                 new QStandardItem(value.toArray()[1].toString())});

    /* passive ASN */
    foreach(const QJsonValue &value, data["passive_ASN"].toArray())
        m_passiveAsn_rootItem->appendRow({new QStandardItem(value.toArray()[0].toString()),
                                                 new QStandardItem(value.toArray()[1].toString())});

    /* passive Subdomain */
    foreach(const QJsonValue &value, data["passive_Subdomain"].toArray())
        m_passiveSubdomain_rootItem->appendRow(new QStandardItem(value.toString()));

    /* passive A */
    foreach(const QJsonValue &value, data["passive_A"].toArray())
        m_passiveA_rootItem->appendRow(new QStandardItem(value.toString()));

    /* passive AAAA */
    foreach(const QJsonValue &value, data["passive_AAAA"].toArray())
        m_passiveAAAA_rootItem->appendRow(new QStandardItem(value.toString()));

    /* passive CIDR */
    foreach(const QJsonValue &value, data["passive_CIDR"].toArray())
        m_passiveCidr_rootItem->appendRow(new QStandardItem(value.toString()));

    /* passive NS */
    foreach(const QJsonValue &value, data["passive_NS"].toArray())
        m_passiveNS_rootItem->appendRow(new QStandardItem(value.toString()));

    /* passive MX */
    foreach(const QJsonValue &value, data["passive_MX"].toArray())
        m_passiveMX_rootItem->appendRow(new QStandardItem(value.toString()));

    /* passive TXT */
    foreach(const QJsonValue &value, data["passive_TXT"].toArray())
        m_passiveTXT_rootItem->appendRow(new QStandardItem(value.toString()));

    /* passive CNAME */
    foreach(const QJsonValue &value, data["passive_CNAME"].toArray())
        m_passiveCNAME_rootItem->appendRow(new QStandardItem(value.toString()));

    /* passive Email */
    foreach(const QJsonValue &value, data["passive_Email"].toArray())
        m_passiveEmail_rootItem->appendRow(new QStandardItem(value.toString()));

    /* passive URL */
    foreach(const QJsonValue &value, data["passive_URL"].toArray())
        m_passiveUrl_rootItem->appendRow(new QStandardItem(value.toString()));

    /* passive SSL */
    foreach(const QJsonValue &value, data["passive_SSL"].toArray())
        m_passiveSSL_rootItem->appendRow(new QStandardItem(value.toString()));

    /* enum IP */
    /* enum MX" */
    /* enum NS" */
    /* enum ASN" */
    /* enum CIDR" */
    /* enum SSL" */
    /* enum Email" */
    /* enum URL" */
    /* custom" */
}

void ProjectModel::saveProject(){

}
