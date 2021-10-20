#include "ProjectDataModel.h"


ProjectDataModel::ProjectDataModel():
    projectModel(new QStandardItemModel),
    m_rootItem(nullptr),
    m_active(new QStandardItem("Active")),
    m_passive(new QStandardItem("Passive")),
    ///
    /// for active...
    ///
    m_activeSubdomains(new QStandardItem("Subdomains")),
    m_activeTlds(new QStandardItem("Tlds")),
    m_activeA(new QStandardItem("A")),
    m_activeAAAA(new QStandardItem("AAAA")),
    m_activeNS(new QStandardItem("NS")),
    m_activeMX(new QStandardItem("MX")),
    m_activeTXT(new QStandardItem("TXT")),
    m_activeCNAME(new QStandardItem("CNAME")),
    m_activeSRV(new QStandardItem("SRV")),
    ///
    /// for passive...
    ///
    m_passiveSubdomains(new QStandardItem("Subdomains")),
    m_passiveTlds(new QStandardItem("Tlds")),
    m_passiveA(new QStandardItem("A")),
    m_passiveAAAA(new QStandardItem("AAAA")),
    m_passiveNS(new QStandardItem("NS")),
    m_passiveMX(new QStandardItem("MX")),
    m_passiveTXT(new QStandardItem("TXT")),
    m_passiveCNAME(new QStandardItem("CNAME")),
    m_passiveSRV(new QStandardItem("SRV")),
    m_passiveEmail(new QStandardItem("Emails")),
    m_passiveUrl(new QStandardItem("Urls")),
    m_passiveAsn(new QStandardItem("ASN"))
{
    m_rootItem = projectModel->invisibleRootItem();
    projectModel->setColumnCount(3);
    ///
    /// for active...
    ///
    m_activeSubdomains->setIcon(QIcon(":/img/res/icons/folder2.png"));
    m_activeTlds->setIcon(QIcon(":/img/res/icons/folder2.png"));
    m_activeA->setIcon(QIcon(":/img/res/icons/folder2.png"));
    m_activeAAAA->setIcon(QIcon(":/img/res/icons/folder2.png"));
    m_activeNS->setIcon(QIcon(":/img/res/icons/folder2.png"));
    m_activeMX->setIcon(QIcon(":/img/res/icons/folder2.png"));
    m_activeTXT->setIcon(QIcon(":/img/res/icons/folder2.png"));
    m_activeCNAME->setIcon(QIcon(":/img/res/icons/folder2.png"));
    m_activeSRV->setIcon(QIcon(":/img/res/icons/folder2.png"));
    ///
    /// for passive...
    ///
    m_passiveSubdomains->setIcon(QIcon(":/img/res/icons/folder2.png"));
    m_passiveTlds->setIcon(QIcon(":/img/res/icons/folder2.png"));
    m_passiveA->setIcon(QIcon(":/img/res/icons/folder2.png"));
    m_passiveAAAA->setIcon(QIcon(":/img/res/icons/folder2.png"));
    m_passiveNS->setIcon(QIcon(":/img/res/icons/folder2.png"));
    m_passiveMX->setIcon(QIcon(":/img/res/icons/folder2.png"));
    m_passiveTXT->setIcon(QIcon(":/img/res/icons/folder2.png"));
    m_passiveCNAME->setIcon(QIcon(":/img/res/icons/folder2.png"));
    m_passiveSRV->setIcon(QIcon(":/img/res/icons/folder2.png"));
    m_passiveEmail->setIcon(QIcon(":/img/res/icons/folder2.png"));
    m_passiveUrl->setIcon(QIcon(":/img/res/icons/folder2.png"));
    m_passiveAsn->setIcon(QIcon(":/img/res/icons/folder2.png"));
    ///
    /// for active...
    ///
    m_activeSubdomains->setForeground(Qt::white);
    m_activeTlds->setForeground(Qt::white);
    m_activeA->setForeground(Qt::white);
    m_activeAAAA->setForeground(Qt::white);
    m_activeNS->setForeground(Qt::white);
    m_activeMX->setForeground(Qt::white);
    m_activeTXT->setForeground(Qt::white);
    m_activeCNAME->setForeground(Qt::white);
    m_activeSRV->setForeground(Qt::white);
    ///
    /// for passive...
    ///
    m_passiveSubdomains->setForeground(Qt::white);
    m_passiveTlds->setForeground(Qt::white);
    m_passiveA->setForeground(Qt::white);
    m_passiveAAAA->setForeground(Qt::white);
    m_passiveNS->setForeground(Qt::white);
    m_passiveMX->setForeground(Qt::white);
    m_passiveTXT->setForeground(Qt::white);
    m_passiveCNAME->setForeground(Qt::white);
    m_passiveSRV->setForeground(Qt::white);
    m_passiveEmail->setForeground(Qt::white);
    m_passiveUrl->setForeground(Qt::white);
    m_passiveAsn->setForeground(Qt::white);
    ///
    /// ...
    ///
    m_rootItem->appendRow(m_active);
    m_rootItem->appendRow(m_passive);
    ///
    /// for active...
    ///
    m_active->appendRow(m_activeSubdomains);
    m_active->appendRow(m_activeTlds);
    m_active->appendRow(m_activeA);
    m_active->appendRow(m_activeAAAA);
    m_active->appendRow(m_activeNS);
    m_active->appendRow(m_activeMX);
    m_active->appendRow(m_activeTXT);
    m_active->appendRow(m_activeCNAME);
    m_active->appendRow(m_activeSRV);
    ///
    /// for passive...
    ///
    m_passive->appendRow(m_passiveSubdomains);
    m_passive->appendRow(m_passiveTlds);
    m_passive->appendRow(m_passiveA);
    m_passive->appendRow(m_passiveAAAA);
    m_passive->appendRow(m_passiveNS);
    m_passive->appendRow(m_passiveMX);
    m_passive->appendRow(m_passiveTXT);
    m_passive->appendRow(m_passiveCNAME);
    m_passive->appendRow(m_passiveSRV);
    m_passive->appendRow(m_passiveEmail);
    m_passive->appendRow(m_passiveUrl);
    m_passive->appendRow(m_passiveAsn);
}
ProjectDataModel::~ProjectDataModel(){
    ///
    /// save subdomainIp files first using RAII model...
    ///

    ///
    /// then delete subdomainIp items...
    ///
    delete projectModel;
    delete m_active;
    delete m_passive;
    //...
    delete m_activeSubdomains;
    delete m_activeTlds;
    delete m_activeA;
    delete m_activeAAAA;
    delete m_activeNS;
    delete m_activeMX;
    delete m_activeTXT;
    delete m_activeCNAME;
    delete m_activeSRV;
    //...
    delete m_passiveSubdomains;
    delete m_passiveTlds;
    delete m_passiveA;
    delete m_passiveAAAA;
    delete m_passiveNS;
    delete m_passiveMX;
    delete m_passiveTXT;
    delete m_passiveCNAME;
    delete m_passiveSRV;
    delete m_passiveEmail;
    delete m_passiveUrl;
    delete m_passiveAsn;
}

