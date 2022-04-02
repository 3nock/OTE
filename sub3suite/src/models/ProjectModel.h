/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :the main project data model
*/

#ifndef PROJECTMODEL_H
#define PROJECTMODEL_H

#include <QSet>
#include <QObject>
#include <QStandardItemModel>
#include <QTextDocument>

#include "src/items/ASNItem.h"
#include "src/items/CIDRItem.h"
#include "src/items/IPItem.h"
#include "src/items/MXItem.h"
#include "src/items/NSItem.h"
#include "src/items/EmailItem.h"
#include "src/items/DNSItem.h"
#include "src/items/URLItem.h"
#include "src/items/SSLItem.h"
#include "src/items/HostItem.h"
#include "src/items/RawItem.h"
#include "src/items/WildcardItem.h"
#include "src/modules/active/DNSScanner.h"
#include "src/modules/active/SSLScanner.h"

#include "ExplorerModel.h"


struct ProjectStruct{
    QString name;
    QString path;
    QString date_created;
    QString last_modified;
    bool isNew = false;
    bool isExisting = false;
    bool isTemporary = false;
    bool isConfigured = false;
};


class ProjectModel: public QObject {
    Q_OBJECT

public:
    ProjectModel(QObject *parent = nullptr);
    ~ProjectModel();

    /* serialization */
    void openProject(ProjectStruct project);
    void openExistingProject(QString name, QString path);
    void saveProject();
    void saveProjectCopy();
    void closeProject();

    /* actions */
    void clearModels();
    int getItemsCount();
    void setHeaderLabels();

    /* project  information */
    ProjectStruct info;

signals:
    void projectLoaded();

public:
    bool modified = false;
    QString notes;
    QTextDocument *notes_document = nullptr;
    ExplorerModel *explorer;

    /* active Results Model */
    QStandardItemModel *activeHost;
    QStandardItemModel *activeWildcard;
    QStandardItemModel *activeDNS;
    QStandardItemModel *activeA;
    QStandardItemModel *activeAAAA;
    QStandardItemModel *activeNS;
    QStandardItemModel *activeMX;
    QStandardItemModel *activeTXT;
    QStandardItemModel *activeCNAME;
    QStandardItemModel *activeSRV;
    QStandardItemModel *activeSSL;
    QStandardItemModel *activeSSL_sha1;
    QStandardItemModel *activeSSL_sha256;
    QStandardItemModel *activeSSL_altNames;
    QStandardItemModel *activeURL;
    /* passive results model */
    QStandardItemModel *passiveSubdomainIp;
    QStandardItemModel *passiveSubdomain;
    QStandardItemModel *passiveA;
    QStandardItemModel *passiveAAAA;
    QStandardItemModel *passiveCIDR;
    QStandardItemModel *passiveNS;
    QStandardItemModel *passiveMX;
    QStandardItemModel *passiveTXT;
    QStandardItemModel *passiveCNAME;
    QStandardItemModel *passiveEmail;
    QStandardItemModel *passiveURL;
    QStandardItemModel *passiveASN;
    QStandardItemModel *passiveSSL;
    /* enum Results model */
    QStandardItemModel *enumIP;
    QStandardItemModel *enumASN;
    QStandardItemModel *enumCIDR;
    QStandardItemModel *enumNS;
    QStandardItemModel *enumMX;
    QStandardItemModel *enumSSL;
    QStandardItemModel *enumEmail;
    /* custom models */
    QStandardItemModel *raw;

    /* item's maps */
    QMap<QString, s3s_item::HOST*> map_activeHost;
    QMap<QString, s3s_item::Wildcard*> map_activeWildcard;
    QMap<QString, s3s_item::DNS*> map_activeDNS;
    QMap<QString, s3s_item::SSL*> map_activeSSL;
    QMap<QString, s3s_item::URL*> map_activeURL;
    QMap<QString, s3s_item::IP*> map_enumIP;
    QMap<QString, s3s_item::ASN*> map_enumASN;
    QMap<QString, s3s_item::CIDR*> map_enumCIDR;
    QMap<QString, s3s_item::NS*> map_enumNS;
    QMap<QString, s3s_item::MX*> map_enumMX;
    QMap<QString, s3s_item::SSL*> map_enumSSL;
    QMap<QString, s3s_item::Email*> map_enumEmail;

    /* for active results */
    void addActiveHost(const s3s_struct::HOST &host);
    void addActiveWildcard(const s3s_struct::Wildcard &wildcard);
    void addActiveDNS(const s3s_struct::DNS &dns);
    void addActiveURL(const s3s_struct::URL &url);
    void addActiveSSL(const QString &target, const QSslCertificate &cert);
    void addActiveSSL_hash(const QString &hash);
    void addActiveSSL_sha1(const QString &sha1);
    void addActiveSSL_sha256(const QString &sha256);
    void addActiveSSL_altNames(const QString &alternative_names);
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
    void addEnumASN(const s3s_struct::ASN &asn);
    void addEnumCIDR(const s3s_struct::CIDR &cidr);
    void addEnumIP(const s3s_struct::IP &ip);
    void addEnumIP_domains(const s3s_struct::IP &ip);
    void addEnumNS(const s3s_struct::NS &ns);
    void addEnumMX(const s3s_struct::MX &xm);
    void addEnumSSL(const QString &target, const QSslCertificate &ssl);
    void addEnumEmail(const s3s_struct::Email &email);
    /* for raw */
    void addRaw(const s3s_struct::RAW &raw);
};

#endif // PROJECTMODEL_H
