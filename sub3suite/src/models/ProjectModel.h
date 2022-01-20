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


class ProjectModel
{

public:
    ProjectModel();
    ~ProjectModel();

    /* for active results */
    void addActiveSubdomainIp(const QString &subdomain, const QString &ip);
    void addActiveSubdomain(const QString &subdomain);
    void addActiveTLD(const QString &subdomain, const QString &ip);
    void addActiveWildcard(const QString &subdomain, const QString &ip);
    // ssl
    void addActiveSSL(const SSL_struct &ssl);
    void addActiveSSL_sha1(const QString &sha1);
    void addActiveSSL_sha256(const QString &sha256);
    // dns
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
    void addEnumDomain();
    void addEnumASN();
    void addEnumCIDR();
    void addEnumNS();
    void addEnumMX();
    void addEnumSSL();
    void addEnumEmail();
    void addEnumURL();

public:
    void setHeaderLabels();

    QStandardItemModel *projectExplorerModel;
    QStandardItem *explorerRootItem = nullptr;
    QStandardItem *active;
    QStandardItem *passive;
    QStandardItem *enums;
    QStandardItem *custom;

    /* active Results Explorer */
    QStandardItem *activeSubdomainIp;
    QStandardItem *activeSubdomain;
    QStandardItem *activeTld;
    QStandardItem *activeWildcard;
    // dns
    QStandardItem *activeDNS;
    QStandardItem *activeA;
    QStandardItem *activeAAAA;
    QStandardItem *activeNS;
    QStandardItem *activeMX;
    QStandardItem *activeTXT;
    QStandardItem *activeCNAME;
    QStandardItem *activeSRV;
    // ssl
    QStandardItem *activeSSL;
    QStandardItem *activeSSL_sha1;
    QStandardItem *activeSSL_sha256;

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
    QStandardItem *enumDomain;
    QStandardItem *enumASN;
    QStandardItem *enumCIDR;
    QStandardItem *enumNS;
    QStandardItem *enumMX;
    QStandardItem *enumSSL;
    QStandardItem *enumEmail;
    QStandardItem *enumURL;

    /* active Results Model */
    QStandardItemModel *activeSubdomainIp_model;
    QStandardItemModel *activeSubdomain_model;
    QStandardItemModel *activeTld_model;
    QStandardItemModel *activeWildcard_model;
    // dns
    QStandardItemModel *activeDNS_model;
    QStandardItemModel *activeA_model;
    QStandardItemModel *activeAAAA_model;
    QStandardItemModel *activeNS_model;
    QStandardItemModel *activeMX_model;
    QStandardItemModel *activeTXT_model;
    QStandardItemModel *activeCNAME_model;
    QStandardItemModel *activeSRV_model;
    // ssl
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
    QStandardItemModel *enumDomain_model;
    QStandardItemModel *enumASN_model;
    QStandardItemModel *enumCIDR_model;
    QStandardItemModel *enumNS_model;
    QStandardItemModel *enumMX_model;
    QStandardItemModel *enumSSL_model;
    QStandardItemModel *enumEmail_model;
    QStandardItemModel *enumURL_model;

private:
    /* active Results  */
    QStandardItem *m_activeSubdomainIp = nullptr;
    QStandardItem *m_activeSubdomain = nullptr;
    QStandardItem *m_activeTld = nullptr;
    QStandardItem *m_activeWildcard = nullptr;
    // dns
    QStandardItem *m_activeDNS = nullptr;
    QStandardItem *m_activeA = nullptr;
    QStandardItem *m_activeAAAA = nullptr;
    QStandardItem *m_activeNS = nullptr;
    QStandardItem *m_activeMX = nullptr;
    QStandardItem *m_activeTXT = nullptr;
    QStandardItem *m_activeCNAME = nullptr;
    QStandardItem *m_activeSRV = nullptr;
    // ssl
    QStandardItem *m_activeSSL = nullptr;
    QStandardItem *m_activeSSL_sha1 = nullptr;
    QStandardItem *m_activeSSL_sha256 = nullptr;

    /* passive results model */
    QStandardItem *m_passiveSubdomainIp = nullptr;
    QStandardItem *m_passiveSubdomain = nullptr;
    QStandardItem *m_passiveA = nullptr;
    QStandardItem *m_passiveAAAA = nullptr;
    QStandardItem *m_passiveCidr = nullptr;
    QStandardItem *m_passiveNS = nullptr;
    QStandardItem *m_passiveMX = nullptr;
    QStandardItem *m_passiveTXT = nullptr;
    QStandardItem *m_passiveCNAME = nullptr;
    QStandardItem *m_passiveEmail = nullptr;
    QStandardItem *m_passiveUrl = nullptr;
    QStandardItem *m_passiveAsn = nullptr;
    QStandardItem *m_passiveSSL = nullptr;

    /* enum Results model */
    QStandardItem *m_enumIp = nullptr;
    QStandardItem *m_enumDomain = nullptr;
    QStandardItem *m_enumASN = nullptr;
    QStandardItem *m_enumCIDR = nullptr;
    QStandardItem *m_enumNS = nullptr;
    QStandardItem *m_enumMX = nullptr;
    QStandardItem *m_enumSSL = nullptr;
    QStandardItem *m_enumEmail = nullptr;
    QStandardItem *m_enumURL = nullptr;
};

#endif // PROJECTMODEL_H