void ProjectDataModel::addActiveSubdomain(QStringList items){
    int prevSize = m_activeSubdomainSet.count();
    m_activeSubdomainSet.insert(items[0]);
    if(m_activeSubdomainSet.count() > prevSize)
        m_activeSubdomains->appendRow({new QStandardItem(items[0]), new QStandardItem(items[1]), new QStandardItem(items[2])});
}
void ProjectDataModel::addPassiveSubdomain(QStringList items){
    int prevSize = m_passiveSubdomainSet.count();
    m_passiveSubdomainSet.insert(items[0]);
    if(m_passiveSubdomainSet.count() > prevSize)
        m_passiveSubdomains->appendRow(new QStandardItem(items[0]));
}

void ProjectDataModel::addActiveTLD(QStringList items){
    int prevSize = m_activeTldSet.count();
    m_activeTldSet.insert(items[0]);
    if(m_activeTldSet.count() > prevSize)
        m_activeTlds->appendRow({new QStandardItem(items[0]), new QStandardItem(items[1]), new QStandardItem(items[2])});
}
void ProjectDataModel::addPassiveTLD(QStringList items){
    int prevSize = m_passiveTldSet.count();
    m_passiveTldSet.insert(items[0]);
    if(m_passiveTldSet.count() > prevSize)
        m_passiveTlds->appendRow(new QStandardItem(items[0]));
}

/* if you arent sure what type of ip it is */
void ProjectDataModel::addActiveIp(QStringList items){
    Q_UNUSED(items)
}
void ProjectDataModel::addPassiveIp(QStringList items){
    if(items[0].contains(":")){
        int prevSize = m_passiveAAAASet.count();
        m_passiveAAAASet.insert(items[0]);
        if(m_passiveAAAASet.count() > prevSize)
            m_passiveAAAA->appendRow(new QStandardItem(items[0]));
    }
    else{
        int prevSize = m_passiveASet.count();
        m_passiveASet.insert(items[0]);
        if(m_passiveASet.count() > prevSize)
            m_passiveA->appendRow(new QStandardItem(items[0]));
    }
}

void ProjectDataModel::addActiveA(QStringList items){
    int prevSize = m_activeASet.count();
    m_activeASet.insert(items[0]);
    if(m_activeASet.count() > prevSize)
        m_activeA->appendRow({new QStandardItem(items[0]), nullptr, new QStandardItem(items[1])});
}
void ProjectDataModel::addPassiveA(QStringList items){
    int prevSize = m_passiveASet.count();
    m_passiveASet.insert(items[0]);
    if(m_passiveASet.count() > prevSize)
        m_passiveA->appendRow(new QStandardItem(items[0]));
}

