/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :the main project data model
*/

#ifndef PROJECTMODEL_H
#define PROJECTMODEL_H

#include "src/modules/active/DNSScanner.h"
#include "src/modules/active/SSLScanner.h"
#include <QStandardItemModel>
#include <QSet>

/* TODO:
 *      compress & decompress data before saving and loading project,
 *      use a fast compression library eg https://github.com/lz4/lz4
 */
enum ModelType {
    activeSubdomainIp,
    activeSubdomain,
    activeTld,
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
    custom
};

struct ProjectStruct{
    QString name;
    QString path;
    bool isNew = false;
    bool isExisting = false;
    bool isTemporary = false;
    bool isConfigured = false;
};


class ProjectModel
{

public:
    ProjectModel();
    ~ProjectModel();

    void openProject(ProjectStruct project);
    void openExistingProject(QString name, QString path);
    void saveProject();
    void closeProject();

    /* ... */
    void clearModels();
    void getRootItems();
    void setHeaderLabels();

    ProjectStruct projectInfo;

    /* for active results */
    void addActiveSubdomainIp(const QString &subdomain, const QString &ip);
    void addActiveSubdomain(const QString &subdomain);
    void addActiveTLD(const QString &subdomain, const QString &ip);
    void addActiveWildcard(const QString &subdomain, const QString &ip);
    void addActiveSSL(const QString &target, const QSslCertificate &cert);
    void addActiveSSL_sha1(const QString &sha1);
    void addActiveSSL_sha256(const QString &sha256);
    void addActiveDNS(const dns::ScanResult &dns);

    /* for passive results */
    void addPassiveSubdomainIp(const QString &subdomain, const QString &ip);
    void addPassiveSubdomain(const QString &subdomain);
    void addPassiveIp(const QString &ip);
    void addPassiveA(const QString &ipv4);
    void addPassiveAAAA(const QString &ipv6);
    void addPassiveCidr(const QString &cidr);
    void addPassiveNS(const QString &ns);
    void addPassiveMX(const QString &mx);
    void addPassiveTXT(const QString &txt);
    void addPassiveCNAME(const QString &cname);
    void addPassiveEMail(const QString &email);
    void addPassiveUrl(const QString &url);
    void addPassiveAsn(const QString &asn, const QString &name);
    void addPassiveSSL(const QString &ssl);

    /* for enum */
    void addEnumIp();
    void addEnumASN();
    void addEnumCIDR();
    void addEnumNS();
    void addEnumMX();
    void addEnumSSL();
    void addEnumEmail();
    void addEnumURL();

public:
    QStandardItemModel *model_explorer;
    QStandardItem *project_explorer;

    QStandardItem *active_explorer;
    QStandardItem *passive_explorer;
    QStandardItem *enums_explorer;
    QStandardItem *custom_explorer;

    /* active Results Explorer */
    QStandardItem *activeSubdomainIp_explorer;
    QStandardItem *activeSubdomain_explorer;
    QStandardItem *activeTld_explorer;
    QStandardItem *activeWildcard_explorer;
    QStandardItem *activeDNS_explorer;
    QStandardItem *activeA_explorer;
    QStandardItem *activeAAAA_explorer;
    QStandardItem *activeNS_explorer;
    QStandardItem *activeMX_explorer;
    QStandardItem *activeTXT_explorer;
    QStandardItem *activeCNAME_explorer;
    QStandardItem *activeSRV_explorer;
    QStandardItem *activeSSL_explorer;
    QStandardItem *activeSSL_sha1_explorer;
    QStandardItem *activeSSL_sha256_explorer;

    /* passive Results explorer */
    QStandardItem *passiveSubdomainIp_explorer;
    QStandardItem *passiveSubdomain_explorer;
    QStandardItem *passiveA_explorer;
    QStandardItem *passiveAAAA_explorer;
    QStandardItem *passiveNS_explorer;
    QStandardItem *passiveMX_explorer;
    QStandardItem *passiveTXT_explorer;
    QStandardItem *passiveCNAME_explorer;
    QStandardItem *passiveCidr_explorer;
    QStandardItem *passiveEmail_explorer;
    QStandardItem *passiveUrl_explorer;
    QStandardItem *passiveAsn_explorer;
    QStandardItem *passiveSSL_explorer;

    /* enum Results explorer */
    QStandardItem *enumIp_explorer;
    QStandardItem *enumASN_explorer;
    QStandardItem *enumCIDR_explorer;
    QStandardItem *enumNS_explorer;
    QStandardItem *enumMX_explorer;
    QStandardItem *enumSSL_explorer;
    QStandardItem *enumEmail_explorer;
    QStandardItem *enumURL_explorer;

