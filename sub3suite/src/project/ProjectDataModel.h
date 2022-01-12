/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#ifndef MODELS_H
#define MODELS_H

#include <QStandardItemModel>
#include <QSet>

class ProjectDataModel{

    public:
        ProjectDataModel();
        ~ProjectDataModel();
        ///
        /// for active results...
        ///
        void addActiveSubdomain(QStringList);
        void addActiveTLD(QStringList);
        void addActiveWildcard(QStringList);
        void addActiveIp(QStringList);
        void addActiveA(QStringList);
        void addActiveAAAA(QStringList);
        void addActiveNS(QStringList);
        void addActiveMX(QStringList);
        void addActiveTXT(QStringList);
        void addActiveCNAME(QStringList);
        void addActiveSRV(QStringList);
        ///
        /// for active results...
        ///
        void addPassiveSubdomain(QStringList);
        void addPassiveTLD(QStringList);
        void addPassiveWildcard(QStringList);
        void addPassiveIp(QStringList);
        void addPassiveA(QStringList);
        void addPassiveAAAA(QStringList);
        void addPassiveCidr(QStringList);
        void addPassiveNS(QStringList);
        void addPassiveMX(QStringList);
        void addPassiveTXT(QStringList);
        void addPassiveCNAME(QStringList);
        void addPassiveSRV(QStringList);
        void addPassiveEMail(QStringList);
        void addPassiveUrl(QStringList);
        void addPassiveAsn(QStringList);
        void addPassiveSSLCert(QStringList);
        ///
        /// the projectModel...
        ///
        QStandardItemModel *projectModel;

    private:
        QSet<QString> m_activeSubdomainSet;
        QSet<QString> m_activeTldSet;
        QSet<QString> m_activeWildcardSet;
        QSet<QString> m_activeASet;
        QSet<QString> m_activeAAAASet;
        QSet<QString> m_activeNSSet;
        QSet<QString> m_activeMXSet;
        QSet<QString> m_activeTXTSet;
        QSet<QString> m_activeCNAMESet;
        QSet<QString> m_activeSRVSet;
        ///
        /// for passive...
        ///
        QSet<QString> m_passiveSubdomainSet;
        QSet<QString> m_passiveTldSet;
        QSet<QString> m_passiveWildcardSet;
        QSet<QString> m_passiveASet;
        QSet<QString> m_passiveAAAASet;
        QSet<QString> m_passiveCidrSet;
        QSet<QString> m_passiveNSSet;
        QSet<QString> m_passiveMXSet;
        QSet<QString> m_passiveTXTSet;
        QSet<QString> m_passiveCNAMESet;
        QSet<QString> m_passiveSRVSet;
        QSet<QString> m_passiveEmailSet;
        QSet<QString> m_passiveUrlSet;
        QSet<QString> m_passiveAsnSet;
        QSet<QString> m_passiveSSLCertSet;

    private:
        QStandardItem *m_rootItem;
        QStandardItem *m_active;
        QStandardItem *m_passive;
        ///
        /// for active Results...
        ///
        QStandardItem *m_activeSubdomains;
        QStandardItem *m_activeTlds;
        QStandardItem *m_activeWildcards;
        QStandardItem *m_activeA;
        QStandardItem *m_activeAAAA;
        QStandardItem *m_activeNS;
        QStandardItem *m_activeMX;
        QStandardItem *m_activeTXT;
        QStandardItem *m_activeCNAME;
        QStandardItem *m_activeSRV;
        ///
        /// for passive Results...
        ///
        QStandardItem *m_passiveSubdomains;
        QStandardItem *m_passiveTlds;
        QStandardItem *m_passiveWildcards;
        QStandardItem *m_passiveA;
        QStandardItem *m_passiveAAAA;
        QStandardItem *m_passiveCidr;
        QStandardItem *m_passiveNS;
        QStandardItem *m_passiveMX;
        QStandardItem *m_passiveTXT;
        QStandardItem *m_passiveCNAME;
        QStandardItem *m_passiveSRV;
        QStandardItem *m_passiveEmail;
        QStandardItem *m_passiveUrl;
        QStandardItem *m_passiveAsn;
        QStandardItem *m_passiveSSLCert;
};

#endif // MODELS_H