void ProjectDataModel::addActiveAAAA(QStringList items){
    int prevSize = m_activeAAAASet.count();
    m_activeAAAASet.insert(items[0]);
    if(m_activeAAAASet.count() > prevSize)
        m_activeAAAA->appendRow({new QStandardItem(items[0]), nullptr, new QStandardItem(items[1])});
}
void ProjectDataModel::addPassiveAAAA(QStringList items){
    int prevSize = m_passiveAAAASet.count();
    m_passiveAAAASet.insert(items[0]);
    if(m_passiveAAAASet.count() > prevSize)
        m_passiveAAAA->appendRow(new QStandardItem(items[0]));
}

void ProjectDataModel::addActiveNS(QStringList items){
    int prevSize = m_activeNSSet.count();
    m_activeNSSet.insert(items[0]);
    if(m_activeNSSet.count() > prevSize)
        m_activeNS->appendRow({new QStandardItem(items[0]), nullptr, new QStandardItem(items[1])});
}
void ProjectDataModel::addPassiveNS(QStringList items){
    int prevSize = m_passiveNSSet.count();
    m_passiveNSSet.insert(items[0]);
    if(m_passiveNSSet.count() > prevSize)
        m_passiveNS->appendRow(new QStandardItem(items[0]));//, nullptr, new QStandardItem(items[1])});
}

void ProjectDataModel::addActiveMX(QStringList items){
    int prevSize = m_activeMXSet.count();
    m_activeMXSet.insert(items[0]);
    if(m_activeMXSet.count() > prevSize)
        m_activeMX->appendRow({new QStandardItem(items[0]), nullptr, new QStandardItem(items[1])});
}
void ProjectDataModel::addPassiveMX(QStringList items){
    int prevSize = m_passiveMXSet.count();
    m_passiveMXSet.insert(items[0]);
    if(m_passiveMXSet.count() > prevSize)
        m_passiveMX->appendRow(new QStandardItem(items[0]));
}

void ProjectDataModel::addActiveTXT(QStringList items){
    int prevSize = m_activeTXTSet.count();
    m_activeTXTSet.insert(items[0]);
    if(m_activeTXTSet.count() > prevSize)
        m_activeTXT->appendRow({new QStandardItem(items[0]), nullptr, new QStandardItem(items[1])});
}
void ProjectDataModel::addPassiveTXT(QStringList items){
    int prevSize = m_passiveTXTSet.count();
    m_passiveTXTSet.insert(items[0]);
    if(m_passiveTXTSet.count() > prevSize)
        m_passiveTXT->appendRow(new QStandardItem(items[0]));
}

void ProjectDataModel::addActiveCNAME(QStringList items){
    int prevSize = m_activeCNAMESet.count();
    m_activeCNAMESet.insert(items[0]);
    if(m_activeCNAMESet.count() > prevSize)
        m_activeCNAME->appendRow({new QStandardItem(items[0]), nullptr, new QStandardItem(items[1])});
}
void ProjectDataModel::addPassiveCNAME(QStringList items){
    int prevSize = m_passiveCNAMESet.count();
    m_passiveCNAMESet.insert(items[0]);
    if(m_passiveCNAMESet.count() > prevSize)
        m_passiveCNAME->appendRow(new QStandardItem(items[0]));
}

void ProjectDataModel::addActiveSRV(QStringList items){
    int prevSize = m_activeSRVSet.count();
    m_activeSRVSet.insert(items[0]);
    if(m_activeSRVSet.count() > prevSize)
        m_activeSRV->appendRow({new QStandardItem(items[0]), new QStandardItem(items[1]), new QStandardItem(items[2])});
}
void ProjectDataModel::addPassiveSRV(QStringList items){
    int prevSize = m_passiveSRVSet.count();
    m_passiveSRVSet.insert(items[0]);
    if(m_passiveSRVSet.count() > prevSize)
        m_passiveSRV->appendRow(new QStandardItem(items[0]));
}

void ProjectDataModel::addPassiveEMail(QStringList items){
    int prevSize = m_passiveEmailSet.count();
    m_passiveEmailSet.insert(items[0]);
    if(m_passiveEmailSet.count() > prevSize)
        m_passiveEmail->appendRow(new QStandardItem(items[0]));
}

void ProjectDataModel::addPassiveUrl(QStringList items){
    int prevSize = m_passiveUrlSet.count();
    m_passiveUrlSet.insert(items[0]);
    if(m_passiveUrlSet.count() > prevSize)
        m_passiveUrl->appendRow(new QStandardItem(items[0]));
}

void ProjectDataModel::addPassiveAsn(QStringList items){
    int prevSize = m_passiveAsnSet.count();
    m_passiveAsnSet.insert(items[0]);
    if(m_passiveAsnSet.count() > prevSize)
        m_passiveAsn->appendRow(new QStandardItem(items[0]));
}