    /* active Results Model */
    QStandardItemModel *activeSubdomainIp_model;
    QStandardItemModel *activeSubdomain_model;
    QStandardItemModel *activeTld_model;
    QStandardItemModel *activeWildcard_model;
    QStandardItemModel *activeDNS_model;
    QStandardItemModel *activeA_model;
    QStandardItemModel *activeAAAA_model;
    QStandardItemModel *activeNS_model;
    QStandardItemModel *activeMX_model;
    QStandardItemModel *activeTXT_model;
    QStandardItemModel *activeCNAME_model;
    QStandardItemModel *activeSRV_model;
    QStandardItemModel *activeSSL_model;
    QStandardItemModel *activeSSL_sha1_model;
    QStandardItemModel *activeSSL_sha256_model;

    /* passive results model */
    QStandardItemModel *passiveSubdomainIp_model;
    QStandardItemModel *passiveSubdomain_model;
    QStandardItemModel *passiveA_model;
    QStandardItemModel *passiveAAAA_model;
    QStandardItemModel *passiveCidr_model;
    QStandardItemModel *passiveNS_model;
    QStandardItemModel *passiveMX_model;
    QStandardItemModel *passiveTXT_model;
    QStandardItemModel *passiveCNAME_model;
    QStandardItemModel *passiveEmail_model;
    QStandardItemModel *passiveUrl_model;
    QStandardItemModel *passiveAsn_model;
    QStandardItemModel *passiveSSL_model;

    /* enum Results model */
    QStandardItemModel *enumIp_model;
    QStandardItemModel *enumASN_model;
    QStandardItemModel *enumCIDR_model;
    QStandardItemModel *enumNS_model;
    QStandardItemModel *enumMX_model;
    QStandardItemModel *enumSSL_model;
    QStandardItemModel *enumEmail_model;
    QStandardItemModel *enumURL_model;

private:
    QByteArray getJson();
    QByteArray m_project_hash;

    /* active results model */
    QStandardItem *m_activeSubdomainIp_rootItem = nullptr;
    QStandardItem *m_activeSubdomain_rootItem = nullptr;
    QStandardItem *m_activeTld_rootItem = nullptr;
    QStandardItem *m_activeWildcard_rootItem = nullptr;
    QStandardItem *m_activeDNS_rootItem = nullptr;
    QStandardItem *m_activeA_rootItem = nullptr;
    QStandardItem *m_activeAAAA_rootItem = nullptr;
    QStandardItem *m_activeNS_rootItem = nullptr;
    QStandardItem *m_activeMX_rootItem = nullptr;
    QStandardItem *m_activeTXT_rootItem = nullptr;
    QStandardItem *m_activeCNAME_rootItem = nullptr;
    QStandardItem *m_activeSRV_rootItem = nullptr;
    QStandardItem *m_activeSSL_rootItem = nullptr;
    QStandardItem *m_activeSSL_sha1_rootItem = nullptr;
    QStandardItem *m_activeSSL_sha256_rootItem = nullptr;

    /* passive results model */
    QStandardItem *m_passiveSubdomainIp_rootItem = nullptr;
    QStandardItem *m_passiveSubdomain_rootItem = nullptr;
    QStandardItem *m_passiveA_rootItem = nullptr;
    QStandardItem *m_passiveAAAA_rootItem = nullptr;
    QStandardItem *m_passiveCidr_rootItem = nullptr;
    QStandardItem *m_passiveNS_rootItem = nullptr;
    QStandardItem *m_passiveMX_rootItem = nullptr;
    QStandardItem *m_passiveTXT_rootItem = nullptr;
    QStandardItem *m_passiveCNAME_rootItem = nullptr;
    QStandardItem *m_passiveEmail_rootItem = nullptr;
    QStandardItem *m_passiveUrl_rootItem = nullptr;
    QStandardItem *m_passiveAsn_rootItem = nullptr;
    QStandardItem *m_passiveSSL_rootItem = nullptr;

    /* enum Results model */
    QStandardItem *m_enumIp_rootItem = nullptr;
    QStandardItem *m_enumASN_rootItem = nullptr;
    QStandardItem *m_enumCIDR_rootItem = nullptr;
    QStandardItem *m_enumNS_rootItem = nullptr;
    QStandardItem *m_enumMX_rootItem = nullptr;
    QStandardItem *m_enumSSL_rootItem = nullptr;
    QStandardItem *m_enumEmail_rootItem = nullptr;
    QStandardItem *m_enumURL_rootItem = nullptr;
};

#endif // PROJECTMODEL_H
